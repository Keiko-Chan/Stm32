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
//------------------------------------------------------------------------------
//rcc config 
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
//gpio config 
//------------------------------------------------------------------------------
static void gpio_config(void)
{
  
    //Init port for indicator     
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_7, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_5, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_11, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_12, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_0, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_1, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_6, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_10, LL_GPIO_MODE_OUTPUT);
    
    
    
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_7, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_6, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_3, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_15, LL_GPIO_MODE_OUTPUT);
    
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
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
    
    
    //Init ports for right buttons     
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_1, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_10, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_11, LL_GPIO_MODE_INPUT);
    
    //Init ports for left buttons     
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_2, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_3, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_13, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_14, LL_GPIO_MODE_INPUT);
    
    //led
    
     LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_8, LL_GPIO_MODE_OUTPUT);
  
    return;
}
//------------------------------------------------------------------------------
//left display config 
//------------------------------------------------------------------------------
void dyn_display_left (uint8_t num, int pos, int point)
{
    uint16_t out;
    uint16_t POS[4];  

    POS[0] = LL_GPIO_PIN_6 | LL_GPIO_PIN_3 | LL_GPIO_PIN_15;
    POS[1] = LL_GPIO_PIN_3 | LL_GPIO_PIN_7 | LL_GPIO_PIN_15;
    POS[2] = LL_GPIO_PIN_6 | LL_GPIO_PIN_7 | LL_GPIO_PIN_15;
    POS[3] = LL_GPIO_PIN_7 | LL_GPIO_PIN_3 | LL_GPIO_PIN_6;         
          
    #define A1 LL_GPIO_PIN_15
    #define B1 LL_GPIO_PIN_13
    #define C1 LL_GPIO_PIN_9
    #define D1 LL_GPIO_PIN_10
    #define E1 LL_GPIO_PIN_11
    #define F1 LL_GPIO_PIN_14
    #define G1 LL_GPIO_PIN_8
    #define POINT1 LL_GPIO_PIN_6

    uint16_t decoder[10];

    decoder[0] =  A1 | B1 | C1 | D1 | E1 | F1;
    decoder[1] =  B1 | C1;
    decoder[2] =  A1 | B1 | D1 | E1 | G1;
    decoder[3] =  A1 | B1 | C1 | D1 | G1;
    decoder[4] =  B1 | C1 | F1 | G1;
    decoder[5] =  A1 | C1 | D1 | F1 | G1;
    decoder[6] =  A1 | C1 | D1 | E1 | F1 | G1;
    decoder[7] =  A1 | B1 | C1 | F1;
    decoder[8] =  A1 | B1 | C1 | D1 | E1 | F1 | G1;
    decoder[9] =  A1 | B1 | C1 | D1 | F1 | G1;


    LL_GPIO_WriteOutputPort(GPIOB,(LL_GPIO_PIN_7 | LL_GPIO_PIN_6 | LL_GPIO_PIN_3 | LL_GPIO_PIN_15));
    //LL_GPIO_WriteOutputPort(GPIOA, 0);

    out = decoder[num];
    
    if(point == 1)
        out = out | POINT1;
    
    LL_GPIO_WriteOutputPort(GPIOA, out);

    LL_GPIO_WriteOutputPort(GPIOB, POS[pos]);

    return;
}
//------------------------------------------------------------------------------
//right display config 
//------------------------------------------------------------------------------
void dyn_display_right (uint8_t num, int pos, int point)
{
    uint16_t out;
    uint16_t POS[4];  

    POS[0] = LL_GPIO_PIN_3 | LL_GPIO_PIN_4 | LL_GPIO_PIN_12;
    POS[1] = LL_GPIO_PIN_0 | LL_GPIO_PIN_4 | LL_GPIO_PIN_12;
    POS[2] = LL_GPIO_PIN_0 | LL_GPIO_PIN_3 | LL_GPIO_PIN_12;
    POS[3] = LL_GPIO_PIN_0 | LL_GPIO_PIN_3 | LL_GPIO_PIN_3;         
          
    #define A LL_GPIO_PIN_0
    #define B LL_GPIO_PIN_5
    #define C LL_GPIO_PIN_7
    #define D LL_GPIO_PIN_11
    #define E LL_GPIO_PIN_12
    #define F LL_GPIO_PIN_1
    #define G LL_GPIO_PIN_6
    #define POINT LL_GPIO_PIN_10

    uint16_t decoder[10];

    decoder[0] =  A | B | C | D | E | F;
    decoder[1] =  B | C;
    decoder[2] =  A | B | D | E | G;
    decoder[3] =  A | B | C | D | G;
    decoder[4] =  B | C | F | G;
    decoder[5] =  A | C | D | F | G;
    decoder[6] =  A | C | D | E | F | G;
    decoder[7] =  A | B | C | F;
    decoder[8] =  A | B | C | D | E | F | G;
    decoder[9] =  A | B | C | D | F | G;


    LL_GPIO_WriteOutputPort(GPIOA,(LL_GPIO_PIN_0 | LL_GPIO_PIN_3 | LL_GPIO_PIN_4 | LL_GPIO_PIN_12));
    //LL_GPIO_WriteOutputPort(GPIOC, 0);

    out = decoder[num];
    
    if(point == 1)
        out = out | POINT;
    
    LL_GPIO_WriteOutputPort(GPIOC, out);

    LL_GPIO_WriteOutputPort(GPIOA, POS[pos]);

    return;
}
//------------------------------------------------------------------------------
//system timer config 
//------------------------------------------------------------------------------
static void systick_config()
{
    LL_InitTick(48000000, 1000);
    LL_SYSTICK_EnableIT();
    NVIC_SetPriority(SysTick_IRQn, 1);
}
//------------------------------------------------------------------------------
//required variables
//------------------------------------------------------------------------------
static uint32_t note1[] = {341, 341, 286, 227, 191};
static uint32_t note2[] = {341, 382, 303, 255, 202, 170, 152, 143, 114};
static int indicator1 = 0;
static int indicator2 = 0;
static int milliseconds = 0;
static int ms_old[] = {0, 0, 0 ,0};
static int statement[] = { 0, 0, 0, 0, 0, 0, 0, 0};
static int left_note = 0;
static int right_note = 0;
int ms_i = 0;
int N = 99;
int note_num_l[] = {2, 4, 6, 2, 4, 6};
int note_num_r[] = {1, 3, 5, 7, 2, 3, 4, 6};
//------------------------------------------------------------------------------
//timer 2 config for sw 1
//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
//timer 14 config for sw 2
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
//timer 3 config for encoder 2
//------------------------------------------------------------------------------
static void timers_config_enc2(void)
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
/*
//------------------------------------------------------------------------------
//timer 2 config for encoder 1
//------------------------------------------------------------------------------
static void timers_config_enc1(void)
{
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_1, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_1, LL_GPIO_AF_2);
    LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_5, LL_GPIO_AF_2);
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_1, LL_GPIO_PULL_UP);
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_5, LL_GPIO_PULL_UP);

    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
   
    LL_TIM_IC_SetActiveInput(TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_ACTIVEINPUT_DIRECTTI);
    LL_TIM_IC_SetActiveInput(TIM2, LL_TIM_CHANNEL_CH2, LL_TIM_ACTIVEINPUT_DIRECTTI);
    LL_TIM_IC_SetPolarity(TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_ETR_POLARITY_NONINVERTED);
    LL_TIM_IC_SetPolarity(TIM2, LL_TIM_CHANNEL_CH2, LL_TIM_ETR_POLARITY_NONINVERTED); 
    LL_TIM_SetEncoderMode(TIM2, LL_TIM_ENCODERMODE_X4_TI12); 

    LL_TIM_SetAutoReload(TIM2, 95);
    LL_TIM_EnableCounter(TIM2);
    
    return;
}*/
//------------------------------------------------------------------------------
//exti_config for left and right buttons
//------------------------------------------------------------------------------
static void exti_config()
{
    LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_0, LL_GPIO_PULL_UP);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_1, LL_GPIO_PULL_UP);
    LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_2, LL_GPIO_PULL_UP);
    LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_3, LL_GPIO_PULL_UP);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_10, LL_GPIO_PULL_UP);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_11, LL_GPIO_PULL_UP);
    LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_13, LL_GPIO_PULL_UP);
    LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_14, LL_GPIO_PULL_UP);
    
    LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB, LL_SYSCFG_EXTI_LINE0);
    LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB, LL_SYSCFG_EXTI_LINE1);
    LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE2);
    LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE3);
    LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB, LL_SYSCFG_EXTI_LINE10);
    LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB, LL_SYSCFG_EXTI_LINE11);
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
//------------------------------------------------------------------------------
//EXTI for B0 and B1 buttons
//------------------------------------------------------------------------------
void EXTI0_1_IRQHandler()
{    
    int ms = milliseconds;
     
    //change indicator2 with buttons B0 and B1
    if(ms - ms_old[0] > 100)
    {    
        LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_8);
        ms_old[0] = milliseconds;  
        
  
        if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_0))
        {
                    
              if(statement[0] == 0) 
              {
                  indicator2 = 1;
                  statement[0] = 1;
                  LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_0);                  
              }
              
              else 
              {
                  if(indicator2 == 1)
                      indicator2 = 0;
                  statement[0] = 0;
                  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_0);                  
              }
        }
        
        if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_1))
        {
          
              if(statement[1] == 0)
              {
                  indicator2 = 2;
                  statement[1] = 1;
                  LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_1);                  
              }
              
              else 
              {
                  if(indicator2 == 2)
                      indicator2 = 0;
                  statement[1] = 0;
                  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_1);                  
              }    
        }
        
    }
    
    if(ms - ms_old[0] <= 100 && ms - ms_old[0] > 0 && statement[0] == 1 && LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_0))
    {    
          ms_old[0] = milliseconds; 
          if(indicator2 == 1)
              indicator2 = 0;
          statement[0] = 0;
          LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_0);         
    }
    
    
    if(ms - ms_old[0] <= 100 && ms - ms_old[0] > 0 && statement[1] == 1 && LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_1))
    {    
          ms_old[0] = milliseconds; 
          if(indicator2 == 2)
              indicator2 = 0;
          statement[1] = 0;
          LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_1);         
    }    
    
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_1);
}
//------------------------------------------------------------------------------
//EXTI for B10, B11 and C13, C14 buttons
//------------------------------------------------------------------------------
void EXTI4_15_IRQHandler()
{    
    int ms = milliseconds;
    int ms0 = milliseconds;
     
    //change indicator with buttons B10 and B11 
    if(ms - ms_old[1] > 100)
    {    
        LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_8);
        ms_old[1] = milliseconds;  
        
  
        if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_10))
        {
                    
              if(statement[2] == 0) 
              {
                  indicator2 = 3;
                  statement[2] = 1;
                  LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_10);                  
              }
              
              else 
              {
                  if(indicator2 == 3)
                      indicator2 = 0;
                  statement[2] = 0;
                  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_10);                  
              }
        }
        
        if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_11))
        {
          
              if(statement[3] == 0)
              {
                  indicator2 = 4;
                  statement[3] = 1;
                  LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_11);                  
              }
              
              else 
              {
                  if(indicator2 == 4)
                      indicator2 = 0;
                  statement[3] = 0;
                  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_11);                  
              }    
        }
        
    }
    
    if(ms - ms_old[1] <= 100 && ms - ms_old[1] > 0   && statement[2] == 1 && LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_10))
    {    
          ms_old[1] = milliseconds; 
          if(indicator2 == 3)
              indicator2 = 0;
          statement[2] = 0;
          LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_10);         
    }
    
    
    if(ms - ms_old[1] <= 100 && ms - ms_old[1] > 0   && statement[3] == 1 && LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_11))
    {    
          ms_old[1] = milliseconds; 
          if(indicator2 == 4)
              indicator2 = 0;
          statement[3] = 0;
          LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_11);         
    }

    //change indicator with buttons C13 and C14
    if(ms0 - ms_old[2] > 100)
    {    
        LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_8);
        ms_old[2] = milliseconds;  
        
  
        if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_13))
        {                    
              if(statement[4] == 0) 
              {
                  indicator1 = 4;
                  statement[4] = 1;
                  LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_13);                  
              }
              
              else 
              {
                  if(indicator1 == 4)
                      indicator1 = 0;
                  statement[4] = 0;
                  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_13);                  
              }
        }
        
        if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_14))
        {
          
              if(statement[5] == 0)
              {
                  indicator1 = 3;
                  statement[5] = 1;
                  LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_14);                  
              }
              
              else 
              {
                  if(indicator1 == 3)
                      indicator1 = 0;
                  statement[5] = 0;
                  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_14);                  
              }    
        }
        
    }
    
    if(ms0 - ms_old[2] <= 100 && ms0 - ms_old[2] > 0   && statement[4] == 1 && LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_13))
    {    
          ms_old[2] = milliseconds; 
          if(indicator1 == 4)
              indicator1 = 0;
          statement[4] = 0;
          LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_13);         
    }
    
    
    if(ms0 - ms_old[2] <= 100 && ms0 - ms_old[2] > 0   && statement[5] == 1 && LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_14))
    {    
          ms_old[2] = milliseconds; 
          if(indicator1 == 3)
              indicator1 = 0;
          statement[5] = 0;
          LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_14);         
    }
    
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_10);
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_11);
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_13);
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_14);
}
//------------------------------------------------------------------------------
//EXTI for C2 and C3 buttons
//------------------------------------------------------------------------------
void EXTI2_3_IRQHandler()
{
    
    int ms = milliseconds;

    //change indicator1 with buttons C2 and C3
    if(ms - ms_old[3] > 100)
    {    
        LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_8);
        ms_old[3] = milliseconds;  
        
  
        if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_2))
        {                    
              if(statement[6] == 0) 
              {
                  indicator1 = 2;
                  statement[6] = 1;
                  LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_2);                  
              }
              
              else 
              {
                  if(indicator1 == 2)
                      indicator1 = 0;
                  statement[6] = 0;
                  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_2);                  
              }
        }
        
        if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_3))
        {
          
              if(statement[7] == 0)
              {
                  indicator1 = 1;
                  statement[7] = 1;
                  LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_3);                  
              }
              
              else 
              {
                  if(indicator1 == 1)
                      indicator1 = 0;
                  statement[7] = 0;
                  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_3);                  
              }    
        }
        
    }
    
    if(ms - ms_old[3] <= 100 && ms - ms_old[3] > 0   && statement[6] == 1 && LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_2))
    {    
          ms_old[3] = milliseconds; 
          if(indicator1 == 2)
              indicator1 = 0;
          statement[6] = 0;
          LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_2);         
    }
    
    
    if(ms - ms_old[3] <= 100 && ms - ms_old[3] > 0   && statement[7] == 1 && LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_3))
    {    
          ms_old[3] = milliseconds; 
          if(indicator1 == 1)
              indicator1 = 0;
          statement[7] = 0;
          LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_3);         
    }  
    
    //clear flags
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_2);
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_3);
}
//------------------------------------------------------------------------------
//SysTick Handler
//------------------------------------------------------------------------------
void SysTick_Handler(void)
{
    milliseconds++;
   
    //do sw 2 silent
    if((LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_0) == 1) && (LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_1) == 1) && (LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_10) == 1) && (LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_11) == 1))
    {
        indicator2 = 0;
        statement[3] = 0;
        LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_10);
        statement[2] = 0;
        LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_11); 
        statement[1] = 0;
        LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_1); 
        statement[0] = 0;
        LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_0); 
    }
    
    if((LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_2) == 1) && (LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_3) == 1) && (LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_13) == 1) && (LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_14) == 1))
    {
        indicator1 = 0;
        statement[4] = 0;
        LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_14);
        statement[5] = 0;
        LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_13); 
        statement[6] = 0;
        LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_3); 
        statement[7] = 0;
        LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_2); 
    }   
       
    //using encoder 2
    if(LL_TIM_GetCounter(TIM3) > N/3 && LL_TIM_GetCounter(TIM3) < N/3*2)
        right_note = 2;
    if(LL_TIM_GetCounter(TIM3) >= N/3*2)
        right_note = 4;
    if(LL_TIM_GetCounter(TIM3) <= N/3)
        right_note = 0;
       
    //change note on sw2
    if(indicator2 == 0)
    {
        LL_TIM_OC_SetCompareCH1(TIM14, (uint32_t)(0.96 * (note2[indicator2])));
        LL_TIM_SetAutoReload(TIM14, note2[indicator2]); 
    }
    
    else
    {
        LL_TIM_OC_SetCompareCH1(TIM14, (uint32_t)(0.96 * (note2[indicator2 + right_note])));
        LL_TIM_SetAutoReload(TIM14, note2[indicator2 + right_note]);
    }    
    
    //change note on sw1    
     LL_TIM_OC_SetCompareCH1(TIM2, (uint32_t)(0.96 * (note1[indicator1 + left_note])));
        LL_TIM_SetAutoReload(TIM2, note1[indicator1 + left_note]);
        
    //work with indicator
    ms_i++;
    int point_l = 0;
    int point_r = 0;
   
    if(ms_i >3)
        ms_i = 0;
    
    if((left_note + ms_i) > 2)
        point_l = 1;
   
    if((right_note + ms_i) > 3)
        point_r = 1;

    dyn_display_left(note_num_r[ms_i + right_note], ms_i, point_r);
    //dyn_display_right(note_num_r[ms_i], ms_i, point_r);        
}
//------------------------------------------------------------------------------
//Main
//------------------------------------------------------------------------------
int main(void)
{
    rcc_config();
    gpio_config();
    timers_config_sw1();
    timers_config_sw2();
    systick_config();
    exti_config();
    timers_config_enc2();
    

    while (1)
    {
        
    }

    return 0;
}
