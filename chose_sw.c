#include "stm32f0xx_ll_adc.h"
#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_system.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_exti.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_utils.h"
#include "stm32f0xx_ll_cortex.h"
#include "system_stm32f0xx.h" // because of there is "uint??_t" this include must be under other.
#include "stm32f0xx_ll_tim.h"

/**
  * System Clock Configuration
  * The system Clock is configured as follow :
  *    System Clock source            = PLL (HSI/2)
  *    SYSCLK(Hz)                     = 48000000
  *    HCLK(Hz)                       = 48000000
  *    AHB Prescaler                  = 1
  *    APB1 Prescaler                 = 1
  *    HSI Frequency(Hz)              = 8000000
  *    PLLMUL                         = 12
  *    Flash Latency(WS)              = 1
  */
static void rcc_config()
{     
        /* Set FLASH latency */
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

    /* Enable HSI and wait for activation*/
    LL_RCC_HSI_Enable();
    while (LL_RCC_HSI_IsReady() != 1);

    /* Main PLL configuration and activation */
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2,
                                LL_RCC_PLL_MUL_12);

    LL_RCC_PLL_Enable();
    while (LL_RCC_PLL_IsReady() != 1);

    /* Sysclk activation on the main PLL */
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);

    /* Set APB1 prescaler */
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

    /* Update CMSIS variable (which can be updated also
     * through SystemCoreClockUpdate function) */
    SystemCoreClock = 48000000;
    SystemCoreClockUpdate(); // we need this func to update SystemClock info.
    
}
static int milliseconds = 0;
static uint32_t note1[] = {1, 382, 341, 303, 286, 255, 227, 202, 191};


static void gpio_config(void)
{
	//led
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_8, LL_GPIO_MODE_OUTPUT);    
  
	////Init ports for buttons 
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_1, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_10, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_11, LL_GPIO_MODE_INPUT);
	
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_2, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_3, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_13, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_14, LL_GPIO_MODE_INPUT);
    
    return;
}

static void systick_config()
{
    LL_InitTick(48000000, 1000);
    LL_SYSTICK_EnableIT();
    NVIC_SetPriority(SysTick_IRQn, 1);
}

//timer 2, pin 5 for sw1
static void timers_config_sw1(void)
{     
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_5, LL_GPIO_AF_2);

    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
    LL_TIM_SetPrescaler(TIM2, 480);
    LL_TIM_OC_SetCompareCH1(TIM2, 95);
    LL_TIM_SetAutoReload(TIM2, 100);
    LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH1);
    LL_TIM_OC_SetPolarity(TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_OCPOLARITY_HIGH);
    LL_TIM_OC_SetMode(TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_OCMODE_PWM1);
    LL_TIM_SetCounterMode(TIM2, LL_TIM_COUNTERMODE_UP);
    LL_TIM_EnableIT_CC1(TIM2);
    LL_TIM_EnableARRPreload(TIM2);
    LL_TIM_EnableCounter(TIM2);
    
    return;
}

//timer 14, pin 7 for sw2
static void timers_config_sw2(void)
{
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_7, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_7, LL_GPIO_AF_4);

    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM14);
    LL_TIM_SetPrescaler(TIM14, 480);
    LL_TIM_OC_SetCompareCH1(TIM14, 95);
    LL_TIM_SetAutoReload(TIM14, 100);
    LL_TIM_CC_EnableChannel(TIM14, LL_TIM_CHANNEL_CH1);
    LL_TIM_OC_SetPolarity(TIM14, LL_TIM_CHANNEL_CH1, LL_TIM_OCPOLARITY_HIGH);
    LL_TIM_OC_SetMode(TIM14, LL_TIM_CHANNEL_CH1, LL_TIM_OCMODE_PWM1);
    LL_TIM_SetCounterMode(TIM14, LL_TIM_COUNTERMODE_UP);
    LL_TIM_EnableIT_CC1(TIM14);
    LL_TIM_EnableARRPreload(TIM14);
    LL_TIM_EnableCounter(TIM14);

    return;
}

