-- Test AOCO XLogging
CREATE TABLE generate_aoco_xlog_table(a INT, b INT) WITH (APPENDONLY=TRUE, ORIENTATION=COLUMN);

-- Store the location of xlog in a temporary table so that we can
-- use it to request walsender to start streaming from this point
CREATE TEMP TABLE tmp(dbid int, startpoint TEXT);
INSERT INTO tmp SELECT gp_execution_segment(),pg_current_xlog_location() FROM
gp_dist_random('gp_id');

-- Generate some xlog records for AOCO
INSERT INTO generate_aoco_xlog_table VALUES(1, 10), (8, 10), (3, 10);

-- Verify that AO xlog record was received
SELECT gp_segment_id, relname, record_type, segment_filenum, recordlen, file_offset
  FROM test_xlog_ao_wrapper(
    (SELECT array_agg(startpoint) FROM 
       (SELECT startpoint from tmp order by dbid) t
    )
  ) 
WHERE spcNode = (SELECT oid FROM pg_tablespace WHERE spcname = 'pg_default')
AND dbNode = (SELECT oid FROM pg_database WHERE datname = current_database())
ORDER BY gp_segment_id, xrecoff;

-- Store the latest xlog offset
DELETE FROM tmp;
INSERT INTO tmp SELECT gp_execution_segment(),pg_current_xlog_location() 
FROM gp_dist_random('gp_id');

-- Generate a truncate XLOG entry for generate_ao_xlog_table.
BEGIN;
INSERT INTO generate_aoco_xlog_table SELECT i,i FROM generate_series(1,10)i;
ABORT;
VACUUM generate_aoco_xlog_table;

-- Verify that truncate AO xlog record was received
SELECT gp_segment_id, relname, record_type, segment_filenum, recordlen, file_offset
  FROM test_xlog_ao_wrapper(
    (SELECT array_agg(startpoint) FROM 
       (SELECT startpoint from tmp order by dbid) t
    )
  ) 
WHERE spcNode = (SELECT oid FROM pg_tablespace WHERE spcname = 'pg_default')
AND dbNode = (SELECT oid FROM pg_database WHERE datname = current_database())
ORDER BY gp_segment_id, xrecoff;

