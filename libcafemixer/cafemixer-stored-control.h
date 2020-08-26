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

#ifndef CAFEMIXER_STORED_CONTROL_H
#define CAFEMIXER_STORED_CONTROL_H

#include <glib.h>
#include <glib-object.h>

#include <libcafemixer/cafemixer-enums.h>
#include <libcafemixer/cafemixer-stream-control.h>
#include <libcafemixer/cafemixer-types.h>

G_BEGIN_DECLS

#define CAFE_MIXER_TYPE_STORED_CONTROL          \
        (cafe_mixer_stored_control_get_type ())
#define CAFE_MIXER_STORED_CONTROL(o)            \
        (G_TYPE_CHECK_INSTANCE_CAST ((o), CAFE_MIXER_TYPE_STORED_CONTROL, CafeMixerStoredControl))
#define CAFE_MIXER_IS_STORED_CONTROL(o)         \
        (G_TYPE_CHECK_INSTANCE_TYPE ((o), CAFE_MIXER_TYPE_STORED_CONTROL))
#define CAFE_MIXER_STORED_CONTROL_CLASS(k)      \
        (G_TYPE_CHECK_CLASS_CAST ((k), CAFE_MIXER_TYPE_STORED_CONTROL, CafeMixerStoredControlClass))
#define CAFE_MIXER_IS_STORED_CONTROL_CLASS(k)   \
        (G_TYPE_CHECK_CLASS_TYPE ((k), CAFE_MIXER_TYPE_STORED_CONTROL))
#define CAFE_MIXER_STORED_CONTROL_GET_CLASS(o)  \
        (G_TYPE_INSTANCE_GET_CLASS ((o), CAFE_MIXER_TYPE_STORED_CONTROL, CafeMixerStoredControlClass))

typedef struct _CafeMixerStoredControlClass    CafeMixerStoredControlClass;
typedef struct _CafeMixerStoredControlPrivate  CafeMixerStoredControlPrivate;

/**
 * CafeMixerStoredControl:
 *
 * The #CafeMixerStoredControl structure contains only private data and should only
 * be accessed using the provided API.
 */
struct _CafeMixerStoredControl
{
    CafeMixerStreamControl object;

    /*< private >*/
    CafeMixerStoredControlPrivate *priv;
};

/**
 * CafeMixerStoredControlClass:
 * @parent_class: The parent class.
 *
 * The class structure for #CafeMixerStoredControl.
 */
struct _CafeMixerStoredControlClass
{
    CafeMixerStreamControlClass parent_class;
};

GType              cafe_mixer_stored_control_get_type      (void) G_GNUC_CONST;

CafeMixerDirection cafe_mixer_stored_control_get_direction (CafeMixerStoredControl *control);

G_END_DECLS

#endif /* CAFEMIXER_STORED_CONTROL_H */
