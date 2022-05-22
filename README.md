# Stm32
some work with stm32
There is code, I made in 2 sem. It has some problems, which I should fix:
1) delete one en-coder  (done)
2) to do another keyboard (done)
3) add settings before start 
4) add semitones
5) try do two-three sounds in one time (third sw doesnt work)
6) better buttons treatment (done, old errors are eliminated in it) 
7) think about usefulness

rcc_config - configures the board clock


gpio_config - configures the necessary pins
systick_config - enable and configure the system timer
timers_config_sw - configures the timer for the piezo tweeter
timers_config_enc - configures the timer for the encoder
exti_config - configures interrupts to respond to button presses, when an interrupt is triggered in the EXTI0_1_IRQHandler, EXTI4_15_IRQHandler, EXTI2_3_IRQHandler functions, but_handler is called.
but_handler - determines the state of the button (pressed or not), eliminates the bounce of the button.
silent - checks if at least some button is pressed and if not, turns off the buzzers, needed to prevent errors in operation
enc_move - checks the position of the encoder and determines the octave shift depending on it.
dyn_display - displays numbers on the display
set_sound - sets the sound for the buzzer
SysTick_Handler - add millisecons, call set_sound, update digits on displays (one digit per display)
