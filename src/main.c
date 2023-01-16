#include "stm32h7xx.h"

#define LD1	(1 << 0)	// Green LED
#define LD2	(1 << 7)	// Blue LED
#define LD3	(1 << 14)	// Red LED

static void delay_ms(unsigned long cycle) {
	for (unsigned long i=0; i<cycle; i++)
		for (int j=0; j<11800; j++);
}

int main(void) {
	// activate GPIOB clock
    RCC->AHB4ENR |= RCC_AHB4ENR_GPIOBEN;

	// set gpio pins as output
	GPIOB->MODER = LD1 * LD1 * GPIO_MODER_MODE0_0;
	GPIOB->MODER |= LD2 * LD2 * GPIO_MODER_MODE0_0;
	GPIOB->MODER |= LD3 * LD3 * GPIO_MODER_MODE0_0;

	// flash leds
    while (1) {
		GPIOB->BSRR = LD1 | LD2 | LD3;	// Set LED
        delay_ms(1000);
		GPIOB->BSRR = (LD1 | LD2 | LD3) << 16;	// Reset LED
        delay_ms(1000);
    }
}
