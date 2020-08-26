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

#include <string.h>
#include <glib.h>
#include <glib-object.h>

#include "cafemixer-device.h"
#include "cafemixer-enums.h"
#include "cafemixer-enum-types.h"
#include "cafemixer-stream.h"
#include "cafemixer-stream-control.h"
#include "cafemixer-stream-private.h"
#include "cafemixer-stream-switch.h"
#include "cafemixer-switch.h"

/**
 * SECTION:cafemixer-stream
 * @include: libcafemixer/cafemixer.h
 */

struct _CafeMixerStreamPrivate
{
    gchar                  *name;
    gchar                  *label;
    CafeMixerDirection      direction;
    CafeMixerDevice        *device;
    CafeMixerStreamControl *control;
};

enum {
    PROP_0,
    PROP_NAME,
    PROP_LABEL,
    PROP_DIRECTION,
    PROP_DEVICE,
    PROP_DEFAULT_CONTROL,
    N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES] = { NULL, };

enum {
    CONTROL_ADDED,
    CONTROL_REMOVED,
    SWITCH_ADDED,
    SWITCH_REMOVED,
    N_SIGNALS
};

static guint signals[N_SIGNALS] = { 0, };

static void cafe_mixer_stream_get_property (GObject              *object,
                                            guint                 param_id,
                                            GValue               *value,
                                            GParamSpec           *pspec);
static void cafe_mixer_stream_set_property (GObject              *object,
                                            guint                 param_id,
                                            const GValue         *value,
                                            GParamSpec           *pspec);

static void cafe_mixer_stream_dispose      (GObject              *object);
static void cafe_mixer_stream_finalize     (GObject              *object);

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE (CafeMixerStream, cafe_mixer_stream, G_TYPE_OBJECT)

static CafeMixerStreamControl *cafe_mixer_stream_real_get_control (CafeMixerStream *stream,
                                                                   const gchar     *name);
static CafeMixerStreamSwitch * cafe_mixer_stream_real_get_switch  (CafeMixerStream *stream,
                                                                   const gchar     *name);

static void
cafe_mixer_stream_class_init (CafeMixerStreamClass *klass)
{
    GObjectClass *object_class;

    klass->get_control = cafe_mixer_stream_real_get_control;
    klass->get_switch  = cafe_mixer_stream_real_get_switch;

    object_class = G_OBJECT_CLASS (klass);
    object_class->dispose      = cafe_mixer_stream_dispose;
    object_class->finalize     = cafe_mixer_stream_finalize;
    object_class->get_property = cafe_mixer_stream_get_property;
    object_class->set_property = cafe_mixer_stream_set_property;

    properties[PROP_NAME] =
        g_param_spec_string ("name",
                             "Name",
                             "Name of the stream",
                             NULL,
                             G_PARAM_READWRITE |
                             G_PARAM_CONSTRUCT_ONLY |
                             G_PARAM_STATIC_STRINGS);

    properties[PROP_LABEL] =
        g_param_spec_string ("label",
                             "Label",
                             "Label of the stream",
                             NULL,
                             G_PARAM_READWRITE |
                             G_PARAM_CONSTRUCT_ONLY |
                             G_PARAM_STATIC_STRINGS);

    properties[PROP_DIRECTION] =
        g_param_spec_enum ("direction",
                           "Direction",
                           "Direction of the stream",
                           CAFE_MIXER_TYPE_DIRECTION,
                           CAFE_MIXER_DIRECTION_UNKNOWN,
                           G_PARAM_READWRITE |
                           G_PARAM_CONSTRUCT_ONLY |
                           G_PARAM_STATIC_STRINGS);

    properties[PROP_DEVICE] =
        g_param_spec_object ("device",
                             "Device",
                             "Device the stream belongs to",
                             CAFE_MIXER_TYPE_DEVICE,
                             G_PARAM_READWRITE |
                             G_PARAM_CONSTRUCT_ONLY |
                             G_PARAM_STATIC_STRINGS);

    properties[PROP_DEFAULT_CONTROL] =
        g_param_spec_object ("default-control",
                             "Default control",
                             "Default control of the stream",
                             CAFE_MIXER_TYPE_STREAM_CONTROL,
                             G_PARAM_READWRITE |
                             G_PARAM_CONSTRUCT_ONLY |
                             G_PARAM_STATIC_STRINGS);

    g_object_class_install_properties (object_class, N_PROPERTIES, properties);

    signals[CONTROL_ADDED] =
        g_signal_new ("control-added",
                      G_TYPE_FROM_CLASS (object_class),
                      G_SIGNAL_RUN_FIRST,
                      G_STRUCT_OFFSET (CafeMixerStreamClass, control_added),
                      NULL,
                      NULL,
                      g_cclosure_marshal_VOID__STRING,
                      G_TYPE_NONE,
                      1,
                      G_TYPE_STRING);

    signals[CONTROL_REMOVED] =
        g_signal_new ("control-removed",
                      G_TYPE_FROM_CLASS (object_class),
                      G_SIGNAL_RUN_FIRST,
                      G_STRUCT_OFFSET (CafeMixerStreamClass, control_removed),
                      NULL,
                      NULL,
                      g_cclosure_marshal_VOID__STRING,
                      G_TYPE_NONE,
                      1,
                      G_TYPE_STRING);

    signals[SWITCH_ADDED] =
        g_signal_new ("switch-added",
                      G_TYPE_FROM_CLASS (object_class),
                      G_SIGNAL_RUN_FIRST,
                      G_STRUCT_OFFSET (CafeMixerStreamClass, switch_added),
                      NULL,
                      NULL,
                      g_cclosure_marshal_VOID__STRING,
                      G_TYPE_NONE,
                      1,
                      G_TYPE_STRING);

    signals[SWITCH_REMOVED] =
        g_signal_new ("switch-removed",
                      G_TYPE_FROM_CLASS (object_class),
                      G_SIGNAL_RUN_FIRST,
                      G_STRUCT_OFFSET (CafeMixerStreamClass, switch_removed),
                      NULL,
                      NULL,
                      g_cclosure_marshal_VOID__STRING,
                      G_TYPE_NONE,
                      1,
                      G_TYPE_STRING);
}

