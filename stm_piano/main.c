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
static void rcc_config(void)
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

#define A1 LL_GPIO_PIN_8
#define B1 LL_GPIO_PIN_0
#define C1 LL_GPIO_PIN_12
#define D1 LL_GPIO_PIN_14
#define E1 LL_GPIO_PIN_15
#define F1 LL_GPIO_PIN_6
#define G1 LL_GPIO_PIN_11
#define POINT1 LL_GPIO_PIN_13

#define A2 LL_GPIO_PIN_0
#define B2 LL_GPIO_PIN_5
#define C2 LL_GPIO_PIN_7
#define D2 LL_GPIO_PIN_10
#define E2 LL_GPIO_PIN_11
#define F2 LL_GPIO_PIN_1
#define G2 LL_GPIO_PIN_6
#define POINT2 LL_GPIO_PIN_12

//combinations of pins for numbers (4 segment indicator1)
static uint16_t decoder1[] = {

	A1 | B1 | C1 | D1 | E1 | F1,
	B1 | C1,
	A1 | B1 | D1 | E1 | G1,
	A1 | B1 | C1 | D1 | G1,
 	B1 | C1 | F1 | G1,
	A1 | C1 | D1 | F1 | G1,
	A1 | C1 | D1 | E1 | F1 | G1,
	A1 | B1 | C1 | F1,
	A1 | B1 | C1 | D1 | E1 | F1 | G1,
	A1 | B1 | C1 | D1 | F1 | G1 		};
	
//combinations of pins for numbers (4 segment indicator 2)
static uint16_t decoder2[] = {

	A2 | B2 | C2 | D2 | E2 | F2,
	B2 | C2,
	A2 | B2 | D2 | E2 | G2,
	A2 | B2 | C2 | D2 | G2,
 	B2 | C2 | F2 | G2,
	A2 | C2 | D2 | F2 | G2,
	A2 | C2 | D2 | E2 | F2 | G2,
	A2 | B2 | C2 | F2,
	A2 | B2 | C2 | D2 | E2 | F2 | G2,
	A2 | B2 | C2 | D2 | F2 | G2 		};

//combinations of pins for a specific number out of four (4 segment indicator2)
uint16_t POS2[] = {	
	
	LL_GPIO_PIN_4 | LL_GPIO_PIN_3 | LL_GPIO_PIN_10,
	LL_GPIO_PIN_9 | LL_GPIO_PIN_3 | LL_GPIO_PIN_10,
	LL_GPIO_PIN_9 | LL_GPIO_PIN_4 | LL_GPIO_PIN_10,
	LL_GPIO_PIN_9 | LL_GPIO_PIN_4 | LL_GPIO_PIN_3, 
	LL_GPIO_PIN_3 | LL_GPIO_PIN_4 | LL_GPIO_PIN_9 | LL_GPIO_PIN_10		};

//combinations of pins for a specific number out of four (4 segment indicator1)
uint16_t POS1[] = {
	
	LL_GPIO_PIN_7 | LL_GPIO_PIN_6 | LL_GPIO_PIN_15,
	LL_GPIO_PIN_3 | LL_GPIO_PIN_6 | LL_GPIO_PIN_15,
	LL_GPIO_PIN_3 | LL_GPIO_PIN_7 | LL_GPIO_PIN_15,
	LL_GPIO_PIN_3 | LL_GPIO_PIN_7 | LL_GPIO_PIN_6,
	LL_GPIO_PIN_3 | LL_GPIO_PIN_6 | LL_GPIO_PIN_7 | LL_GPIO_PIN_15		};

static volatile uint32_t milliseconds = 0;												//time counter
static const uint32_t note1[] = {1, 573, 510, 455, 405, 382, 341, 303, 286, 255, 227, 202, 191, 170, 152, 143, 128, 114, 101, 96};	//number wich get us needed note frequency on sw
static const uint8_t note_num[] = {4, 5, 6, 7, 1, 2, 3, 4, 5, 6, 7, 1, 2, 3, 4, 5, 6, 7, 1};						//note number for indicator
static const uint32_t N = 78;														//en-coder round
static uint8_t move_note = 4;														//octave shift
static uint8_t ms_i = 0;														//counts down evry 4 seconds (in Systick handler)
static uint16_t portA_state = 0;													//current state of the port (for indicator)

