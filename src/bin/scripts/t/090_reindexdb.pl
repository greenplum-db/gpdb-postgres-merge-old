use strict;
use warnings;

use PostgresNode;
use TestLib;
use Test::More tests => 18;

program_help_ok('reindexdb');
program_version_ok('reindexdb');
program_options_handling_ok('reindexdb');

my $node = get_new_node('main');
$node->init;
$node->start;

$ENV{PGOPTIONS} = '-c gp_session_role=utility --client-min-messages=WARNING';

$node->issues_sql_like(
	[ 'reindexdb', 'postgres' ],
	qr/statement: REINDEX DATABASE postgres;/,
	'SQL REINDEX run');

$node->safe_psql('postgres',
	'CREATE TABLE test1 (a int); CREATE INDEX test1x ON test1 (a);');
$node->issues_sql_like(
	[ 'reindexdb', '-t', 'test1', 'postgres' ],
	qr/statement: REINDEX TABLE public\.test1;/,
	'reindex specific table');
$node->issues_sql_like(
	[ 'reindexdb', '-i', 'test1x', 'postgres' ],
	qr/statement: REINDEX INDEX public\.test1x;/,
	'reindex specific index');
<<<<<<< HEAD
issues_sql_like(
=======
$node->issues_sql_like(
	[ 'reindexdb', '-S', 'pg_catalog', 'postgres' ],
	qr/statement: REINDEX SCHEMA pg_catalog;/,
	'reindex specific schema');
$node->issues_sql_like(
>>>>>>> b5bce6c1ec6061c8a4f730d927e162db7e2ce365
	[ 'reindexdb', '-s', 'postgres' ],
	qr/statement: REINDEX SYSTEM postgres;/,
	'reindex system tables');
$node->issues_sql_like(
	[ 'reindexdb', '-v', '-t', 'test1', 'postgres' ],
	qr/statement: REINDEX \(VERBOSE\) TABLE public\.test1;/,
	'reindex with verbose output');
