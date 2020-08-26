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
#include "cafemixer-device-switch.h"
#include "cafemixer-stream.h"
#include "cafemixer-switch.h"

/**
 * SECTION:cafemixer-device
 * @short_description: Hardware or software device in the sound system
 * @include: libcafemixer/cafemixer.h
 *
 * A #CafeMixerDevice represents a sound device, most typically a sound card.
 *
 * Each device may contain an arbitrary number of streams.
 */

struct _CafeMixerDevicePrivate
{
    gchar *name;
    gchar *label;
    gchar *icon;
};

enum {
    PROP_0,
    PROP_NAME,
    PROP_LABEL,
    PROP_ICON,
    N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES] = { NULL, };

enum {
    STREAM_ADDED,
    STREAM_REMOVED,
    SWITCH_ADDED,
    SWITCH_REMOVED,
    N_SIGNALS
};

static guint signals[N_SIGNALS] = { 0, };

static void cafe_mixer_device_get_property (GObject              *object,
                                            guint                 param_id,
                                            GValue               *value,
                                            GParamSpec           *pspec);
static void cafe_mixer_device_set_property (GObject              *object,
                                            guint                 param_id,
                                            const GValue         *value,
                                            GParamSpec           *pspec);

static void cafe_mixer_device_finalize     (GObject              *object);

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE (CafeMixerDevice, cafe_mixer_device, G_TYPE_OBJECT)

static CafeMixerStream *      cafe_mixer_device_real_get_stream (CafeMixerDevice *device,
                                                                 const gchar     *name);
static CafeMixerDeviceSwitch *cafe_mixer_device_real_get_switch (CafeMixerDevice *device,
                                                                 const gchar     *name);

static void
cafe_mixer_device_class_init (CafeMixerDeviceClass *klass)
{
    GObjectClass *object_class;

    klass->get_stream = cafe_mixer_device_real_get_stream;
    klass->get_switch = cafe_mixer_device_real_get_switch;

    object_class = G_OBJECT_CLASS (klass);
    object_class->finalize     = cafe_mixer_device_finalize;
    object_class->get_property = cafe_mixer_device_get_property;
    object_class->set_property = cafe_mixer_device_set_property;

    /**
     * CafeMixerDevice:name:
     *
     * The name of the device. The name serves as a unique identifier and
     * in most cases it is not in a user-readable form.
     */
    properties[PROP_NAME] =
        g_param_spec_string ("name",
                             "Name",
                             "Name of the device",
                             NULL,
                             G_PARAM_READWRITE |
                             G_PARAM_CONSTRUCT_ONLY |
                             G_PARAM_STATIC_STRINGS);

    /**
     * CafeMixerDevice:label:
     *
     * The label of the device. This is a potentially translated string
     * that should be presented to users in the user interface.
     */
    properties[PROP_LABEL] =
        g_param_spec_string ("label",
                             "Label",
                             "Label of the device",
                             NULL,
                             G_PARAM_READWRITE |
                             G_PARAM_CONSTRUCT_ONLY |
                             G_PARAM_STATIC_STRINGS);

    /**
     * CafeMixerDevice:icon:
     *
     * The XDG icon name of the device.
     */
    properties[PROP_ICON] =
        g_param_spec_string ("icon",
                             "Icon",
                             "XDG icon name of the device",
                             NULL,
                             G_PARAM_READWRITE |
                             G_PARAM_CONSTRUCT_ONLY |
                             G_PARAM_STATIC_STRINGS);

    g_object_class_install_properties (object_class, N_PROPERTIES, properties);

    /**
     * CafeMixerDevice::stream-added:
     * @device: a #CafeMixerDevice
     * @name: name of the added stream
     *
     * The signal is emitted each time a device stream is added.
     *
     * Note that at the time this signal is emitted, the controls and switches
     * of the stream may not yet be known.
     */
    signals[STREAM_ADDED] =
        g_signal_new ("stream-added",
                      G_TYPE_FROM_CLASS (object_class),
                      G_SIGNAL_RUN_FIRST,
                      G_STRUCT_OFFSET (CafeMixerDeviceClass, stream_added),
                      NULL,
                      NULL,
                      g_cclosure_marshal_VOID__STRING,
                      G_TYPE_NONE,
                      1,
                      G_TYPE_STRING);

    /**
     * CafeMixerDevice::stream-removed:
     * @device: a #CafeMixerDevice
     * @name: name of the removed stream
     *
     * The signal is emitted each time a device stream is removed.
     *
     * When this signal is emitted, the stream is no longer known to the library,
     * it will not be included in the stream list provided by the
     * cafe_mixer_device_list_streams() function and it is not possible to get
     * the stream with cafe_mixer_device_get_stream().
     */
    signals[STREAM_REMOVED] =
        g_signal_new ("stream-removed",
                      G_TYPE_FROM_CLASS (object_class),
                      G_SIGNAL_RUN_FIRST,
                      G_STRUCT_OFFSET (CafeMixerDeviceClass, stream_removed),
                      NULL,
                      NULL,
                      g_cclosure_marshal_VOID__STRING,
                      G_TYPE_NONE,
                      1,
                      G_TYPE_STRING);

    /**
     * CafeMixerDevice::switch-added:
     * @device: a #CafeMixerDevice
     * @name: name of the added switch
     *
     * The signal is emitted each time a device switch is added.
     */
    signals[SWITCH_ADDED] =
        g_signal_new ("switch-added",
                      G_TYPE_FROM_CLASS (object_class),
                      G_SIGNAL_RUN_FIRST,
                      G_STRUCT_OFFSET (CafeMixerDeviceClass, switch_added),
                      NULL,
                      NULL,
                      g_cclosure_marshal_VOID__STRING,
                      G_TYPE_NONE,
                      1,
                      G_TYPE_STRING);

    /**
     * CafeMixerDevice::switch-removed:
     * @device: a #CafeMixerDevice
     * @name: name of the removed switch
     *
     * The signal is emitted each time a device switch is removed.
     *
     * When this signal is emitted, the switch is no longer known to the library,
     * it will not be included in the switch list provided by the
     * cafe_mixer_device_list_switches() function and it is not possible to get
     * the switch with cafe_mixer_device_get_switch().
     */
    signals[SWITCH_REMOVED] =
        g_signal_new ("switch-removed",
                      G_TYPE_FROM_CLASS (object_class),
                      G_SIGNAL_RUN_FIRST,
                      G_STRUCT_OFFSET (CafeMixerDeviceClass, switch_removed),
                      NULL,
                      NULL,
                      g_cclosure_marshal_VOID__STRING,
                      G_TYPE_NONE,
                      1,
                      G_TYPE_STRING);
}

