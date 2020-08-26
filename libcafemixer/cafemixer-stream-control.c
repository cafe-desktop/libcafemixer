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

#include <glib.h>
#include <glib-object.h>

#include "cafemixer-enums.h"
#include "cafemixer-enum-types.h"
#include "cafemixer-stream.h"
#include "cafemixer-stream-control.h"
#include "cafemixer-stream-control-private.h"

/**
 * SECTION:cafemixer-stream-control
 * @include: libcafemixer/cafemixer.h
 */

struct _MateMixerStreamControlPrivate
{
    gchar                          *name;
    gchar                          *label;
    gboolean                        mute;
    gfloat                          balance;
    gfloat                          fade;
    MateMixerStream                *stream;
    MateMixerStreamControlFlags     flags;
    MateMixerStreamControlRole      role;
    MateMixerStreamControlMediaRole media_role;
};

enum {
    PROP_0,
    PROP_NAME,
    PROP_LABEL,
    PROP_FLAGS,
    PROP_ROLE,
    PROP_MEDIA_ROLE,
    PROP_STREAM,
    PROP_MUTE,
    PROP_VOLUME,
    PROP_BALANCE,
    PROP_FADE,
    N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES] = { NULL, };

enum {
    MONITOR_VALUE,
    N_SIGNALS
};

static guint signals[N_SIGNALS] = { 0, };

static void cafe_mixer_stream_control_get_property (GObject                     *object,
                                                    guint                        param_id,
                                                    GValue                      *value,
                                                    GParamSpec                  *pspec);
static void cafe_mixer_stream_control_set_property (GObject                     *object,
                                                    guint                        param_id,
                                                    const GValue                *value,
                                                    GParamSpec                  *pspec);

static void cafe_mixer_stream_control_finalize     (GObject                     *object);

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE (MateMixerStreamControl, cafe_mixer_stream_control, G_TYPE_OBJECT)

