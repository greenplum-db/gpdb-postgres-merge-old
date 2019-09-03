/*-------------------------------------------------------------------------
 *
 * tablesample.c
 *		  Support functions for TABLESAMPLE feature
 *
 * Portions Copyright (c) 1996-2016, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *		  src/backend/access/tablesample/tablesample.c
 *
 * -------------------------------------------------------------------------
 */

#include "postgres.h"

#include "access/tsmapi.h"

<<<<<<< HEAD
#include "catalog/pg_tablesample_method.h"
#include "miscadmin.h"
#include "pgstat.h"
#include "storage/bufmgr.h"
#include "storage/predicate.h"
#include "utils/rel.h"
#include "utils/tqual.h"


static bool SampleTupleVisible(HeapTuple tuple, OffsetNumber tupoffset, HeapScanDesc scan);


/*
 * Initialize the TABLESAMPLE Descriptor and the TABLESAMPLE Method.
 */
TableSampleDesc *
tablesample_init(SampleScanState *scanstate, TableSampleClause *tablesample)
{
	FunctionCallInfoData fcinfo;
	int			i;
	List	   *args = tablesample->args;
	ListCell   *arg;
	ExprContext *econtext = scanstate->ss.ps.ps_ExprContext;
	TableSampleDesc *tsdesc = (TableSampleDesc *) palloc0(sizeof(TableSampleDesc));

	/* Load functions */
	fmgr_info(tablesample->tsminit, &(tsdesc->tsminit));
	fmgr_info(tablesample->tsmnextblock, &(tsdesc->tsmnextblock));
	fmgr_info(tablesample->tsmnexttuple, &(tsdesc->tsmnexttuple));
	if (OidIsValid(tablesample->tsmexaminetuple))
		fmgr_info(tablesample->tsmexaminetuple, &(tsdesc->tsmexaminetuple));
	else
		tsdesc->tsmexaminetuple.fn_oid = InvalidOid;
	fmgr_info(tablesample->tsmreset, &(tsdesc->tsmreset));
	fmgr_info(tablesample->tsmend, &(tsdesc->tsmend));

	InitFunctionCallInfoData(fcinfo, &tsdesc->tsminit,
							 list_length(args) + 2,
							 InvalidOid, NULL, NULL);

	tsdesc->tupDesc = scanstate->ss.ss_ScanTupleSlot->tts_tupleDescriptor;
	tsdesc->heapScan = scanstate->ss_currentScanDesc_heap;

	/* First argument for init function is always TableSampleDesc */
	fcinfo.arg[0] = PointerGetDatum(tsdesc);
	fcinfo.argnull[0] = false;

	/*
	 * Second arg for init function is always REPEATABLE.
	 *
	 * If tablesample->repeatable is NULL then REPEATABLE clause was not
	 * specified, and we insert a random value as default.
	 *
	 * When specified, the expression cannot evaluate to NULL.
	 */
	if (tablesample->repeatable)
	{
		ExprState  *argstate = ExecInitExpr((Expr *) tablesample->repeatable,
											(PlanState *) scanstate);

		fcinfo.arg[1] = ExecEvalExpr(argstate, econtext,
									 &fcinfo.argnull[1], NULL);
		if (fcinfo.argnull[1])
			ereport(ERROR,
					(errcode(ERRCODE_NULL_VALUE_NOT_ALLOWED),
				errmsg("REPEATABLE clause must be NOT NULL numeric value")));
	}
	else
	{
		fcinfo.arg[1] = UInt32GetDatum(random());
		fcinfo.argnull[1] = false;
	}

	/* Rest of the arguments come from user. */
	i = 2;
	foreach(arg, args)
	{
		Expr	   *argexpr = (Expr *) lfirst(arg);
		ExprState  *argstate = ExecInitExpr(argexpr, (PlanState *) scanstate);

		fcinfo.arg[i] = ExecEvalExpr(argstate, econtext,
									 &fcinfo.argnull[i], NULL);
		i++;
	}
	Assert(i == fcinfo.nargs);

	(void) FunctionCallInvoke(&fcinfo);

	return tsdesc;
}

/*
 * Get next tuple from TABLESAMPLE Method.
 */
