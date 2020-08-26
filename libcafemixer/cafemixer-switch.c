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

#include "cafemixer-enums.h"
#include "cafemixer-enum-types.h"
#include "cafemixer-switch.h"
#include "cafemixer-switch-private.h"
#include "cafemixer-switch-option.h"

/**
 * SECTION:cafemixer-switch
 * @include: libcafemixer/cafemixer.h
 */

struct _CafeMixerSwitchPrivate
{
    gchar                 *name;
    gchar                 *label;
    CafeMixerSwitchOption *active;
};

enum {
    PROP_0,
    PROP_NAME,
    PROP_LABEL,
    PROP_ACTIVE_OPTION,
    N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES] = { NULL, };

static void cafe_mixer_switch_get_property (GObject              *object,
                                            guint                 param_id,
                                            GValue               *value,
                                            GParamSpec           *pspec);
static void cafe_mixer_switch_set_property (GObject              *object,
                                            guint                 param_id,
                                            const GValue         *value,
                                            GParamSpec           *pspec);

static void cafe_mixer_switch_dispose      (GObject              *object);
static void cafe_mixer_switch_finalize     (GObject              *object);

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE (CafeMixerSwitch, cafe_mixer_switch, G_TYPE_OBJECT)

static CafeMixerSwitchOption *cafe_mixer_switch_real_get_option (CafeMixerSwitch *swtch,
                                                                 const gchar     *name);

static void
cafe_mixer_switch_class_init (CafeMixerSwitchClass *klass)
{
    GObjectClass *object_class;

    klass->get_option = cafe_mixer_switch_real_get_option;

    object_class = G_OBJECT_CLASS (klass);
    object_class->dispose      = cafe_mixer_switch_dispose;
    object_class->finalize     = cafe_mixer_switch_finalize;
    object_class->get_property = cafe_mixer_switch_get_property;
    object_class->set_property = cafe_mixer_switch_set_property;

    properties[PROP_NAME] =
        g_param_spec_string ("name",
                             "Name",
                             "Name of the switch",
                             NULL,
                             G_PARAM_READWRITE |
                             G_PARAM_CONSTRUCT_ONLY |
                             G_PARAM_STATIC_STRINGS);

    properties[PROP_LABEL] =
        g_param_spec_string ("label",
                             "Label",
                             "Label of the switch",
                             NULL,
                             G_PARAM_READWRITE |
                             G_PARAM_CONSTRUCT_ONLY |
                             G_PARAM_STATIC_STRINGS);

    properties[PROP_ACTIVE_OPTION] =
        g_param_spec_object ("active-option",
                             "Active option",
                             "Active option of the switch",
                             CAFE_MIXER_TYPE_SWITCH_OPTION,
                             G_PARAM_READWRITE |
                             G_PARAM_CONSTRUCT_ONLY |
                             G_PARAM_STATIC_STRINGS);

    g_object_class_install_properties (object_class, N_PROPERTIES, properties);
}

static void
cafe_mixer_switch_get_property (GObject    *object,
                                guint       param_id,
                                GValue     *value,
                                GParamSpec *pspec)
{
    CafeMixerSwitch *swtch;

    swtch = CAFE_MIXER_SWITCH (object);

    switch (param_id) {
    case PROP_NAME:
        g_value_set_string (value, swtch->priv->name);
        break;
    case PROP_LABEL:
        g_value_set_string (value, swtch->priv->label);
        break;
    case PROP_ACTIVE_OPTION:
        g_value_set_object (value, swtch->priv->active);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
        break;
    }
}

