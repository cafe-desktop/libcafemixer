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

#ifndef CAFEMIXER_STREAM_CONTROL_H
#define CAFEMIXER_STREAM_CONTROL_H

#include <math.h>
#include <glib.h>
#include <glib-object.h>

#include <libcafemixer/cafemixer-enums.h>
#include <libcafemixer/cafemixer-types.h>

G_BEGIN_DECLS

#ifdef INFINITY
#  define CAFE_MIXER_INFINITY INFINITY
#else
#  define CAFE_MIXER_INFINITY G_MAXDOUBLE
#endif

#define CAFE_MIXER_TYPE_STREAM_CONTROL          \
        (cafe_mixer_stream_control_get_type ())
#define CAFE_MIXER_STREAM_CONTROL(o)            \
        (G_TYPE_CHECK_INSTANCE_CAST ((o), CAFE_MIXER_TYPE_STREAM_CONTROL, MateMixerStreamControl))
#define CAFE_MIXER_IS_STREAM_CONTROL(o)         \
        (G_TYPE_CHECK_INSTANCE_TYPE ((o), CAFE_MIXER_TYPE_STREAM_CONTROL))
#define CAFE_MIXER_STREAM_CONTROL_CLASS(k)      \
        (G_TYPE_CHECK_CLASS_CAST ((k), CAFE_MIXER_TYPE_STREAM_CONTROL, MateMixerStreamControlClass))
#define CAFE_MIXER_IS_STREAM_CONTROL_CLASS(k)   \
        (G_TYPE_CHECK_CLASS_TYPE ((k), CAFE_MIXER_TYPE_STREAM_CONTROL))
#define CAFE_MIXER_STREAM_CONTROL_GET_CLASS(o)  \
        (G_TYPE_INSTANCE_GET_CLASS ((o), CAFE_MIXER_TYPE_STREAM_CONTROL, MateMixerStreamControlClass))

typedef struct _MateMixerStreamControlClass    MateMixerStreamControlClass;
typedef struct _MateMixerStreamControlPrivate  MateMixerStreamControlPrivate;

/**
 * MateMixerStreamControl:
 *
 * The #MateMixerStreamControl structure contains only private data and should only
 * be accessed using the provided API.
 */
struct _MateMixerStreamControl
{
    GObject object;

    /*< private >*/
    MateMixerStreamControlPrivate *priv;
};

/**
 * MateMixerStreamControlClass:
 * @parent_class: The parent class.
 *
 * The class structure for #MateMixerStreamControl.
 */
struct _MateMixerStreamControlClass
{
    GObjectClass parent_class;

    /*< private >*/
    MateMixerAppInfo *       (*get_app_info)         (MateMixerStreamControl  *control);

    gboolean                 (*set_stream)           (MateMixerStreamControl  *control,
                                                      MateMixerStream         *stream);

    gboolean                 (*set_mute)             (MateMixerStreamControl  *control,
                                                      gboolean                 mute);

    guint                    (*get_num_channels)     (MateMixerStreamControl  *control);

    guint                    (*get_volume)           (MateMixerStreamControl  *control);
    gboolean                 (*set_volume)           (MateMixerStreamControl  *control,
                                                      guint                    volume);

    gdouble                  (*get_decibel)          (MateMixerStreamControl  *control);
    gboolean                 (*set_decibel)          (MateMixerStreamControl  *control,
                                                      gdouble                  decibel);

    gboolean                 (*has_channel_position) (MateMixerStreamControl  *control,
                                                      MateMixerChannelPosition position);
    MateMixerChannelPosition (*get_channel_position) (MateMixerStreamControl  *control,
                                                      guint                    channel);

    guint                    (*get_channel_volume)   (MateMixerStreamControl  *control,
                                                      guint                    channel);
    gboolean                 (*set_channel_volume)   (MateMixerStreamControl  *control,
                                                      guint                    channel,
                                                      guint                    volume);

    gdouble                  (*get_channel_decibel)  (MateMixerStreamControl  *control,
                                                      guint                    channel);
    gboolean                 (*set_channel_decibel)  (MateMixerStreamControl  *control,
                                                      guint                    channel,
                                                      gdouble                  decibel);

    gboolean                 (*set_balance)          (MateMixerStreamControl  *control,
                                                      gfloat                   balance);

    gboolean                 (*set_fade)             (MateMixerStreamControl  *control,
                                                      gfloat                   fade);

    gboolean                 (*get_monitor_enabled)  (MateMixerStreamControl  *control);
    gboolean                 (*set_monitor_enabled)  (MateMixerStreamControl  *control,
                                                      gboolean                 enabled);

