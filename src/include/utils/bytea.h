/*-------------------------------------------------------------------------
 *
 * bytea.h
 *	  Declarations for BYTEA data type support.
 *
 *
<<<<<<< HEAD
 * Portions Copyright (c) 1996-2011, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * src/include/utils/bytea.h
=======
 * Portions Copyright (c) 1996-2009, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $PostgreSQL: pgsql/src/include/utils/bytea.h,v 1.1 2009/08/04 16:08:36 tgl Exp $
>>>>>>> 78a09145e0
 *
 *-------------------------------------------------------------------------
 */
#ifndef BYTEA_H
#define BYTEA_H

#include "fmgr.h"


typedef enum
{
	BYTEA_OUTPUT_ESCAPE,
	BYTEA_OUTPUT_HEX
<<<<<<< HEAD
}	ByteaOutputType;
=======
} ByteaOutputType;
>>>>>>> 78a09145e0

extern int	bytea_output;		/* ByteaOutputType, but int for GUC enum */

/* functions are in utils/adt/varlena.c */
extern Datum byteain(PG_FUNCTION_ARGS);
extern Datum byteaout(PG_FUNCTION_ARGS);
extern Datum bytearecv(PG_FUNCTION_ARGS);
extern Datum byteasend(PG_FUNCTION_ARGS);
extern Datum byteaoctetlen(PG_FUNCTION_ARGS);
extern Datum byteaGetByte(PG_FUNCTION_ARGS);
extern Datum byteaGetBit(PG_FUNCTION_ARGS);
extern Datum byteaSetByte(PG_FUNCTION_ARGS);
extern Datum byteaSetBit(PG_FUNCTION_ARGS);
extern Datum byteaeq(PG_FUNCTION_ARGS);
extern Datum byteane(PG_FUNCTION_ARGS);
extern Datum bytealt(PG_FUNCTION_ARGS);
extern Datum byteale(PG_FUNCTION_ARGS);
extern Datum byteagt(PG_FUNCTION_ARGS);
extern Datum byteage(PG_FUNCTION_ARGS);
extern Datum byteacmp(PG_FUNCTION_ARGS);
extern Datum byteacat(PG_FUNCTION_ARGS);
extern Datum byteapos(PG_FUNCTION_ARGS);
extern Datum bytea_substr(PG_FUNCTION_ARGS);
extern Datum bytea_substr_no_len(PG_FUNCTION_ARGS);
<<<<<<< HEAD
extern Datum byteaoverlay(PG_FUNCTION_ARGS);
extern Datum byteaoverlay_no_len(PG_FUNCTION_ARGS);
=======
>>>>>>> 78a09145e0

#endif   /* BYTEA_H */