static void
cafe_mixer_stream_control_class_init (MateMixerStreamControlClass *klass)
{
    GObjectClass *object_class;

    object_class = G_OBJECT_CLASS (klass);
    object_class->finalize     = cafe_mixer_stream_control_finalize;
    object_class->get_property = cafe_mixer_stream_control_get_property;
    object_class->set_property = cafe_mixer_stream_control_set_property;

    properties[PROP_NAME] =
        g_param_spec_string ("name",
                             "Name",
                             "Name of the stream control",
                             NULL,
                             G_PARAM_READWRITE |
                             G_PARAM_CONSTRUCT_ONLY |
                             G_PARAM_STATIC_STRINGS);

    properties[PROP_LABEL] =
        g_param_spec_string ("label",
                             "Label",
                             "Label of the stream control",
                             NULL,
                             G_PARAM_READWRITE |
                             G_PARAM_CONSTRUCT_ONLY |
                             G_PARAM_STATIC_STRINGS);

    properties[PROP_FLAGS] =
        g_param_spec_flags ("flags",
                            "Flags",
                            "Capability flags of the stream control",
                            CAFE_MIXER_TYPE_STREAM_CONTROL_FLAGS,
                            CAFE_MIXER_STREAM_CONTROL_NO_FLAGS,
                            G_PARAM_READWRITE |
                            G_PARAM_CONSTRUCT_ONLY |
                            G_PARAM_STATIC_STRINGS);

    properties[PROP_ROLE] =
        g_param_spec_enum ("role",
                           "Role",
                           "Role of the stream control",
                           CAFE_MIXER_TYPE_STREAM_CONTROL_ROLE,
                           CAFE_MIXER_STREAM_CONTROL_ROLE_UNKNOWN,
                           G_PARAM_READWRITE |
                           G_PARAM_CONSTRUCT_ONLY |
                           G_PARAM_STATIC_STRINGS);

    properties[PROP_MEDIA_ROLE] =
        g_param_spec_enum ("media-role",
                           "Media role",
                           "Media role of the stream control",
                           CAFE_MIXER_TYPE_STREAM_CONTROL_MEDIA_ROLE,
                           CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_UNKNOWN,
                           G_PARAM_READWRITE |
                           G_PARAM_CONSTRUCT_ONLY |
                           G_PARAM_STATIC_STRINGS);

    properties[PROP_STREAM] =
        g_param_spec_object ("stream",
                             "Stream",
                             "Stream which owns the control",
                             CAFE_MIXER_TYPE_STREAM,
                             G_PARAM_READWRITE |
                             G_PARAM_CONSTRUCT_ONLY |
                             G_PARAM_STATIC_STRINGS);

    properties[PROP_MUTE] =
        g_param_spec_boolean ("mute",
                              "Mute",
                              "Mute state of the stream control",
                              FALSE,
                              G_PARAM_READABLE |
                              G_PARAM_STATIC_STRINGS);

    properties[PROP_VOLUME] =
        g_param_spec_uint ("volume",
                           "Volume",
                           "Volume of the stream control",
                           0,
                           G_MAXUINT,
                           0,
                           G_PARAM_READABLE |
                           G_PARAM_STATIC_STRINGS);

    properties[PROP_BALANCE] =
        g_param_spec_float ("balance",
                            "Balance",
                            "Balance value of the stream control",
                            -1.0f,
                            1.0f,
                            0.0f,
                            G_PARAM_READABLE |
                            G_PARAM_STATIC_STRINGS);

    properties[PROP_FADE] =
        g_param_spec_float ("fade",
                            "Fade",
                            "Fade value of the stream control",
                            -1.0f,
                            1.0f,
                            0.0f,
                            G_PARAM_READABLE |
                            G_PARAM_STATIC_STRINGS);

    g_object_class_install_properties (object_class, N_PROPERTIES, properties);

    signals[MONITOR_VALUE] =
        g_signal_new ("monitor-value",
                      G_TYPE_FROM_CLASS (klass),
                      G_SIGNAL_RUN_FIRST,
                      G_STRUCT_OFFSET (MateMixerStreamControlClass, monitor_value),
                      NULL,
                      NULL,
                      g_cclosure_marshal_VOID__DOUBLE,
                      G_TYPE_NONE,
                      1,
                      G_TYPE_DOUBLE);
}

static void
cafe_mixer_stream_control_get_property (GObject    *object,
                                        guint       param_id,
                                        GValue     *value,
                                        GParamSpec *pspec)
{
    MateMixerStreamControl *control;

    control = CAFE_MIXER_STREAM_CONTROL (object);

    switch (param_id) {
    case PROP_NAME:
        g_value_set_string (value, control->priv->name);
        break;
    case PROP_LABEL:
        g_value_set_string (value, control->priv->label);
        break;
    case PROP_FLAGS:
        g_value_set_flags (value, control->priv->flags);
        break;
    case PROP_ROLE:
        g_value_set_enum (value, control->priv->role);
        break;
    case PROP_MEDIA_ROLE:
        g_value_set_enum (value, control->priv->media_role);
        break;
    case PROP_STREAM:
        g_value_set_object (value, control->priv->stream);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
        break;
    }
}

static void
cafe_mixer_stream_control_set_property (GObject      *object,
                                        guint         param_id,
                                        const GValue *value,
                                        GParamSpec   *pspec)
{
    MateMixerStreamControl *control;

    control = CAFE_MIXER_STREAM_CONTROL (object);

    switch (param_id) {
    case PROP_NAME:
        /* Construct-only string */
        control->priv->name = g_value_dup_string (value);
        break;
    case PROP_LABEL:
        /* Construct-only string */
        control->priv->label = g_value_dup_string (value);
        break;
    case PROP_FLAGS:
        control->priv->flags = g_value_get_flags (value);
        break;
    case PROP_ROLE:
        control->priv->role = g_value_get_enum (value);
        break;
    case PROP_MEDIA_ROLE:
        control->priv->media_role = g_value_get_enum (value);
        break;
    case PROP_STREAM:
        /* Construct-only object */
        control->priv->stream = g_value_get_object (value);

        if (control->priv->stream != NULL)
            g_object_add_weak_pointer (G_OBJECT (control->priv->stream),
                                       (gpointer *) &control->priv->stream);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
        break;
    }
}

