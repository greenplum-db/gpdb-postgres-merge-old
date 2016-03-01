/*-----------------------------------------------------------------------
 *
 * PostgreSQL locale utilities
 *
<<<<<<< HEAD
 * $PostgreSQL: pgsql/src/include/utils/pg_locale.h,v 1.27 2009/01/01 17:24:02 momjian Exp $
 *
 * Copyright (c) 2002-2009, PostgreSQL Global Development Group
=======
 * $PostgreSQL: pgsql/src/include/utils/pg_locale.h,v 1.24 2008/01/01 19:45:59 momjian Exp $
 *
 * Copyright (c) 2002-2008, PostgreSQL Global Development Group
>>>>>>> 632e7b6353a99dd139b999efce4cb78db9a1e588
 *
 *-----------------------------------------------------------------------
 */

#ifndef _PG_LOCALE_
#define _PG_LOCALE_

#include <locale.h>

#include "utils/guc.h"


/* GUC settings */
extern char *locale_messages;
extern char *locale_monetary;
extern char *locale_numeric;
extern char *locale_time;
extern char *locale_collate;

/* lc_time localization cache */
extern char *localized_abbrev_days[];
extern char *localized_full_days[];
extern char *localized_abbrev_months[];
extern char *localized_full_months[];


extern const char *locale_messages_assign(const char *value,
					   bool doit, GucSource source);
extern const char *locale_monetary_assign(const char *value,
					   bool doit, GucSource source);
extern const char *locale_numeric_assign(const char *value,
					  bool doit, GucSource source);
extern const char *locale_time_assign(const char *value,
				   bool doit, GucSource source);

extern bool check_locale(int category, const char *locale);
extern bool check_locale_encoding(const char *locale, int user_enc);
extern char *pg_perm_setlocale(int category, const char *locale);

extern bool lc_collate_is_c(void);
extern bool lc_ctype_is_c(void);
extern void lc_guess_strxfrm_scaling_factor(int *scaleFactorOut, int *constantFactorOut);

/*
 * Return the POSIX lconv struct (contains number/money formatting
 * information) with locale information for all categories.
 */
extern struct lconv *PGLC_localeconv(void);

extern void cache_locale_time(void);

#endif   /* _PG_LOCALE_ */
