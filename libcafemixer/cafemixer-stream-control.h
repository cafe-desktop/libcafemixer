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
        (G_TYPE_CHECK_INSTANCE_CAST ((o), CAFE_MIXER_TYPE_STREAM_CONTROL, CafeMixerStreamControl))
#define CAFE_MIXER_IS_STREAM_CONTROL(o)         \
        (G_TYPE_CHECK_INSTANCE_TYPE ((o), CAFE_MIXER_TYPE_STREAM_CONTROL))
#define CAFE_MIXER_STREAM_CONTROL_CLASS(k)      \
        (G_TYPE_CHECK_CLASS_CAST ((k), CAFE_MIXER_TYPE_STREAM_CONTROL, CafeMixerStreamControlClass))
#define CAFE_MIXER_IS_STREAM_CONTROL_CLASS(k)   \
        (G_TYPE_CHECK_CLASS_TYPE ((k), CAFE_MIXER_TYPE_STREAM_CONTROL))
#define CAFE_MIXER_STREAM_CONTROL_GET_CLASS(o)  \
        (G_TYPE_INSTANCE_GET_CLASS ((o), CAFE_MIXER_TYPE_STREAM_CONTROL, CafeMixerStreamControlClass))

typedef struct _CafeMixerStreamControlClass    CafeMixerStreamControlClass;
typedef struct _CafeMixerStreamControlPrivate  CafeMixerStreamControlPrivate;

/**
 * CafeMixerStreamControl:
 *
 * The #CafeMixerStreamControl structure contains only private data and should only
 * be accessed using the provided API.
 */
struct _CafeMixerStreamControl
{
    GObject object;

    /*< private >*/
    CafeMixerStreamControlPrivate *priv;
};

/**
 * CafeMixerStreamControlClass:
 * @parent_class: The parent class.
 *
 * The class structure for #CafeMixerStreamControl.
 */
struct _CafeMixerStreamControlClass
{
    GObjectClass parent_class;

    /*< private >*/
    CafeMixerAppInfo *       (*get_app_info)         (CafeMixerStreamControl  *control);

    gboolean                 (*set_stream)           (CafeMixerStreamControl  *control,
                                                      CafeMixerStream         *stream);

    gboolean                 (*set_mute)             (CafeMixerStreamControl  *control,
                                                      gboolean                 mute);

    guint                    (*get_num_channels)     (CafeMixerStreamControl  *control);

    guint                    (*get_volume)           (CafeMixerStreamControl  *control);
    gboolean                 (*set_volume)           (CafeMixerStreamControl  *control,
                                                      guint                    volume);

    gdouble                  (*get_decibel)          (CafeMixerStreamControl  *control);
    gboolean                 (*set_decibel)          (CafeMixerStreamControl  *control,
                                                      gdouble                  decibel);

    gboolean                 (*has_channel_position) (CafeMixerStreamControl  *control,
                                                      CafeMixerChannelPosition position);
    CafeMixerChannelPosition (*get_channel_position) (CafeMixerStreamControl  *control,
                                                      guint                    channel);

    guint                    (*get_channel_volume)   (CafeMixerStreamControl  *control,
                                                      guint                    channel);
    gboolean                 (*set_channel_volume)   (CafeMixerStreamControl  *control,
                                                      guint                    channel,
                                                      guint                    volume);

    gdouble                  (*get_channel_decibel)  (CafeMixerStreamControl  *control,
                                                      guint                    channel);
    gboolean                 (*set_channel_decibel)  (CafeMixerStreamControl  *control,
                                                      guint                    channel,
                                                      gdouble                  decibel);

    gboolean                 (*set_balance)          (CafeMixerStreamControl  *control,
                                                      gfloat                   balance);

    gboolean                 (*set_fade)             (CafeMixerStreamControl  *control,
                                                      gfloat                   fade);

    gboolean                 (*get_monitor_enabled)  (CafeMixerStreamControl  *control);
    gboolean                 (*set_monitor_enabled)  (CafeMixerStreamControl  *control,
                                                      gboolean                 enabled);

    guint                    (*get_min_volume)       (CafeMixerStreamControl  *control);
    guint                    (*get_max_volume)       (CafeMixerStreamControl  *control);
    guint                    (*get_normal_volume)    (CafeMixerStreamControl  *control);
    guint                    (*get_base_volume)      (CafeMixerStreamControl  *control);