static void
cafe_mixer_stream_control_init (MateMixerStreamControl *control)
{
    control->priv = cafe_mixer_stream_control_get_instance_private (control);
}

static void
cafe_mixer_stream_control_finalize (GObject *object)
{
    MateMixerStreamControl *control;

    control = CAFE_MIXER_STREAM_CONTROL (object);

    g_free (control->priv->name);
    g_free (control->priv->label);

    G_OBJECT_CLASS (cafe_mixer_stream_control_parent_class)->finalize (object);
}

/**
 * cafe_mixer_stream_control_get_name:
 * @control: a #MateMixerStreamControl
 */
const gchar *
cafe_mixer_stream_control_get_name (MateMixerStreamControl *control)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), NULL);

    return control->priv->name;
}

/**
 * cafe_mixer_stream_control_get_label:
 * @control: a #MateMixerStreamControl
 */
const gchar *
cafe_mixer_stream_control_get_label (MateMixerStreamControl *control)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), NULL);

    return control->priv->label;
}

/**
 * cafe_mixer_stream_control_get_flags:
 * @control: a #MateMixerStreamControl
 */
MateMixerStreamControlFlags
cafe_mixer_stream_control_get_flags (MateMixerStreamControl *control)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), CAFE_MIXER_STREAM_CONTROL_NO_FLAGS);

    return control->priv->flags;
}

/**
 * cafe_mixer_stream_control_get_role:
 * @control: a #MateMixerStreamControl
 */
MateMixerStreamControlRole
cafe_mixer_stream_control_get_role (MateMixerStreamControl *control)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), CAFE_MIXER_STREAM_CONTROL_ROLE_UNKNOWN);

    return control->priv->role;
}

/**
 * cafe_mixer_stream_control_get_media_role:
 * @control: a #MateMixerStreamControl
 */
MateMixerStreamControlMediaRole
cafe_mixer_stream_control_get_media_role (MateMixerStreamControl *control)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_UNKNOWN);

    return control->priv->media_role;
}

/**
 * cafe_mixer_stream_control_get_app_info:
 * @control: a #MateMixerStreamControl
 */
MateMixerAppInfo *
cafe_mixer_stream_control_get_app_info (MateMixerStreamControl *control)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), NULL);

    if (control->priv->role == CAFE_MIXER_STREAM_CONTROL_ROLE_APPLICATION) {
        MateMixerStreamControlClass *klass =
            CAFE_MIXER_STREAM_CONTROL_GET_CLASS (control);

        /* Implementation required for application role controls */
        return klass->get_app_info (control);
    }
    return NULL;
}

/**
 * cafe_mixer_stream_control_get_stream:
 * @control: a #MateMixerStreamControl
 */
MateMixerStream *
cafe_mixer_stream_control_get_stream (MateMixerStreamControl *control)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), NULL);

    return control->priv->stream;
}

/**
 * cafe_mixer_stream_control_set_stream:
 * @control: a #MateMixerStreamControl
 * @stream: a #MateMixerStream
 */
gboolean
cafe_mixer_stream_control_set_stream (MateMixerStreamControl *control,
                                      MateMixerStream        *stream)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), FALSE);
    g_return_val_if_fail (stream == NULL || CAFE_MIXER_IS_STREAM (stream), FALSE);

    if ((control->priv->flags & CAFE_MIXER_STREAM_CONTROL_MOVABLE) == 0)
        return FALSE;

    if (control->priv->stream != stream) {
        MateMixerStreamControlClass *klass =
            CAFE_MIXER_STREAM_CONTROL_GET_CLASS (control);

        /* Implementation required when the flag is available */
        if (klass->set_stream (control, stream) == FALSE)
            return FALSE;

        _cafe_mixer_stream_control_set_stream (control, stream);
    }
    return TRUE;
}

