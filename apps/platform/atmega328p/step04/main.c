#include <xinu.h>

/* Test de getmem, freemem y strncpy
 * El programa utiliza punteros y arreglos de diferentes maneras
 * para verificar el port de esas funciones.
 */

char msg_2[80];
char *msg_3;

int main()
{
	int i;
        char mensaje[] ="hola mundo! \r\n";
	char *msg;

	kprintf("Programa para verificar getmem, freemem y strncpy.\r\n");

        msg = getmem(16);
        kprintf("    Direccion asignada por getmem a msg:[0x%08X]\r\n", (uint32)msg);

        char * msg_2 = (char *)getmem(16);
        kprintf("    Direccion asignada port getmem a msg_2:[0x%08X]\r\n", (uint32)msg_2);

        strncpy(msg_2, mensaje, 13);

        kprintf("Copiamos 'mensaje' (hola mundo) a msg_2\r\n", msg);
        kprintf("    Mensaje original en memoria:%s\r\n", mensaje);
        kprintf("    Copia del mensaje en memoria:%s\r\n", msg_2);
        kprintf("    Direccion original del mensaje en memoria:0X%08X\r\n", (uint32)mensaje);
        kprintf("    Direccion del mensaje copia en memoria:0X%08X\r\n", (uint32)msg_2);

        kprintf("Tomamos varios bloques.\r\n");
	for (i=0;i<20;i++) {
                msg_2 = (char *)getmem(16);
        	kprintf("getmem(16)=0X%08X \r\n", (uint32)msg_2);
        };

	for (i=0;i<10;i++) {
        	kprintf("freemem(addr, 16): 0X%08X \r\n", (uint32)msg_2);
                freemem(msg_2, 16);
		msg_2 = msg_2 - 32;
        };
	for (i=0;i<20;i++) {
                msg_2 = (char *)getmem(8);
        	kprintf("getmem(8)=0X%08X \r\n", (uint32)msg_2);
        };
	/*
	 * Solicitamos y usamos memoria hasta superar la capacidad de la RAM
	 * RAM en atmega328p: [0x100-0x800]
	 */
/*
        while (msg_2<0x1000) {
                msg_2 = (char *)getmem(16);
                kprintf("Direccion de msg_2:0X%08X - ", (uint32)msg_2);
                strncpy(msg_2, mensaje, 14);
                kprintf("Mensaje en msg_2:%s", msg_2);
        }
*/

	return 0;
}

