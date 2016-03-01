/*-------------------------------------------------------------------------
 *
 * be-fsstubs.h
 *
 *
 *
<<<<<<< HEAD
 * Portions Copyright (c) 1996-2010, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $PostgreSQL: pgsql/src/include/libpq/be-fsstubs.h,v 1.35 2010/02/26 02:01:24 momjian Exp $
=======
 * Portions Copyright (c) 1996-2008, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $PostgreSQL: pgsql/src/include/libpq/be-fsstubs.h,v 1.30 2008/01/01 19:45:58 momjian Exp $
>>>>>>> 632e7b6353a99dd139b999efce4cb78db9a1e588
 *
 *-------------------------------------------------------------------------
 */
#ifndef BE_FSSTUBS_H
#define BE_FSSTUBS_H

#include "fmgr.h"

/*
 * LO functions available via pg_proc entries
 */
extern Datum lo_import(PG_FUNCTION_ARGS);
extern Datum lo_import_with_oid(PG_FUNCTION_ARGS);
extern Datum lo_export(PG_FUNCTION_ARGS);

extern Datum lo_creat(PG_FUNCTION_ARGS);
extern Datum lo_create(PG_FUNCTION_ARGS);

extern Datum lo_open(PG_FUNCTION_ARGS);
extern Datum lo_close(PG_FUNCTION_ARGS);

extern Datum loread(PG_FUNCTION_ARGS);
extern Datum lowrite(PG_FUNCTION_ARGS);

extern Datum lo_lseek(PG_FUNCTION_ARGS);
extern Datum lo_tell(PG_FUNCTION_ARGS);
extern Datum lo_unlink(PG_FUNCTION_ARGS);
extern Datum lo_truncate(PG_FUNCTION_ARGS);
<<<<<<< HEAD

/*
 * compatibility option for access control
 */
extern bool lo_compat_privileges;
=======
>>>>>>> 632e7b6353a99dd139b999efce4cb78db9a1e588

/*
 * These are not fmgr-callable, but are available to C code.
 * Probably these should have had the underscore-free names,
 * but too late now...
 */
extern int	lo_read(int fd, char *buf, int len);
extern int	lo_write(int fd, const char *buf, int len);

/*
 * Cleanup LOs at xact commit/abort
 */
extern void AtEOXact_LargeObject(bool isCommit);
extern void AtEOSubXact_LargeObject(bool isCommit, SubTransactionId mySubid,
						SubTransactionId parentSubid);

#endif   /* BE_FSSTUBS_H */