/**
 * cafe_mixer_stream_control_get_mute:
 * @control: a #MateMixerStreamControl
 */
gboolean
cafe_mixer_stream_control_get_mute (MateMixerStreamControl *control)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), FALSE);

    return control->priv->mute;
}

/**
 * cafe_mixer_stream_control_set_mute:
 * @control: a #MateMixerStreamControl
 * @mute: the mute toggle state to set
 */
gboolean
cafe_mixer_stream_control_set_mute (MateMixerStreamControl *control, gboolean mute)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), FALSE);

    if ((control->priv->flags & CAFE_MIXER_STREAM_CONTROL_MUTE_WRITABLE) == 0)
        return FALSE;

    if (control->priv->mute != mute) {
        MateMixerStreamControlClass *klass =
            CAFE_MIXER_STREAM_CONTROL_GET_CLASS (control);

        /* Implementation required when the flag is available */
        if (klass->set_mute (control, mute) == FALSE)
            return FALSE;

        _cafe_mixer_stream_control_set_mute (control, mute);
    }
    return TRUE;
}

/**
 * cafe_mixer_stream_control_get_num_channels:
 * @control: a #MateMixerStreamControl
 */
guint
cafe_mixer_stream_control_get_num_channels (MateMixerStreamControl *control)
{
    MateMixerStreamControlClass *klass;

    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), 0);

    klass = CAFE_MIXER_STREAM_CONTROL_GET_CLASS (control);

    if (klass->get_num_channels != NULL)
        return klass->get_num_channels (control);

    return 0;
}

/**
 * cafe_mixer_stream_control_get_volume:
 * @control: a #MateMixerStreamControl
 */
guint
cafe_mixer_stream_control_get_volume (MateMixerStreamControl *control)
{
    MateMixerStreamControlClass *klass;

    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), 0);

    klass = CAFE_MIXER_STREAM_CONTROL_GET_CLASS (control);

    if (control->priv->flags & CAFE_MIXER_STREAM_CONTROL_VOLUME_READABLE) {
        /* Implementation required when the flag is available */
        return klass->get_volume (control);
    }
    return klass->get_min_volume (control);
}

/**
 * cafe_mixer_stream_control_set_volume:
 * @control: a #MateMixerStreamControl
 * @volume: the volume to set
 */
gboolean
cafe_mixer_stream_control_set_volume (MateMixerStreamControl *control, guint volume)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), FALSE);

    if (control->priv->flags & CAFE_MIXER_STREAM_CONTROL_VOLUME_WRITABLE) {
        MateMixerStreamControlClass *klass =
            CAFE_MIXER_STREAM_CONTROL_GET_CLASS (control);

        /* Implementation required when the flag is available */
        return klass->set_volume (control, volume);
    }
    return FALSE;
}

/**
 * cafe_mixer_stream_control_get_decibel:
 * @control: a #MateMixerStreamControl
 */
gdouble
cafe_mixer_stream_control_get_decibel (MateMixerStreamControl *control)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), -CAFE_MIXER_INFINITY);

    if (control->priv->flags & CAFE_MIXER_STREAM_CONTROL_HAS_DECIBEL &&
        control->priv->flags & CAFE_MIXER_STREAM_CONTROL_VOLUME_READABLE) {
        MateMixerStreamControlClass *klass =
            CAFE_MIXER_STREAM_CONTROL_GET_CLASS (control);

        /* Implementation required when the flags are available */
        return klass->get_decibel (control);
    }
    return -CAFE_MIXER_INFINITY;
}

/**
 * cafe_mixer_stream_control_set_decibel:
 * @control: a #MateMixerStreamControl
 * @decibel: the volume to set in decibels
 */
gboolean
cafe_mixer_stream_control_set_decibel (MateMixerStreamControl *control, gdouble decibel)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), FALSE);

    if (control->priv->flags & CAFE_MIXER_STREAM_CONTROL_HAS_DECIBEL &&
        control->priv->flags & CAFE_MIXER_STREAM_CONTROL_VOLUME_WRITABLE) {
        MateMixerStreamControlClass *klass =
            CAFE_MIXER_STREAM_CONTROL_GET_CLASS (control);

        /* Implementation required when the flags are available */
        return klass->set_decibel (control, decibel);
    }
    return FALSE;
}