HeapTuple
tablesample_getnext(TableSampleDesc *desc)
{
	HeapScanDesc scan = desc->heapScan;
	HeapTuple	tuple = &(scan->rs_ctup);
	bool		pagemode = scan->rs_pageatatime;
	BlockNumber blockno;
	Page		page;
	bool		page_all_visible;
	ItemId		itemid;
	OffsetNumber tupoffset,
				maxoffset;

	if (!scan->rs_inited)
	{
		/*
		 * return null immediately if relation is empty
		 */
		if (scan->rs_nblocks == 0)
		{
			Assert(!BufferIsValid(scan->rs_cbuf));
			tuple->t_data = NULL;
			return NULL;
		}
		blockno = DatumGetInt32(FunctionCall1(&desc->tsmnextblock,
											  PointerGetDatum(desc)));
		if (!BlockNumberIsValid(blockno))
		{
			tuple->t_data = NULL;
			return NULL;
		}

		heapgetpage(scan, blockno);
		scan->rs_inited = true;
	}
	else
	{
		/* continue from previously returned page/tuple */
		blockno = scan->rs_cblock;		/* current page */
	}

	/*
	 * When pagemode is disabled, the scan will do visibility checks for each
	 * tuple it finds so the buffer needs to be locked.
	 */
	if (!pagemode)
		LockBuffer(scan->rs_cbuf, BUFFER_LOCK_SHARE);

	page = (Page) BufferGetPage(scan->rs_cbuf);
	page_all_visible = PageIsAllVisible(page);
	maxoffset = PageGetMaxOffsetNumber(page);

	for (;;)
	{
		CHECK_FOR_INTERRUPTS();

		tupoffset = DatumGetUInt16(FunctionCall3(&desc->tsmnexttuple,
												 PointerGetDatum(desc),
												 UInt32GetDatum(blockno),
												 UInt16GetDatum(maxoffset)));

		if (OffsetNumberIsValid(tupoffset))
		{
			bool		visible;
			bool		found;

			/* Skip invalid tuple pointers. */
			itemid = PageGetItemId(page, tupoffset);
			if (!ItemIdIsNormal(itemid))
				continue;

			tuple->t_data = (HeapTupleHeader) PageGetItem((Page) page, itemid);
			tuple->t_len = ItemIdGetLength(itemid);
			ItemPointerSet(&(tuple->t_self), blockno, tupoffset);

			if (page_all_visible)
				visible = true;
			else
				visible = SampleTupleVisible(tuple, tupoffset, scan);

			/*
			 * Let the sampling method examine the actual tuple and decide if
			 * we should return it.
			 *
			 * Note that we let it examine even invisible tuples for
			 * statistical purposes, but not return them since user should
			 * never see invisible tuples.
			 */
			if (OidIsValid(desc->tsmexaminetuple.fn_oid))
			{
				found = DatumGetBool(FunctionCall4(&desc->tsmexaminetuple,
												   PointerGetDatum(desc),
												   UInt32GetDatum(blockno),
												   PointerGetDatum(tuple),
												   BoolGetDatum(visible)));
				/* Should not happen if sampling method is well written. */
				if (found && !visible)
					elog(ERROR, "Sampling method wanted to return invisible tuple");
			}
			else
				found = visible;

			/* Found visible tuple, return it. */
			if (found)
			{
				if (!pagemode)
					LockBuffer(scan->rs_cbuf, BUFFER_LOCK_UNLOCK);
				break;
			}
			else
			{
				/* Try next tuple from same page. */
				continue;
			}
		}


		if (!pagemode)
			LockBuffer(scan->rs_cbuf, BUFFER_LOCK_UNLOCK);

		blockno = DatumGetInt32(FunctionCall1(&desc->tsmnextblock,
											  PointerGetDatum(desc)));

		/*
		 * Report our new scan position for synchronization purposes. We don't
		 * do that when moving backwards, however. That would just mess up any
		 * other forward-moving scanners.
		 *
		 * Note: we do this before checking for end of scan so that the final
		 * state of the position hint is back at the start of the rel.  That's
		 * not strictly necessary, but otherwise when you run the same query
		 * multiple times the starting position would shift a little bit
		 * backwards on every invocation, which is confusing. We don't
		 * guarantee any specific ordering in general, though.
		 */
		if (scan->rs_syncscan)
			ss_report_location(scan->rs_rd, BlockNumberIsValid(blockno) ?
							   blockno : scan->rs_startblock);

		/*
		 * Reached end of scan.
		 */
		if (!BlockNumberIsValid(blockno))
		{
			if (BufferIsValid(scan->rs_cbuf))
				ReleaseBuffer(scan->rs_cbuf);
			scan->rs_cbuf = InvalidBuffer;
			scan->rs_cblock = InvalidBlockNumber;
			tuple->t_data = NULL;
			scan->rs_inited = false;
			return NULL;
		}

		heapgetpage(scan, blockno);

		if (!pagemode)
			LockBuffer(scan->rs_cbuf, BUFFER_LOCK_SHARE);

		page = (Page) BufferGetPage(scan->rs_cbuf);
		page_all_visible = PageIsAllVisible(page);
		maxoffset = PageGetMaxOffsetNumber(page);
	}

	pgstat_count_heap_getnext(scan->rs_rd);

	return &(scan->rs_ctup);
}

/*
 * Reset the sampling to starting state
 */
void
tablesample_reset(TableSampleDesc *desc)
{
	(void) FunctionCall1(&desc->tsmreset, PointerGetDatum(desc));
}

/*
 * Signal the sampling method that the scan has finished.
 */
void
tablesample_end(TableSampleDesc *desc)
{
	(void) FunctionCall1(&desc->tsmend, PointerGetDatum(desc));
}
=======
>>>>>>> b5bce6c1ec6061c8a4f730d927e162db7e2ce365

/*
 * GetTsmRoutine --- get a TsmRoutine struct by invoking the handler.
 *
 * This is a convenience routine that's just meant to check for errors.
 */
TsmRoutine *
GetTsmRoutine(Oid tsmhandler)
{
	Datum		datum;
	TsmRoutine *routine;

<<<<<<< HEAD
		bool		visible = HeapTupleSatisfiesVisibility(scan->rs_rd, tuple, snapshot, buffer);
=======
	datum = OidFunctionCall1(tsmhandler, PointerGetDatum(NULL));
	routine = (TsmRoutine *) DatumGetPointer(datum);
>>>>>>> b5bce6c1ec6061c8a4f730d927e162db7e2ce365

	if (routine == NULL || !IsA(routine, TsmRoutine))
		elog(ERROR, "tablesample handler function %u did not return a TsmRoutine struct",
			 tsmhandler);

	return routine;
}
