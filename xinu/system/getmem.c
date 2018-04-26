/* getmem.c - getmem */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  getmem  -  Allocate heap storage, returning lowest word address
 *------------------------------------------------------------------------
 */
char  	*getmem(
	  uint32	nbytes		/* size of memory requested	*/
	)
{
	intmask	mask;			/* saved interrupt mask		*/
	struct	memblk	*prev, *curr, *leftover;

	// MODIFICADO-TEMPORALMENTE mask = disable();
	if (nbytes == 0) {
		// MODIFICADO-TEMPORALMENTE restore(mask);
		return (char *)SYSERR;
	}

	nbytes = (uint32) roundmb(nbytes);	/* use memblk multiples	*/

	prev = &memlist;
	curr = memlist.mnext;
	while (curr != NULL) {			/* search free list	*/

       // MODIFICADO-TEMPORALMENTE kprintf("   curr mlenght [0x%08X]\r\n\r\n",
              // MODIFICADO-TEMPORALMENTE   (uint32)curr->mlength);
       // MODIFICADO-TEMPORALMENTE kprintf("   curr address [0x%08X]\r\n\r\n", (uint32)curr);

		if (curr->mlength == nbytes) {	/* block is exact match	*/
			prev->mnext = curr->mnext;
			memlist.mlength -= nbytes;
			// MODIFICADO-TEMPORALMENTE restore(mask);
			return (char *)(curr);

		} else if (curr->mlength > nbytes) { /* split big block	*/
			leftover = (struct memblk *)((uint32) curr +
					nbytes);
			prev->mnext = leftover;
			leftover->mnext = curr->mnext;
			leftover->mlength = curr->mlength - nbytes;
			memlist.mlength -= nbytes;
			// MODIFICADO-TEMPORALMENTE restore(mask);
			return (char *)(curr);
		} else {			/* move to next block	*/
			prev = curr;
			curr = curr->mnext;
		}
	}
	// MODIFICADO-TEMPORALMENTE restore(mask);
	return (char *)SYSERR;
}
