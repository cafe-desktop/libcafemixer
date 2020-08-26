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

#include "cafemixer-backend.h"
#include "cafemixer-device.h"
#include "cafemixer-enums.h"
#include "cafemixer-enum-types.h"
#include "cafemixer-stream.h"
#include "cafemixer-stream-control.h"
#include "cafemixer-stored-control.h"

struct _CafeMixerBackendPrivate
{
    GHashTable           *devices;
    CafeMixerStream      *default_input;
    CafeMixerStream      *default_output;
    CafeMixerState        state;
    CafeMixerBackendFlags flags;
};

enum {
    PROP_0,
    PROP_STATE,
    PROP_DEFAULT_INPUT_STREAM,
    PROP_DEFAULT_OUTPUT_STREAM,
    N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES] = { NULL, };

enum {
    DEVICE_ADDED,
    DEVICE_REMOVED,
    STREAM_ADDED,
    STREAM_REMOVED,
    STORED_CONTROL_ADDED,
    STORED_CONTROL_REMOVED,
    N_SIGNALS
};

static guint signals[N_SIGNALS] = { 0, };

static void cafe_mixer_backend_get_property (GObject               *object,
                                             guint                  param_id,
                                             GValue                *value,
                                             GParamSpec            *pspec);
static void cafe_mixer_backend_set_property (GObject               *object,
                                             guint                  param_id,
                                             const GValue          *value,
                                             GParamSpec            *pspec);

static void cafe_mixer_backend_dispose      (GObject               *object);
static void cafe_mixer_backend_finalize     (GObject               *object);

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE (CafeMixerBackend, cafe_mixer_backend, G_TYPE_OBJECT)

static void device_added          (CafeMixerBackend *backend,
                                   const gchar      *name);
static void device_removed        (CafeMixerBackend *backend,
                                   const gchar      *name);

static void device_stream_added   (CafeMixerBackend *backend,
                                   const gchar      *name);
static void device_stream_removed (CafeMixerBackend *backend,
                                   const gchar      *name);