    /* Signals */
    void (*monitor_value) (CafeMixerStreamControl *control, gdouble value);
};

GType                           cafe_mixer_stream_control_get_type             (void) G_GNUC_CONST;

const gchar *                   cafe_mixer_stream_control_get_name             (CafeMixerStreamControl  *control);
const gchar *                   cafe_mixer_stream_control_get_label            (CafeMixerStreamControl  *control);
CafeMixerStreamControlFlags     cafe_mixer_stream_control_get_flags            (CafeMixerStreamControl  *control);
CafeMixerStreamControlRole      cafe_mixer_stream_control_get_role             (CafeMixerStreamControl  *control);
CafeMixerStreamControlMediaRole cafe_mixer_stream_control_get_media_role       (CafeMixerStreamControl  *control);

CafeMixerAppInfo *              cafe_mixer_stream_control_get_app_info         (CafeMixerStreamControl  *control);

CafeMixerStream *               cafe_mixer_stream_control_get_stream           (CafeMixerStreamControl  *control);
gboolean                        cafe_mixer_stream_control_set_stream           (CafeMixerStreamControl  *control,
                                                                                CafeMixerStream         *stream);

gboolean                        cafe_mixer_stream_control_get_mute             (CafeMixerStreamControl  *control);
gboolean                        cafe_mixer_stream_control_set_mute             (CafeMixerStreamControl  *control,
                                                                                gboolean                 mute);

guint                           cafe_mixer_stream_control_get_num_channels     (CafeMixerStreamControl  *control);

guint                           cafe_mixer_stream_control_get_volume           (CafeMixerStreamControl  *control);
gboolean                        cafe_mixer_stream_control_set_volume           (CafeMixerStreamControl  *control,
                                                                                guint                    volume);

gdouble                         cafe_mixer_stream_control_get_decibel          (CafeMixerStreamControl  *control);
gboolean                        cafe_mixer_stream_control_set_decibel          (CafeMixerStreamControl  *control,
                                                                                gdouble                  decibel);

gboolean                        cafe_mixer_stream_control_has_channel_position (CafeMixerStreamControl  *control,
                                                                                CafeMixerChannelPosition position);
CafeMixerChannelPosition        cafe_mixer_stream_control_get_channel_position (CafeMixerStreamControl  *control,
                                                                                guint                    channel);

guint                           cafe_mixer_stream_control_get_channel_volume   (CafeMixerStreamControl  *control,
                                                                                guint                    channel);
gboolean                        cafe_mixer_stream_control_set_channel_volume   (CafeMixerStreamControl  *control,
                                                                                guint                    channel,
                                                                                guint                    volume);

gdouble                         cafe_mixer_stream_control_get_channel_decibel  (CafeMixerStreamControl  *control,
                                                                                guint                    channel);
gboolean                        cafe_mixer_stream_control_set_channel_decibel  (CafeMixerStreamControl  *control,
                                                                                guint                    channel,
                                                                                gdouble                  decibel);

gfloat                          cafe_mixer_stream_control_get_balance          (CafeMixerStreamControl  *control);
gboolean                        cafe_mixer_stream_control_set_balance          (CafeMixerStreamControl  *control,
                                                                                gfloat                   balance);

gfloat                          cafe_mixer_stream_control_get_fade             (CafeMixerStreamControl  *control);
gboolean                        cafe_mixer_stream_control_set_fade             (CafeMixerStreamControl  *control,
                                                                                gfloat                   fade);

gboolean                        cafe_mixer_stream_control_get_monitor_enabled  (CafeMixerStreamControl  *control);
gboolean                        cafe_mixer_stream_control_set_monitor_enabled  (CafeMixerStreamControl  *control,
                                                                                gboolean                 enabled);

guint                           cafe_mixer_stream_control_get_min_volume       (CafeMixerStreamControl  *control);
guint                           cafe_mixer_stream_control_get_max_volume       (CafeMixerStreamControl  *control);
guint                           cafe_mixer_stream_control_get_normal_volume    (CafeMixerStreamControl  *control);
guint                           cafe_mixer_stream_control_get_base_volume      (CafeMixerStreamControl  *control);

G_END_DECLS

#endif /* CAFEMIXER_STREAM_CONTROL_H */