static void
cafe_mixer_stream_get_property (GObject    *object,
                                guint       param_id,
                                GValue     *value,
                                GParamSpec *pspec)
{
    CafeMixerStream *stream;

    stream = CAFE_MIXER_STREAM (object);

    switch (param_id) {
    case PROP_NAME:
        g_value_set_string (value, stream->priv->name);
        break;
    case PROP_LABEL:
        g_value_set_string (value, stream->priv->label);
        break;
    case PROP_DIRECTION:
        g_value_set_enum (value, stream->priv->direction);
        break;
    case PROP_DEVICE:
        g_value_set_object (value, stream->priv->device);
        break;
    case PROP_DEFAULT_CONTROL:
        g_value_set_object (value, stream->priv->control);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
        break;
    }
}

static void
cafe_mixer_stream_set_property (GObject      *object,
                                guint         param_id,
                                const GValue *value,
                                GParamSpec   *pspec)
{
    CafeMixerStream *stream;

    stream = CAFE_MIXER_STREAM (object);

    switch (param_id) {
    case PROP_NAME:
        /* Construct-only string */
        stream->priv->name = g_value_dup_string (value);
        break;
    case PROP_LABEL:
        /* Construct-only string */
        stream->priv->label = g_value_dup_string (value);
        break;
    case PROP_DIRECTION:
        stream->priv->direction = g_value_get_enum (value);
        break;
    case PROP_DEVICE:
        /* Construct-only object */
        stream->priv->device = g_value_get_object (value);

        if (stream->priv->device != NULL)
            g_object_add_weak_pointer (G_OBJECT (stream->priv->device),
                                       (gpointer *) &stream->priv->device);
        break;
    case PROP_DEFAULT_CONTROL:
        /* Construct-only object */
        stream->priv->control = g_value_dup_object (value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
        break;
    }
}

static void
cafe_mixer_stream_init (CafeMixerStream *stream)
{
    stream->priv = cafe_mixer_stream_get_instance_private (stream);
}

static void
cafe_mixer_stream_dispose (GObject *object)
{
    CafeMixerStream *stream;

    stream = CAFE_MIXER_STREAM (object);

    g_clear_object (&stream->priv->control);

    G_OBJECT_CLASS (cafe_mixer_stream_parent_class)->dispose (object);
}

static void
cafe_mixer_stream_finalize (GObject *object)
{
    CafeMixerStream *stream;

    stream = CAFE_MIXER_STREAM (object);

    g_free (stream->priv->name);
    g_free (stream->priv->label);

    G_OBJECT_CLASS (cafe_mixer_stream_parent_class)->finalize (object);
}

/**
 * cafe_mixer_stream_get_name:
 * @stream: a #CafeMixerStream
 */
const gchar *
cafe_mixer_stream_get_name (CafeMixerStream *stream)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM (stream), NULL);

    return stream->priv->name;
}

/**
 * cafe_mixer_stream_get_label:
 * @stream: a #CafeMixerStream
 */
const gchar *
cafe_mixer_stream_get_label (CafeMixerStream *stream)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM (stream), NULL);

    return stream->priv->label;
}

/**
 * cafe_mixer_stream_get_direction:
 * @stream: a #CafeMixerStream
 */
CafeMixerDirection
cafe_mixer_stream_get_direction (CafeMixerStream *stream)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM (stream), CAFE_MIXER_DIRECTION_UNKNOWN);

    return stream->priv->direction;
}

/**
 * cafe_mixer_stream_get_device:
 * @stream: a #CafeMixerStream
 */
CafeMixerDevice *
cafe_mixer_stream_get_device (CafeMixerStream *stream)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM (stream), NULL);

    return stream->priv->device;
}

/**
 * cafe_mixer_stream_get_control:
 * @stream: a #CafeMixerStream
 * @name: the name of a stream control
 */
