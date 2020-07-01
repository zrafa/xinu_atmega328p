#define	GPIO_PINB	(*(volatile unsigned char *)0x23)
#define GPIO_DDRB	(*(volatile unsigned char *)0x24)
#define	GPIO_PORTB	(*(volatile unsigned char *)0x25)
#define	GPIO_PINC	(*(volatile unsigned char *)0x26)
#define GPIO_DDRC	(*(volatile unsigned char *)0x27)
#define	GPIO_PORTC	(*(volatile unsigned char *)0x28)
#define	GPIO_PIND	(*(volatile unsigned char *)0x29)
#define GPIO_DDRD	(*(volatile unsigned char *)0x2A)
#define	GPIO_PORTD	(*(volatile unsigned char *)0x2B)

#define	GPIO_DDB0	0
#define	GPIO_DDB1	1
#define	GPIO_DDB2	2
#define	GPIO_DDB3	3
#define	GPIO_DDB4	4
#define	GPIO_DDB5	5
#define	GPIO_DDB6	6
#define	GPIO_DDB7	7

#define	GPIO_PORTB0	0
#define	GPIO_PORTB1	1
#define	GPIO_PORTB2	2
#define	GPIO_PORTB3	3
#define	GPIO_PORTB4	4
#define	GPIO_PORTB5	5
#define	GPIO_PORTB6	6
#define	GPIO_PORTB7	7

void delay_ms(unsigned int n)
{
    volatile unsigned int i;

    while(n--)
        /* delay 1ms */
        for(i = 0; i < 500; i++);
}

int main()
{
    GPIO_DDRB |= (1 << GPIO_DDB5);

    while(1) {
        GPIO_PORTB |= (1 << GPIO_PORTB5);
        delay_ms(10000);        
        GPIO_PORTB &= ~(1 << GPIO_PORTB5);
        delay_ms(10000);        
    }

	return 0;
}

