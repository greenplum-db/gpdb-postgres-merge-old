/*-------------------------------------------------------------------------
 *
 * pg_rewind.h
 *
 *
 * Portions Copyright (c) 1996-2016, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *-------------------------------------------------------------------------
 */
#ifndef PG_REWIND_H
#define PG_REWIND_H

#include "c.h"

#include "datapagemap.h"

#include "access/timeline.h"
#include "storage/block.h"
#include "storage/relfilenode.h"

/* Configuration options */
extern char *datadir_target;
extern char *datadir_source;
extern char *connstr_source;
extern bool debug;
extern bool showprogress;
extern bool dry_run;

<<<<<<< HEAD
extern int32 dbid_target;

extern const char *progname;
=======
/* Target history */
extern TimeLineHistoryEntry *targetHistory;
extern int	targetNentries;
>>>>>>> b5bce6c1ec6061c8a4f730d927e162db7e2ce365

/* in parsexlog.c */
extern void extractPageMap(const char *datadir, XLogRecPtr startpoint,
			   int tliIndex, XLogRecPtr endpoint);
extern void findLastCheckpoint(const char *datadir, XLogRecPtr searchptr,
				   int tliIndex,
				   XLogRecPtr *lastchkptrec, TimeLineID *lastchkpttli,
				   XLogRecPtr *lastchkptredo);
extern XLogRecPtr readOneRecord(const char *datadir, XLogRecPtr ptr,
			  int tliIndex);

/* in timeline.c */
extern TimeLineHistoryEntry *rewind_parseTimeLineHistory(char *buffer,
							TimeLineID targetTLI, int *nentries);

#endif   /* PG_REWIND_H */