static void
cafe_mixer_switch_set_property (GObject      *object,
                                guint         param_id,
                                const GValue *value,
                                GParamSpec   *pspec)
{
    CafeMixerSwitch *swtch;

    swtch = CAFE_MIXER_SWITCH (object);

    switch (param_id) {
    case PROP_NAME:
        /* Construct-only string */
        swtch->priv->name = g_value_dup_string (value);
        break;
    case PROP_LABEL:
        /* Construct-only string */
        swtch->priv->label = g_value_dup_string (value);
        break;
    case PROP_ACTIVE_OPTION:
        /* Construct-only object */
        swtch->priv->active = g_value_dup_object (value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
        break;
    }
}

static void
cafe_mixer_switch_init (CafeMixerSwitch *swtch)
{
    swtch->priv = cafe_mixer_switch_get_instance_private (swtch);
}

static void
cafe_mixer_switch_dispose (GObject *object)
{
    CafeMixerSwitch *swtch;

    swtch = CAFE_MIXER_SWITCH (object);

    g_clear_object (&swtch->priv->active);

    G_OBJECT_CLASS (cafe_mixer_switch_parent_class)->dispose (object);
}

static void
cafe_mixer_switch_finalize (GObject *object)
{
    CafeMixerSwitch *swtch;

    swtch = CAFE_MIXER_SWITCH (object);

    g_free (swtch->priv->name);
    g_free (swtch->priv->label);

    G_OBJECT_CLASS (cafe_mixer_switch_parent_class)->finalize (object);
}

/**
 * cafe_mixer_switch_get_name:
 * @swtch: a #CafeMixerSwitch
 *
 * Gets the name of the switch.
 *
 * The name serves as a unique identifier and in most cases it is not in a
 * user-readable form.
 *
 * Returns: the name of the switch.
 */
const gchar *
cafe_mixer_switch_get_name (CafeMixerSwitch *swtch)
{
    g_return_val_if_fail (CAFE_MIXER_IS_SWITCH (swtch), NULL);

    return swtch->priv->name;
}

/**
 * cafe_mixer_switch_get_label:
 * @swtch: a #CafeMixerSwitch
 *
 * Gets the label of the switch.
 *
 * This is a potentially translated string that should be presented to users
 * in the user interface.
 *
 * Returns: the label of the switch option.
 */
const gchar *
cafe_mixer_switch_get_label (CafeMixerSwitch *swtch)
{
    g_return_val_if_fail (CAFE_MIXER_IS_SWITCH (swtch), NULL);

    return swtch->priv->label;
}

/**
 * cafe_mixer_switch_get_option:
 * @swtch: a #CafeMixerSwitch
 * @name: the name of an option
 *
 * Gets the #CafeMixerSwitchOption with the given name.
 *
 * Returns: a #CafeMixerSwitchOption or %NULL if there is no such switch option.
 */
CafeMixerSwitchOption *
cafe_mixer_switch_get_option (CafeMixerSwitch *swtch, const gchar *name)
{
    g_return_val_if_fail (CAFE_MIXER_IS_SWITCH (swtch), NULL);
    g_return_val_if_fail (name != NULL, NULL);

    return CAFE_MIXER_SWITCH_GET_CLASS (swtch)->get_option (swtch, name);
}

/**
 * cafe_mixer_switch_get_active_option:
 * @swtch: a #CafeMixerSwitch
 *
 * Gets the #CafeMixerSwitchOption which is currently active.
 *
 * Returns: a #CafeMixerSwitchOption.
 */
CafeMixerSwitchOption *
cafe_mixer_switch_get_active_option (CafeMixerSwitch *swtch)
{
    g_return_val_if_fail (CAFE_MIXER_IS_SWITCH (swtch), NULL);

    return swtch->priv->active;
}

/**
 * cafe_mixer_switch_set_active_option:
 * @swtch: a #CafeMixerSwitch
 * @option: the #CafeMixerSwitchOption to set as the active option
 *
 * Changes the currently active switch option.
 *
 * Returns: %TRUE on success or %FALSE on failure.
 */
gboolean
cafe_mixer_switch_set_active_option (CafeMixerSwitch       *swtch,
                                     CafeMixerSwitchOption *option)
{
    CafeMixerSwitchClass *klass;

    g_return_val_if_fail (CAFE_MIXER_IS_SWITCH (swtch), FALSE);
    g_return_val_if_fail (CAFE_MIXER_IS_SWITCH_OPTION (option), FALSE);

    klass = CAFE_MIXER_SWITCH_GET_CLASS (swtch);
    if (klass->set_active_option == NULL)
        return FALSE;

    if (swtch->priv->active != option) {
        if (klass->set_active_option (swtch, option) == FALSE)
            return FALSE;

        _cafe_mixer_switch_set_active_option (swtch, option);
    }
    return TRUE;
}

/**
 * cafe_mixer_switch_list_options:
 * @swtch: a #CafeMixerSwitch
 *
 * Gets the list of switch options that belong to the switch.
 *
 * The returned #GList is owned by the #CafeMixerSwitch and may be invalidated
 * at any time.
 *
 * Returns: a #GList of the switch options.
 */
const GList *
cafe_mixer_switch_list_options (CafeMixerSwitch *swtch)
{
    g_return_val_if_fail (CAFE_MIXER_IS_SWITCH (swtch), NULL);

    return CAFE_MIXER_SWITCH_GET_CLASS (swtch)->list_options (swtch);
}

/* Protected functions */
void
_cafe_mixer_switch_set_active_option (CafeMixerSwitch       *swtch,
                                      CafeMixerSwitchOption *option)
{
    g_return_if_fail (CAFE_MIXER_IS_SWITCH (swtch));
    g_return_if_fail (CAFE_MIXER_IS_SWITCH_OPTION (option));

    if (swtch->priv->active == option)
        return;

    if (swtch->priv->active != NULL)
        g_object_unref (swtch->priv->active);

    swtch->priv->active = g_object_ref (option);

    g_object_notify_by_pspec (G_OBJECT (swtch), properties[PROP_ACTIVE_OPTION]);
}

static CafeMixerSwitchOption *
cafe_mixer_switch_real_get_option (CafeMixerSwitch *swtch, const gchar *name)
{
    const GList *list;

    g_return_val_if_fail (CAFE_MIXER_IS_SWITCH (swtch), NULL);
    g_return_val_if_fail (name != NULL, NULL);

    list = cafe_mixer_switch_list_options (swtch);
    while (list != NULL) {
        CafeMixerSwitchOption *option = CAFE_MIXER_SWITCH_OPTION (list->data);

        if (strcmp (name, cafe_mixer_switch_option_get_name (option)) == 0)
            return option;

        list = list->next;
    }
    return NULL;
}
