/*-------------------------------------------------------------------------
 *
 * linux.h
 *		Port-specific prototypes for Linux
 *
 *
<<<<<<< HEAD
 * Portions Copyright (c) 1996-2009, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $PostgreSQL: pgsql/src/backend/port/dynloader/linux.h,v 1.28 2009/01/01 17:23:46 momjian Exp $
=======
 * Portions Copyright (c) 1996-2008, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $PostgreSQL: pgsql/src/backend/port/dynloader/linux.h,v 1.27 2008/01/01 19:45:51 momjian Exp $
>>>>>>> 632e7b6353a99dd139b999efce4cb78db9a1e588
 *
 *-------------------------------------------------------------------------
 */
#ifndef PORT_PROTOS_H
#define PORT_PROTOS_H

#include "utils/dynamic_loader.h"
#ifdef HAVE_DLOPEN
#include <dlfcn.h>
#endif


#ifdef HAVE_DLOPEN

/*
 * In some older systems, the RTLD_NOW flag isn't defined and the mode
 * argument to dlopen must always be 1.  The RTLD_GLOBAL flag is wanted
 * if available, but it doesn't exist everywhere.
 * If it doesn't exist, set it to 0 so it has no effect.
 */
#ifndef RTLD_NOW
#define RTLD_NOW 1
#endif
#ifndef RTLD_GLOBAL
#define RTLD_GLOBAL 0
#endif

#define pg_dlopen(f)	dlopen((f), RTLD_NOW | RTLD_GLOBAL)
#define pg_dlsym		dlsym
#define pg_dlclose		dlclose
#define pg_dlerror		dlerror
#endif   /* HAVE_DLOPEN */

#endif   /* PORT_PROTOS_H */