CafeMixerStreamControl *
cafe_mixer_stream_get_control (CafeMixerStream *stream, const gchar *name)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM (stream), NULL);
    g_return_val_if_fail (name != NULL, NULL);

    return CAFE_MIXER_STREAM_GET_CLASS (stream)->get_control (stream, name);
}

/**
 * cafe_mixer_stream_get_switch:
 * @stream: a #CafeMixerStream
 * @name: the name of a stream switch
 *
 * Gets the switch with the given name.
 *
 * Returns: a #CafeMixerStreamSwitch or %NULL if there is no such switch.
 */
CafeMixerStreamSwitch *
cafe_mixer_stream_get_switch (CafeMixerStream *stream, const gchar *name)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM (stream), NULL);
    g_return_val_if_fail (name != NULL, NULL);

    return CAFE_MIXER_STREAM_GET_CLASS (stream)->get_switch (stream, name);
}

/**
 * cafe_mixer_stream_get_default_control:
 * @stream: a #CafeMixerStream
 */
CafeMixerStreamControl *
cafe_mixer_stream_get_default_control (CafeMixerStream *stream)
{
    const GList *list;

    g_return_val_if_fail (CAFE_MIXER_IS_STREAM (stream), NULL);

    if (stream->priv->control != NULL)
        return stream->priv->control;

    /* If the stream does not have a default control, just return the first one */
    list = cafe_mixer_stream_list_controls (stream);
    if (list != NULL)
        return CAFE_MIXER_STREAM_CONTROL (list->data);

    return NULL;
}

/**
 * cafe_mixer_stream_list_controls:
 * @stream: a #CafeMixerStream
 */
const GList *
cafe_mixer_stream_list_controls (CafeMixerStream *stream)
{
    CafeMixerStreamClass *klass;

    g_return_val_if_fail (CAFE_MIXER_IS_STREAM (stream), NULL);

    klass = CAFE_MIXER_STREAM_GET_CLASS (stream);

    if (G_LIKELY (klass->list_controls != NULL))
        return klass->list_controls (stream);

    return NULL;
}

/**
 * cafe_mixer_stream_list_switches:
 * @stream: a #CafeMixerStream
 */
const GList *
cafe_mixer_stream_list_switches (CafeMixerStream *stream)
{
    CafeMixerStreamClass *klass;

    g_return_val_if_fail (CAFE_MIXER_IS_STREAM (stream), NULL);

    klass = CAFE_MIXER_STREAM_GET_CLASS (stream);

    if (G_LIKELY (klass->list_switches != NULL))
        return klass->list_switches (stream);

    return NULL;
}

static CafeMixerStreamControl *
cafe_mixer_stream_real_get_control (CafeMixerStream *stream, const gchar *name)
{
    const GList *list;

    g_return_val_if_fail (CAFE_MIXER_IS_STREAM (stream), NULL);
    g_return_val_if_fail (name != NULL, NULL);

    list = cafe_mixer_stream_list_controls (stream);
    while (list != NULL) {
        CafeMixerStreamControl *control = CAFE_MIXER_STREAM_CONTROL (list->data);

        if (strcmp (name, cafe_mixer_stream_control_get_name (control)) == 0)
            return control;

        list = list->next;
    }
    return NULL;
}

static CafeMixerStreamSwitch *
cafe_mixer_stream_real_get_switch (CafeMixerStream *stream, const gchar *name)
{
    const GList *list;

    g_return_val_if_fail (CAFE_MIXER_IS_STREAM (stream), NULL);
    g_return_val_if_fail (name != NULL, NULL);

    list = cafe_mixer_stream_list_switches (stream);
    while (list != NULL) {
        CafeMixerSwitch *swtch = CAFE_MIXER_SWITCH (list->data);

        if (strcmp (name, cafe_mixer_switch_get_name (swtch)) == 0)
            return CAFE_MIXER_STREAM_SWITCH (swtch);

        list = list->next;
    }
    return NULL;
}

void
_cafe_mixer_stream_set_default_control (CafeMixerStream        *stream,
                                        CafeMixerStreamControl *control)
{
    g_return_if_fail (CAFE_MIXER_IS_STREAM (stream));
    g_return_if_fail (control == NULL || CAFE_MIXER_IS_STREAM_CONTROL (control));

    if (stream->priv->control == control)
        return;

    if (stream->priv->control != NULL)
        g_object_unref (stream->priv->control);

    /* The default control is allowed to be NULL */
    if (control != NULL) {
        stream->priv->control = g_object_ref (control);

        g_debug ("Stream %s default control changed to %s",
                 cafe_mixer_stream_get_name (stream),
                 cafe_mixer_stream_control_get_name (control));
    } else {
        stream->priv->control = NULL;

        g_debug ("Stream %s default control unset",
                 cafe_mixer_stream_get_name (stream));
    }

    g_object_notify_by_pspec (G_OBJECT (stream), properties[PROP_DEFAULT_CONTROL]);
}