//gpio configuration (ports and pins)
static void gpio_config(void)	
{
	//enable ports C, B, A
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);									
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);									
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);									
	
	//led
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_8, LL_GPIO_MODE_OUTPUT);    
	
	//Init pins for indicator
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_7, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_5, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_11, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_12, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_0, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_1, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_6, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_10, LL_GPIO_MODE_OUTPUT);

    
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_7, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_6, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_3, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_15, LL_GPIO_MODE_OUTPUT);

    
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_3, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_4, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_14, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_15, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_13, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_6, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_8, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_10, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_12, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_11, LL_GPIO_MODE_OUTPUT);
  
	////Init pins for buttons 
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_1, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_10, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_11, LL_GPIO_MODE_INPUT);
	
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_2, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_3, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_13, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_14, LL_GPIO_MODE_INPUT);
    
	return;
}

//system timer
static void systick_config(void)
{
	LL_InitTick(48000000, 1000);
	LL_SYSTICK_EnableIT();
	NVIC_SetPriority(SysTick_IRQn, 1);
}

//timers for sw
static void timers_config_sw(GPIO_TypeDef *GPIO, uint32_t Pin, uint32_t Alternate, uint32_t PeriphsT, uint32_t PeriphsP, TIM_TypeDef *TIM)
{     
	LL_AHB1_GRP1_EnableClock(PeriphsP);
	LL_GPIO_SetPinMode(GPIO, Pin, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_0_7(GPIO, Pin, Alternate);

	LL_APB1_GRP1_EnableClock(PeriphsT);
	LL_TIM_SetPrescaler(TIM, 480);
	LL_TIM_OC_SetCompareCH1(TIM, 95);
	LL_TIM_SetAutoReload(TIM, 100);
	LL_TIM_CC_EnableChannel(TIM, LL_TIM_CHANNEL_CH1);
	LL_TIM_OC_SetPolarity(TIM, LL_TIM_CHANNEL_CH1, LL_TIM_OCPOLARITY_HIGH);
	LL_TIM_OC_SetMode(TIM, LL_TIM_CHANNEL_CH1, LL_TIM_OCMODE_PWM1);
	LL_TIM_SetCounterMode(TIM, LL_TIM_COUNTERMODE_UP);
	LL_TIM_EnableIT_CC1(TIM);
	LL_TIM_EnableARRPreload(TIM);
	LL_TIM_EnableCounter(TIM);
    
	return;
}

//exti line for button
void set_exti_line(GPIO_TypeDef *GPIO, uint32_t Pin, uint32_t Line, uint32_t Port, uint32_t LLine)
{
	LL_GPIO_SetPinPull(GPIO, Pin, LL_GPIO_PULL_UP);
	LL_SYSCFG_SetEXTISource(Port, Line);
	
	LL_EXTI_EnableIT_0_31(LLine);
	LL_EXTI_EnableFallingTrig_0_31(LLine);
}

//exti for buttons
static void exti_config(void)
{
	LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
	
	//set where we will read the interrupts
	set_exti_line(GPIOB, LL_GPIO_PIN_0, LL_SYSCFG_EXTI_LINE0, LL_SYSCFG_EXTI_PORTB, LL_EXTI_LINE_0);
	set_exti_line(GPIOB, LL_GPIO_PIN_1, LL_SYSCFG_EXTI_LINE1, LL_SYSCFG_EXTI_PORTB, LL_EXTI_LINE_1);
	set_exti_line(GPIOB, LL_GPIO_PIN_10, LL_SYSCFG_EXTI_LINE10, LL_SYSCFG_EXTI_PORTB, LL_EXTI_LINE_10);
	set_exti_line(GPIOB, LL_GPIO_PIN_11, LL_SYSCFG_EXTI_LINE11, LL_SYSCFG_EXTI_PORTB, LL_EXTI_LINE_11);
	
	set_exti_line(GPIOC, LL_GPIO_PIN_2, LL_SYSCFG_EXTI_LINE2, LL_SYSCFG_EXTI_PORTC, LL_EXTI_LINE_2);
	set_exti_line(GPIOC, LL_GPIO_PIN_3, LL_SYSCFG_EXTI_LINE3, LL_SYSCFG_EXTI_PORTC, LL_EXTI_LINE_3);
	set_exti_line(GPIOC, LL_GPIO_PIN_13, LL_SYSCFG_EXTI_LINE13, LL_SYSCFG_EXTI_PORTC, LL_EXTI_LINE_13);
	set_exti_line(GPIOC, LL_GPIO_PIN_14, LL_SYSCFG_EXTI_LINE14, LL_SYSCFG_EXTI_PORTC, LL_EXTI_LINE_14);
	
	//enable interrupts
	NVIC_EnableIRQ(EXTI0_1_IRQn);
	NVIC_SetPriority(EXTI0_1_IRQn, 0);
	
	NVIC_EnableIRQ(EXTI2_3_IRQn);
	NVIC_SetPriority(EXTI2_3_IRQn, 0);

	NVIC_EnableIRQ(EXTI4_15_IRQn);
	NVIC_SetPriority(EXTI4_15_IRQn, 0);
}

//timer configuration for en-coder
static void timers_config_enc(void)
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_4, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_5, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_4, LL_GPIO_AF_1);
	LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_5, LL_GPIO_AF_1);
	LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_4, LL_GPIO_PULL_UP);
	LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_5, LL_GPIO_PULL_UP);

	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
   
	LL_TIM_IC_SetActiveInput(TIM3, LL_TIM_CHANNEL_CH1, LL_TIM_ACTIVEINPUT_DIRECTTI);
	LL_TIM_IC_SetActiveInput(TIM3, LL_TIM_CHANNEL_CH2, LL_TIM_ACTIVEINPUT_DIRECTTI);
	LL_TIM_IC_SetPolarity(TIM3, LL_TIM_CHANNEL_CH1, LL_TIM_ETR_POLARITY_NONINVERTED);
	LL_TIM_IC_SetPolarity(TIM3, LL_TIM_CHANNEL_CH2, LL_TIM_ETR_POLARITY_NONINVERTED); 
	LL_TIM_SetEncoderMode(TIM3, LL_TIM_ENCODERMODE_X4_TI12); 

	LL_TIM_SetAutoReload(TIM3, N);
	LL_TIM_EnableCounter(TIM3);
    
	return;
}

