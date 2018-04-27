/* initialize.c - nulluser, sysinit */

/* Handle system initialization and become the null process */

#include <xinu.h>
#include <string.h>

#define VERSION "0.1"

extern	void	_start(void);	/* start of Xinu code */
extern	void	*_end;		/* end of Xinu code */

// MODIFICADO-TEMPORALMENTE (linea agregada)
extern unsigned char _bss_end;

/* Function prototypes */

extern	void main(void);	/* main is the first process created	*/
extern	void xdone(void);	/* system "shutdown" procedure		*/
static	void sysinit(void);	/* initializes system structures	*/

/* Declarations of major kernel variables */

struct	procent	proctab[NPROC];	/* Process table			*/
struct	sentry	semtab[NSEM];	/* Semaphore table			*/
struct	memblk	memlist;	/* List of free memory blocks		*/

/* Active system status */

int32	prcount;		/* Total number of live processes	*/
pid32	currpid;		/* ID of currently executing process	*/

/* Memory bounds set by start.S */

void	*minheap;		/* start of heap			*/
void	*maxheap;		/* highest valid memory address		*/

/*------------------------------------------------------------------------
 * nulluser - initialize the system and become the null process
 *
 * Note: execution begins here after the C run-time environment has been
 * established.  Interrupts are initially DISABLED, and must eventually
 * be enabled explicitly.  The code turns itself into the null process
 * after initialization.  Because it must always remain ready to execute,
 * the null process cannot execute code that might cause it to be
 * suspended, wait for a semaphore, put to sleep, or exit.  In
 * particular, the code must not perform I/O except for polled versions
 * such as kprintf.
 *------------------------------------------------------------------------
 */

void	nulluser(void)
{
	kprintf("\n\r%s\n\n\r", VERSION);

	sysinit();

	/* Output Xinu memory layout */

	kprintf("0X%08X bytes physical memory.\r\n",
	// MODIFICADO-TEMPORALMENTE	(uint32)maxheap - (uint32)addressp2k(0));
		(uint32)maxheap - (uint32)minheap);
	kprintf("           [0x%08X to 0x%08X]\r\n",
	// MODIFICADO-TEMPORALMENTE	(uint32)addressp2k(0), (uint32)maxheap - 1);
		(uint32)minheap, (uint32)maxheap - 1);
	// MODIFICADO-TEMPORALMENTE kprintf("%10d bytes reserved system area.\r\n",
	// MODIFICADO-TEMPORALMENTE 	(uint32)_start - (uint32)addressp2k(0));
	// MODIFICADO-TEMPORALMENTE kprintf("           [0x%08X to 0x%08X]\r\n",
	// MODIFICADO-TEMPORALMENTE 	(uint32)addressp2k(0), (uint32)_start - 1);
	// MODIFICADO-TEMPORALMENTE kprintf("%10d bytes Xinu code.\r\n",
	// MODIFICADO-TEMPORALMENTE 	(uint32)&_end - (uint32)_start);
	// MODIFICADO-TEMPORALMENTE kprintf("           [0x%08X to 0x%08X]\r\n",
	// MODIFICADO-TEMPORALMENTE 	(uint32)_start, (uint32)&_end - 1);
	// MODIFICADO-TEMPORALMENTE kprintf("%10d bytes stack space.\r\n",
	// MODIFICADO-TEMPORALMENTE 	(uint32)minheap - (uint32)&_end);
	// MODIFICADO-TEMPORALMENTE kprintf("           [0x%08X to 0x%08X]\r\n",
	// MODIFICADO-TEMPORALMENTE 	(uint32)&_end, (uint32)minheap - 1);
	kprintf("0X%08X bytes heap space.\r\n",
		(uint32)maxheap - (uint32)minheap);
	kprintf("           [0x%08X to 0x%08X]\r\n\r\n",
		(uint32)minheap, (uint32)maxheap - 1);

	main();
	/* Enable interrupts */

	// MODIFICADO-TEMPORALMENTE enable();

	/* Create a process to execute function main() */

	// MODIFICADO-TEMPORALMENTE resume(create
        // MODIFICADO-TEMPORALMENTE   ((void *)main, INITSTK, INITPRIO, "Main process", 0, NULL));

	/* Become the Null process (i.e., guarantee that the CPU has	*/
	/*  something to run when no other process is ready to execute)	*/

	while (TRUE) {
		;		/* do nothing */
	}
}

/*------------------------------------------------------------------------
 *
 * sysinit - intialize all Xinu data structures and devices
 *
 *------------------------------------------------------------------------
 */

static	void	sysinit(void)
{
	int32	i;
	struct	procent	*prptr;		/* ptr to process table entry	*/
	struct	sentry	*semptr;	/* prr to semaphore table entry	*/
	struct	memblk	*memptr;	/* ptr to memory block		*/

	/* Initialize system variables */

	/* Count the Null process as the first process in the system */

	prcount = 1;

	/* Scheduling is not currently blocked */

	// MODIFICADO-TEMPORALMENTE Defer.ndefers = 0;

	/* Initialize the free memory list */

	// MODIFICADO-TEMPORALMENTE maxheap = (void *)addressp2k(MAXADDR);
	maxheap = (void *)0x00700; // MODIFICADO-TEMPORALMENTE
	// MODIFICADO-TEMPORALMENTE minheap = (void *)0x00400; // MODIFICADO-TEMPORALMENTE
	minheap = (void *)&_bss_end; // MODIFICADO-TEMPORALMENTE

	memlist.mnext = (struct memblk *)minheap;

	/* Overlay memblk structure on free memory and set fields */

	memptr = (struct memblk *)minheap;
	memptr->mnext = NULL;
	memptr->mlength = memlist.mlength = (uint32)(maxheap - minheap);

	/* Initialize process table entries free */

	for (i = 0; i < NPROC; i++) {
		prptr = &proctab[i];
		prptr->prstate = PR_FREE;
		prptr->prname[0] = NULLCH;
		prptr->prstkbase = NULL;
		prptr->prprio = 0;
	}

	/* Initialize the Null process entry */

	prptr = &proctab[NULLPROC];
	prptr->prstate = PR_CURR;
	prptr->prprio = 0;
	strncpy(prptr->prname, "prnull", 7);
	prptr->prstkbase = minheap;
	prptr->prstklen = NULLSTK;
	prptr->prstkptr = 0;
	currpid = NULLPROC;

	/* Initialize semaphores */

	// MODIFICADO-TEMPORALMENTE for (i = 0; i < NSEM; i++) {
	// MODIFICADO-TEMPORALMENTE 	semptr = &semtab[i];
	// MODIFICADO-TEMPORALMENTE 	semptr->sstate = S_FREE;
	// MODIFICADO-TEMPORALMENTE 	semptr->scount = 0;
	// MODIFICADO-TEMPORALMENTE 	semptr->squeue = newqueue();
	// MODIFICADO-TEMPORALMENTE }

	/* Initialize buffer pools */

	// MODIFICADO-TEMPORALMENTE bufinit();

	/* Create a ready list for processes */

	// MODIFICADO-TEMPORALMENTE readylist = newqueue();

	/* Initialize real time clock */

	// MODIFICADO-TEMPORALMENTE clkinit();

	/* Initialize non-volative RAM storage */

	// MODIFICADO-TEMPORALMENTE nvramInit();

	/* Initialize devices */

	// MODIFICADO-TEMPORALMENTE for (i = 0; i < NDEVS; i++) {
	// MODIFICADO-TEMPORALMENTE 	init(i);
	// MODIFICADO-TEMPORALMENTE }
	return;
}
