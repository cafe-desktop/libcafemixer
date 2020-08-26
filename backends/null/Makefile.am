NULL =

backenddir = $(libdir)/libcafemixer

backend_LTLIBRARIES = libcafemixer-null.la

AM_CPPFLAGS =							\
	-I$(top_srcdir)						\
	-DG_LOG_DOMAIN=\"libcafemixer-null\"			\
	$(GLIB_CFLAGS)						\
	$(NULL)

libcafemixer_null_la_CFLAGS =					\
	$(WARN_CFLAGS)						\
	$(NULL)

libcafemixer_null_la_SOURCES =                                  \
	null-backend.c                                          \
	null-backend.h

libcafemixer_null_la_LIBADD = $(GLIB_LIBS)

libcafemixer_null_la_LDFLAGS =                                  \
	-avoid-version                                          \
	-no-undefined                                           \
	-export-dynamic                                         \
	-module

-include $(top_srcdir)/git.mk
