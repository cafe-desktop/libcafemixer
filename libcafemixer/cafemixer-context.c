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

#include "cafemixer.h"
#include "cafemixer-backend.h"
#include "cafemixer-backend-module.h"
#include "cafemixer-context.h"
#include "cafemixer-enums.h"
#include "cafemixer-enum-types.h"
#include "cafemixer-private.h"
#include "cafemixer-stream.h"

/**
 * SECTION:cafemixer-context
 * @short_description:The main class for interfacing with the library
 * @include: libcafemixer/cafemixer.h
 *
 * After the library is initialized, a context should be created to gain
 * access to a sound system.
 *
 * To create a new context, use the cafe_mixer_context_new() function.
 *
 * The cafe_mixer_context_set_backend_type() function can be used to associate
 * the context with a particular type of sound system. Using this function is
 * not necessary, by default the context will select a working sound system
 * backend automatically.
 *
 * To connect to a sound system, use cafe_mixer_context_open().
 *
 * When the connection is established, it is possible to query a list of sound
 * devices with cafe_mixer_context_list_devices() and streams with
 * cafe_mixer_context_list_streams().
 *
 * A device represents a hardware or software sound device in the system,
 * typically a sound card.
 *
 * A stream is an input or output channel that may exist either as a part of a
 * sound device, or independently. Streams essentially serve as containers for
 * volume controls and switches, for example a sound card with microphone and
 * line-in connectors may have an input stream containing volume controls for
 * each of these connectors and possibly a switch allowing to change the active
 * connector.
 *
 * Streams may also exist independently as the sound system may for example
 * allow audio streaming over a network.
 *
 * For a more thorough description of devices and streams, see #CafeMixerDevice
 * and #CafeMixerStream.
 *
 * Devices and streams (as almost all other elements in the library) may appear
 * and disappear at any time, for example when external sound cards are plugged
 * and unplugged. The application should connect to the appropriate signals to
 * handle these events.
 */

struct _CafeMixerContextPrivate
{
    gboolean                backend_chosen;
    gchar                  *server_address;
    CafeMixerState          state;
    CafeMixerBackend       *backend;
    CafeMixerAppInfo       *app_info;
    CafeMixerBackendType    backend_type;
    CafeMixerBackendModule *module;
};

