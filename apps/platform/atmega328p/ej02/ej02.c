#include "iom328p.h"

int main(void);

void delay_ms(uint16_t ms)
{
    uint16_t delay_count = 16000000/17500;
    volatile uint16_t i;

    while(ms != 0) {
        for(i = 0; i != delay_count; i++);
        ms--;
    }
}

int main()
{
    DDRB |= (1 << DDB5);

    while(1) {
        PORTB |= (1 << PORTB5);
        delay_ms(10000);        
        PORTB &= ~(1 << PORTB5);
        delay_ms(10000);        
    }
	return 0;
}