static void
cafe_mixer_backend_class_init (CafeMixerBackendClass *klass)
{
    GObjectClass *object_class;

    object_class = G_OBJECT_CLASS (klass);
    object_class->dispose      = cafe_mixer_backend_dispose;
    object_class->finalize     = cafe_mixer_backend_finalize;
    object_class->get_property = cafe_mixer_backend_get_property;
    object_class->set_property = cafe_mixer_backend_set_property;

    properties[PROP_STATE] =
        g_param_spec_enum ("state",
                           "State",
                           "Current backend connection state",
                           CAFE_MIXER_TYPE_STATE,
                           CAFE_MIXER_STATE_IDLE,
                           G_PARAM_READABLE |
                           G_PARAM_STATIC_STRINGS);

    properties[PROP_DEFAULT_INPUT_STREAM] =
        g_param_spec_object ("default-input-stream",
                             "Default input stream",
                             "Default input stream",
                             CAFE_MIXER_TYPE_STREAM,
                             G_PARAM_READABLE |
                             G_PARAM_STATIC_STRINGS);

    properties[PROP_DEFAULT_OUTPUT_STREAM] =
        g_param_spec_object ("default-output-stream",
                             "Default output stream",
                             "Default output stream",
                             CAFE_MIXER_TYPE_STREAM,
                             G_PARAM_READABLE |
                             G_PARAM_STATIC_STRINGS);

    g_object_class_install_properties (object_class, N_PROPERTIES, properties);

    signals[DEVICE_ADDED] =
        g_signal_new ("device-added",
                      G_TYPE_FROM_CLASS (object_class),
                      G_SIGNAL_RUN_FIRST,
                      G_STRUCT_OFFSET (CafeMixerBackendClass, device_added),
                      NULL,
                      NULL,
                      g_cclosure_marshal_VOID__STRING,
                      G_TYPE_NONE,
                      1,
                      G_TYPE_STRING);

    signals[DEVICE_REMOVED] =
        g_signal_new ("device-removed",
                      G_TYPE_FROM_CLASS (object_class),
                      G_SIGNAL_RUN_FIRST,
                      G_STRUCT_OFFSET (CafeMixerBackendClass, device_removed),
                      NULL,
                      NULL,
                      g_cclosure_marshal_VOID__STRING,
                      G_TYPE_NONE,
                      1,
                      G_TYPE_STRING);

    signals[STREAM_ADDED] =
        g_signal_new ("stream-added",
                      G_TYPE_FROM_CLASS (object_class),
                      G_SIGNAL_RUN_FIRST,
                      G_STRUCT_OFFSET (CafeMixerBackendClass, stream_added),
                      NULL,
                      NULL,
                      g_cclosure_marshal_VOID__STRING,
                      G_TYPE_NONE,
                      1,
                      G_TYPE_STRING);

    signals[STREAM_REMOVED] =
        g_signal_new ("stream-removed",
                      G_TYPE_FROM_CLASS (object_class),
                      G_SIGNAL_RUN_FIRST,
                      G_STRUCT_OFFSET (CafeMixerBackendClass, stream_removed),
                      NULL,
                      NULL,
                      g_cclosure_marshal_VOID__STRING,
                      G_TYPE_NONE,
                      1,
                      G_TYPE_STRING);

    signals[STORED_CONTROL_ADDED] =
        g_signal_new ("stored-control-added",
                      G_TYPE_FROM_CLASS (object_class),
                      G_SIGNAL_RUN_FIRST,
                      G_STRUCT_OFFSET (CafeMixerBackendClass, stored_control_added),
                      NULL,
                      NULL,
                      g_cclosure_marshal_VOID__STRING,
                      G_TYPE_NONE,
                      1,
                      G_TYPE_STRING);

    signals[STORED_CONTROL_REMOVED] =
        g_signal_new ("stored-control-removed",
                      G_TYPE_FROM_CLASS (object_class),
                      G_SIGNAL_RUN_FIRST,
                      G_STRUCT_OFFSET (CafeMixerBackendClass, stored_control_removed),
                      NULL,
                      NULL,
                      g_cclosure_marshal_VOID__STRING,
                      G_TYPE_NONE,
                      1,
                      G_TYPE_STRING);
}

static void
cafe_mixer_backend_get_property (GObject    *object,
                                 guint       param_id,
                                 GValue     *value,
                                 GParamSpec *pspec)
{
    CafeMixerBackend *backend;

    backend = CAFE_MIXER_BACKEND (object);

    switch (param_id) {
    case PROP_STATE:
        g_value_set_enum (value, backend->priv->state);
        break;
    case PROP_DEFAULT_INPUT_STREAM:
        g_value_set_object (value, backend->priv->default_input);
        break;
    case PROP_DEFAULT_OUTPUT_STREAM:
        g_value_set_object (value, backend->priv->default_output);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
        break;
    }
}

static void
cafe_mixer_backend_set_property (GObject      *object,
                                 guint         param_id,
                                 const GValue *value,
                                 GParamSpec   *pspec)
{
    CafeMixerBackend *backend;

    backend = CAFE_MIXER_BACKEND (object);

    switch (param_id) {
    case PROP_DEFAULT_INPUT_STREAM:
        cafe_mixer_backend_set_default_input_stream (backend, g_value_get_object (value));
        break;
    case PROP_DEFAULT_OUTPUT_STREAM:
        cafe_mixer_backend_set_default_output_stream (backend, g_value_get_object (value));
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
        break;
    }
}

static void
cafe_mixer_backend_init (CafeMixerBackend *backend)
{
    backend->priv = cafe_mixer_backend_get_instance_private (backend);

    backend->priv->devices = g_hash_table_new_full (g_str_hash,
                                                    g_str_equal,
                                                    g_free,
                                                    g_object_unref);

    g_signal_connect (G_OBJECT (backend),
                      "device-added",
                      G_CALLBACK (device_added),
                      NULL);

    g_signal_connect (G_OBJECT (backend),
                      "device-removed",
                      G_CALLBACK (device_removed),
                      NULL);
}