/**
 * cafe_mixer_stream_control_has_channel_position:
 * @control: a #MateMixerStreamControl
 * @position: to channel position to check
 */
gboolean
cafe_mixer_stream_control_has_channel_position (MateMixerStreamControl  *control,
                                                MateMixerChannelPosition position)
{
    MateMixerStreamControlClass *klass;

    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), FALSE);

    klass = CAFE_MIXER_STREAM_CONTROL_GET_CLASS (control);

    if (klass->has_channel_position != NULL)
        return klass->has_channel_position (control, position);

    return FALSE;
}

/**
 * cafe_mixer_stream_control_get_channel_position:
 * @control: a #MateMixerStreamControl
 * @channel: a channel index
 */
MateMixerChannelPosition
cafe_mixer_stream_control_get_channel_position (MateMixerStreamControl *control, guint channel)
{
    MateMixerStreamControlClass *klass;

    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), CAFE_MIXER_CHANNEL_UNKNOWN);

    klass = CAFE_MIXER_STREAM_CONTROL_GET_CLASS (control);

    if (klass->get_channel_position != NULL)
        return klass->get_channel_position (control, channel);

    return CAFE_MIXER_CHANNEL_UNKNOWN;
}

/**
 * cafe_mixer_stream_control_get_channel_volume:
 * @control: a #MateMixerStreamControl
 * @channel: a channel index
 */
guint
cafe_mixer_stream_control_get_channel_volume (MateMixerStreamControl *control, guint channel)
{
    MateMixerStreamControlClass *klass;

    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), 0);

    klass = CAFE_MIXER_STREAM_CONTROL_GET_CLASS (control);

    if (control->priv->flags & CAFE_MIXER_STREAM_CONTROL_VOLUME_READABLE) {
        /* Implementation required when the flag is available */
        return klass->get_channel_volume (control, channel);
    }
    return klass->get_min_volume (control);
}

/**
 * cafe_mixer_stream_control_set_channel_volume:
 * @control: a #MateMixerStreamControl
 * @channel: a channel index
 * @volume: the volume to set
 */
gboolean
cafe_mixer_stream_control_set_channel_volume (MateMixerStreamControl *control,
                                              guint                   channel,
                                              guint                   volume)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), FALSE);

    if (control->priv->flags & CAFE_MIXER_STREAM_CONTROL_VOLUME_WRITABLE) {
        MateMixerStreamControlClass *klass =
            CAFE_MIXER_STREAM_CONTROL_GET_CLASS (control);

        /* Implementation required when the flag is available */
        return klass->set_channel_volume (control, channel, volume);
    }
    return FALSE;
}

/**
 * cafe_mixer_stream_control_get_channel_decibel:
 * @control: a #MateMixerStreamControl
 * @channel: a channel index
 */
gdouble
cafe_mixer_stream_control_get_channel_decibel (MateMixerStreamControl *control, guint channel)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), -CAFE_MIXER_INFINITY);

    if (control->priv->flags & CAFE_MIXER_STREAM_CONTROL_HAS_DECIBEL &&
        control->priv->flags & CAFE_MIXER_STREAM_CONTROL_VOLUME_READABLE) {
        MateMixerStreamControlClass *klass =
            CAFE_MIXER_STREAM_CONTROL_GET_CLASS (control);

        /* Implementation required when the flags are available */
        return klass->get_channel_decibel (control, channel);
    }
    return -CAFE_MIXER_INFINITY;
}

/**
 * cafe_mixer_stream_control_set_channel_decibel:
 * @control: a #MateMixerStreamControl
 * @channel: a channel index
 * @decibel: the volume to set in decibels
 */
