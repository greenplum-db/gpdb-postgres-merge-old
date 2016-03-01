<<<<<<< HEAD
/* $PostgreSQL: pgsql/src/include/port/linux.h,v 1.42.2.1 2007/07/02 20:12:00 tgl Exp $ */
=======
/* $PostgreSQL: pgsql/src/include/port/linux.h,v 1.44 2007/11/15 21:14:44 momjian Exp $ */
>>>>>>> 632e7b6353a99dd139b999efce4cb78db9a1e588

/*
 * As of July 2007, all known versions of the Linux kernel will sometimes
 * return EIDRM for a shmctl() operation when EINVAL is correct (it happens
 * when the low-order 15 bits of the supplied shm ID match the slot number
<<<<<<< HEAD
 * assigned to a newer shmem segment).  We deal with this by assuming that
=======
 * assigned to a newer shmem segment).	We deal with this by assuming that
>>>>>>> 632e7b6353a99dd139b999efce4cb78db9a1e588
 * EIDRM means EINVAL in PGSharedMemoryIsInUse().  This is reasonably safe
 * since in fact Linux has no excuse for ever returning EIDRM; it doesn't
 * track removed segments in a way that would allow distinguishing them from
 * private ones.  But someday that code might get upgraded, and we'd have
 * to have a kernel version test here.
 */
#define HAVE_LINUX_EIDRM_BUG
<<<<<<< HEAD
=======

/*
 * Set the default wal_sync_method to fdatasync.  With recent Linux versions,
 * xlogdefs.h's normal rules will prefer open_datasync, which (a) doesn't
 * perform better and (b) causes outright failures on ext4 data=journal
 * filesystems, because those don't support O_DIRECT.
 */
#define PLATFORM_DEFAULT_SYNC_METHOD_STR "fdatasync"
#define PLATFORM_DEFAULT_SYNC_METHOD	SYNC_METHOD_FDATASYNC
#define PLATFORM_DEFAULT_SYNC_FLAGBIT	0
>>>>>>> 632e7b6353a99dd139b999efce4cb78db9a1e588
