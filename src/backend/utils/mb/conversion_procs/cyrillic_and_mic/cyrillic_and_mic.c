/*-------------------------------------------------------------------------
 *
 *	  Cyrillic and MULE_INTERNAL
 *
 * Portions Copyright (c) 1996-2016, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * IDENTIFICATION
 *	  src/backend/utils/mb/conversion_procs/cyrillic_and_mic/cyrillic_and_mic.c
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"
#include "fmgr.h"
#include "mb/pg_wchar.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(koi8r_to_mic);
PG_FUNCTION_INFO_V1(mic_to_koi8r);
PG_FUNCTION_INFO_V1(iso_to_mic);
PG_FUNCTION_INFO_V1(mic_to_iso);
PG_FUNCTION_INFO_V1(win1251_to_mic);
PG_FUNCTION_INFO_V1(mic_to_win1251);
PG_FUNCTION_INFO_V1(win866_to_mic);
PG_FUNCTION_INFO_V1(mic_to_win866);
PG_FUNCTION_INFO_V1(koi8r_to_win1251);
PG_FUNCTION_INFO_V1(win1251_to_koi8r);
PG_FUNCTION_INFO_V1(koi8r_to_win866);
PG_FUNCTION_INFO_V1(win866_to_koi8r);
PG_FUNCTION_INFO_V1(win866_to_win1251);
PG_FUNCTION_INFO_V1(win1251_to_win866);
PG_FUNCTION_INFO_V1(iso_to_koi8r);
PG_FUNCTION_INFO_V1(koi8r_to_iso);
PG_FUNCTION_INFO_V1(iso_to_win1251);
PG_FUNCTION_INFO_V1(win1251_to_iso);
PG_FUNCTION_INFO_V1(iso_to_win866);
PG_FUNCTION_INFO_V1(win866_to_iso);

/* ----------
 * conv_proc(
 *		INTEGER,	-- source encoding id
 *		INTEGER,	-- destination encoding id
 *		CSTRING,	-- source string (null terminated C string)
 *		CSTRING,	-- destination string (null terminated C string)
 *		INTEGER		-- source string length
 * ) returns VOID;
 * ----------
 */

/*
 * Cyrillic support
 * currently supported Cyrillic encodings:
 *
 * KOI8-R (this is also the charset for the mule internal code for Cyrillic)
 * ISO-8859-5
 * Microsoft's CP1251 (windows-1251)
 * Alternativny Variant (MS-DOS CP866)
 */