static void
cafe_mixer_device_get_property (GObject    *object,
                                guint       param_id,
                                GValue     *value,
                                GParamSpec *pspec)
{
    CafeMixerDevice *device;

    device = CAFE_MIXER_DEVICE (object);

    switch (param_id) {
    case PROP_NAME:
        g_value_set_string (value, device->priv->name);
        break;
    case PROP_LABEL:
        g_value_set_string (value, device->priv->label);
        break;
    case PROP_ICON:
        g_value_set_string (value, device->priv->icon);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
        break;
    }
}

static void
cafe_mixer_device_set_property (GObject      *object,
                                guint         param_id,
                                const GValue *value,
                                GParamSpec   *pspec)
{
    CafeMixerDevice *device;

    device = CAFE_MIXER_DEVICE (object);

    switch (param_id) {
    case PROP_NAME:
        /* Construct-only string */
        device->priv->name = g_value_dup_string (value);
        break;
    case PROP_LABEL:
        /* Construct-only string */
        device->priv->label = g_value_dup_string (value);
        break;
    case PROP_ICON:
        /* Construct-only string */
        device->priv->icon = g_value_dup_string (value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
        break;
    }
}

static void
cafe_mixer_device_init (CafeMixerDevice *device)
{
    device->priv = cafe_mixer_device_get_instance_private (device);
}

static void
cafe_mixer_device_finalize (GObject *object)
{
    CafeMixerDevice *device;

    device = CAFE_MIXER_DEVICE (object);

    g_free (device->priv->name);
    g_free (device->priv->label);
    g_free (device->priv->icon);

    G_OBJECT_CLASS (cafe_mixer_device_parent_class)->finalize (object);
}

/**
 * cafe_mixer_device_get_name:
 * @device: a #CafeMixerDevice
 *
 * Gets the name of the device.
 *
 * The name serves as a unique identifier and in most cases it is not in a
 * user-readable form.
 *
 * The returned name is guaranteed to be unique across all the known devices
 * and may be used to get the #CafeMixerDevice using
 * cafe_mixer_context_get_device().
 *
 * Returns: the name of the device.
 */
const gchar *
cafe_mixer_device_get_name (CafeMixerDevice *device)
{
    g_return_val_if_fail (CAFE_MIXER_IS_DEVICE (device), NULL);

    return device->priv->name;
}

/**
 * cafe_mixer_device_get_label:
 * @device: a #CafeMixerDevice
 *
 * Gets the label of the device.
 *
 * This is a potentially translated string that should be presented to users
 * in the user interface.
 *
 * Returns: the label of the device.
 */
const gchar *
cafe_mixer_device_get_label (CafeMixerDevice *device)
{
    g_return_val_if_fail (CAFE_MIXER_IS_DEVICE (device), NULL);

    return device->priv->label;
}

/**
 * cafe_mixer_device_get_icon:
 * @device: a #CafeMixerDevice
 *
 * Gets the XDG icon name of the device.
 *
 * Returns: the icon name or %NULL.
 */
const gchar *
cafe_mixer_device_get_icon (CafeMixerDevice *device)
{
    g_return_val_if_fail (CAFE_MIXER_IS_DEVICE (device), NULL);

    return device->priv->icon;
}

/**
 * cafe_mixer_device_get_stream:
 * @device: a #CafeMixerDevice
 * @name: a stream name
 *
 * Gets the device stream with the given name.
 *
 * Returns: a #CafeMixerStream or %NULL if there is no such stream.
 */
CafeMixerStream *
cafe_mixer_device_get_stream (CafeMixerDevice *device, const gchar *name)
{
    g_return_val_if_fail (CAFE_MIXER_IS_DEVICE (device), NULL);
    g_return_val_if_fail (name != NULL, NULL);

    return CAFE_MIXER_DEVICE_GET_CLASS (device)->get_stream (device, name);
}

/**
 * cafe_mixer_device_get_switch:
 * @device: a #CafeMixerDevice
 * @name: a switch name
 *
 * Gets the device switch with the given name.
 *
 * Note that this function will only return a switch that belongs to the device
 * and not to a stream of the device. See cafe_mixer_device_list_switches() for
 * information about the difference between device and stream switches.
 *
 * To get a stream switch, rather than a device switch, use
 * cafe_mixer_stream_get_switch().
 *
 * Returns: a #CafeMixerDeviceSwitch or %NULL if there is no such device switch.
 */
CafeMixerDeviceSwitch *
cafe_mixer_device_get_switch (CafeMixerDevice *device, const gchar *name)
{
    g_return_val_if_fail (CAFE_MIXER_IS_DEVICE (device), NULL);
    g_return_val_if_fail (name != NULL, NULL);

    return CAFE_MIXER_DEVICE_GET_CLASS (device)->get_switch (device, name);
}

/**
 * cafe_mixer_device_list_streams:
 * @device: a #CafeMixerDevice
 *
 * Gets the list of streams that belong to the device.
 *
 * The returned #GList is owned by the #CafeMixerDevice and may be invalidated
 * at any time.
 *
 * Returns: a #GList of the device streams or %NULL if the device does not have
 * any streams.
 */
const GList *
cafe_mixer_device_list_streams (CafeMixerDevice *device)
{
    CafeMixerDeviceClass *klass;

    g_return_val_if_fail (CAFE_MIXER_IS_DEVICE (device), NULL);

    klass = CAFE_MIXER_DEVICE_GET_CLASS (device);

    if (G_LIKELY (klass->list_streams != NULL))
        return klass->list_streams (device);

    return NULL;
}

/**
 * cafe_mixer_device_list_switches:
 * @device: a #CafeMixerDevice
 *
 * Gets the list of switches the belong to the device.
 *
 * Note that a switch may belong either to a device, or to a stream. Unlike
 * stream switches, device switches returned by this function are not classified
 * as input or output (as streams are), but they operate on the whole device.
 *
 * Use cafe_mixer_stream_list_switches() to get a list of switches that belong
 * to a stream.
 *
 * The returned #GList is owned by the #CafeMixerDevice and may be invalidated
 * at any time.
 *
 * Returns: a #GList of the device switches or %NULL if the device does not have
 * any switches.
 */
const GList *
cafe_mixer_device_list_switches (CafeMixerDevice *device)
{
    CafeMixerDeviceClass *klass;

    g_return_val_if_fail (CAFE_MIXER_IS_DEVICE (device), NULL);

    klass = CAFE_MIXER_DEVICE_GET_CLASS (device);

    if (G_LIKELY (klass->list_switches != NULL))
        return klass->list_switches (device);

    return NULL;
}

static CafeMixerStream *
cafe_mixer_device_real_get_stream (CafeMixerDevice *device, const gchar *name)
{
    const GList *list;

    g_return_val_if_fail (CAFE_MIXER_IS_DEVICE (device), NULL);
    g_return_val_if_fail (name != NULL, NULL);

    list = cafe_mixer_device_list_streams (device);
    while (list != NULL) {
        CafeMixerStream *stream = CAFE_MIXER_STREAM (list->data);

        if (strcmp (name, cafe_mixer_stream_get_name (stream)) == 0)
            return stream;

        list = list->next;
    }
    return NULL;
}

static CafeMixerDeviceSwitch *
cafe_mixer_device_real_get_switch (CafeMixerDevice *device, const gchar *name)
{
    const GList *list;

    g_return_val_if_fail (CAFE_MIXER_IS_DEVICE (device), NULL);
    g_return_val_if_fail (name != NULL, NULL);

    list = cafe_mixer_device_list_switches (device);
    while (list != NULL) {
        CafeMixerSwitch *swtch = CAFE_MIXER_SWITCH (list->data);

        if (strcmp (name, cafe_mixer_switch_get_name (swtch)) == 0)
            return CAFE_MIXER_DEVICE_SWITCH (swtch);

        list = list->next;
    }
    return NULL;
}
