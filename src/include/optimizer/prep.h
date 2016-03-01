/*-------------------------------------------------------------------------
 *
 * prep.h
 *	  prototypes for files in optimizer/prep/
 *
 *
<<<<<<< HEAD
 * Portions Copyright (c) 2006-2008, Greenplum inc
 * Portions Copyright (c) 1996-2008, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $PostgreSQL: pgsql/src/include/optimizer/prep.h,v 1.57.2.1 2008/11/11 18:13:54 tgl Exp $
=======
 * Portions Copyright (c) 1996-2008, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $PostgreSQL: pgsql/src/include/optimizer/prep.h,v 1.59.2.1 2008/11/11 18:13:44 tgl Exp $
>>>>>>> 632e7b6353a99dd139b999efce4cb78db9a1e588
 *
 *-------------------------------------------------------------------------
 */
#ifndef PREP_H
#define PREP_H

#include "nodes/plannodes.h"
#include "nodes/relation.h"


/*
 * prototypes for prepjointree.c
 */
//extern Node *pull_up_IN_clauses(PlannerInfo *root, Node *node);
extern Node *pull_up_subqueries(PlannerInfo *root, Node *jtnode,
				   bool below_outer_join, bool append_rel_member);
extern void reduce_outer_joins(PlannerInfo *root);
extern Relids get_relids_in_jointree(Node *jtnode);
extern Relids get_relids_for_join(PlannerInfo *root, int joinrelid);

extern List *init_list_cteplaninfo(int numCtes);

/*
 * prototypes for prepqual.c
 */
extern Expr *canonicalize_qual(Expr *qual);

/*
 * prototypes for preptlist.c
 */
extern List *preprocess_targetlist(PlannerInfo *root, List *tlist);

/*
 * prototypes for prepunion.c
 */
extern Plan *plan_set_operations(PlannerInfo *root, double tuple_fraction,
					List **sortClauses);

extern List *find_all_inheritors(Oid parentrel);

extern void expand_inherited_tables(PlannerInfo *root);

extern Node *adjust_appendrel_attrs(PlannerInfo *root, Node *node, AppendRelInfo *appinfo);

<<<<<<< HEAD
extern Relids *adjust_appendrel_attr_needed(PlannerInfo *root, RelOptInfo *oldrel,
							 AppendRelInfo *appinfo,
							 AttrNumber new_min_attr,
							 AttrNumber new_max_attr);

=======
>>>>>>> 632e7b6353a99dd139b999efce4cb78db9a1e588
#endif   /* PREP_H */
