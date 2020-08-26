NULL =

AM_CPPFLAGS =							\
	-I$(top_srcdir)						\
	$(GLIB_CFLAGS)						\
	$(NULL)

noinst_PROGRAMS = cafemixer-monitor

cafemixer_monitor_SOURCES = monitor.c

cafemixer_monitor_CFLAGS =					\
	$(WARN_CFLAGS)						\
	$(NULL)

cafemixer_monitor_LDADD =                                       \
	$(GLIB_LIBS)                                            \
	$(top_builddir)/libcafemixer/libcafemixer.la

EXTRA_DIST = monitor.c

-include $(top_srcdir)/git.mk