//struct for buttons
struct but
{
	int num;						//number of button
	uint32_t el;			        		//exti line
	int statement;						//pressed or not
	uint32_t ms_old;					//time beetween pressed (to eliminate inaccuracy)
	uint32_t t_pressed;					//when button change statement to 1;
};

//structure for buttons 
int but_create(int n, uint32_t e, struct but* b)
{
	b->num = n;
	b->el = e;
	b->statement = 0;
	b->ms_old = 0;
	b->t_pressed = 0;
	
	return 0;
}

//structure for sw
struct sw
{
	int indicator;				//note should be played
	TIM_TypeDef *TIM;			//timer number for sw
};

static struct sw s1 = {0, TIM2};		//sw 1
static struct sw s2 = {0, TIM14};		//sw 2
const int but_ar_size = 8;			//number of buttonst
static struct but b[8];				//button array

//populate an array of buttons
int but_fill(void)
{
	but_create(0, LL_EXTI_LINE_13, b);
	but_create(1, LL_EXTI_LINE_14, b + 1);
	but_create(2, LL_EXTI_LINE_2, b + 2);
	but_create(3, LL_EXTI_LINE_3, b + 3);
	but_create(4, LL_EXTI_LINE_0, b + 4);
	but_create(5, LL_EXTI_LINE_1, b + 5);
	but_create(6, LL_EXTI_LINE_10, b + 6);
	but_create(7, LL_EXTI_LINE_11, b + 7);

	return 0;
}