gboolean
cafe_mixer_stream_control_set_channel_decibel (MateMixerStreamControl *control,
                                               guint                   channel,
                                               gdouble                 decibel)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), FALSE);

    if (control->priv->flags & CAFE_MIXER_STREAM_CONTROL_HAS_DECIBEL &&
        control->priv->flags & CAFE_MIXER_STREAM_CONTROL_VOLUME_WRITABLE) {
        MateMixerStreamControlClass *klass =
            CAFE_MIXER_STREAM_CONTROL_GET_CLASS (control);

        /* Implementation required when the flags are available */
        return klass->set_channel_decibel (control, channel, decibel);
    }
    return FALSE;
}

/**
 * cafe_mixer_stream_control_get_balance:
 * @control: a #MateMixerStreamControl
 */
gfloat
cafe_mixer_stream_control_get_balance (MateMixerStreamControl *control)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), 0.0f);

    if (control->priv->flags & CAFE_MIXER_STREAM_CONTROL_CAN_BALANCE)
        return control->priv->balance;
    else
        return 0.0f;
}

/**
 * cafe_mixer_stream_control_set_balance:
 * @control: a #MateMixerStreamControl
 * @balance: the balance value
 */
gboolean
cafe_mixer_stream_control_set_balance (MateMixerStreamControl *control, gfloat balance)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), FALSE);
    g_return_val_if_fail (balance >= -1.0f && balance <= 1.0f, FALSE);

    if ((control->priv->flags & CAFE_MIXER_STREAM_CONTROL_CAN_BALANCE) == 0)
        return FALSE;

    if (control->priv->balance != balance) {
        MateMixerStreamControlClass *klass =
            CAFE_MIXER_STREAM_CONTROL_GET_CLASS (control);

        /* Implementation required when the flag is available */
        if (klass->set_balance (control, balance) == FALSE)
            return FALSE;

        _cafe_mixer_stream_control_set_balance (control, balance);
    }
    return TRUE;
}

/**
 * cafe_mixer_stream_control_get_fade:
 * @control: a #MateMixerStreamControl
 */
gfloat
cafe_mixer_stream_control_get_fade (MateMixerStreamControl *control)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), 0.0f);

    if (control->priv->flags & CAFE_MIXER_STREAM_CONTROL_CAN_FADE)
        return control->priv->fade;
    else
        return 0.0f;
}

/**
 * cafe_mixer_stream_control_set_fade:
 * @control: a #MateMixerStreamControl
 * @fade: the fade value
 */
gboolean
cafe_mixer_stream_control_set_fade (MateMixerStreamControl *control, gfloat fade)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), FALSE);
    g_return_val_if_fail (fade >= -1.0f && fade <= 1.0f, FALSE);

    if ((control->priv->flags & CAFE_MIXER_STREAM_CONTROL_CAN_FADE) == 0)
        return FALSE;

    if (control->priv->fade != fade) {
        MateMixerStreamControlClass *klass =
            CAFE_MIXER_STREAM_CONTROL_GET_CLASS (control);

        /* Implementation required when the flag is available */
        if (klass->set_fade (control, fade) == FALSE)
            return FALSE;

        _cafe_mixer_stream_control_set_fade (control, fade);
    }
    return TRUE;
}

/**
 * cafe_mixer_stream_control_get_monitor_enabled:
 * @control: a #MateMixerStreamControl
 */
gboolean
cafe_mixer_stream_control_get_monitor_enabled (MateMixerStreamControl *control)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), FALSE);

    if (control->priv->flags & CAFE_MIXER_STREAM_CONTROL_HAS_MONITOR) {
        MateMixerStreamControlClass *klass =
            CAFE_MIXER_STREAM_CONTROL_GET_CLASS (control);

        /* Implementation required when the flag is available */
        return klass->get_monitor_enabled (control);
    }
    return FALSE;
}

/**
 * cafe_mixer_stream_control_set_monitor_enabled:
 * @control: a #MateMixerStreamControl
 * @enabled: a boolean value
 */