static void exti_config()
{
	LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
	LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_0, LL_GPIO_PULL_UP);
	LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_1, LL_GPIO_PULL_UP);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_10, LL_GPIO_PULL_UP);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_11, LL_GPIO_PULL_UP);
	
	LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_2, LL_GPIO_PULL_UP);
    LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_3, LL_GPIO_PULL_UP);
    LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_13, LL_GPIO_PULL_UP);
    LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_14, LL_GPIO_PULL_UP);

    LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB, LL_SYSCFG_EXTI_LINE0);
	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB, LL_SYSCFG_EXTI_LINE1);
    LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB, LL_SYSCFG_EXTI_LINE10);
    LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB, LL_SYSCFG_EXTI_LINE11);
	
	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE2);
    LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE3);
	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE13);
    LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE14);
    
    LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_0);
    LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_0);

	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_1);
    LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_1);
	
	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_2);
    LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_2);
    
    LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_3);
    LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_3);

	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_10);
    LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_10);
    
    LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_11);
    LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_11);
	
	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_13);
    LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_13);
    
    LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_14);
    LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_14);

    NVIC_EnableIRQ(EXTI0_1_IRQn);
    NVIC_SetPriority(EXTI0_1_IRQn, 0);
	
	NVIC_EnableIRQ(EXTI2_3_IRQn);
    NVIC_SetPriority(EXTI2_3_IRQn, 0);

	NVIC_EnableIRQ(EXTI4_15_IRQn);
    NVIC_SetPriority(EXTI4_15_IRQn, 0);
}

struct but
{
	int num;				//number of button
	uint32_t el;	        //exti line
	int statement;			//pressed or not
	int ms_old;				//time beetween pressed (to eliminate inaccuracy)
	int t_pressed;			//when button change statement to 1;
};

struct but but_create(int n, uint32_t e, int s, int m, int t)
{
	struct but b = {n, e, s, m, t};
	return b;
}

struct sw
{
	int indicator;
	int t_busy;
};

static struct sw s1 = {0, 0};
static struct sw s2 = {0, 0};
const int but_ar_size = 8;
static struct but b[8];

int but_fill(void)
{
	b[0] = but_create(0, LL_EXTI_LINE_13, 0, 0, 0);
	b[1] = but_create(1, LL_EXTI_LINE_14, 0, 0, 0);
	b[2] = but_create(2, LL_EXTI_LINE_2, 0, 0, 0);
	b[3] = but_create(3, LL_EXTI_LINE_3, 0, 0, 0);
	b[4] = but_create(4, LL_EXTI_LINE_0, 0, 0, 0);
	b[5] = but_create(5, LL_EXTI_LINE_1, 0, 0, 0);
	b[6] = but_create(6, LL_EXTI_LINE_10, 0, 0, 0);
	b[7] = but_create(7, LL_EXTI_LINE_11, 0, 0, 0);

	return 0;
}

int chose_sw ()
{	
	int ind1 = 0;
	int ind2 = 0;

	for(int i = 0; i < but_ar_size; i++)
	{
		if(b[i].statement == 1)
		{
			if(ind1 == 0)
				ind1 = i + 1;
			else
			{
				if(ind2 == 0)
					ind2 = i + 1;
				else
				{
					if(b[ind1-1].t_pressed < b[i].t_pressed)
						ind1 = i + 1;
					else
						if(b[ind2-1].t_pressed < b[i].t_pressed)
							ind2 = i + 1;
				}
			}			
		}
	}
	
	if((ind1 == s1.indicator && ind2 == s2.indicator) || (ind2 == s1.indicator && ind1 == s2.indicator))
		return 0;		
	
	if(ind1 == s1.indicator)
	{
		s2.indicator = ind2;
		return 0;
	}
	
	if(ind2 == s1.indicator)
	{
		s2.indicator = ind1;
		return 0;
	}
	
	if(ind1 == s2.indicator)
	{
		s1.indicator = ind2;
		return 0;
	}
	
	if(ind2 == s2.indicator)
	{
		s1.indicator = ind1;
		return 0;
	}
	
	s1.indicator = ind1;
	s2.indicator = ind2;
	
	return 0; 
}

