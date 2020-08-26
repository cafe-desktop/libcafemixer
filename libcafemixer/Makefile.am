NULL =

lib_LTLIBRARIES = libcafemixer.la

AM_CPPFLAGS =							\
	-I$(top_srcdir)						\
	-I$(top_srcdir)/libcafemixer				\
	-DG_LOG_DOMAIN=\"libcafemixer\"				\
	-DLIBCAFEMIXER_BACKEND_DIR=\"$(libdir)/libcafemixer\"	\
	$(GLIB_CFLAGS)						\
	$(NULL)

libcafemixer_includedir = $(includedir)/cafe-mixer/libcafemixer

libcafemixer_include_HEADERS =                                  \
	cafemixer.h                                             \
	cafemixer-app-info.h                                    \
	cafemixer-context.h                                     \
	cafemixer-device.h                                      \
	cafemixer-device-switch.h                               \
	cafemixer-enums.h                                       \
	cafemixer-enum-types.h                                  \
	cafemixer-stored-control.h                              \
	cafemixer-stream.h                                      \
	cafemixer-stream-control.h                              \
	cafemixer-stream-switch.h                               \
	cafemixer-stream-toggle.h                               \
	cafemixer-switch.h                                      \
	cafemixer-switch-option.h                               \
	cafemixer-types.h                                       \
	cafemixer-version.h

libcafemixer_la_CFLAGS =					\
	$(WARN_CFLAGS)						\
	$(NULL)

libcafemixer_la_SOURCES =                                       \
	cafemixer.c                                             \
	cafemixer-private.h                                     \
	cafemixer-app-info.c                                    \
	cafemixer-app-info-private.h                            \
	cafemixer-backend.c                                     \
	cafemixer-backend.h                                     \
	cafemixer-backend-module.c                              \
	cafemixer-backend-module.h                              \
	cafemixer-context.c                                     \
	cafemixer-device.c                                      \
	cafemixer-device-switch.c                               \
	cafemixer-enum-types.c                                  \
	cafemixer-stored-control.c                              \
	cafemixer-stream.c                                      \
	cafemixer-stream-private.h                              \
	cafemixer-stream-control.c                              \
	cafemixer-stream-control-private.h                      \
	cafemixer-stream-switch.c                               \
	cafemixer-stream-toggle.c                               \
	cafemixer-switch.c                                      \
	cafemixer-switch-private.h                              \
	cafemixer-switch-option.c                               \
	cafemixer-switch-option-private.h

libcafemixer_la_LIBADD = $(GLIB_LIBS)

libcafemixer_la_LDFLAGS =                                       \
	-version-info $(LT_VERSION)                             \
	-no-undefined                                           \
	-export-dynamic

-include $(top_srcdir)/git.mk
