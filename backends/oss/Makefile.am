NULL =

backenddir = $(libdir)/libcafemixer

backend_LTLIBRARIES = libcafemixer-oss.la

AM_CPPFLAGS =							\
	-I$(top_srcdir)						\
	-DG_LOG_DOMAIN=\"libcafemixer-oss\"			\
	$(GLIB_CFLAGS)						\
	$(OSS_CFLAGS)						\
	$(NULL)

libcafemixer_oss_la_CFLAGS =					\
	$(WARN_CFLAGS)						\
	$(NULL)

libcafemixer_oss_la_SOURCES =                                   \
	oss-common.h                                            \
	oss-backend.c                                           \
	oss-backend.h                                           \
	oss-device.c                                            \
	oss-device.h                                            \
	oss-stream.c                                            \
	oss-stream.h                                            \
	oss-stream-control.c                                    \
	oss-stream-control.h                                    \
	oss-switch.c                                            \
	oss-switch.h                                            \
	oss-switch-option.c                                     \
	oss-switch-option.h                                     \
	oss-types.h

libcafemixer_oss_la_LIBADD =                                    \
	$(GLIB_LIBS)                                            \
	$(OSS_LIBS)

libcafemixer_oss_la_LDFLAGS =                                   \
	-avoid-version                                          \
	-no-undefined                                           \
	-export-dynamic                                         \
	-module

-include $(top_srcdir)/git.mk
