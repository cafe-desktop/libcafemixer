/*
 * Copyright (C) 2014 Michal Ratajsky <michal.ratajsky@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the licence, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CAFEMIXER_STREAM_H
#define CAFEMIXER_STREAM_H

#include <glib.h>
#include <glib-object.h>

#include <libcafemixer/cafemixer-enums.h>
#include <libcafemixer/cafemixer-types.h>

G_BEGIN_DECLS

#define CAFE_MIXER_TYPE_STREAM                  \
        (cafe_mixer_stream_get_type ())
#define CAFE_MIXER_STREAM(o)                    \
        (G_TYPE_CHECK_INSTANCE_CAST ((o), CAFE_MIXER_TYPE_STREAM, CafeMixerStream))
#define CAFE_MIXER_IS_STREAM(o)                 \
        (G_TYPE_CHECK_INSTANCE_TYPE ((o), CAFE_MIXER_TYPE_STREAM))
#define CAFE_MIXER_STREAM_CLASS(k)              \
        (G_TYPE_CHECK_CLASS_CAST ((k), CAFE_MIXER_TYPE_STREAM, CafeMixerStreamClass))
#define CAFE_MIXER_IS_STREAM_CLASS(k)           \
        (G_TYPE_CHECK_CLASS_TYPE ((k), CAFE_MIXER_TYPE_STREAM))
#define CAFE_MIXER_STREAM_GET_CLASS(o)          \
        (G_TYPE_INSTANCE_GET_CLASS ((o), CAFE_MIXER_TYPE_STREAM, CafeMixerStreamClass))

typedef struct _CafeMixerStreamClass    CafeMixerStreamClass;
typedef struct _CafeMixerStreamPrivate  CafeMixerStreamPrivate;

/**
 * CafeMixerStream:
 *
 * The #CafeMixerStream structure contains only private data and should only
 * be accessed using the provided API.
 */
struct _CafeMixerStream
{
    GObject object;

    /*< private >*/
    CafeMixerStreamPrivate *priv;
};

/**
 * CafeMixerStreamClass:
 * @parent_class: The parent class.
 *
 * The class structure for #CafeMixerStream.
 */
struct _CafeMixerStreamClass
{
    GObjectClass parent_class;

    /*< private >*/
    CafeMixerStreamControl *(*get_control)   (CafeMixerStream *stream,
                                              const gchar     *name);
    CafeMixerStreamSwitch  *(*get_switch)    (CafeMixerStream *stream,
                                              const gchar     *name);

    const GList            *(*list_controls) (CafeMixerStream *stream);
    const GList            *(*list_switches) (CafeMixerStream *stream);

    /* Signals */
    void (*control_added)   (CafeMixerStream *stream,
                             const gchar     *name);
    void (*control_removed) (CafeMixerStream *stream,
                             const gchar     *name);

    void (*switch_added)    (CafeMixerStream *stream,
                             const gchar     *name);
    void (*switch_removed)  (CafeMixerStream *stream,
                             const gchar     *name);
};

GType                   cafe_mixer_stream_get_type            (void) G_GNUC_CONST;

const gchar *           cafe_mixer_stream_get_name            (CafeMixerStream *stream);
const gchar *           cafe_mixer_stream_get_label           (CafeMixerStream *stream);

CafeMixerDirection      cafe_mixer_stream_get_direction       (CafeMixerStream *stream);

CafeMixerDevice *       cafe_mixer_stream_get_device          (CafeMixerStream *stream);
CafeMixerStreamControl *cafe_mixer_stream_get_control         (CafeMixerStream *stream,
                                                               const gchar     *name);
CafeMixerStreamSwitch * cafe_mixer_stream_get_switch          (CafeMixerStream *stream,
                                                               const gchar     *name);

CafeMixerStreamControl *cafe_mixer_stream_get_default_control (CafeMixerStream *stream);

const GList *           cafe_mixer_stream_list_controls       (CafeMixerStream *stream);
const GList *           cafe_mixer_stream_list_switches       (CafeMixerStream *stream);

G_END_DECLS

#endif /* CAFEMIXER_STREAM_H */
