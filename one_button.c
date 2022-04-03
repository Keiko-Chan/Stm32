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
static int indicator2 = 0;
static int milliseconds = 0;
static int statement = 0;
static uint32_t note1[] = {1, 341, 286, 227, 191};

/*
 * Clock on GPIOC and set one led
 */
static void gpio_config(void)
{
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_8, LL_GPIO_MODE_OUTPUT);    
  
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_INPUT);
    
    return;
}

static void systick_config()
{
    LL_InitTick(48000000, 1000);
    LL_SYSTICK_EnableIT();
    NVIC_SetPriority(SysTick_IRQn, 1);
}

/*
 * Configure timer to output compare mode
 */
static void timers_config(void)
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

static void exti_config()
{
	LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
	LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_0, LL_GPIO_PULL_UP);
    LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB, LL_SYSCFG_EXTI_LINE0);
    
    LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_0);
    LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_0);

    NVIC_EnableIRQ(EXTI0_1_IRQn);
    NVIC_SetPriority(EXTI0_1_IRQn, 0);
}

struct butt
{
	int num;				//number of button
	uint32_t el;	        //exti line
	int statement;			//pressed or not
	int ms_old;				//time beetween pressed (to eliminate inaccuracy)	
};

struct sw
{
	int indicator;
	int t_busy;
};

static struct butt b0 = {0, LL_EXTI_LINE_0, 0, 0};
static struct sw s1 = {0, 0};

int chose_sw (int num_but)
{
	s1.indicator = num_but + 1;
	s1.t_busy = milliseconds;
	return 0; 
}

int butt_handler(struct butt * but)
{
	int ms = milliseconds;
     
    if(ms - but->ms_old > 90)
    {    
        LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_8);
        but->ms_old = milliseconds;  
        
        if(LL_EXTI_IsActiveFlag_0_31(but->el))
        {                    
         	if(statement == 0) 
            {
                but->statement = 1;
                LL_EXTI_EnableFallingTrig_0_31(but->el);                  
            }
              
            else 
            {
            	but->statement = 0;
                LL_EXTI_EnableRisingTrig_0_31(but->el);                  
            }
		}      
   	}
   
    if(ms - but->ms_old <= 90 && ms - but->ms_old > 0 && but->statement == 1 && LL_EXTI_IsActiveFlag_0_31(but->el))
    {    
    	but->ms_old = milliseconds; 
        but->statement = 0;
        LL_EXTI_EnableRisingTrig_0_31(but->el);         
    }

	if(but->statement == 1)
		chose_sw (but->num);
    
   	LL_EXTI_ClearFlag_0_31(but->el);
    return 0;
}

void EXTI0_1_IRQHandler()
{    
	butt_handler(&b0);
}

void SysTick_Handler(void)
{
    milliseconds++;
   
   
    if(LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_0) == 1)
    {
        s1.indicator = 0;
        b0.statement = 0;
        LL_EXTI_EnableRisingTrig_0_31(b0.el); 
    }      

    LL_TIM_OC_SetCompareCH1(TIM2, (uint32_t)(0.96 * (note1[s1.indicator])));
        LL_TIM_SetAutoReload(TIM2, note1[s1.indicator]);
}

int main(void)
{
    rcc_config();
    gpio_config();
    timers_config();
    systick_config();
    exti_config();
    
    

    while (1)
    {
        
    }

    return 0;
}