//function to select unoccupied or the most previously occupied sw to use
int chose_sw (void)
{	
	int ind1 = 0;				//number (note) that we will set on the sw1
	int ind2 = 0;				//number (note) that we will set on the sw2

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

//button reading function
int but_handler(struct but * but)
{
	uint32_t ms = milliseconds;
     
	if(ms - but->ms_old > 50)
	{            
		if(LL_EXTI_IsActiveFlag_0_31(but->el))
        	{      
			
			//button pressed
			if(but->statement == 0 && LL_EXTI_IsEnabledFallingTrig_0_31(but->el) == 1) 
			{
				LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_8);
				
                		but->statement = 1;
				but->t_pressed = milliseconds;
				
                		LL_EXTI_EnableRisingTrig_0_31(but->el);   
				but->ms_old = milliseconds; 
			}
            
			//button isn`t pressed
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
   
	//debouncing
	if(ms - but->ms_old <= 50 && ms - but->ms_old > 0 && LL_EXTI_IsActiveFlag_0_31(but->el))
	{    
		but->ms_old = milliseconds; 
		but->t_pressed = 0;
	}
    
   	LL_EXTI_ClearFlag_0_31(but->el);
	return 0;
}

//what to do with interruption on 0-1 lines
void EXTI0_1_IRQHandler(void)
{    
	but_handler(b + 4);
	but_handler(b + 5);
}

//what to do with interruption on 2-3 lines
void EXTI2_3_IRQHandler(void)
{    
	but_handler(b + 2);
	but_handler(b + 3);
}

//what to do with interruption on 4-15 lines
void EXTI4_15_IRQHandler(void)
{ 
	but_handler(b + 0);
	but_handler(b + 1);
	but_handler(b + 6);
	but_handler(b + 7);
}

//to do all silent if none button is pressed
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

//how to move note see on en-coder
//static int old_counter = 0;
//static int counter = 0;
//static int p = 1;

void enc_move(void)	
{
	if(LL_TIM_GetCounter(TIM3) >= 5*N/8 && LL_TIM_GetCounter(TIM3) < 7*N/8)
		move_note = 0;
	if(LL_TIM_GetCounter(TIM3) >= 7*N/8 || LL_TIM_GetCounter(TIM3) < N/8)
		move_note = 4;
	if(LL_TIM_GetCounter(TIM3) >= N/8 && LL_TIM_GetCounter(TIM3) < 3*N/8)
		move_note = 8;
	if(LL_TIM_GetCounter(TIM3) >= 3*N/8 && LL_TIM_GetCounter(TIM3) < 5*N/8)
		move_note = 11;
	/*
	if((LL_TIM_GetCounter(TIM3) - counter) < 0 || (LL_TIM_GetCounter(TIM3) - counter) == N)
		p = -1;
	else
		p = 1;
		
	
	if(abs((LL_TIM_GetCounter(TIM3) - old_counter) > 10))
	{
		move_note = move_note + p;
		old_counter = LL_TIM_GetCounter(TIM3);
	}
	 
	 if(move_note > 11)
		 move_note = 11;
	 
	 if(move_note < 0 )
		 move_note = 0; 
	
	counter = LL_TIM_GetCounter(TIM3);*/
}

//set number on display
void dyn_display (uint8_t num, int pos, int point, uint16_t *decoder, uint16_t *POS, GPIO_TypeDef *GPIOX, GPIO_TypeDef *GPIOY, uint16_t POINT )
{
	uint16_t out;       

	LL_GPIO_WriteOutputPort(GPIOY,(POS[4]));
	//LL_GPIO_WriteOutputPort(GPIOA, 0);

	out = decoder[num];

	if(point == 1)
		out = out | POINT;

	
	if(GPIOX != GPIOA)
   		LL_GPIO_WriteOutputPort(GPIOX, out);
	else
	{
		LL_GPIO_WriteOutputPort(GPIOX, out | portA_state);
	}
	
	if(GPIOY != GPIOA)
    		LL_GPIO_WriteOutputPort(GPIOY, POS[pos]);
	else
	{
		LL_GPIO_WriteOutputPort(GPIOY, POS[pos] | portA_state);
	}
		
		if(GPIOX == GPIOA)
			portA_state = out;
				
		if(GPIOY == GPIOA)
			portA_state = POS[pos];
		
	return;
}

//set sound on sw
void set_sound(struct sw s, int move_note)
{
	if(s.indicator != 0)
	{
		LL_TIM_OC_SetCompareCH1(s.TIM, (uint32_t)(0.95 * (note1[s.indicator + move_note])));
        	LL_TIM_SetAutoReload(s.TIM, note1[s.indicator + move_note]);
	}	
	else
	{
		LL_TIM_OC_SetCompareCH1(s.TIM, (uint32_t)(0.95 * (note1[s.indicator])));
        	LL_TIM_SetAutoReload(s.TIM, note1[s.indicator]);
	}	
}

//system timer handler
void SysTick_Handler(void)
{
	milliseconds++;   
   
	silent();  

	chose_sw ();

	enc_move();
	
	set_sound(s1, move_note);
	set_sound(s2, move_note);
	
	ms_i++;
	
	if(ms_i > 3)
		ms_i = 0;
	
	int p1 = 0;				//number (1 display) with a point or not
	int p2 = 0;				//number (2 display) with a point or not
	if(ms_i + move_note >= 11)		//check what octave is the note in
		p1 = 1;
	if(ms_i + move_note + 4 >= 11)
		p2 = 1;
	
	dyn_display(note_num[ms_i + move_note], ms_i, p1, decoder1, POS1, GPIOA, GPIOB, POINT1);
	dyn_display(note_num[ms_i + move_note + 4], ms_i, p2, decoder2, POS2, GPIOC, GPIOA, POINT2);
}

int main(void)
{
	but_fill();
	rcc_config();
	gpio_config();
	timers_config_sw(GPIOA, LL_GPIO_PIN_5, LL_GPIO_AF_2, LL_APB1_GRP1_PERIPH_TIM2, LL_AHB1_GRP1_PERIPH_GPIOA, TIM2);
	timers_config_sw(GPIOA, LL_GPIO_PIN_7, LL_GPIO_AF_4, LL_APB1_GRP1_PERIPH_TIM14, LL_AHB1_GRP1_PERIPH_GPIOA, TIM14);
	systick_config();
	exti_config();
	timers_config_enc();

	while (1)
	{
        
	}

	return 0;
}
