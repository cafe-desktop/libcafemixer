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
        (G_TYPE_CHECK_INSTANCE_CAST ((o), CAFE_MIXER_TYPE_STREAM, MateMixerStream))
#define CAFE_MIXER_IS_STREAM(o)                 \
        (G_TYPE_CHECK_INSTANCE_TYPE ((o), CAFE_MIXER_TYPE_STREAM))
#define CAFE_MIXER_STREAM_CLASS(k)              \
        (G_TYPE_CHECK_CLASS_CAST ((k), CAFE_MIXER_TYPE_STREAM, MateMixerStreamClass))
#define CAFE_MIXER_IS_STREAM_CLASS(k)           \
        (G_TYPE_CHECK_CLASS_TYPE ((k), CAFE_MIXER_TYPE_STREAM))
#define CAFE_MIXER_STREAM_GET_CLASS(o)          \
        (G_TYPE_INSTANCE_GET_CLASS ((o), CAFE_MIXER_TYPE_STREAM, MateMixerStreamClass))

typedef struct _MateMixerStreamClass    MateMixerStreamClass;
typedef struct _MateMixerStreamPrivate  MateMixerStreamPrivate;

/**
 * MateMixerStream:
 *
 * The #MateMixerStream structure contains only private data and should only
 * be accessed using the provided API.
 */
struct _MateMixerStream
{
    GObject object;

    /*< private >*/
    MateMixerStreamPrivate *priv;
};

/**
 * MateMixerStreamClass:
 * @parent_class: The parent class.
 *
 * The class structure for #MateMixerStream.
 */
struct _MateMixerStreamClass
{
    GObjectClass parent_class;

    /*< private >*/
    MateMixerStreamControl *(*get_control)   (MateMixerStream *stream,
                                              const gchar     *name);
    MateMixerStreamSwitch  *(*get_switch)    (MateMixerStream *stream,
                                              const gchar     *name);

    const GList            *(*list_controls) (MateMixerStream *stream);
    const GList            *(*list_switches) (MateMixerStream *stream);

    /* Signals */
    void (*control_added)   (MateMixerStream *stream,
                             const gchar     *name);
    void (*control_removed) (MateMixerStream *stream,
                             const gchar     *name);

    void (*switch_added)    (MateMixerStream *stream,
                             const gchar     *name);
    void (*switch_removed)  (MateMixerStream *stream,
                             const gchar     *name);
};

GType                   cafe_mixer_stream_get_type            (void) G_GNUC_CONST;

const gchar *           cafe_mixer_stream_get_name            (MateMixerStream *stream);
const gchar *           cafe_mixer_stream_get_label           (MateMixerStream *stream);

MateMixerDirection      cafe_mixer_stream_get_direction       (MateMixerStream *stream);

MateMixerDevice *       cafe_mixer_stream_get_device          (MateMixerStream *stream);
MateMixerStreamControl *cafe_mixer_stream_get_control         (MateMixerStream *stream,
                                                               const gchar     *name);
MateMixerStreamSwitch * cafe_mixer_stream_get_switch          (MateMixerStream *stream,
                                                               const gchar     *name);

MateMixerStreamControl *cafe_mixer_stream_get_default_control (MateMixerStream *stream);

const GList *           cafe_mixer_stream_list_controls       (MateMixerStream *stream);
const GList *           cafe_mixer_stream_list_switches       (MateMixerStream *stream);

G_END_DECLS

#endif /* CAFEMIXER_STREAM_H */