static void
cafe_mixer_backend_dispose (GObject *object)
{
    CafeMixerBackend *backend;

    backend = CAFE_MIXER_BACKEND (object);

    g_clear_object (&backend->priv->default_input);
    g_clear_object (&backend->priv->default_output);

    g_hash_table_remove_all (backend->priv->devices);

    G_OBJECT_CLASS (cafe_mixer_backend_parent_class)->dispose (object);
}

static void
cafe_mixer_backend_finalize (GObject *object)
{
    CafeMixerBackend *backend;

    backend = CAFE_MIXER_BACKEND (object);

    g_hash_table_unref (backend->priv->devices);

    G_OBJECT_CLASS (cafe_mixer_backend_parent_class)->finalize (object);
}

void
cafe_mixer_backend_set_app_info (CafeMixerBackend *backend, CafeMixerAppInfo *info)
{
    CafeMixerBackendClass *klass;

    g_return_if_fail (CAFE_MIXER_IS_BACKEND (backend));

    klass = CAFE_MIXER_BACKEND_GET_CLASS (backend);

    if (klass->set_app_info != NULL)
        klass->set_app_info (backend, info);
}

void
cafe_mixer_backend_set_server_address (CafeMixerBackend *backend, const gchar *address)
{
    CafeMixerBackendClass *klass;

    g_return_if_fail (CAFE_MIXER_IS_BACKEND (backend));

    klass = CAFE_MIXER_BACKEND_GET_CLASS (backend);

    if (klass->set_server_address != NULL)
        klass->set_server_address (backend, address);
}

gboolean
cafe_mixer_backend_open (CafeMixerBackend *backend)
{
    g_return_val_if_fail (CAFE_MIXER_IS_BACKEND (backend), FALSE);

    /* Implementation required */
    return CAFE_MIXER_BACKEND_GET_CLASS (backend)->open (backend);
}

void
cafe_mixer_backend_close (CafeMixerBackend *backend)
{
    CafeMixerBackendClass *klass;

    g_return_if_fail (CAFE_MIXER_IS_BACKEND (backend));

    klass = CAFE_MIXER_BACKEND_GET_CLASS (backend);

    if (klass->close != NULL)
        klass->close (backend);
}

CafeMixerState
cafe_mixer_backend_get_state (CafeMixerBackend *backend)
{
    g_return_val_if_fail (CAFE_MIXER_IS_BACKEND (backend), CAFE_MIXER_STATE_UNKNOWN);

    return backend->priv->state;
}

CafeMixerDevice *
cafe_mixer_backend_get_device (CafeMixerBackend *backend, const gchar *name)
{
    const GList *list;

    g_return_val_if_fail (CAFE_MIXER_IS_BACKEND (backend), NULL);
    g_return_val_if_fail (name != NULL, NULL);

    list = cafe_mixer_backend_list_devices (backend);
    while (list != NULL) {
        CafeMixerDevice *device = CAFE_MIXER_DEVICE (list->data);

        if (strcmp (name, cafe_mixer_device_get_name (device)) == 0)
            return device;

        list = list->next;
    }
    return NULL;
}

CafeMixerStream *
cafe_mixer_backend_get_stream (CafeMixerBackend *backend, const gchar *name)
{
    const GList *list;

    g_return_val_if_fail (CAFE_MIXER_IS_BACKEND (backend), NULL);
    g_return_val_if_fail (name != NULL, NULL);

    list = cafe_mixer_backend_list_streams (backend);
    while (list != NULL) {
        CafeMixerStream *stream = CAFE_MIXER_STREAM (list->data);

        if (strcmp (name, cafe_mixer_stream_get_name (stream)) == 0)
            return stream;

        list = list->next;
    }
    return NULL;
}