int butt_handler(struct but * but)
{
	int ms = milliseconds;
     
    if(ms - but->ms_old > 90)
    {            
        if(LL_EXTI_IsActiveFlag_0_31(but->el))
        {      
			
			
         	if(but->statement == 0 && LL_EXTI_IsEnabledFallingTrig_0_31(but->el) == 1) 
            {
				LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_8);
				
                but->statement = 1;
				but->t_pressed = milliseconds;
				
                LL_EXTI_EnableRisingTrig_0_31(but->el);   
				but->ms_old = milliseconds; 
            }
            
			else
            	if(but->statement == 1 && LL_EXTI_IsEnabledRisingTrig_0_31(but->el) == 1) 
            	{
            		but->statement = 0;
					but->t_pressed = 0;
			
                	LL_EXTI_EnableFallingTrig_0_31(but->el);   
					but->ms_old = milliseconds;
            	}
		}      
   	}
   
    if(ms - but->ms_old <= 90 && ms - but->ms_old > 0 && LL_EXTI_IsActiveFlag_0_31(but->el))
    {    
    	but->ms_old = milliseconds; 
		but->t_pressed = 0;
    }
    
   	LL_EXTI_ClearFlag_0_31(but->el);
    return 0;
}

void EXTI0_1_IRQHandler()
{    
	butt_handler(b + 4);
	butt_handler(b + 5);
}

void EXTI2_3_IRQHandler()
{    
	butt_handler(b + 2);
	butt_handler(b + 3);
}

void EXTI4_15_IRQHandler()
{ 
	butt_handler(b + 0);
	butt_handler(b + 1);
	butt_handler(b + 6);
	butt_handler(b + 7);
}

void silent(void)
{
	if((LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_0) == 1) && (LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_1) == 1) && (LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_10) == 1) && (LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_11) == 1) &&
	   (LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_2) == 1) && (LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_3) == 1) && (LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_13) == 1) && (LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_14) == 1))	
	{
		s1.indicator = 0;
		s2.indicator = 0;
        b[0].statement = 0;
		b[1].statement = 0;
		b[2].statement = 0;
		b[3].statement = 0;
		b[4].statement = 0;
		b[5].statement = 0;
		b[6].statement = 0;
		b[7].statement = 0;
        LL_EXTI_EnableRisingTrig_0_31(b[0].el); 
		LL_EXTI_EnableRisingTrig_0_31(b[1].el);
		LL_EXTI_EnableRisingTrig_0_31(b[2].el);
		LL_EXTI_EnableRisingTrig_0_31(b[3].el);	
		LL_EXTI_EnableRisingTrig_0_31(b[4].el); 
		LL_EXTI_EnableRisingTrig_0_31(b[5].el);
		LL_EXTI_EnableRisingTrig_0_31(b[6].el);
		LL_EXTI_EnableRisingTrig_0_31(b[7].el);
	}
	return;
}

void SysTick_Handler(void)
{
    milliseconds++;   
   
    silent();  

	chose_sw ();

    LL_TIM_OC_SetCompareCH1(TIM2, (uint32_t)(0.95 * (note1[s1.indicator])));
        LL_TIM_SetAutoReload(TIM2, note1[s1.indicator]);
		
	LL_TIM_OC_SetCompareCH1(TIM14, (uint32_t)(0.95 * (note1[s2.indicator])));
        LL_TIM_SetAutoReload(TIM14, note1[s2.indicator]);
}

int main(void)
{
	but_fill();
    rcc_config();
    gpio_config();
    timers_config_sw1();
	timers_config_sw2();
    systick_config();
    exti_config();
    
    

    while (1)
    {
        
    }

    return 0;
}