<<<<<<< HEAD
# $PostgreSQL: pgsql/src/interfaces/libpq/nls.mk,v 1.25 2010/05/13 15:56:42 petere Exp $
CATALOG_NAME	:= libpq
AVAIL_LANGUAGES	:= cs de es fr it ja ko pt_BR ru sv ta tr zh_CN
=======
# $PostgreSQL: pgsql/src/interfaces/libpq/nls.mk,v 1.24 2009/10/20 18:23:26 petere Exp $
CATALOG_NAME	:= libpq
AVAIL_LANGUAGES	:= cs de es fr it ja ko pt_BR ru sv ta tr
>>>>>>> 78a09145e0
GETTEXT_FILES	:= fe-auth.c fe-connect.c fe-exec.c fe-lobj.c fe-misc.c fe-protocol2.c fe-protocol3.c fe-secure.c
GETTEXT_TRIGGERS:= libpq_gettext pqInternalNotice:2