/* ISO-8859-5 to KOI8-R */
static const unsigned char iso2koi[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xB3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xE1, 0xE2, 0xF7, 0xE7, 0xE4, 0xE5, 0xF6, 0xFA,
	0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF, 0xF0,
	0xF2, 0xF3, 0xF4, 0xF5, 0xE6, 0xE8, 0xE3, 0xFE,
	0xFB, 0xFD, 0xFF, 0xF9, 0xF8, 0xFC, 0xE0, 0xF1,
	0xC1, 0xC2, 0xD7, 0xC7, 0xC4, 0xC5, 0xD6, 0xDA,
	0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0,
	0xD2, 0xD3, 0xD4, 0xD5, 0xC6, 0xC8, 0xC3, 0xDE,
	0xDB, 0xDD, 0xDF, 0xD9, 0xD8, 0xDC, 0xC0, 0xD1,
	0x00, 0xA3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* KOI8-R to ISO-8859-5 */
static const unsigned char koi2iso[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xF1, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xA1, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xEE, 0xD0, 0xD1, 0xE6, 0xD4, 0xD5, 0xE4, 0xD3,
	0xE5, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE,
	0xDF, 0xEF, 0xE0, 0xE1, 0xE2, 0xE3, 0xD6, 0xD2,
	0xEC, 0xEB, 0xD7, 0xE8, 0xED, 0xE9, 0xE7, 0xEA,
	0xCE, 0xB0, 0xB1, 0xC6, 0xB4, 0xB5, 0xC4, 0xB3,
	0xC5, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE,
	0xBF, 0xCF, 0xC0, 0xC1, 0xC2, 0xC3, 0xB6, 0xB2,
	0xCC, 0xCB, 0xB7, 0xC8, 0xCD, 0xC9, 0xC7, 0xCA
};

/* WIN1251 to KOI8-R */
static const unsigned char win12512koi[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xBD, 0x00, 0x00,
	0xB3, 0x00, 0xB4, 0x00, 0x00, 0x00, 0x00, 0xB7,
	0x00, 0x00, 0xB6, 0xA6, 0xAD, 0x00, 0x00, 0x00,
	0xA3, 0x00, 0xA4, 0x00, 0x00, 0x00, 0x00, 0xA7,
	0xE1, 0xE2, 0xF7, 0xE7, 0xE4, 0xE5, 0xF6, 0xFA,
	0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF, 0xF0,
	0xF2, 0xF3, 0xF4, 0xF5, 0xE6, 0xE8, 0xE3, 0xFE,
	0xFB, 0xFD, 0xFF, 0xF9, 0xF8, 0xFC, 0xE0, 0xF1,
	0xC1, 0xC2, 0xD7, 0xC7, 0xC4, 0xC5, 0xD6, 0xDA,
	0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0,
	0xD2, 0xD3, 0xD4, 0xD5, 0xC6, 0xC8, 0xC3, 0xDE,
	0xDB, 0xDD, 0xDF, 0xD9, 0xD8, 0xDC, 0xC0, 0xD1
};

/* KOI8-R to WIN1251 */
static const unsigned char koi2win1251[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xB8, 0xBA, 0x00, 0xB3, 0xBF,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xB4, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xA8, 0xAA, 0x00, 0xB2, 0xAF,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xA5, 0x00, 0x00,
	0xFE, 0xE0, 0xE1, 0xF6, 0xE4, 0xE5, 0xF4, 0xE3,
	0xF5, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE,
	0xEF, 0xFF, 0xF0, 0xF1, 0xF2, 0xF3, 0xE6, 0xE2,
	0xFC, 0xFB, 0xE7, 0xF8, 0xFD, 0xF9, 0xF7, 0xFA,
	0xDE, 0xC0, 0xC1, 0xD6, 0xC4, 0xC5, 0xD4, 0xC3,
	0xD5, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE,
	0xCF, 0xDF, 0xD0, 0xD1, 0xD2, 0xD3, 0xC6, 0xC2,
	0xDC, 0xDB, 0xC7, 0xD8, 0xDD, 0xD9, 0xD7, 0xDA
};

/* WIN866 to KOI8-R */
static const unsigned char win8662koi[] = {
	0xE1, 0xE2, 0xF7, 0xE7, 0xE4, 0xE5, 0xF6, 0xFA,
	0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF, 0xF0,
	0xF2, 0xF3, 0xF4, 0xF5, 0xE6, 0xE8, 0xE3, 0xFE,
	0xFB, 0xFD, 0xFF, 0xF9, 0xF8, 0xFC, 0xE0, 0xF1,
	0xC1, 0xC2, 0xD7, 0xC7, 0xC4, 0xC5, 0xD6, 0xDA,
	0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xBD, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xD2, 0xD3, 0xD4, 0xD5, 0xC6, 0xC8, 0xC3, 0xDE,
	0xDB, 0xDD, 0xDF, 0xD9, 0xD8, 0xDC, 0xC0, 0xD1,
	0xB3, 0xA3, 0xB4, 0xA4, 0xB7, 0xA7, 0x00, 0x00,
	0xB6, 0xA6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* KOI8-R to WIN866 */
static const unsigned char koi2win866[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xF1, 0xF3, 0x00, 0xF9, 0xF5,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xAD, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xF0, 0xF2, 0x00, 0xF8, 0xF4,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xBD, 0x00, 0x00,
	0xEE, 0xA0, 0xA1, 0xE6, 0xA4, 0xA5, 0xE4, 0xA3,
	0xE5, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE,
	0xAF, 0xEF, 0xE0, 0xE1, 0xE2, 0xE3, 0xA6, 0xA2,
	0xEC, 0xEB, 0xA7, 0xE8, 0xED, 0xE9, 0xE7, 0xEA,
	0x9E, 0x80, 0x81, 0x96, 0x84, 0x85, 0x94, 0x83,
	0x95, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E,
	0x8F, 0x9F, 0x90, 0x91, 0x92, 0x93, 0x86, 0x82,
	0x9C, 0x9B, 0x87, 0x98, 0x9D, 0x99, 0x97, 0x9A
};

/* WIN866 to WIN1251 */
static const unsigned char win8662win1251[] = {
	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
	0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
	0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
	0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
	0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7,
	0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xA5, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,
	0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,
	0xA8, 0xB8, 0xAA, 0xBA, 0xAF, 0xBF, 0x00, 0x00,
	0xB2, 0xB3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* WIN1251 to WIN866 */
static const unsigned char win12512win866[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xBD, 0x00, 0x00,
	0xF0, 0x00, 0xF2, 0x00, 0x00, 0x00, 0x00, 0xF4,
	0x00, 0x00, 0xF8, 0xF9, 0xAD, 0x00, 0x00, 0x00,
	0xF1, 0x00, 0xF3, 0x00, 0x00, 0x00, 0x00, 0xF5,
	0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
	0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
	0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
	0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
	0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
	0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
	0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7,
	0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF
};

/* ISO-8859-5 to WIN1251 */
static const unsigned char iso2win1251[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xA8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
	0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
	0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
	0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
	0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7,
	0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
	0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,
	0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,
	0x00, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* WIN1251 to ISO-8859-5 */
static const unsigned char win12512iso[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xF1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,
	0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
	0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
	0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
	0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
	0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7,
	0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF
};

/* ISO-8859-5 to WIN866 */
static const unsigned char iso2win866[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
	0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
	0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
	0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
	0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
	0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
	0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7,
	0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
	0x00, 0xF1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* WIN866 to ISO-8859-5 */
static const unsigned char win8662iso[] = {
	0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,
	0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
	0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
	0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
	0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7,
	0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
	0xA1, 0xF1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


Datum
koi8r_to_mic(PG_FUNCTION_ARGS)
{
	unsigned char *src = (unsigned char *) PG_GETARG_CSTRING(2);
	unsigned char *dest = (unsigned char *) PG_GETARG_CSTRING(3);
	int			len = PG_GETARG_INT32(4);

	CHECK_ENCODING_CONVERSION_ARGS(PG_KOI8R, PG_MULE_INTERNAL);

	latin2mic(src, dest, len, LC_KOI8_R, PG_KOI8R);

	PG_RETURN_VOID();
}

Datum
mic_to_koi8r(PG_FUNCTION_ARGS)
{
	unsigned char *src = (unsigned char *) PG_GETARG_CSTRING(2);
	unsigned char *dest = (unsigned char *) PG_GETARG_CSTRING(3);
	int			len = PG_GETARG_INT32(4);

	CHECK_ENCODING_CONVERSION_ARGS(PG_MULE_INTERNAL, PG_KOI8R);

	mic2latin(src, dest, len, LC_KOI8_R, PG_KOI8R);

	PG_RETURN_VOID();
}

Datum
iso_to_mic(PG_FUNCTION_ARGS)
{
	unsigned char *src = (unsigned char *) PG_GETARG_CSTRING(2);
	unsigned char *dest = (unsigned char *) PG_GETARG_CSTRING(3);
	int			len = PG_GETARG_INT32(4);

	CHECK_ENCODING_CONVERSION_ARGS(PG_ISO_8859_5, PG_MULE_INTERNAL);

	latin2mic_with_table(src, dest, len, LC_KOI8_R, PG_ISO_8859_5, iso2koi);

	PG_RETURN_VOID();
}

Datum
mic_to_iso(PG_FUNCTION_ARGS)
{
	unsigned char *src = (unsigned char *) PG_GETARG_CSTRING(2);
	unsigned char *dest = (unsigned char *) PG_GETARG_CSTRING(3);
	int			len = PG_GETARG_INT32(4);

	CHECK_ENCODING_CONVERSION_ARGS(PG_MULE_INTERNAL, PG_ISO_8859_5);

	mic2latin_with_table(src, dest, len, LC_KOI8_R, PG_ISO_8859_5, koi2iso);

	PG_RETURN_VOID();
}

Datum
win1251_to_mic(PG_FUNCTION_ARGS)
{
	unsigned char *src = (unsigned char *) PG_GETARG_CSTRING(2);
	unsigned char *dest = (unsigned char *) PG_GETARG_CSTRING(3);
	int			len = PG_GETARG_INT32(4);

	CHECK_ENCODING_CONVERSION_ARGS(PG_WIN1251, PG_MULE_INTERNAL);

	latin2mic_with_table(src, dest, len, LC_KOI8_R, PG_WIN1251, win12512koi);

	PG_RETURN_VOID();
}

Datum
mic_to_win1251(PG_FUNCTION_ARGS)
{
	unsigned char *src = (unsigned char *) PG_GETARG_CSTRING(2);
	unsigned char *dest = (unsigned char *) PG_GETARG_CSTRING(3);
	int			len = PG_GETARG_INT32(4);

	CHECK_ENCODING_CONVERSION_ARGS(PG_MULE_INTERNAL, PG_WIN1251);

	mic2latin_with_table(src, dest, len, LC_KOI8_R, PG_WIN1251, koi2win1251);

	PG_RETURN_VOID();
}

Datum
win866_to_mic(PG_FUNCTION_ARGS)
{
	unsigned char *src = (unsigned char *) PG_GETARG_CSTRING(2);
	unsigned char *dest = (unsigned char *) PG_GETARG_CSTRING(3);
	int			len = PG_GETARG_INT32(4);

	CHECK_ENCODING_CONVERSION_ARGS(PG_WIN866, PG_MULE_INTERNAL);

	latin2mic_with_table(src, dest, len, LC_KOI8_R, PG_WIN866, win8662koi);

	PG_RETURN_VOID();
}

Datum
mic_to_win866(PG_FUNCTION_ARGS)
{
	unsigned char *src = (unsigned char *) PG_GETARG_CSTRING(2);
	unsigned char *dest = (unsigned char *) PG_GETARG_CSTRING(3);
	int			len = PG_GETARG_INT32(4);

	CHECK_ENCODING_CONVERSION_ARGS(PG_MULE_INTERNAL, PG_WIN866);

	mic2latin_with_table(src, dest, len, LC_KOI8_R, PG_WIN866, koi2win866);

	PG_RETURN_VOID();
}

Datum
koi8r_to_win1251(PG_FUNCTION_ARGS)
{
	unsigned char *src = (unsigned char *) PG_GETARG_CSTRING(2);
	unsigned char *dest = (unsigned char *) PG_GETARG_CSTRING(3);
	int			len = PG_GETARG_INT32(4);

	CHECK_ENCODING_CONVERSION_ARGS(PG_KOI8R, PG_WIN1251);

	local2local(src, dest, len, PG_KOI8R, PG_WIN1251, koi2win1251);

	PG_RETURN_VOID();
}

Datum
win1251_to_koi8r(PG_FUNCTION_ARGS)
{
	unsigned char *src = (unsigned char *) PG_GETARG_CSTRING(2);
	unsigned char *dest = (unsigned char *) PG_GETARG_CSTRING(3);
	int			len = PG_GETARG_INT32(4);

	CHECK_ENCODING_CONVERSION_ARGS(PG_WIN1251, PG_KOI8R);

	local2local(src, dest, len, PG_WIN1251, PG_KOI8R, win12512koi);

	PG_RETURN_VOID();
}

Datum
koi8r_to_win866(PG_FUNCTION_ARGS)
{
	unsigned char *src = (unsigned char *) PG_GETARG_CSTRING(2);
	unsigned char *dest = (unsigned char *) PG_GETARG_CSTRING(3);
	int			len = PG_GETARG_INT32(4);

	CHECK_ENCODING_CONVERSION_ARGS(PG_KOI8R, PG_WIN866);

	local2local(src, dest, len, PG_KOI8R, PG_WIN866, koi2win866);

	PG_RETURN_VOID();
}

Datum
win866_to_koi8r(PG_FUNCTION_ARGS)
{
	unsigned char *src = (unsigned char *) PG_GETARG_CSTRING(2);
	unsigned char *dest = (unsigned char *) PG_GETARG_CSTRING(3);
	int			len = PG_GETARG_INT32(4);

	CHECK_ENCODING_CONVERSION_ARGS(PG_WIN866, PG_KOI8R);

	local2local(src, dest, len, PG_WIN866, PG_KOI8R, win8662koi);

	PG_RETURN_VOID();
}

Datum
win866_to_win1251(PG_FUNCTION_ARGS)
{
	unsigned char *src = (unsigned char *) PG_GETARG_CSTRING(2);
	unsigned char *dest = (unsigned char *) PG_GETARG_CSTRING(3);
	int			len = PG_GETARG_INT32(4);

	CHECK_ENCODING_CONVERSION_ARGS(PG_WIN866, PG_WIN1251);

	local2local(src, dest, len, PG_WIN866, PG_WIN1251, win8662win1251);

	PG_RETURN_VOID();
}

Datum
win1251_to_win866(PG_FUNCTION_ARGS)
{
	unsigned char *src = (unsigned char *) PG_GETARG_CSTRING(2);
	unsigned char *dest = (unsigned char *) PG_GETARG_CSTRING(3);
	int			len = PG_GETARG_INT32(4);

	CHECK_ENCODING_CONVERSION_ARGS(PG_WIN1251, PG_WIN866);

	local2local(src, dest, len, PG_WIN1251, PG_WIN866, win12512win866);

	PG_RETURN_VOID();
}

Datum
iso_to_koi8r(PG_FUNCTION_ARGS)
{
	unsigned char *src = (unsigned char *) PG_GETARG_CSTRING(2);
	unsigned char *dest = (unsigned char *) PG_GETARG_CSTRING(3);
	int			len = PG_GETARG_INT32(4);

	CHECK_ENCODING_CONVERSION_ARGS(PG_ISO_8859_5, PG_KOI8R);

	local2local(src, dest, len, PG_ISO_8859_5, PG_KOI8R, iso2koi);

	PG_RETURN_VOID();
}

Datum
koi8r_to_iso(PG_FUNCTION_ARGS)
{
	unsigned char *src = (unsigned char *) PG_GETARG_CSTRING(2);
	unsigned char *dest = (unsigned char *) PG_GETARG_CSTRING(3);
	int			len = PG_GETARG_INT32(4);

	CHECK_ENCODING_CONVERSION_ARGS(PG_KOI8R, PG_ISO_8859_5);

	local2local(src, dest, len, PG_KOI8R, PG_ISO_8859_5, koi2iso);

	PG_RETURN_VOID();
}

Datum
iso_to_win1251(PG_FUNCTION_ARGS)
{
	unsigned char *src = (unsigned char *) PG_GETARG_CSTRING(2);
	unsigned char *dest = (unsigned char *) PG_GETARG_CSTRING(3);
	int			len = PG_GETARG_INT32(4);

	CHECK_ENCODING_CONVERSION_ARGS(PG_ISO_8859_5, PG_WIN1251);

	local2local(src, dest, len, PG_ISO_8859_5, PG_WIN1251, iso2win1251);

	PG_RETURN_VOID();
}

Datum
win1251_to_iso(PG_FUNCTION_ARGS)
{
	unsigned char *src = (unsigned char *) PG_GETARG_CSTRING(2);
	unsigned char *dest = (unsigned char *) PG_GETARG_CSTRING(3);
	int			len = PG_GETARG_INT32(4);

	CHECK_ENCODING_CONVERSION_ARGS(PG_WIN1251, PG_ISO_8859_5);

	local2local(src, dest, len, PG_WIN1251, PG_ISO_8859_5, win12512iso);

	PG_RETURN_VOID();
}

Datum
iso_to_win866(PG_FUNCTION_ARGS)
{
	unsigned char *src = (unsigned char *) PG_GETARG_CSTRING(2);
	unsigned char *dest = (unsigned char *) PG_GETARG_CSTRING(3);
	int			len = PG_GETARG_INT32(4);

	CHECK_ENCODING_CONVERSION_ARGS(PG_ISO_8859_5, PG_WIN866);

	local2local(src, dest, len, PG_ISO_8859_5, PG_WIN866, iso2win866);

	PG_RETURN_VOID();
}

Datum
win866_to_iso(PG_FUNCTION_ARGS)
{
	unsigned char *src = (unsigned char *) PG_GETARG_CSTRING(2);
	unsigned char *dest = (unsigned char *) PG_GETARG_CSTRING(3);
	int			len = PG_GETARG_INT32(4);

	CHECK_ENCODING_CONVERSION_ARGS(PG_WIN866, PG_ISO_8859_5);

	local2local(src, dest, len, PG_WIN866, PG_ISO_8859_5, win8662iso);

	PG_RETURN_VOID();
}
