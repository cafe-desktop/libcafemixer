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
#include <libcafemixer/cafemixer.h>
#include <libcafemixer/cafemixer-private.h>

#include "alsa-device.h"
#include "alsa-element.h"
#include "alsa-stream.h"
#include "alsa-stream-control.h"
#include "alsa-switch.h"
#include "alsa-toggle.h"

struct _AlsaStreamPrivate
{
    GList *switches;
    GList *controls;
};

static void alsa_stream_dispose    (GObject         *object);

G_DEFINE_TYPE_WITH_PRIVATE (AlsaStream, alsa_stream, CAFE_MIXER_TYPE_STREAM)

static const GList *alsa_stream_list_controls (CafeMixerStream *mms);
static const GList *alsa_stream_list_switches (CafeMixerStream *mms);

static gint         compare_control_name      (gconstpointer    a,
                                               gconstpointer    b);
static gint         compare_switch_name       (gconstpointer    a,
                                               gconstpointer    b);

static void
alsa_stream_class_init (AlsaStreamClass *klass)
{
    GObjectClass         *object_class;
    CafeMixerStreamClass *stream_class;

    object_class = G_OBJECT_CLASS (klass);
    object_class->dispose = alsa_stream_dispose;

    stream_class = CAFE_MIXER_STREAM_CLASS (klass);
    stream_class->list_controls = alsa_stream_list_controls;
    stream_class->list_switches = alsa_stream_list_switches;
}

static void
alsa_stream_init (AlsaStream *stream)
{
    stream->priv = alsa_stream_get_instance_private (stream);
}

static void
alsa_stream_dispose (GObject *object)
{
    AlsaStream *stream;

    stream = ALSA_STREAM (object);

    if (stream->priv->controls != NULL) {
        g_list_free_full (stream->priv->controls, g_object_unref);
        stream->priv->controls = NULL;
    }
    if (stream->priv->switches != NULL) {
        g_list_free_full (stream->priv->switches, g_object_unref);
        stream->priv->switches = NULL;
    }

    G_OBJECT_CLASS (alsa_stream_parent_class)->dispose (object);
}

AlsaStream *
alsa_stream_new (const gchar       *name,
                 CafeMixerDevice   *device,
                 CafeMixerDirection direction)
{
    const gchar *label;

    g_return_val_if_fail (name != NULL, NULL);
    g_return_val_if_fail (ALSA_IS_DEVICE (device), NULL);

    label = cafe_mixer_device_get_label (device);

    return g_object_new (ALSA_TYPE_STREAM,
                         "name", name,
                         "label", label,
                         "device", device,
                         "direction", direction,
                         NULL);
}

void
alsa_stream_add_control (AlsaStream *stream, AlsaStreamControl *control)
{
    const gchar *name;

    g_return_if_fail (ALSA_IS_STREAM (stream));
    g_return_if_fail (ALSA_IS_STREAM_CONTROL (control));

    name = cafe_mixer_stream_control_get_name (CAFE_MIXER_STREAM_CONTROL (control));

    stream->priv->controls =
        g_list_append (stream->priv->controls, g_object_ref (control));

    g_signal_emit_by_name (G_OBJECT (stream),
                           "control-added",
                           name);

    if (alsa_stream_has_default_control (stream) == FALSE)
        alsa_stream_set_default_control (stream, control);
}

void
alsa_stream_add_switch (AlsaStream *stream, AlsaSwitch *swtch)
{
    const gchar *name;

    g_return_if_fail (ALSA_IS_STREAM (stream));
    g_return_if_fail (ALSA_IS_SWITCH (swtch));

    name = cafe_mixer_switch_get_name (CAFE_MIXER_SWITCH (swtch));

    stream->priv->switches =
        g_list_append (stream->priv->switches, g_object_ref (swtch));

    g_signal_emit_by_name (G_OBJECT (stream),
                           "switch-added",
                           name);
}

void
alsa_stream_add_toggle (AlsaStream *stream, AlsaToggle *toggle)
{
    const gchar *name;

    g_return_if_fail (ALSA_IS_STREAM (stream));
    g_return_if_fail (ALSA_IS_TOGGLE (toggle));

    name = cafe_mixer_switch_get_name (CAFE_MIXER_SWITCH (toggle));

    /* Toggle is CafeMixerSwitch, but not AlsaSwitch */
    stream->priv->switches =
        g_list_append (stream->priv->switches, g_object_ref (toggle));

    g_signal_emit_by_name (G_OBJECT (stream),
                           "switch-added",
                           name);
}

gboolean
alsa_stream_has_controls (AlsaStream *stream)
{
    g_return_val_if_fail (ALSA_IS_STREAM (stream), FALSE);

    if (stream->priv->controls != NULL)
        return TRUE;

    return FALSE;
}

gboolean
alsa_stream_has_switches (AlsaStream *stream)
{
    g_return_val_if_fail (ALSA_IS_STREAM (stream), FALSE);

    if (stream->priv->switches != NULL)
        return TRUE;

    return FALSE;
}

gboolean
alsa_stream_has_controls_or_switches (AlsaStream *stream)
{
    g_return_val_if_fail (ALSA_IS_STREAM (stream), FALSE);

    if (stream->priv->controls != NULL ||
        stream->priv->switches != NULL)
        return TRUE;

    return FALSE;
}

gboolean
alsa_stream_has_default_control (AlsaStream *stream)
{
    g_return_val_if_fail (ALSA_IS_STREAM (stream), FALSE);

    if (cafe_mixer_stream_get_default_control (CAFE_MIXER_STREAM (stream)) != NULL)
        return TRUE;

    return FALSE;
}

