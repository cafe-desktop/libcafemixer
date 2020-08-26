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

#include "cafemixer-device.h"
#include "cafemixer-enums.h"
#include "cafemixer-enum-types.h"
#include "cafemixer-switch.h"
#include "cafemixer-device-switch.h"

/**
 * SECTION:cafemixer-device-switch
 * @include: libcafemixer/cafemixer.h
 */

struct _CafeMixerDeviceSwitchPrivate
{
    CafeMixerDevice          *device;
    CafeMixerDeviceSwitchRole role;
};

enum {
    PROP_0,
    PROP_ROLE,
    PROP_DEVICE,
    N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES] = { NULL, };

static void cafe_mixer_device_switch_get_property (GObject                    *object,
                                                   guint                       param_id,
                                                   GValue                     *value,
                                                   GParamSpec                 *pspec);
static void cafe_mixer_device_switch_set_property (GObject                    *object,
                                                   guint                       param_id,
                                                   const GValue               *value,
                                                   GParamSpec                 *pspec);

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE (CafeMixerDeviceSwitch, cafe_mixer_device_switch, CAFE_MIXER_TYPE_SWITCH)

static void
cafe_mixer_device_switch_class_init (CafeMixerDeviceSwitchClass *klass)
{
    GObjectClass *object_class;

    object_class = G_OBJECT_CLASS (klass);
    object_class->get_property = cafe_mixer_device_switch_get_property;
    object_class->set_property = cafe_mixer_device_switch_set_property;

    properties[PROP_ROLE] =
        g_param_spec_enum ("role",
                           "Role",
                           "Role of the switch",
                           CAFE_MIXER_TYPE_DEVICE_SWITCH_ROLE,
                           CAFE_MIXER_DEVICE_SWITCH_ROLE_UNKNOWN,
                           G_PARAM_READWRITE |
                           G_PARAM_CONSTRUCT_ONLY |
                           G_PARAM_STATIC_STRINGS);

    properties[PROP_DEVICE] =
        g_param_spec_object ("device",
                             "Device",
                             "Device owning the switch",
                             CAFE_MIXER_TYPE_DEVICE,
                             G_PARAM_READWRITE |
                             G_PARAM_CONSTRUCT_ONLY |
                             G_PARAM_STATIC_STRINGS);

    g_object_class_install_properties (object_class, N_PROPERTIES, properties);
}

static void
cafe_mixer_device_switch_get_property (GObject    *object,
                                       guint       param_id,
                                       GValue     *value,
                                       GParamSpec *pspec)
{
    CafeMixerDeviceSwitch *swtch;

    swtch = CAFE_MIXER_DEVICE_SWITCH (object);

    switch (param_id) {
    case PROP_ROLE:
        g_value_set_enum (value, swtch->priv->role);
        break;
    case PROP_DEVICE:
        g_value_set_object (value, swtch->priv->device);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
        break;
    }
}

static void
cafe_mixer_device_switch_set_property (GObject      *object,
                                       guint         param_id,
                                       const GValue *value,
                                       GParamSpec   *pspec)
{
    CafeMixerDeviceSwitch *swtch;

    swtch = CAFE_MIXER_DEVICE_SWITCH (object);

    switch (param_id) {
    case PROP_ROLE:
        swtch->priv->role = g_value_get_enum (value);
        break;
    case PROP_DEVICE:
        /* Construct-only object */
        swtch->priv->device = g_value_get_object (value);

        if (swtch->priv->device != NULL)
            g_object_add_weak_pointer (G_OBJECT (swtch->priv->device),
                                       (gpointer *) &swtch->priv->device);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
        break;
    }
}

static void
cafe_mixer_device_switch_init (CafeMixerDeviceSwitch *swtch)
{
    swtch->priv = cafe_mixer_device_switch_get_instance_private (swtch);
}

/**
 * cafe_mixer_device_switch_get_role:
 * @swtch: a #CafeMixerDeviceSwitch
 *
 * Gets the role of the switch. The role identifies the purpose of the switch.
 *
 * Returns: the switch role.
 */
CafeMixerDeviceSwitchRole
cafe_mixer_device_switch_get_role (CafeMixerDeviceSwitch *swtch)
{
    g_return_val_if_fail (CAFE_MIXER_IS_DEVICE_SWITCH (swtch), CAFE_MIXER_DEVICE_SWITCH_ROLE_UNKNOWN);

    return swtch->priv->role;
}

/**
 * cafe_mixer_device_switch_get_device:
 * @swtch: a #CafeMixerDeviceSwitch
 */
CafeMixerDevice *
cafe_mixer_device_switch_get_device (CafeMixerDeviceSwitch *swtch)
{
    g_return_val_if_fail (CAFE_MIXER_IS_DEVICE_SWITCH (swtch), NULL);

    return swtch->priv->device;
}
