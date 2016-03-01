/*-------------------------------------------------------------------------
 *
 * explain.h
 *	  prototypes for explain.c
 *
<<<<<<< HEAD
 * Portions Copyright (c) 1996-2009, PostgreSQL Global Development Group
=======
 * Portions Copyright (c) 1996-2008, PostgreSQL Global Development Group
>>>>>>> 632e7b6353a99dd139b999efce4cb78db9a1e588
 * Portions Copyright (c) 1994-5, Regents of the University of California
 *
 * $PostgreSQL: pgsql/src/include/commands/explain.h,v 1.35 2008/01/01 19:45:57 momjian Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef EXPLAIN_H
#define EXPLAIN_H

#include "executor/executor.h"

/* Hook for plugins to get control in ExplainOneQuery() */
typedef void (*ExplainOneQuery_hook_type) (Query *query,
													   ExplainStmt *stmt,
													 const char *queryString,
													   ParamListInfo params,
													 TupOutputState *tstate);
extern PGDLLIMPORT ExplainOneQuery_hook_type ExplainOneQuery_hook;

<<<<<<< HEAD
extern void ExplainQuery(ExplainStmt *stmt, const char *queryString,
						 ParamListInfo params, DestReceiver *dest);
=======
/* Hook for plugins to get control in explain_get_index_name() */
typedef const char *(*explain_get_index_name_hook_type) (Oid indexId);
extern PGDLLIMPORT explain_get_index_name_hook_type explain_get_index_name_hook;


extern void ExplainQuery(ExplainStmt *stmt, const char *queryString,
			 ParamListInfo params, DestReceiver *dest);
>>>>>>> 632e7b6353a99dd139b999efce4cb78db9a1e588

extern TupleDesc ExplainResultDesc(ExplainStmt *stmt);

extern void ExplainOneUtility(Node *utilityStmt, ExplainStmt *stmt,
				  const char *queryString,
				  ParamListInfo params,
				  TupOutputState *tstate);

<<<<<<< HEAD
extern void ExplainOnePlan(PlannedStmt *plannedstmt, ExplainStmt *stmt,
		   const char *queryString, ParamListInfo params,
		   TupOutputState *tstate);
=======
extern void ExplainOnePlan(PlannedStmt *plannedstmt, ParamListInfo params,
			   ExplainStmt *stmt, TupOutputState *tstate);
>>>>>>> 632e7b6353a99dd139b999efce4cb78db9a1e588

#endif   /* EXPLAIN_H */