CafeMixerStoredControl *
cafe_mixer_backend_get_stored_control (CafeMixerBackend *backend, const gchar *name)
{
    const GList *list;

    g_return_val_if_fail (CAFE_MIXER_IS_BACKEND (backend), NULL);
    g_return_val_if_fail (name != NULL, NULL);

    list = cafe_mixer_backend_list_stored_controls (backend);
    while (list != NULL) {
        CafeMixerStreamControl *control = CAFE_MIXER_STREAM_CONTROL (list->data);

        if (strcmp (name, cafe_mixer_stream_control_get_name (control)) == 0)
            return CAFE_MIXER_STORED_CONTROL (control);

        list = list->next;
    }
    return NULL;
}

const GList *
cafe_mixer_backend_list_devices (CafeMixerBackend *backend)
{
    CafeMixerBackendClass *klass;

    g_return_val_if_fail (CAFE_MIXER_IS_BACKEND (backend), NULL);

    klass = CAFE_MIXER_BACKEND_GET_CLASS (backend);

    if (klass->list_devices != NULL)
        return klass->list_devices (backend);

    return NULL;
}

const GList *
cafe_mixer_backend_list_streams (CafeMixerBackend *backend)
{
    CafeMixerBackendClass *klass;

    g_return_val_if_fail (CAFE_MIXER_IS_BACKEND (backend), NULL);

    klass = CAFE_MIXER_BACKEND_GET_CLASS (backend);

    if (klass->list_streams != NULL)
        return klass->list_streams (backend);

    return NULL;
}

const GList *
cafe_mixer_backend_list_stored_controls (CafeMixerBackend *backend)
{
    CafeMixerBackendClass *klass;

    g_return_val_if_fail (CAFE_MIXER_IS_BACKEND (backend), NULL);

    klass = CAFE_MIXER_BACKEND_GET_CLASS (backend);

    if (klass->list_stored_controls != NULL)
        return klass->list_stored_controls (backend);

    return NULL;
}

CafeMixerStream *
cafe_mixer_backend_get_default_input_stream (CafeMixerBackend *backend)
{
    g_return_val_if_fail (CAFE_MIXER_IS_BACKEND (backend), NULL);

    return backend->priv->default_input;
}

gboolean
cafe_mixer_backend_set_default_input_stream (CafeMixerBackend *backend,
                                             CafeMixerStream  *stream)
{
    CafeMixerBackendClass *klass;

    g_return_val_if_fail (CAFE_MIXER_IS_BACKEND (backend), FALSE);
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM (stream), FALSE);

    klass = CAFE_MIXER_BACKEND_GET_CLASS (backend);
    if (klass->set_default_input_stream == NULL)
        return FALSE;

    if (backend->priv->default_input != stream) {
        if (cafe_mixer_stream_get_direction (stream) != CAFE_MIXER_DIRECTION_INPUT) {
            g_warning ("Unable to set non-input stream as the default input stream");
            return FALSE;
        }

        if (klass->set_default_input_stream (backend, stream) == FALSE)
            return FALSE;

        _cafe_mixer_backend_set_default_input_stream (backend, stream);
    }
    return TRUE;
}

CafeMixerStream *
cafe_mixer_backend_get_default_output_stream (CafeMixerBackend *backend)
{
    g_return_val_if_fail (CAFE_MIXER_IS_BACKEND (backend), NULL);

    return backend->priv->default_output;
}

gboolean
cafe_mixer_backend_set_default_output_stream (CafeMixerBackend *backend,
                                              CafeMixerStream  *stream)
{
    CafeMixerBackendClass *klass;

    g_return_val_if_fail (CAFE_MIXER_IS_BACKEND (backend), FALSE);
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM (stream), FALSE);

    klass = CAFE_MIXER_BACKEND_GET_CLASS (backend);
    if (klass->set_default_output_stream == NULL)
        return FALSE;

    if (backend->priv->default_input != stream) {
        if (cafe_mixer_stream_get_direction (stream) != CAFE_MIXER_DIRECTION_OUTPUT) {
            g_warning ("Unable to set non-output stream as the default output stream");
            return FALSE;
        }

        if (klass->set_default_output_stream (backend, stream) == FALSE)
            return FALSE;

        _cafe_mixer_backend_set_default_output_stream (backend, stream);
    }
    return TRUE;
}