enum {
    PROP_0,
    PROP_APP_NAME,
    PROP_APP_ID,
    PROP_APP_VERSION,
    PROP_APP_ICON,
    PROP_SERVER_ADDRESS,
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

static void cafe_mixer_context_get_property (GObject               *object,
                                             guint                  param_id,
                                             GValue                *value,
                                             GParamSpec            *pspec);
static void cafe_mixer_context_set_property (GObject               *object,
                                             guint                  param_id,
                                             const GValue          *value,
                                             GParamSpec            *pspec);

static void cafe_mixer_context_dispose      (GObject               *object);
static void cafe_mixer_context_finalize     (GObject               *object);

G_DEFINE_TYPE_WITH_PRIVATE (CafeMixerContext, cafe_mixer_context, G_TYPE_OBJECT);

static void     on_backend_state_notify                 (CafeMixerBackend *backend,
                                                         GParamSpec       *pspec,
                                                         CafeMixerContext *context);

static void     on_backend_device_added                 (CafeMixerBackend *backend,
                                                         const gchar      *name,
                                                         CafeMixerContext *context);
static void     on_backend_device_removed               (CafeMixerBackend *backend,
                                                         const gchar      *name,
                                                         CafeMixerContext *context);

static void     on_backend_stream_added                 (CafeMixerBackend *backend,
                                                         const gchar      *name,
                                                         CafeMixerContext *context);
static void     on_backend_stream_removed               (CafeMixerBackend *backend,
                                                         const gchar      *name,
                                                         CafeMixerContext *context);

static void     on_backend_stored_control_added         (CafeMixerBackend *backend,
                                                         const gchar      *name,
                                                         CafeMixerContext *context);
static void     on_backend_stored_control_removed       (CafeMixerBackend *backend,
                                                         const gchar      *name,
                                                         CafeMixerContext *context);

static void     on_backend_default_input_stream_notify  (CafeMixerBackend *backend,
                                                         GParamSpec       *pspec,
                                                         CafeMixerContext *context);
static void     on_backend_default_output_stream_notify (CafeMixerBackend *backend,
                                                         GParamSpec       *pspec,
                                                         CafeMixerContext *context);

static gboolean try_next_backend                        (CafeMixerContext *context);

static void     change_state                            (CafeMixerContext *context,
                                                         CafeMixerState    state);

static void     close_context                           (CafeMixerContext *context);

static void
cafe_mixer_context_class_init (CafeMixerContextClass *klass)
{
    GObjectClass *object_class;

    object_class = G_OBJECT_CLASS (klass);
    object_class->dispose      = cafe_mixer_context_dispose;
    object_class->finalize     = cafe_mixer_context_finalize;
    object_class->get_property = cafe_mixer_context_get_property;
    object_class->set_property = cafe_mixer_context_set_property;

    /**
     * CafeMixerContext:app-name:
     *
     * Localized human readable name of the application.
     */
    properties[PROP_APP_NAME] =
        g_param_spec_string ("app-name",
                             "App name",
                             "Application name",
                             NULL,
                             G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

    /**
     * CafeMixerContext:app-id:
     *
     * Identifier of the application (e.g. org.example.app).
     */
    properties[PROP_APP_ID] =
        g_param_spec_string ("app-id",
                             "App ID",
                             "Application identifier",
                             NULL,
                             G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

    /**
     * CafeMixerContext:app-version:
     *
     * Version of the application.
     */
    properties[PROP_APP_VERSION] =
        g_param_spec_string ("app-version",
                             "App version",
                             "Application version",
                             NULL,
                             G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

    /**
     * CafeMixerContext:app-icon:
     *
     * The XDG icon name of the application.
     */
    properties[PROP_APP_ICON] =
        g_param_spec_string ("app-icon",
                             "App icon",
                             "Application XDG icon name",
                             NULL,
                             G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

    /**
     * CafeMixerContext:server-address:
     *
     * Address of the sound server to connect to.
     *
     * This feature is only supported by the PulseAudio sound system.
     * There is no need to specify an address in order to connect to the
     * local PulseAudio daemon.
     */
    properties[PROP_SERVER_ADDRESS] =
        g_param_spec_string ("server-address",
                             "Server address",
                             "Sound server address",
                             NULL,
                             G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

    /**
     * CafeMixerContext:state:
     *
     * The current state of the connection to a sound system.
     */
    properties[PROP_STATE] =
        g_param_spec_enum ("state",
                           "State",
                           "Current backend connection state",
                           CAFE_MIXER_TYPE_STATE,
                           CAFE_MIXER_STATE_IDLE,
                           G_PARAM_READABLE | G_PARAM_STATIC_STRINGS);

    /**
     * CafeMixerContext:default-input-stream:
     *
     * The stream sound input most likely comes from by default.
     *
     * See cafe_mixer_context_set_default_input_stream() for more information
     * about changing the default input stream.
     */
    properties[PROP_DEFAULT_INPUT_STREAM] =
        g_param_spec_object ("default-input-stream",
                             "Default input stream",
                             "Default input stream",
                             CAFE_MIXER_TYPE_STREAM,
                             G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

    /**
     * CafeMixerContext:default-output-stream:
     *
     * The stream sound output is most likely directed to by default.
     *
     * See cafe_mixer_context_set_default_output_stream() for more information
     * about changing the default output stream.
     */
    properties[PROP_DEFAULT_OUTPUT_STREAM] =
        g_param_spec_object ("default-output-stream",
                             "Default output stream",
                             "Default output stream",
                             CAFE_MIXER_TYPE_STREAM,
                             G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

    g_object_class_install_properties (object_class, N_PROPERTIES, properties);

    /**
     * CafeMixerContext::device-added:
     * @context: a #CafeMixerContext
     * @name: name of the added device
     *
     * The signal is emitted each time a device is added to the system.
     *
     * Use cafe_mixer_context_get_device() to get the #CafeMixerDevice.
     *
     * Note that at the time this signal is emitted, the streams and switches
     * of the device may not yet be known.
     */
    signals[DEVICE_ADDED] =
        g_signal_new ("device-added",
                      G_TYPE_FROM_CLASS (object_class),
                      G_SIGNAL_RUN_FIRST,
                      G_STRUCT_OFFSET (CafeMixerContextClass, device_added),
                      NULL,
                      NULL,
                      g_cclosure_marshal_VOID__STRING,
                      G_TYPE_NONE,
                      1,
                      G_TYPE_STRING);

    /**
     * CafeMixerContext::device-removed:
     * @context: a #CafeMixerContext
     * @name: name of the removed device
     *
     * The signal is emitted each time a device is removed from the system.
     *
     * When this signal is emitted, the device is no longer known to the library,
     * it will not be included in the device list provided by the
     * cafe_mixer_context_list_devices() function and it is not possible to get
     * the device with cafe_mixer_context_get_device().
     */
    signals[DEVICE_REMOVED] =
        g_signal_new ("device-removed",
                      G_TYPE_FROM_CLASS (object_class),
                      G_SIGNAL_RUN_FIRST,
                      G_STRUCT_OFFSET (CafeMixerContextClass, device_removed),
                      NULL,
                      NULL,
                      g_cclosure_marshal_VOID__STRING,
                      G_TYPE_NONE,
                      1,
                      G_TYPE_STRING);

    /**
     * CafeMixerContext::stream-added:
     * @context: a #CafeMixerContext
     * @name: name of the added stream
     *
     * The signal is emitted each time a stream is added.
     *
     * This signal is emitted for streams which belong to devices as well as
     * streams which do not. If you are only interested in streams of a
     * specific device, the signal is also available in #CafeMixerDevice.
     *
     * Note that at the time this signal is emitted, the controls and switches
     * of the stream may not yet be known.
     */
    signals[STREAM_ADDED] =
        g_signal_new ("stream-added",
                      G_TYPE_FROM_CLASS (object_class),
                      G_SIGNAL_RUN_FIRST,
                      G_STRUCT_OFFSET (CafeMixerContextClass, stream_added),
                      NULL,
                      NULL,
                      g_cclosure_marshal_VOID__STRING,
                      G_TYPE_NONE,
                      1,
                      G_TYPE_STRING);

    /**
     * CafeMixerContext::stream-removed:
     * @context: a #CafeMixerContext
     * @name: name of the removed stream
     *
     * The signal is emitted each time a stream is removed.
     *
     * When this signal is emitted, the stream is no longer known to the library,
     * it will not be included in the stream list provided by the
     * cafe_mixer_context_list_streams() function and it is not possible to get
     * the stream with cafe_mixer_context_get_stream().
     *
     * This signal is emitted for streams which belong to devices as well as
     * streams which do not. If you are only interested in streams of a
     * specific device, the signal is also available in #CafeMixerDevice.
     */
    signals[STREAM_REMOVED] =
        g_signal_new ("stream-removed",
                      G_TYPE_FROM_CLASS (object_class),
                      G_SIGNAL_RUN_FIRST,
                      G_STRUCT_OFFSET (CafeMixerContextClass, stream_removed),
                      NULL,
                      NULL,
                      g_cclosure_marshal_VOID__STRING,
                      G_TYPE_NONE,
                      1,
                      G_TYPE_STRING);

    /**
     * CafeMixerContext::stored-control-added:
     * @context: a #CafeMixerContext
     * @name: name of the added stored control
     *
     * The signal is emitted each time a stored control is added.
     *
     * Use cafe_mixer_context_get_stored_control() to get the #CafeMixerStoredControl.
     */
    signals[STORED_CONTROL_ADDED] =
        g_signal_new ("stored-control-added",
                      G_TYPE_FROM_CLASS (object_class),
                      G_SIGNAL_RUN_FIRST,
                      G_STRUCT_OFFSET (CafeMixerContextClass, stored_control_added),
                      NULL,
                      NULL,
                      g_cclosure_marshal_VOID__STRING,
                      G_TYPE_NONE,
                      1,
                      G_TYPE_STRING);

    /**
     * CafeMixerContext::stored-control-removed:
     * @context: a #CafeMixerContext
     * @name: name of the removed stored control
     *
     * The signal is emitted each time a stored control is removed.
     *
     * When this signal is emitted, the stored control is no longer known to the
     * library, it will not be included in the stream list provided by the
     * cafe_mixer_context_list_stored_controls() function and it is not possible to
     * get the stored control with cafe_mixer_context_get_stored_control().
     */
    signals[STORED_CONTROL_REMOVED] =
        g_signal_new ("stored-control-removed",
                      G_TYPE_FROM_CLASS (object_class),
                      G_SIGNAL_RUN_FIRST,
                      G_STRUCT_OFFSET (CafeMixerContextClass, stored_control_removed),
                      NULL,
                      NULL,
                      g_cclosure_marshal_VOID__STRING,
                      G_TYPE_NONE,
                      1,
                      G_TYPE_STRING);
}

static void
cafe_mixer_context_get_property (GObject    *object,
                                 guint       param_id,
                                 GValue     *value,
                                 GParamSpec *pspec)
{
    CafeMixerContext *context;

    context = CAFE_MIXER_CONTEXT (object);

    switch (param_id) {
    case PROP_APP_NAME:
        g_value_set_string (value, cafe_mixer_app_info_get_name (context->priv->app_info));
        break;
    case PROP_APP_ID:
        g_value_set_string (value, cafe_mixer_app_info_get_id (context->priv->app_info));
        break;
    case PROP_APP_VERSION:
        g_value_set_string (value, cafe_mixer_app_info_get_version (context->priv->app_info));
        break;
    case PROP_APP_ICON:
        g_value_set_string (value, cafe_mixer_app_info_get_icon (context->priv->app_info));
        break;
    case PROP_SERVER_ADDRESS:
        g_value_set_string (value, context->priv->server_address);
        break;
    case PROP_STATE:
        g_value_set_enum (value, context->priv->state);
        break;
    case PROP_DEFAULT_INPUT_STREAM:
        g_value_set_object (value, cafe_mixer_context_get_default_input_stream (context));
        break;
    case PROP_DEFAULT_OUTPUT_STREAM:
        g_value_set_object (value, cafe_mixer_context_get_default_output_stream (context));
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
        break;
    }
}

static void
cafe_mixer_context_set_property (GObject      *object,
                                 guint         param_id,
                                 const GValue *value,
                                 GParamSpec   *pspec)
{
    CafeMixerContext *context;

    context = CAFE_MIXER_CONTEXT (object);

    switch (param_id) {
    case PROP_APP_NAME:
        cafe_mixer_context_set_app_name (context, g_value_get_string (value));
        break;
    case PROP_APP_ID:
        cafe_mixer_context_set_app_id (context, g_value_get_string (value));
        break;
    case PROP_APP_VERSION:
        cafe_mixer_context_set_app_version (context, g_value_get_string (value));
        break;
    case PROP_APP_ICON:
        cafe_mixer_context_set_app_icon (context, g_value_get_string (value));
        break;
    case PROP_SERVER_ADDRESS:
        cafe_mixer_context_set_server_address (context, g_value_get_string (value));
        break;
    case PROP_DEFAULT_INPUT_STREAM:
        cafe_mixer_context_set_default_input_stream (context, g_value_get_object (value));
        break;
    case PROP_DEFAULT_OUTPUT_STREAM:
        cafe_mixer_context_set_default_output_stream (context, g_value_get_object (value));
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
        break;
    }
}

static void
cafe_mixer_context_init (CafeMixerContext *context)
{
    context->priv = cafe_mixer_context_get_instance_private (context);

    context->priv->app_info = _cafe_mixer_app_info_new ();
}

static void
cafe_mixer_context_dispose (GObject *object)
{
    CafeMixerContext *context;

    context = CAFE_MIXER_CONTEXT (object);

    close_context (context);

    G_OBJECT_CLASS (cafe_mixer_context_parent_class)->dispose (object);
}

static void
cafe_mixer_context_finalize (GObject *object)
{
    CafeMixerContext *context;

    context = CAFE_MIXER_CONTEXT (object);

    _cafe_mixer_app_info_free (context->priv->app_info);

    g_free (context->priv->server_address);

    G_OBJECT_CLASS (cafe_mixer_context_parent_class)->finalize (object);
}

/**
 * cafe_mixer_context_new:
 *
 * Creates a new #CafeMixerContext instance.
 *
 * Returns: a new #CafeMixerContext instance or %NULL if the library has not
 * been initialized with cafe_mixer_init().
 */
CafeMixerContext *
cafe_mixer_context_new (void)
{
    if (cafe_mixer_is_initialized () == FALSE) {
        g_critical ("The library has not been initialized");
        return NULL;
    }

    return g_object_new (CAFE_MIXER_TYPE_CONTEXT, NULL);
}

/**
 * cafe_mixer_context_set_backend_type:
 * @context: a #CafeMixerContext
 * @backend_type: the sound system backend to use
 *
 * Makes the #CafeMixerContext use the given #CafeMixerBackendType.
 *
 * By default the backend type is determined automatically. This function can
 * be used to alter this behavior and make the @context use the selected sound
 * system.
 *
 * Setting the backend type only succeeds if the selected backend module is
 * available in the target system.
 *
 * If you have used this function before and want restore the default automatic
 * backend type discovery, set the backend type to %CAFE_MIXER_BACKEND_UNKNOWN.
 *
 * This function must be used before opening a connection to a sound system with
 * cafe_mixer_context_open(), otherwise it will fail.
 *
 * Returns: %TRUE on success or %FALSE on failure.
 */
gboolean
cafe_mixer_context_set_backend_type (CafeMixerContext    *context,
                                     CafeMixerBackendType backend_type)
{
    const GList                *modules;

    g_return_val_if_fail (CAFE_MIXER_IS_CONTEXT (context), FALSE);

    if (context->priv->state == CAFE_MIXER_STATE_CONNECTING ||
        context->priv->state == CAFE_MIXER_STATE_READY)
        return FALSE;

    /* Allow setting the backend to unknown to restore the auto-detection */
    if (backend_type == CAFE_MIXER_BACKEND_UNKNOWN) {
        context->priv->backend_type = backend_type;
        return TRUE;
    }

    modules = _cafe_mixer_list_modules ();
    while (modules != NULL) {
        CafeMixerBackendModule     *module;
        const CafeMixerBackendInfo *info;

        module = CAFE_MIXER_BACKEND_MODULE (modules->data);
        info   = cafe_mixer_backend_module_get_info (module);

        if (info->backend_type == backend_type) {
            context->priv->backend_type = backend_type;
            return TRUE;
        }
        modules = modules->next;
    }
    return FALSE;
}

/**
 * cafe_mixer_context_set_app_name:
 * @context: a #CafeMixerContext
 * @app_name: the name of your application, or %NULL to unset
 *
 * Sets the name of your application. This information may be used when
 * registering with the sound system.
 *
 * This function must be used before opening a connection to a sound system with
 * cafe_mixer_context_open(), otherwise it will fail.
 *
 * Returns: %TRUE on success or %FALSE on failure.
 */
gboolean
cafe_mixer_context_set_app_name (CafeMixerContext *context, const gchar *app_name)
{
    g_return_val_if_fail (CAFE_MIXER_IS_CONTEXT (context), FALSE);

    if (context->priv->state == CAFE_MIXER_STATE_CONNECTING ||
        context->priv->state == CAFE_MIXER_STATE_READY)
        return FALSE;

    _cafe_mixer_app_info_set_name (context->priv->app_info, app_name);

    g_object_notify_by_pspec (G_OBJECT (context), properties[PROP_APP_NAME]);
    return TRUE;
}

/**
 * cafe_mixer_context_set_app_id:
 * @context: a #CafeMixerContext
 * @app_id: the identifier of your application, or %NULL to unset
 *
 * Sets the identifier of your application (e.g. org.example.app). This
 * information may be used when registering with the sound system.
 *
 * This function must be used before opening a connection to a sound system with
 * cafe_mixer_context_open(), otherwise it will fail.
 *
 * Returns: %TRUE on success or %FALSE on failure.
 */
gboolean
cafe_mixer_context_set_app_id (CafeMixerContext *context, const gchar *app_id)
{
    g_return_val_if_fail (CAFE_MIXER_IS_CONTEXT (context), FALSE);

    if (context->priv->state == CAFE_MIXER_STATE_CONNECTING ||
        context->priv->state == CAFE_MIXER_STATE_READY)
        return FALSE;

    _cafe_mixer_app_info_set_id (context->priv->app_info, app_id);

    g_object_notify_by_pspec (G_OBJECT (context), properties[PROP_APP_ID]);
    return TRUE;
}

/**
 * cafe_mixer_context_set_app_version:
 * @context: a #CafeMixerContext
 * @app_version: the version of your application, or %NULL to unset
 *
 * Sets the version of your application. This information may be used when
 * registering with the sound system.
 *
 * This function must be used before opening a connection to a sound system with
 * cafe_mixer_context_open(), otherwise it will fail.
 *
 * Returns: %TRUE on success or %FALSE on failure.
 */
gboolean
cafe_mixer_context_set_app_version (CafeMixerContext *context, const gchar *app_version)
{
    g_return_val_if_fail (CAFE_MIXER_IS_CONTEXT (context), FALSE);

    if (context->priv->state == CAFE_MIXER_STATE_CONNECTING ||
        context->priv->state == CAFE_MIXER_STATE_READY)
        return FALSE;

    _cafe_mixer_app_info_set_version (context->priv->app_info, app_version);

    g_object_notify_by_pspec (G_OBJECT (context), properties[PROP_APP_VERSION]);
    return TRUE;
}

/**
 * cafe_mixer_context_set_app_icon:
 * @context: a #CafeMixerContext
 * @app_icon: the XDG icon name of your application, or %NULL to unset
 *
 * Sets the XDG icon name of your application. This information may be used when
 * registering with the sound system.
 *
 * This function must be used before opening a connection to a sound system with
 * cafe_mixer_context_open(), otherwise it will fail.
 *
 * Returns: %TRUE on success or %FALSE on failure.
 */
gboolean
cafe_mixer_context_set_app_icon (CafeMixerContext *context, const gchar *app_icon)
{
    g_return_val_if_fail (CAFE_MIXER_IS_CONTEXT (context), FALSE);

    if (context->priv->state == CAFE_MIXER_STATE_CONNECTING ||
        context->priv->state == CAFE_MIXER_STATE_READY)
        return FALSE;

    _cafe_mixer_app_info_set_icon (context->priv->app_info, app_icon);

    g_object_notify_by_pspec (G_OBJECT (context), properties[PROP_APP_ICON]);
    return TRUE;
}

/**
 * cafe_mixer_context_set_server_address:
 * @context: a #CafeMixerContext
 * @address: the address of the sound server to connect to or %NULL
 *
 * Sets the address of the sound server. This feature is only supported in the
 * PulseAudio backend. If the address is not set, the default PulseAudio sound
 * server will be used, which is normally the local daemon.
 *
 * This function must be used before opening a connection to a sound system with
 * cafe_mixer_context_open(), otherwise it will fail.
 *
 * Returns: %TRUE on success or %FALSE on failure.
 */
gboolean
cafe_mixer_context_set_server_address (CafeMixerContext *context, const gchar *address)
{
    g_return_val_if_fail (CAFE_MIXER_IS_CONTEXT (context), FALSE);

    if (context->priv->state == CAFE_MIXER_STATE_CONNECTING ||
        context->priv->state == CAFE_MIXER_STATE_READY)
        return FALSE;

    g_free (context->priv->server_address);

    context->priv->server_address = g_strdup (address);

    g_object_notify_by_pspec (G_OBJECT (context), properties[PROP_SERVER_ADDRESS]);
    return TRUE;
}

/**
 * cafe_mixer_context_open:
 * @context: a #CafeMixerContext
 *
 * Opens connection to a sound system. Unless the sound system backend type
 * was chosen manually with cafe_mixer_context_set_backend_type(), the library
 * will find a working sound system automatically.
 *
 * This function can complete the operation either synchronously or
 * asynchronously and it may go through a series of connection
 * #CafeMixerContext:state transitions.
 *
 * If this function returns %TRUE, the connection has either been established, or
 * it hasn't been established yet and the result will be determined asynchronously.
 * You can differentiate between these two possibilities by checking the connection
 * #CafeMixerContext:state after this function returns.
 *
 * The %CAFE_MIXER_STATE_READY state indicates that the connection has been
 * established successfully.
 *
 * The %CAFE_MIXER_STATE_CONNECTING state is reached when the connection has not been
 * established yet and you should wait for the state to change to either
 * %CAFE_MIXER_STATE_READY or %CAFE_MIXER_STATE_FAILED. It is required to have a main
 * loop running to allow an asynchronous connection to proceed. The library will use
 * the thread's default main context for this purpose.
 *
 * If this function returns %FALSE, it was not possible to connect to a sound system
 * and the #CafeMixerContext:state will be set to %CAFE_MIXER_STATE_FAILED.
 *
 * Returns: %TRUE on success or if the result will be determined asynchronously,
 * or %FALSE on failure.
 */
gboolean
cafe_mixer_context_open (CafeMixerContext *context)
{
    CafeMixerBackendModule     *module = NULL;
    CafeMixerState              state;
    const GList                *modules;
    const CafeMixerBackendInfo *info = NULL;

    g_return_val_if_fail (CAFE_MIXER_IS_CONTEXT (context), FALSE);

    if (context->priv->state == CAFE_MIXER_STATE_CONNECTING ||
        context->priv->state == CAFE_MIXER_STATE_READY)
        return FALSE;

    /* We are going to choose the first backend to try. It will be either the one
     * selected by the application or the one with the highest priority */
    modules = _cafe_mixer_list_modules ();

    if (context->priv->backend_type != CAFE_MIXER_BACKEND_UNKNOWN) {
        while (modules != NULL) {
            const CafeMixerBackendInfo *info;

            module = CAFE_MIXER_BACKEND_MODULE (modules->data);
            info   = cafe_mixer_backend_module_get_info (module);

            if (info->backend_type == context->priv->backend_type)
                break;

            module = NULL;
            modules = modules->next;
        }
        if (module == NULL) {
            /* The selected backend is not available */
            change_state (context, CAFE_MIXER_STATE_FAILED);
            return FALSE;
        }
    } else {
        /* The highest priority module is on the top of the list */
        module = CAFE_MIXER_BACKEND_MODULE (modules->data);
    }

    if (info == NULL)
        info = cafe_mixer_backend_module_get_info (module);

    context->priv->module  = g_object_ref (module);
    context->priv->backend = g_object_new (info->g_type, NULL);

    cafe_mixer_backend_set_app_info (context->priv->backend, context->priv->app_info);
    cafe_mixer_backend_set_server_address (context->priv->backend, context->priv->server_address);

    g_debug ("Trying to open backend %s", info->name);

    /* This transitional state is always present, it will change to CAFE_MIXER_STATE_READY
     * or CAFE_MIXER_STATE_FAILED either instantly or asynchronously */
    change_state (context, CAFE_MIXER_STATE_CONNECTING);

    /* The backend initialization might fail in case it is known right now that
     * the backend is unusable */
    if (cafe_mixer_backend_open (context->priv->backend) == FALSE) {
        if (context->priv->backend_type == CAFE_MIXER_BACKEND_UNKNOWN) {
            /* User didn't request a specific backend, so try another one */
            return try_next_backend (context);
        }

        /* User requested a specific backend and it failed */
        close_context (context);
        change_state (context, CAFE_MIXER_STATE_FAILED);
        return FALSE;
    }

    state = cafe_mixer_backend_get_state (context->priv->backend);

    if (G_UNLIKELY (state != CAFE_MIXER_STATE_READY &&
                    state != CAFE_MIXER_STATE_CONNECTING)) {
        /* This would be a backend bug */
        g_warn_if_reached ();

        if (context->priv->backend_type == CAFE_MIXER_BACKEND_UNKNOWN)
            return try_next_backend (context);

        close_context (context);
        change_state (context, CAFE_MIXER_STATE_FAILED);
        return FALSE;
    }

    g_signal_connect (G_OBJECT (context->priv->backend),
                      "notify::state",
                      G_CALLBACK (on_backend_state_notify),
                      context);

    change_state (context, state);
    return TRUE;
}

/**
 * cafe_mixer_context_close:
 * @context: a #CafeMixerContext
 *
 * Closes an open connection to the sound system. The #CafeMixerContext:state
 * will be set to %CAFE_MIXER_STATE_IDLE.
 */
void
cafe_mixer_context_close (CafeMixerContext *context)
{
    g_return_if_fail (CAFE_MIXER_IS_CONTEXT (context));

    close_context (context);
    change_state (context, CAFE_MIXER_STATE_IDLE);
}

/**
 * cafe_mixer_context_get_state:
 * @context: a #CafeMixerContext
 *
 * Gets the state of the @context's connection to a sound system.
 *
 * Returns: the connection state.
 */
CafeMixerState
cafe_mixer_context_get_state (CafeMixerContext *context)
{
    g_return_val_if_fail (CAFE_MIXER_IS_CONTEXT (context), CAFE_MIXER_STATE_UNKNOWN);

    return context->priv->state;
}

/**
 * cafe_mixer_context_get_device:
 * @context: a #CafeMixerContext
 * @name: a device name
 *
 * Gets the device with the given name.
 *
 * Returns: a #CafeMixerDevice or %NULL if there is no such device.
 */
CafeMixerDevice *
cafe_mixer_context_get_device (CafeMixerContext *context, const gchar *name)
{
    g_return_val_if_fail (CAFE_MIXER_IS_CONTEXT (context), NULL);
    g_return_val_if_fail (name != NULL, NULL);

    if (context->priv->state != CAFE_MIXER_STATE_READY)
        return NULL;

    return cafe_mixer_backend_get_device (CAFE_MIXER_BACKEND (context->priv->backend), name);
}

/**
 * cafe_mixer_context_get_stream:
 * @context: a #CafeMixerContext
 * @name: a stream name
 *
 * Gets the stream with the given name.
 *
 * Returns: a #CafeMixerStream or %NULL if there is no such stream.
 */
CafeMixerStream *
cafe_mixer_context_get_stream (CafeMixerContext *context, const gchar *name)
{
    g_return_val_if_fail (CAFE_MIXER_IS_CONTEXT (context), NULL);
    g_return_val_if_fail (name != NULL, NULL);

    if (context->priv->state != CAFE_MIXER_STATE_READY)
        return NULL;

    return cafe_mixer_backend_get_stream (CAFE_MIXER_BACKEND (context->priv->backend), name);
}

/**
 * cafe_mixer_context_get_stored_control:
 * @context: a #CafeMixerContext
 * @name: a stored control name
 *
 * Gets the stored control with the given name.
 *
 * Returns: a #CafeMixerStoredControl or %NULL if there is no such stored control.
 */
CafeMixerStoredControl *
cafe_mixer_context_get_stored_control (CafeMixerContext *context, const gchar *name)
{
    g_return_val_if_fail (CAFE_MIXER_IS_CONTEXT (context), NULL);
    g_return_val_if_fail (name != NULL, NULL);

    if (context->priv->state != CAFE_MIXER_STATE_READY)
        return NULL;

    return cafe_mixer_backend_get_stored_control (CAFE_MIXER_BACKEND (context->priv->backend), name);
}

/**
 * cafe_mixer_context_list_devices:
 * @context: a #CafeMixerContext
 *
 * Gets a list of devices. Each item in the list is a #CafeMixerDevice representing
 * a sound device in the system.
 *
 * The returned #GList is owned by the library and may be invalidated at any time.
 *
 * Returns: a #GList of all devices in the system or %NULL if there are none or
 * you are not connected to a sound system.
 */
const GList *
cafe_mixer_context_list_devices (CafeMixerContext *context)
{
    g_return_val_if_fail (CAFE_MIXER_IS_CONTEXT (context), NULL);

    if (context->priv->state != CAFE_MIXER_STATE_READY)
        return NULL;

    return cafe_mixer_backend_list_devices (CAFE_MIXER_BACKEND (context->priv->backend));
}

/**
 * cafe_mixer_context_list_streams:
 * @context: a #CafeMixerContext
 *
 * Gets a list of streams. Each item in the list is a #CafeMixerStream representing
 * an input or output stream.
 *
 * Note that the list will contain streams which belong to devices as well
 * as streams which do not. If you are only interested in streams of a specific
 * device, use cafe_mixer_device_list_streams().
 *
 * The returned #GList is owned by the library and may be invalidated at any time.
 *
 * Returns: a #GList of all streams in the system or %NULL if there are none or
 * you are not connected to a sound system.
 */
const GList *
cafe_mixer_context_list_streams (CafeMixerContext *context)
{
    g_return_val_if_fail (CAFE_MIXER_IS_CONTEXT (context), NULL);

    if (context->priv->state != CAFE_MIXER_STATE_READY)
        return NULL;

    return cafe_mixer_backend_list_streams (CAFE_MIXER_BACKEND (context->priv->backend));
}

/**
 * cafe_mixer_context_list_stored_controls:
 * @context: a #CafeMixerContext
 *
 * Gets a list of stored controls. Each item in the list is a #CafeMixerStoredControl.
 *
 * The returned #GList is owned by the library and may be invalidated at any time.
 *
 * Returns: a #GList of stored controls or %NULL if there are none or you are not
 * connected to a sound system.
 */
const GList *
cafe_mixer_context_list_stored_controls (CafeMixerContext *context)
{
    g_return_val_if_fail (CAFE_MIXER_IS_CONTEXT (context), NULL);

    if (context->priv->state != CAFE_MIXER_STATE_READY)
        return NULL;

    return cafe_mixer_backend_list_stored_controls (CAFE_MIXER_BACKEND (context->priv->backend));
}

/**
 * cafe_mixer_context_get_default_input_stream:
 * @context: a #CafeMixerContext
 *
 * Gets the default input stream. The returned stream is where sound input
 * most likely comes from by default.
 *
 * Returns: a #CafeMixerStream or %NULL if there is no default input stream.
 */
CafeMixerStream *
cafe_mixer_context_get_default_input_stream (CafeMixerContext *context)
{
    g_return_val_if_fail (CAFE_MIXER_IS_CONTEXT (context), NULL);

    if (context->priv->state != CAFE_MIXER_STATE_READY)
        return NULL;

    return cafe_mixer_backend_get_default_input_stream (context->priv->backend);
}

/**
 * cafe_mixer_context_set_default_input_stream:
 * @context: a #CafeMixerContext
 * @stream: a #CafeMixerStream to set as the default input stream
 *
 * Changes the default input stream. The given @stream must be an input stream.
 *
 * Changing the default input stream may not be supported by the sound system.
 * Use cafe_mixer_context_get_backend_flags() to find out.
 *
 * Returns: %TRUE on success or %FALSE on failure.
 */
gboolean
cafe_mixer_context_set_default_input_stream (CafeMixerContext *context,
                                             CafeMixerStream  *stream)
{
    g_return_val_if_fail (CAFE_MIXER_IS_CONTEXT (context), FALSE);
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM (stream), FALSE);

    if (context->priv->state != CAFE_MIXER_STATE_READY)
        return FALSE;

    return cafe_mixer_backend_set_default_input_stream (context->priv->backend, stream);
}

/**
 * cafe_mixer_context_get_default_output_stream:
 * @context: a #CafeMixerContext
 *
 * Gets the default output stream. The returned stream is where sound output is
 * most likely directed to by default.
 *
 * Returns: a #CafeMixerStream or %NULL if there are no output streams in
 * the system.
 */
CafeMixerStream *
cafe_mixer_context_get_default_output_stream (CafeMixerContext *context)
{
    g_return_val_if_fail (CAFE_MIXER_IS_CONTEXT (context), NULL);

    if (context->priv->state != CAFE_MIXER_STATE_READY)
        return NULL;

    return cafe_mixer_backend_get_default_output_stream (context->priv->backend);
}

/**
 * cafe_mixer_context_set_default_output_stream:
 * @context: a #CafeMixerContext
 * @stream: a #CafeMixerStream to set as the default output stream
 *
 * Changes the default output stream. The given @stream must be an output stream.
 *
 * Changing the default output stream may not be supported by the sound system.
 * Use cafe_mixer_context_get_backend_flags() to find out.
 *
 * Returns: %TRUE on success or %FALSE on failure.
 */
gboolean
cafe_mixer_context_set_default_output_stream (CafeMixerContext *context,
                                              CafeMixerStream *stream)
{
    g_return_val_if_fail (CAFE_MIXER_IS_CONTEXT (context), FALSE);
    g_return_val_if_fail (CAFE_MIXER_IS_STREAM (stream), FALSE);

    if (context->priv->state != CAFE_MIXER_STATE_READY)
        return FALSE;

    return cafe_mixer_backend_set_default_output_stream (context->priv->backend, stream);
}

/**
 * cafe_mixer_context_get_backend_name:
 * @context: a #CafeMixerContext
 *
 * Gets the name of the currently used sound system backend.
 *
 * This function will not work until the @context is connected to a sound system.
 *
 * Returns: the name or %NULL on error.
 */
const gchar *
cafe_mixer_context_get_backend_name (CafeMixerContext *context)
{
    g_return_val_if_fail (CAFE_MIXER_IS_CONTEXT (context), NULL);

    if (context->priv->backend_chosen == FALSE)
        return NULL;

    return cafe_mixer_backend_module_get_info (context->priv->module)->name;
}

/**
 * cafe_mixer_context_get_backend_type:
 * @context: a #CafeMixerContext
 *
 * Gets the type of the currently used sound system backend.
 *
 * This function will not work until the @context is connected to a sound system.
 *
 * Returns: the backend type or %CAFE_MIXER_BACKEND_UNKNOWN on error.
 */
CafeMixerBackendType
cafe_mixer_context_get_backend_type (CafeMixerContext *context)
{
    g_return_val_if_fail (CAFE_MIXER_IS_CONTEXT (context), CAFE_MIXER_BACKEND_UNKNOWN);

    if (context->priv->backend_chosen == FALSE)
        return CAFE_MIXER_BACKEND_UNKNOWN;

    return cafe_mixer_backend_module_get_info (context->priv->module)->backend_type;
}

/**
 * cafe_mixer_context_get_backend_flags:
 * @context: a #CafeMixerContext
 *
 * Gets the capability flags of the currently used sound system backend.
 *
 * This function will not work until the @context is connected to a sound system.
 *
 * Returns: the capability flags.
 */
CafeMixerBackendFlags
cafe_mixer_context_get_backend_flags (CafeMixerContext *context)
{
    g_return_val_if_fail (CAFE_MIXER_IS_CONTEXT (context), CAFE_MIXER_BACKEND_NO_FLAGS);

    if (context->priv->backend_chosen == FALSE)
        return CAFE_MIXER_BACKEND_NO_FLAGS;

    return cafe_mixer_backend_module_get_info (context->priv->module)->backend_flags;
}

static void
on_backend_state_notify (CafeMixerBackend *backend,
                         GParamSpec       *pspec,
                         CafeMixerContext *context)
{
    CafeMixerState state = cafe_mixer_backend_get_state (backend);

    switch (state) {
    case CAFE_MIXER_STATE_CONNECTING:
        g_debug ("Backend %s changed state to CONNECTING",
                 cafe_mixer_backend_module_get_info (context->priv->module)->name);

        change_state (context, state);
        break;

    case CAFE_MIXER_STATE_READY:
        g_debug ("Backend %s changed state to READY",
                 cafe_mixer_backend_module_get_info (context->priv->module)->name);

        change_state (context, state);
        break;

    case CAFE_MIXER_STATE_FAILED:
        g_debug ("Backend %s changed state to FAILED",
                 cafe_mixer_backend_module_get_info (context->priv->module)->name);

        if (context->priv->backend_type == CAFE_MIXER_BACKEND_UNKNOWN) {
            /* User didn't request a specific backend, so try another one */
            try_next_backend (context);
        } else {
            /* User requested a specific backend and it failed */
            close_context (context);
            change_state (context, state);
        }
        break;

    default:
        break;
    }
}

static void
on_backend_device_added (CafeMixerBackend *backend,
                         const gchar      *name,
                         CafeMixerContext *context)
{
    g_signal_emit (G_OBJECT (context),
                   signals[DEVICE_ADDED],
                   0,
                   name);
}

static void
on_backend_device_removed (CafeMixerBackend *backend,
                           const gchar      *name,
                           CafeMixerContext *context)
{
    g_signal_emit (G_OBJECT (context),
                   signals[DEVICE_REMOVED],
                   0,
                   name);
}

static void
on_backend_stream_added (CafeMixerBackend *backend,
                         const gchar      *name,
                         CafeMixerContext *context)
{
    g_signal_emit (G_OBJECT (context),
                   signals[STREAM_ADDED],
                   0,
                   name);
}

static void
on_backend_stream_removed (CafeMixerBackend *backend,
                           const gchar      *name,
                           CafeMixerContext *context)
{
    g_signal_emit (G_OBJECT (context),
                   signals[STREAM_REMOVED],
                   0,
                   name);
}

static void
on_backend_stored_control_added (CafeMixerBackend *backend,
                                 const gchar      *name,
                                 CafeMixerContext *context)
{
    g_signal_emit (G_OBJECT (context),
                   signals[STORED_CONTROL_ADDED],
                   0,
                   name);
}

static void
on_backend_stored_control_removed (CafeMixerBackend *backend,
                                   const gchar      *name,
                                   CafeMixerContext *context)
{
    g_signal_emit (G_OBJECT (context),
                   signals[STORED_CONTROL_REMOVED],
                   0,
                   name);
}

static void
on_backend_default_input_stream_notify (CafeMixerBackend *backend,
                                        GParamSpec       *pspec,
                                        CafeMixerContext *context)
{
    g_object_notify_by_pspec (G_OBJECT (context), properties[PROP_DEFAULT_INPUT_STREAM]);
}

static void
on_backend_default_output_stream_notify (CafeMixerBackend *backend,
                                         GParamSpec       *pspec,
                                         CafeMixerContext *context)
{
    g_object_notify_by_pspec (G_OBJECT (context), properties[PROP_DEFAULT_OUTPUT_STREAM]);
}

static gboolean
try_next_backend (CafeMixerContext *context)
{
    CafeMixerBackendModule     *module = NULL;
    CafeMixerState              state;
    const GList                *modules;
    const CafeMixerBackendInfo *info = NULL;

    modules = _cafe_mixer_list_modules ();

    while (modules != NULL) {
        if (context->priv->module == modules->data) {
            /* Found the last tested backend, try to use the next one with a lower
             * priority unless we have reached the end of the list */
            if (modules->next != NULL)
                module = CAFE_MIXER_BACKEND_MODULE (modules->next->data);
            break;
        }
        modules = modules->next;
    }
    close_context (context);

    if (module == NULL) {
        /* We have tried all the modules and all of them failed */
        change_state (context, CAFE_MIXER_STATE_FAILED);
        return FALSE;
    }

    info = cafe_mixer_backend_module_get_info (module);

    context->priv->module  = g_object_ref (module);
    context->priv->backend = g_object_new (info->g_type, NULL);

    cafe_mixer_backend_set_app_info (context->priv->backend, context->priv->app_info);
    cafe_mixer_backend_set_server_address (context->priv->backend, context->priv->server_address);

    g_debug ("Trying to open backend %s", info->name);

    /* Try to open this backend and in case of failure keep trying until we find
     * one that works or reach the end of the list */
    if (cafe_mixer_backend_open (context->priv->backend) == FALSE)
        return try_next_backend (context);

    state = cafe_mixer_backend_get_state (context->priv->backend);

    if (G_UNLIKELY (state != CAFE_MIXER_STATE_READY &&
                    state != CAFE_MIXER_STATE_CONNECTING)) {
        /* This would be a backend bug */
        g_warn_if_reached ();

        return try_next_backend (context);
    }

    g_signal_connect (G_OBJECT (context->priv->backend),
                      "notify::state",
                      G_CALLBACK (on_backend_state_notify),
                      context);

    change_state (context, state);
    return TRUE;
}

static void
change_state (CafeMixerContext *context, CafeMixerState state)
{
    if (context->priv->state == state)
        return;

    context->priv->state = state;

    if (state == CAFE_MIXER_STATE_READY && context->priv->backend_chosen == FALSE) {
        /* It is safe to connect to the backend signals after reaching the READY
         * state, because the app is not allowed to query any data before that state;
         * therefore we won't end up in an inconsistent state by caching a list and
         * then missing a notification about a change in the list */
        g_signal_connect (G_OBJECT (context->priv->backend),
                          "device-added",
                          G_CALLBACK (on_backend_device_added),
                          context);
        g_signal_connect (G_OBJECT (context->priv->backend),
                          "device-removed",
                          G_CALLBACK (on_backend_device_removed),
                          context);
        g_signal_connect (G_OBJECT (context->priv->backend),
                          "stream-added",
                          G_CALLBACK (on_backend_stream_added),
                          context);
        g_signal_connect (G_OBJECT (context->priv->backend),
                          "stream-removed",
                          G_CALLBACK (on_backend_stream_removed),
                          context);
        g_signal_connect (G_OBJECT (context->priv->backend),
                          "stored-control-added",
                          G_CALLBACK (on_backend_stored_control_added),
                          context);
        g_signal_connect (G_OBJECT (context->priv->backend),
                          "stored-control-removed",
                          G_CALLBACK (on_backend_stored_control_removed),
                          context);

        g_signal_connect (G_OBJECT (context->priv->backend),
                          "notify::default-input-stream",
                          G_CALLBACK (on_backend_default_input_stream_notify),
                          context);
        g_signal_connect (G_OBJECT (context->priv->backend),
                          "notify::default-output-stream",
                          G_CALLBACK (on_backend_default_output_stream_notify),
                          context);

        context->priv->backend_chosen = TRUE;
    }

    g_object_notify_by_pspec (G_OBJECT (context), properties[PROP_STATE]);
}

static void
close_context (CafeMixerContext *context)
{
    if (context->priv->backend != NULL) {
        g_signal_handlers_disconnect_by_data (G_OBJECT (context->priv->backend),
                                              context);

        cafe_mixer_backend_close (context->priv->backend);
        g_clear_object (&context->priv->backend);
    }

    g_clear_object (&context->priv->module);

    context->priv->backend_chosen = FALSE;
}
