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

#ifndef CAFEMIXER_DEVICE_SWITCH_H
#define CAFEMIXER_DEVICE_SWITCH_H

#include <glib.h>
#include <glib-object.h>

#include <libcafemixer/cafemixer-switch.h>
#include <libcafemixer/cafemixer-types.h>

G_BEGIN_DECLS

#define CAFE_MIXER_TYPE_DEVICE_SWITCH           \
        (cafe_mixer_device_switch_get_type ())
#define CAFE_MIXER_DEVICE_SWITCH(o)             \
        (G_TYPE_CHECK_INSTANCE_CAST ((o), CAFE_MIXER_TYPE_DEVICE_SWITCH, MateMixerDeviceSwitch))
#define CAFE_MIXER_IS_DEVICE_SWITCH(o)          \
        (G_TYPE_CHECK_INSTANCE_TYPE ((o), CAFE_MIXER_TYPE_DEVICE_SWITCH))
#define CAFE_MIXER_DEVICE_SWITCH_CLASS(k)      \
        (G_TYPE_CHECK_CLASS_CAST ((k), CAFE_MIXER_TYPE_DEVICE_SWITCH, MateMixerDeviceSwitchClass))
#define CAFE_MIXER_IS_DEVICE_SWITCH_CLASS(k)   \
        (G_TYPE_CHECK_CLASS_TYPE ((k), CAFE_MIXER_TYPE_DEVICE_SWITCH))
#define CAFE_MIXER_DEVICE_SWITCH_GET_CLASS(o)  \
        (G_TYPE_INSTANCE_GET_CLASS ((o), CAFE_MIXER_TYPE_DEVICE_SWITCH, MateMixerDeviceSwitchClass))

typedef struct _MateMixerDeviceSwitchClass    MateMixerDeviceSwitchClass;
typedef struct _MateMixerDeviceSwitchPrivate  MateMixerDeviceSwitchPrivate;

/**
 * MateMixerDeviceSwitch:
 *
 * The #MateMixerDeviceSwitch structure contains only private data and should only
 * be accessed using the provided API.
 */
struct _MateMixerDeviceSwitch
{
    MateMixerSwitch object;

    /*< private >*/
    MateMixerDeviceSwitchPrivate *priv;
};

/**
 * MateMixerDeviceSwitchClass:
 * @parent_class: The parent class.
 *
 * The class structure for #MateMixerDeviceSwitch.
 */
struct _MateMixerDeviceSwitchClass
{
    MateMixerSwitchClass parent_class;
};

GType                     cafe_mixer_device_switch_get_type   (void) G_GNUC_CONST;

MateMixerDeviceSwitchRole cafe_mixer_device_switch_get_role   (MateMixerDeviceSwitch *swtch);
MateMixerDevice *         cafe_mixer_device_switch_get_device (MateMixerDeviceSwitch *swtch);

G_END_DECLS

#endif /* CAFEMIXER_DEVICE_SWITCH_H */
