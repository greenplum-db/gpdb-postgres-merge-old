/*-------------------------------------------------------------------------
 *
 * spin.c
 *	   Hardware-independent implementation of spinlocks.
 *
 *
 * For machines that have test-and-set (TAS) instructions, s_lock.h/.c
 * define the spinlock implementation.	This file contains only a stub
 * implementation for spinlocks using PGSemaphores.  Unless semaphores
 * are implemented in a way that doesn't involve a kernel call, this
 * is too slow to be very useful :-(
 *
 *
<<<<<<< HEAD
 * Portions Copyright (c) 1996-2009, PostgreSQL Global Development Group
=======
 * Portions Copyright (c) 1996-2008, PostgreSQL Global Development Group
>>>>>>> 632e7b6353a99dd139b999efce4cb78db9a1e588
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
<<<<<<< HEAD
 *	  $PostgreSQL: pgsql/src/backend/storage/lmgr/spin.c,v 1.24 2009/01/01 17:23:48 momjian Exp $
=======
 *	  $PostgreSQL: pgsql/src/backend/storage/lmgr/spin.c,v 1.23 2008/01/01 19:45:52 momjian Exp $
>>>>>>> 632e7b6353a99dd139b999efce4cb78db9a1e588
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "miscadmin.h"
#include "storage/lwlock.h"
#include "storage/spin.h"


#ifdef HAVE_SPINLOCKS

/*
 * Report number of semaphores needed to support spinlocks.
 */
int
SpinlockSemas(void)
{
	return 0;
}
#else							/* !HAVE_SPINLOCKS */

/*
 * No TAS, so spinlocks are implemented as PGSemaphores.
 */


/*
 * Report number of semaphores needed to support spinlocks.
 */
int
SpinlockSemas(void)
{
	/*
	 * It would be cleaner to distribute this logic into the affected modules,
	 * similar to the way shmem space estimation is handled.
	 *
	 * plus one for each LWLock and one for each buffer header,plus
	 * NUM_ATOMICS_SEMAPHORES for atomic api(without sufficient spinlock
	 * and/or atomics support)
	 */
	return NumLWLocks() + NBuffers + 10 + NUM_ATOMICS_SEMAPHORES;
}

/*
 * s_lock.h hardware-spinlock emulation
 */

void
s_init_lock_sema(volatile slock_t *lock)
{
	PGSemaphoreCreate((PGSemaphore) lock);
}

void
s_unlock_sema(volatile slock_t *lock)
{
	PGSemaphoreUnlock((PGSemaphore) lock);
}

bool
s_lock_free_sema(volatile slock_t *lock)
{
	/* We don't currently use S_LOCK_FREE anyway */
	elog(ERROR, "spin.c does not support S_LOCK_FREE()");
	return false;
}

int
tas_sema(volatile slock_t *lock)
{
	/* Note that TAS macros return 0 if *success* */
	return !PGSemaphoreTryLock((PGSemaphore) lock);
}

#endif   /* !HAVE_SPINLOCKS */
