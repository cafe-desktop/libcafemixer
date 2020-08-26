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
#include "cafemixer-switch.h"
#include "cafemixer-stream-switch.h"

/**
 * SECTION:cafemixer-stream-switch
 * @include: libcafemixer/cafemixer.h
 */

struct _CafeMixerStreamSwitchPrivate
{
    CafeMixerStream           *stream;
    CafeMixerStreamSwitchFlags flags;
    CafeMixerStreamSwitchRole  role;
};

enum {
    PROP_0,
    PROP_FLAGS,
    PROP_ROLE,
    PROP_STREAM,
    N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES] = { NULL, };

static void cafe_mixer_stream_switch_get_property (GObject                    *object,
                                                   guint                       param_id,
                                                   GValue                     *value,
                                                   GParamSpec                 *pspec);
static void cafe_mixer_stream_switch_set_property (GObject                    *object,
                                                   guint                       param_id,
                                                   const GValue               *value,
                                                   GParamSpec                 *pspec);

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE (CafeMixerStreamSwitch, cafe_mixer_stream_switch, CAFE_MIXER_TYPE_SWITCH)

static void
cafe_mixer_stream_switch_class_init (CafeMixerStreamSwitchClass *klass)
{
    GObjectClass *object_class;

    object_class = G_OBJECT_CLASS (klass);
    object_class->get_property = cafe_mixer_stream_switch_get_property;
    object_class->set_property = cafe_mixer_stream_switch_set_property;

    properties[PROP_FLAGS] =
        g_param_spec_flags ("flags",
                            "Flags",
                            "Flags of the switch",
                            CAFE_MIXER_TYPE_STREAM_SWITCH_FLAGS,
                            CAFE_MIXER_STREAM_SWITCH_NO_FLAGS,
                            G_PARAM_READWRITE |
                            G_PARAM_CONSTRUCT_ONLY |
                            G_PARAM_STATIC_STRINGS);

    properties[PROP_ROLE] =
        g_param_spec_enum ("role",
                           "Role",
                           "Role of the switch",
                           CAFE_MIXER_TYPE_STREAM_SWITCH_ROLE,
                           CAFE_MIXER_STREAM_SWITCH_ROLE_UNKNOWN,
                           G_PARAM_READWRITE |
                           G_PARAM_CONSTRUCT_ONLY |
                           G_PARAM_STATIC_STRINGS);

    properties[PROP_STREAM] =
        g_param_spec_object ("stream",
                             "Stream",
                             "Stream owning the switch",
                             CAFE_MIXER_TYPE_STREAM,
                             G_PARAM_READWRITE |
                             G_PARAM_CONSTRUCT_ONLY |
                             G_PARAM_STATIC_STRINGS);

    g_object_class_install_properties (object_class, N_PROPERTIES, properties);
}

static void
cafe_mixer_stream_switch_get_property (GObject    *object,
                                       guint       param_id,
                                       GValue     *value,
                                       GParamSpec *pspec)
{
    CafeMixerStreamSwitch *swtch;

    swtch = CAFE_MIXER_STREAM_SWITCH (object);

    switch (param_id) {
    case PROP_FLAGS:
        g_value_set_flags (value, swtch->priv->flags);
        break;
    case PROP_ROLE:
        g_value_set_enum (value, swtch->priv->role);
        break;
    case PROP_STREAM:
        g_value_set_object (value, swtch->priv->stream);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
        break;
    }
}

static void
cafe_mixer_stream_switch_set_property (GObject      *object,
                                       guint         param_id,
                                       const GValue *value,
                                       GParamSpec   *pspec)
{
    CafeMixerStreamSwitch *swtch;

    swtch = CAFE_MIXER_STREAM_SWITCH (object);

    switch (param_id) {
    case PROP_FLAGS:
        swtch->priv->flags = g_value_get_flags (value);
        break;
    case PROP_ROLE:
        swtch->priv->role = g_value_get_enum (value);
        break;
    case PROP_STREAM:
        /* Construct-only object */
        swtch->priv->stream = g_value_get_object (value);

        if (swtch->priv->stream != NULL)
            g_object_add_weak_pointer (G_OBJECT (swtch->priv->stream),
                                       (gpointer *) &swtch->priv->stream);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
        break;
    }
}

static void
cafe_mixer_stream_switch_init (CafeMixerStreamSwitch *swtch)
{
    swtch->priv = cafe_mixer_stream_switch_get_instance_private (swtch);
}

/**
 * cafe_mixer_stream_switch_get_flags:
 * @swtch: a #CafeMixerStreamSwitch
 *
 * Gets the flags of the switch. See #CafeMixerStreamSwitchFlags for information
 * about the meaning of the individual flags.
 *
 * Returns: the flags of the switch.
 */
CafeMixerStreamSwitchFlags
cafe_mixer_stream_switch_get_flags (CafeMixerStreamSwitch *swtch)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_SWITCH (swtch), CAFE_MIXER_STREAM_SWITCH_NO_FLAGS);

    return swtch->priv->flags;
}

/**
 * cafe_mixer_stream_switch_get_role:
 * @swtch: a #CafeMixerStreamSwitch
 *
 * Gets the role of the switch. The role identifies the purpose of the switch.
 *
 * Returns: the switch role.
 */
CafeMixerStreamSwitchRole
cafe_mixer_stream_switch_get_role (CafeMixerStreamSwitch *swtch)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_SWITCH (swtch), CAFE_MIXER_STREAM_SWITCH_ROLE_UNKNOWN);

    return swtch->priv->role;
}

/**
 * cafe_mixer_stream_switch_get_stream:
 * @swtch: a #CafeMixerStreamSwitch
 */
CafeMixerStream *
cafe_mixer_stream_switch_get_stream (CafeMixerStreamSwitch *swtch)
{
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM_SWITCH (swtch), NULL);

    return swtch->priv->stream;
}
