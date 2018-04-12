/* kprintf.c -  kprintf */
#include <stdarg.h>
#include <stddef.h>

extern	void	_doprnt(char *, va_list ap, int (*)(int));
extern int kputc(unsigned char c);

/*------------------------------------------------------------------------
 * kprintf  -  use polled I/O to print formatted output on the console
 *------------------------------------------------------------------------
 */
int kprintf(char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	_doprnt(fmt, ap, (int (*)(int))kputc);
	va_end(ap);

	return OK;
}