static void
device_added (CafeMixerBackend *backend, const gchar *name)
{
    CafeMixerDevice *device;

    device = cafe_mixer_backend_get_device (backend, name);
    if (G_UNLIKELY (device == NULL)) {
        g_warn_if_reached ();
        return;
    }

    /* Keep the device in a hash table as it won't be possible to retrieve
     * it when the remove signal is received */
    g_hash_table_insert (backend->priv->devices,
                         g_strdup (name),
                         g_object_ref (device));

    /* Connect to the stream signals from devices so we can forward them on
     * the backend */
    g_signal_connect_swapped (G_OBJECT (device),
                              "stream-added",
                              G_CALLBACK (device_stream_added),
                              backend);
    g_signal_connect_swapped (G_OBJECT (device),
                              "stream-removed",
                              G_CALLBACK (device_stream_removed),
                              backend);
}

static void
device_removed (CafeMixerBackend *backend, const gchar *name)
{
    CafeMixerDevice *device;

    device = g_hash_table_lookup (backend->priv->devices, name);
    if (G_UNLIKELY (device == NULL)) {
        g_warn_if_reached ();
        return;
    }

    g_signal_handlers_disconnect_by_func (G_OBJECT (device),
                                          G_CALLBACK (device_stream_added),
                                          backend);
    g_signal_handlers_disconnect_by_func (G_OBJECT (device),
                                          G_CALLBACK (device_stream_removed),
                                          backend);

    g_hash_table_remove (backend->priv->devices, name);
}

static void
device_stream_added (CafeMixerBackend *backend, const gchar *name)
{
    g_signal_emit (G_OBJECT (backend),
                   signals[STREAM_ADDED],
                   0,
                   name);
}

static void
device_stream_removed (CafeMixerBackend *backend, const gchar *name)
{
    g_signal_emit (G_OBJECT (backend),
                   signals[STREAM_REMOVED],
                   0,
                   name);
}

/* Protected functions */
void
_cafe_mixer_backend_set_state (CafeMixerBackend *backend, CafeMixerState state)
{
    g_return_if_fail (CAFE_MIXER_IS_BACKEND (backend));

    if (backend->priv->state == state)
        return;

    backend->priv->state = state;

    g_object_notify_by_pspec (G_OBJECT (backend), properties[PROP_STATE]);
}

void
_cafe_mixer_backend_set_default_input_stream (CafeMixerBackend *backend,
                                              CafeMixerStream  *stream)
{
    g_return_if_fail (CAFE_MIXER_IS_BACKEND (backend));
    g_return_if_fail (stream == NULL || CAFE_MIXER_IS_STREAM (stream));

    if (backend->priv->default_input == stream)
        return;

    if (backend->priv->default_input != NULL)
        g_object_unref (backend->priv->default_input);

    if (stream != NULL)
        backend->priv->default_input = g_object_ref (stream);
    else
        backend->priv->default_input = NULL;

    g_debug ("Default input stream changed to %s",
             (stream != NULL) ? cafe_mixer_stream_get_name (stream) : "none");

    g_object_notify_by_pspec (G_OBJECT (backend),
                              properties[PROP_DEFAULT_INPUT_STREAM]);
}

void
_cafe_mixer_backend_set_default_output_stream (CafeMixerBackend *backend,
                                               CafeMixerStream  *stream)
{
    g_return_if_fail (CAFE_MIXER_IS_BACKEND (backend));
    g_return_if_fail (stream == NULL || CAFE_MIXER_IS_STREAM (stream));

    if (backend->priv->default_output == stream)
        return;

    if (backend->priv->default_output != NULL)
        g_object_unref (backend->priv->default_output);

    if (stream != NULL)
        backend->priv->default_output = g_object_ref (stream);
    else
        backend->priv->default_output = NULL;

    g_debug ("Default output stream changed to %s",
             (stream != NULL) ? cafe_mixer_stream_get_name (stream) : "none");

    g_object_notify_by_pspec (G_OBJECT (backend),
                              properties[PROP_DEFAULT_OUTPUT_STREAM]);
}
