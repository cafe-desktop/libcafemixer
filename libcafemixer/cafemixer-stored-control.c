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
#include "cafemixer-stream-control.h"
#include "cafemixer-stored-control.h"

struct _CafeMixerStoredControlPrivate
{
    CafeMixerDirection direction;
};

enum {
    PROP_0,
    PROP_DIRECTION,
    N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES] = { NULL, };

static void cafe_mixer_stored_control_get_property (GObject                     *object,
                                                    guint                        param_id,
                                                    GValue                      *value,
                                                    GParamSpec                  *pspec);
static void cafe_mixer_stored_control_set_property (GObject                     *object,
                                                    guint                        param_id,
                                                    const GValue                *value,
                                                    GParamSpec                  *pspec);

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE (CafeMixerStoredControl, cafe_mixer_stored_control, CAFE_MIXER_TYPE_STREAM_CONTROL)

static void
cafe_mixer_stored_control_class_init (CafeMixerStoredControlClass *klass)
{
    GObjectClass *object_class;

    object_class = G_OBJECT_CLASS (klass);
    object_class->get_property = cafe_mixer_stored_control_get_property;
    object_class->set_property = cafe_mixer_stored_control_set_property;

    properties[PROP_DIRECTION] =
        g_param_spec_enum ("direction",
                           "Direction",
                           "Direction of the stored control",
                           CAFE_MIXER_TYPE_DIRECTION,
                           CAFE_MIXER_DIRECTION_UNKNOWN,
                           G_PARAM_READWRITE |
                           G_PARAM_CONSTRUCT_ONLY |
                           G_PARAM_STATIC_STRINGS);

    g_object_class_install_properties (object_class, N_PROPERTIES, properties);
}

static void
cafe_mixer_stored_control_get_property (GObject    *object,
                                        guint       param_id,
                                        GValue     *value,
                                        GParamSpec *pspec)
{
    CafeMixerStoredControl *control;

    control = CAFE_MIXER_STORED_CONTROL (object);

    switch (param_id) {
    case PROP_DIRECTION:
        g_value_set_enum (value, control->priv->direction);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
        break;
    }
}

static void
cafe_mixer_stored_control_set_property (GObject      *object,
                                        guint         param_id,
                                        const GValue *value,
                                        GParamSpec   *pspec)
{
    CafeMixerStoredControl *control;

    control = CAFE_MIXER_STORED_CONTROL (object);

    switch (param_id) {
    case PROP_DIRECTION:
        control->priv->direction = g_value_get_enum (value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
        break;
    }
}

static void
cafe_mixer_stored_control_init (CafeMixerStoredControl *control)
{
    control->priv = cafe_mixer_stored_control_get_instance_private (control);
}

/**
 * cafe_mixer_stored_control_get_direction:
 * @control: a #CafeMixerStoredControl
 */
CafeMixerDirection
cafe_mixer_stored_control_get_direction (CafeMixerStoredControl *control)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STORED_CONTROL (control), CAFE_MIXER_DIRECTION_UNKNOWN);

    return control->priv->direction;
}