    guint                    (*get_min_volume)       (MateMixerStreamControl  *control);
    guint                    (*get_max_volume)       (MateMixerStreamControl  *control);
    guint                    (*get_normal_volume)    (MateMixerStreamControl  *control);
    guint                    (*get_base_volume)      (MateMixerStreamControl  *control);

    /* Signals */
    void (*monitor_value) (MateMixerStreamControl *control, gdouble value);
};

GType                           cafe_mixer_stream_control_get_type             (void) G_GNUC_CONST;

const gchar *                   cafe_mixer_stream_control_get_name             (MateMixerStreamControl  *control);
const gchar *                   cafe_mixer_stream_control_get_label            (MateMixerStreamControl  *control);
MateMixerStreamControlFlags     cafe_mixer_stream_control_get_flags            (MateMixerStreamControl  *control);
MateMixerStreamControlRole      cafe_mixer_stream_control_get_role             (MateMixerStreamControl  *control);
MateMixerStreamControlMediaRole cafe_mixer_stream_control_get_media_role       (MateMixerStreamControl  *control);

MateMixerAppInfo *              cafe_mixer_stream_control_get_app_info         (MateMixerStreamControl  *control);

MateMixerStream *               cafe_mixer_stream_control_get_stream           (MateMixerStreamControl  *control);
gboolean                        cafe_mixer_stream_control_set_stream           (MateMixerStreamControl  *control,
                                                                                MateMixerStream         *stream);

gboolean                        cafe_mixer_stream_control_get_mute             (MateMixerStreamControl  *control);
gboolean                        cafe_mixer_stream_control_set_mute             (MateMixerStreamControl  *control,
                                                                                gboolean                 mute);

guint                           cafe_mixer_stream_control_get_num_channels     (MateMixerStreamControl  *control);

guint                           cafe_mixer_stream_control_get_volume           (MateMixerStreamControl  *control);
gboolean                        cafe_mixer_stream_control_set_volume           (MateMixerStreamControl  *control,
                                                                                guint                    volume);

gdouble                         cafe_mixer_stream_control_get_decibel          (MateMixerStreamControl  *control);
gboolean                        cafe_mixer_stream_control_set_decibel          (MateMixerStreamControl  *control,
                                                                                gdouble                  decibel);

gboolean                        cafe_mixer_stream_control_has_channel_position (MateMixerStreamControl  *control,
                                                                                MateMixerChannelPosition position);
MateMixerChannelPosition        cafe_mixer_stream_control_get_channel_position (MateMixerStreamControl  *control,
                                                                                guint                    channel);

guint                           cafe_mixer_stream_control_get_channel_volume   (MateMixerStreamControl  *control,
                                                                                guint                    channel);
gboolean                        cafe_mixer_stream_control_set_channel_volume   (MateMixerStreamControl  *control,
                                                                                guint                    channel,
                                                                                guint                    volume);

gdouble                         cafe_mixer_stream_control_get_channel_decibel  (MateMixerStreamControl  *control,
                                                                                guint                    channel);
gboolean                        cafe_mixer_stream_control_set_channel_decibel  (MateMixerStreamControl  *control,
                                                                                guint                    channel,
                                                                                gdouble                  decibel);

gfloat                          cafe_mixer_stream_control_get_balance          (MateMixerStreamControl  *control);
gboolean                        cafe_mixer_stream_control_set_balance          (MateMixerStreamControl  *control,
                                                                                gfloat                   balance);

gfloat                          cafe_mixer_stream_control_get_fade             (MateMixerStreamControl  *control);
gboolean                        cafe_mixer_stream_control_set_fade             (MateMixerStreamControl  *control,
                                                                                gfloat                   fade);

gboolean                        cafe_mixer_stream_control_get_monitor_enabled  (MateMixerStreamControl  *control);
gboolean                        cafe_mixer_stream_control_set_monitor_enabled  (MateMixerStreamControl  *control,
                                                                                gboolean                 enabled);

guint                           cafe_mixer_stream_control_get_min_volume       (MateMixerStreamControl  *control);
guint                           cafe_mixer_stream_control_get_max_volume       (MateMixerStreamControl  *control);
guint                           cafe_mixer_stream_control_get_normal_volume    (MateMixerStreamControl  *control);
guint                           cafe_mixer_stream_control_get_base_volume      (MateMixerStreamControl  *control);

G_END_DECLS

#endif /* CAFEMIXER_STREAM_CONTROL_H */
