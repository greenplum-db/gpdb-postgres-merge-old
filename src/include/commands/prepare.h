/*-------------------------------------------------------------------------
 *
 * prepare.h
 *	  PREPARE, EXECUTE and DEALLOCATE commands, and prepared-stmt storage
 *
 *
 * Copyright (c) 2002-2008, PostgreSQL Global Development Group
 *
 * $PostgreSQL: pgsql/src/include/commands/prepare.h,v 1.29 2008/01/01 19:45:57 momjian Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef PREPARE_H
#define PREPARE_H

<<<<<<< HEAD
#include "executor/execdesc.h"
=======
#include "executor/executor.h"
#include "utils/plancache.h"
>>>>>>> 632e7b6353a99dd139b999efce4cb78db9a1e588
#include "utils/timestamp.h"

struct TupOutputState;                  /* #include "executor/executor.h" */

/*
 * The data structure representing a prepared statement.  This is now just
 * a thin veneer over a plancache entry --- the main addition is that of
 * a name.
 *
<<<<<<< HEAD
 * A prepared statement might be fully planned, or only parsed-and-rewritten.
 * If fully planned, stmt_list contains PlannedStmts and/or utility statements;
 * if not, it contains Query nodes.
 *
 * Note: all subsidiary storage lives in the context denoted by the context
 * field.  However, the string referenced by commandTag is not subsidiary
 * storage; it is assumed to be a compile-time-constant string.  As with
 * portals, commandTag shall be NULL if and only if the original query string
 * (before rewriting) was an empty string.
=======
 * Note: all subsidiary storage lives in the referenced plancache entry.
>>>>>>> 632e7b6353a99dd139b999efce4cb78db9a1e588
 */
typedef struct
{
	/* dynahash.c requires key to be first field */
	char		stmt_name[NAMEDATALEN];
<<<<<<< HEAD
	char	   *query_string;	/* text of query, or NULL */
	NodeTag		sourceTag;		/* GPDB: Original statement NodeTag */
	const char *commandTag;		/* command tag (a constant!), or NULL */
	List	   *query_list;		/* list of queries, rewritten, in case of replan */
	List	   *argtype_list;	/* list of parameter type OIDs */
=======
	CachedPlanSource *plansource;		/* the actual cached plan */
	bool		from_sql;		/* prepared via SQL, not FE/BE protocol? */
>>>>>>> 632e7b6353a99dd139b999efce4cb78db9a1e588
	TimestampTz prepare_time;	/* the time when the stmt was prepared */
} PreparedStatement;


/* Utility statements PREPARE, EXECUTE, DEALLOCATE, EXPLAIN EXECUTE */
extern void PrepareQuery(PrepareStmt *stmt, const char *queryString);
<<<<<<< HEAD
extern void ExecuteQuery(ExecuteStmt *stmt, const char *queryString, 
             ParamListInfo params,
			 DestReceiver *dest, char *completionTag);
extern void DeallocateQuery(DeallocateStmt *stmt);
extern void ExplainExecuteQuery(ExecuteStmt *execstmt, ExplainStmt *stmt,
					const char * queryString,
					ParamListInfo params,
					struct TupOutputState *tstate);
=======
extern void ExecuteQuery(ExecuteStmt *stmt, const char *queryString,
			 ParamListInfo params,
			 DestReceiver *dest, char *completionTag);
extern void DeallocateQuery(DeallocateStmt *stmt);
extern void ExplainExecuteQuery(ExecuteStmt *execstmt, ExplainStmt *stmt,
					const char *queryString,
					ParamListInfo params, TupOutputState *tstate);
>>>>>>> 632e7b6353a99dd139b999efce4cb78db9a1e588

/* Low-level access to stored prepared statements */
extern void StorePreparedStatement(const char *stmt_name,
					   Node *raw_parse_tree,
					   const char *query_string,
					   NodeTag	   sourceTag, /* GPDB */
					   const char *commandTag,
<<<<<<< HEAD
					   List *query_list,
					   List *argtype_list,
=======
					   Oid *param_types,
					   int num_params,
					   int cursor_options,
					   List *stmt_list,
>>>>>>> 632e7b6353a99dd139b999efce4cb78db9a1e588
					   bool from_sql);
extern PreparedStatement *FetchPreparedStatement(const char *stmt_name,
					   bool throwError);
extern void DropPreparedStatement(const char *stmt_name, bool showError);
extern TupleDesc FetchPreparedStatementResultDesc(PreparedStatement *stmt);
extern List *FetchPreparedStatementTargetList(PreparedStatement *stmt);

void		DropAllPreparedStatements(void);

#endif   /* PREPARE_H */