gboolean
cafe_mixer_stream_control_set_monitor_enabled (MateMixerStreamControl *control, gboolean enabled)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), FALSE);

    if ((control->priv->flags & CAFE_MIXER_STREAM_CONTROL_HAS_MONITOR) == 0)
        return FALSE;

    /* Implementation required when the flag is available */
    return CAFE_MIXER_STREAM_CONTROL_GET_CLASS (control)->set_monitor_enabled (control, enabled);
}

/**
 * cafe_mixer_stream_control_get_min_volume:
 * @control: a #MateMixerStreamControl
 */
guint
cafe_mixer_stream_control_get_min_volume (MateMixerStreamControl *control)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), 0);

    /* Implementation required */
    return CAFE_MIXER_STREAM_CONTROL_GET_CLASS (control)->get_min_volume (control);
}

/**
 * cafe_mixer_stream_control_get_max_volume:
 * @control: a #MateMixerStreamControl
 */
guint
cafe_mixer_stream_control_get_max_volume (MateMixerStreamControl *control)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), 0);

    /* Implementation required */
    return CAFE_MIXER_STREAM_CONTROL_GET_CLASS (control)->get_max_volume (control);
}

/**
 * cafe_mixer_stream_control_get_normal_volume:
 * @control: a #MateMixerStreamControl
 */
guint
cafe_mixer_stream_control_get_normal_volume (MateMixerStreamControl *control)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), 0);

    /* Implementation required */
    return CAFE_MIXER_STREAM_CONTROL_GET_CLASS (control)->get_normal_volume (control);
}

/**
 * cafe_mixer_stream_control_get_base_volume:
 * @control: a #MateMixerStreamControl
 */
guint
cafe_mixer_stream_control_get_base_volume (MateMixerStreamControl *control)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control), 0);

    /* Implementation required */
    return CAFE_MIXER_STREAM_CONTROL_GET_CLASS (control)->get_base_volume (control);
}

/* Protected functions */
void
_cafe_mixer_stream_control_set_flags (MateMixerStreamControl     *control,
                                      MateMixerStreamControlFlags flags)
{
    g_return_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control));

    if (control->priv->flags == flags)
        return;

    control->priv->flags = flags;

    g_object_notify_by_pspec (G_OBJECT (control), properties[PROP_FLAGS]);
}

void
_cafe_mixer_stream_control_set_stream (MateMixerStreamControl *control,
                                       MateMixerStream        *stream)
{
    g_return_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control));
    g_return_if_fail (stream == NULL || CAFE_MIXER_IS_STREAM (stream));

    if (control->priv->stream == stream)
        return;

    if (control->priv->stream != NULL)
        g_object_remove_weak_pointer (G_OBJECT (control->priv->stream),
                                      (gpointer *) &control->priv->stream);

    if (stream != NULL) {
        control->priv->stream = stream;
        g_object_add_weak_pointer (G_OBJECT (control->priv->stream),
                                   (gpointer *) &control->priv->stream);
    } else
        control->priv->stream = NULL;

    g_object_notify_by_pspec (G_OBJECT (control), properties[PROP_STREAM]);
}

void
_cafe_mixer_stream_control_set_mute (MateMixerStreamControl *control, gboolean mute)
{
    g_return_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control));

    if (control->priv->mute == mute)
        return;

    control->priv->mute = mute;

    g_object_notify_by_pspec (G_OBJECT (control), properties[PROP_MUTE]);
}

void
_cafe_mixer_stream_control_set_balance (MateMixerStreamControl *control, gfloat balance)
{
    g_return_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control));

    if (control->priv->balance == balance)
        return;

    control->priv->balance = balance;

    g_object_notify_by_pspec (G_OBJECT (control), properties[PROP_BALANCE]);
}

void
_cafe_mixer_stream_control_set_fade (MateMixerStreamControl *control, gfloat fade)
{
    g_return_if_fail (CAFE_MIXER_IS_STREAM_CONTROL (control));

    if (control->priv->fade == fade)
        return;

    control->priv->fade = fade;

    g_object_notify_by_pspec (G_OBJECT (control), properties[PROP_FADE]);
}
