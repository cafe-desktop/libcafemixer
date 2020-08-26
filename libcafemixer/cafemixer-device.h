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

#ifndef CAFEMIXER_DEVICE_H
#define CAFEMIXER_DEVICE_H

#include <glib.h>
#include <glib-object.h>

#include <libcafemixer/cafemixer-types.h>

G_BEGIN_DECLS

#define CAFE_MIXER_TYPE_DEVICE                  \
        (cafe_mixer_device_get_type ())
#define CAFE_MIXER_DEVICE(o)                    \
        (G_TYPE_CHECK_INSTANCE_CAST ((o), CAFE_MIXER_TYPE_DEVICE, CafeMixerDevice))
#define CAFE_MIXER_IS_DEVICE(o)                 \
        (G_TYPE_CHECK_INSTANCE_TYPE ((o), CAFE_MIXER_TYPE_DEVICE))
#define CAFE_MIXER_DEVICE_CLASS(k)              \
        (G_TYPE_CHECK_CLASS_CAST ((k), CAFE_MIXER_TYPE_DEVICE, CafeMixerDeviceClass))
#define CAFE_MIXER_IS_DEVICE_CLASS(k)           \
        (G_TYPE_CHECK_CLASS_TYPE ((k), CAFE_MIXER_TYPE_DEVICE))
#define CAFE_MIXER_DEVICE_GET_CLASS(o)          \
        (G_TYPE_INSTANCE_GET_CLASS ((o), CAFE_MIXER_TYPE_DEVICE, CafeMixerDeviceClass))

typedef struct _CafeMixerDeviceClass    CafeMixerDeviceClass;
typedef struct _CafeMixerDevicePrivate  CafeMixerDevicePrivate;

/**
 * CafeMixerDevice:
 *
 * The #CafeMixerDevice structure contains only private data and should only
 * be accessed using the provided API.
 */
struct _CafeMixerDevice
{
    GObject object;

    /*< private >*/
    CafeMixerDevicePrivate *priv;
};

/**
 * CafeMixerDeviceClass:
 * @parent_class: The parent class.
 *
 * The class structure for #CafeMixerDevice.
 */
struct _CafeMixerDeviceClass
{
    GObjectClass parent_class;

    /*< private >*/
    CafeMixerStream       *(*get_stream)    (CafeMixerDevice *device,
                                             const gchar     *name);
    CafeMixerDeviceSwitch *(*get_switch)    (CafeMixerDevice *device,
                                             const gchar     *name);

    const GList           *(*list_streams)  (CafeMixerDevice *device);
    const GList           *(*list_switches) (CafeMixerDevice *device);

    /* Signals */
    void (*stream_added)   (CafeMixerDevice *device,
                            const gchar     *name);
    void (*stream_removed) (CafeMixerDevice *device,
                            const gchar     *name);
    void (*switch_added)   (CafeMixerDevice *device,
                            const gchar     *name);
    void (*switch_removed) (CafeMixerDevice *device,
                            const gchar     *name);
};

GType                  cafe_mixer_device_get_type      (void) G_GNUC_CONST;

const gchar *          cafe_mixer_device_get_name      (CafeMixerDevice *device);
const gchar *          cafe_mixer_device_get_label     (CafeMixerDevice *device);
const gchar *          cafe_mixer_device_get_icon      (CafeMixerDevice *device);

CafeMixerStream *      cafe_mixer_device_get_stream    (CafeMixerDevice *device,
                                                        const gchar     *name);

CafeMixerDeviceSwitch *cafe_mixer_device_get_switch    (CafeMixerDevice *device,
                                                        const gchar     *name);

const GList *          cafe_mixer_device_list_streams  (CafeMixerDevice *device);
const GList *          cafe_mixer_device_list_switches (CafeMixerDevice *device);

G_END_DECLS

#endif /* CAFEMIXER_DEVICE_H */
