/*-------------------------------------------------------------------------
 *
 * smgrtype.c
 *	  storage manager type
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
 *	  $PostgreSQL: pgsql/src/backend/storage/smgr/smgrtype.c,v 1.30 2009/01/01 17:23:48 momjian Exp $
=======
 *	  $PostgreSQL: pgsql/src/backend/storage/smgr/smgrtype.c,v 1.29 2008/01/01 19:45:52 momjian Exp $
>>>>>>> 632e7b6353a99dd139b999efce4cb78db9a1e588
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "storage/smgr.h"


typedef struct smgrid
{
	const char *smgr_name;
} smgrid;

/*
 *	StorageManager[] -- List of defined storage managers.
 */
static const smgrid StorageManager[] = {
	{"magnetic disk"}
};

static const int NStorageManagers = lengthof(StorageManager);


Datum
smgrin(PG_FUNCTION_ARGS)
{
	char	   *s = PG_GETARG_CSTRING(0);
	int16		i;

	for (i = 0; i < NStorageManagers; i++)
	{
		if (strcmp(s, StorageManager[i].smgr_name) == 0)
			PG_RETURN_INT16(i);
	}
	elog(ERROR, "unrecognized storage manager name \"%s\"", s);
	PG_RETURN_INT16(0);
}

Datum
smgrout(PG_FUNCTION_ARGS)
{
	int16		i = PG_GETARG_INT16(0);
	char	   *s;

	if (i >= NStorageManagers || i < 0)
		elog(ERROR, "invalid storage manager id: %d", i);

	s = pstrdup(StorageManager[i].smgr_name);
	PG_RETURN_CSTRING(s);
}

Datum
smgreq(PG_FUNCTION_ARGS)
{
	int16		a = PG_GETARG_INT16(0);
	int16		b = PG_GETARG_INT16(1);

	PG_RETURN_BOOL(a == b);
}

Datum
smgrne(PG_FUNCTION_ARGS)
{
	int16		a = PG_GETARG_INT16(0);
	int16		b = PG_GETARG_INT16(1);

	PG_RETURN_BOOL(a != b);
}