AlsaStreamControl *
alsa_stream_get_default_control (AlsaStream *stream)
{
    CafeMixerStreamControl *control;

    g_return_val_if_fail (ALSA_IS_STREAM (stream), NULL);

    control = cafe_mixer_stream_get_default_control (CAFE_MIXER_STREAM (stream));
    if (control != NULL)
        return ALSA_STREAM_CONTROL (control);

    return NULL;
}

void
alsa_stream_set_default_control (AlsaStream *stream, AlsaStreamControl *control)
{
    g_return_if_fail (ALSA_IS_STREAM (stream));
    g_return_if_fail (control == NULL || ALSA_IS_STREAM_CONTROL (control));

    if (control == NULL)
        _cafe_mixer_stream_set_default_control (CAFE_MIXER_STREAM (stream), NULL);
    else
        _cafe_mixer_stream_set_default_control (CAFE_MIXER_STREAM (stream),
                                                CAFE_MIXER_STREAM_CONTROL (control));
}

void
alsa_stream_load_elements (AlsaStream *stream, const gchar *name)
{
    GList *item;

    g_return_if_fail (ALSA_IS_STREAM (stream));
    g_return_if_fail (name != NULL);

    item = g_list_find_custom (stream->priv->controls, name, compare_control_name);
    if (item != NULL)
        alsa_element_load (ALSA_ELEMENT (item->data));

    item = g_list_find_custom (stream->priv->switches, name, compare_switch_name);
    if (item != NULL)
        alsa_element_load (ALSA_ELEMENT (item->data));
}

gboolean
alsa_stream_remove_elements (AlsaStream *stream, const gchar *name)
{
    GList   *item;
    gboolean removed = FALSE;

    g_return_val_if_fail (ALSA_IS_STREAM (stream), FALSE);
    g_return_val_if_fail (name != NULL, FALSE);

    item = g_list_find_custom (stream->priv->controls, name, compare_control_name);
    if (item != NULL) {
        CafeMixerStreamControl *control = CAFE_MIXER_STREAM_CONTROL (item->data);

        alsa_element_close (ALSA_ELEMENT (control));
        stream->priv->controls = g_list_delete_link (stream->priv->controls, item);

        /* Change the default control if we have just removed it */
        if (control == cafe_mixer_stream_get_default_control (CAFE_MIXER_STREAM (stream))) {
            AlsaStreamControl *first = NULL;

            if (stream->priv->controls != NULL)
                first = ALSA_STREAM_CONTROL (stream->priv->controls->data);

            alsa_stream_set_default_control (stream, first);
        }

        g_signal_emit_by_name (G_OBJECT (stream),
                               "control-removed",
                               cafe_mixer_stream_control_get_name (control));

        g_object_unref (control);
        removed = TRUE;
    }

    item = g_list_find_custom (stream->priv->switches, name, compare_switch_name);
    if (item != NULL) {
        CafeMixerSwitch *swtch = CAFE_MIXER_SWITCH (item->data);

        alsa_element_close (ALSA_ELEMENT (swtch));

        stream->priv->switches = g_list_delete_link (stream->priv->switches, item);
        g_signal_emit_by_name (G_OBJECT (stream),
                               "switch-removed",
                               cafe_mixer_switch_get_name (swtch));

        g_object_unref (swtch);
        removed = TRUE;
    }

    return removed;
}

void
alsa_stream_remove_all (AlsaStream *stream)
{
    GList *list;

    g_return_if_fail (ALSA_IS_STREAM (stream));

    /* Remove all stream controls */
    list = stream->priv->controls;
    while (list != NULL) {
        CafeMixerStreamControl *control = CAFE_MIXER_STREAM_CONTROL (list->data);
        GList *next = list->next;

        alsa_element_close (ALSA_ELEMENT (control));

        stream->priv->controls = g_list_delete_link (stream->priv->controls, list);
        g_signal_emit_by_name (G_OBJECT (stream),
                               "control-removed",
                               cafe_mixer_stream_control_get_name (control));

        g_object_unref (control);
        list = next;
    }

    /* Unset the default stream control */
    alsa_stream_set_default_control (stream, NULL);

    /* Remove all stream switches */
    list = stream->priv->switches;
    while (list != NULL) {
        CafeMixerSwitch *swtch = CAFE_MIXER_SWITCH (list->data);
        GList *next = list->next;

        alsa_element_close (ALSA_ELEMENT (swtch));

        stream->priv->switches = g_list_delete_link (stream->priv->switches, list);
        g_signal_emit_by_name (G_OBJECT (stream),
                               "switch-removed",
                               cafe_mixer_switch_get_name (swtch));

        g_object_unref (swtch);
        list = next;
    }
}

static const GList *
alsa_stream_list_controls (CafeMixerStream *mms)
{
    g_return_val_if_fail (ALSA_IS_STREAM (mms), NULL);

    return ALSA_STREAM (mms)->priv->controls;
}

static const GList *
alsa_stream_list_switches (CafeMixerStream *mms)
{
    g_return_val_if_fail (ALSA_IS_STREAM (mms), NULL);

    return ALSA_STREAM (mms)->priv->switches;
}

static gint
compare_control_name (gconstpointer a, gconstpointer b)
{
    CafeMixerStreamControl *control = CAFE_MIXER_STREAM_CONTROL (a);
    const gchar            *name    = (const gchar *) b;

    return strcmp (cafe_mixer_stream_control_get_name (control), name);
}

static gint
compare_switch_name (gconstpointer a, gconstpointer b)
{
    CafeMixerSwitch *swtch = CAFE_MIXER_SWITCH (a);
    const gchar     *name  = (const gchar *) b;

    return strcmp (cafe_mixer_switch_get_name (swtch), name);
}
