
#define uint8_t unsigned char

typedef struct
{
        //Estos son los registros del HW USART asociados al perferico UART
    uint8_t status_control_a;    /* ucsr0a USART Control and Status A */

    /* demas registros */
        
        uint8_t status_control_b;   /* ucsr0b USART Control and Status B */
        uint8_t status_control_c;   /* ucsr0c USART Control and Status C */
        uint8_t _reserved;
        uint8_t baud_rate_l;        /* ubrr0l baud rate low */
        uint8_t baud_rate_h;        /* ubrr0h baud rate high */

    uint8_t data_es;    /* udr0 i/o data */

} volatile uart_t;


#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)



/*------------------------------------------------------------------------
 * kputc  -  use polled I/O to write a character to the console
 *------------------------------------------------------------------------
 */
int kputc(unsigned char c)	/* Character to write	*/
{
	uart_t *puerto_serial = (uart_t *) (0xc0);

        puerto_serial->baud_rate_h=(uint8_t) (BAUD_PRESCALE >> 8);

        puerto_serial->baud_rate_l=(uint8_t) BAUD_PRESCALE;

        puerto_serial->status_control_c=(uint8_t) (0x06);
        
        puerto_serial->status_control_b=(uint8_t) (0x18);       



	while(!((puerto_serial->status_control_a & (0x20))==0x20))
	;
        puerto_serial->data_es=c;


	return 0;
}

