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

#ifndef CAFEMIXER_BACKEND_H
#define CAFEMIXER_BACKEND_H

#include <glib.h>
#include <glib-object.h>

#include "cafemixer-enums.h"
#include "cafemixer-types.h"

G_BEGIN_DECLS

#define CAFE_MIXER_TYPE_BACKEND                 \
        (cafe_mixer_backend_get_type ())
#define CAFE_MIXER_BACKEND(o)                   \
        (G_TYPE_CHECK_INSTANCE_CAST ((o), CAFE_MIXER_TYPE_BACKEND, CafeMixerBackend))
#define CAFE_MIXER_IS_BACKEND(o)                \
        (G_TYPE_CHECK_INSTANCE_TYPE ((o), CAFE_MIXER_TYPE_BACKEND))
#define CAFE_MIXER_BACKEND_CLASS(k)             \
        (G_TYPE_CHECK_CLASS_CAST ((k), CAFE_MIXER_TYPE_BACKEND, CafeMixerBackendClass))
#define CAFE_MIXER_IS_BACKEND_CLASS(k)          \
        (G_TYPE_CHECK_CLASS_TYPE ((k), CAFE_MIXER_TYPE_BACKEND))
#define CAFE_MIXER_BACKEND_GET_CLASS(o)         \
        (G_TYPE_INSTANCE_GET_CLASS ((o), CAFE_MIXER_TYPE_BACKEND, CafeMixerBackendClass))

typedef struct _CafeMixerBackend         CafeMixerBackend;
typedef struct _CafeMixerBackendClass    CafeMixerBackendClass;
typedef struct _CafeMixerBackendPrivate  CafeMixerBackendPrivate;

struct _CafeMixerBackend
{
    GObject object;

    /*< private >*/
    CafeMixerBackendPrivate *priv;
};

struct _CafeMixerBackendClass
{
    GObjectClass parent_class;

    /*< private >*/
    void         (*set_app_info)              (CafeMixerBackend *backend,
                                               CafeMixerAppInfo *info);
    void         (*set_server_address)        (CafeMixerBackend *backend,
                                               const gchar      *address);

    gboolean     (*open)                      (CafeMixerBackend *backend);
    void         (*close)                     (CafeMixerBackend *backend);

    const GList *(*list_devices)              (CafeMixerBackend *backend);
    const GList *(*list_streams)              (CafeMixerBackend *backend);
    const GList *(*list_stored_controls)      (CafeMixerBackend *backend);

    gboolean     (*set_default_input_stream)  (CafeMixerBackend *backend,
                                               CafeMixerStream  *stream);
    gboolean     (*set_default_output_stream) (CafeMixerBackend *backend,
                                               CafeMixerStream  *stream);

    /* Signals */
    void         (*device_added)              (CafeMixerBackend *backend,
                                               const gchar      *name);
    void         (*device_removed)            (CafeMixerBackend *backend,
                                               const gchar      *name);
    void         (*stream_added)              (CafeMixerBackend *backend,
                                               const gchar      *name);
    void         (*stream_removed)            (CafeMixerBackend *backend,
                                               const gchar      *name);
    void         (*stored_control_added)      (CafeMixerBackend *backend,
                                               const gchar      *name);
    void         (*stored_control_removed)    (CafeMixerBackend *backend,
                                               const gchar      *name);
};

GType                   cafe_mixer_backend_get_type                  (void) G_GNUC_CONST;

void                    cafe_mixer_backend_set_app_info              (CafeMixerBackend *backend,
                                                                      CafeMixerAppInfo *info);
void                    cafe_mixer_backend_set_server_address        (CafeMixerBackend *backend,
                                                                      const gchar      *address);

gboolean                cafe_mixer_backend_open                      (CafeMixerBackend *backend);
void                    cafe_mixer_backend_close                     (CafeMixerBackend *backend);

CafeMixerState          cafe_mixer_backend_get_state                 (CafeMixerBackend *backend);

CafeMixerDevice *       cafe_mixer_backend_get_device                (CafeMixerBackend *backend,
                                                                      const gchar      *name);
CafeMixerStream *       cafe_mixer_backend_get_stream                (CafeMixerBackend *backend,
                                                                      const gchar      *name);
CafeMixerStoredControl *cafe_mixer_backend_get_stored_control        (CafeMixerBackend *backend,
                                                                      const gchar      *name);

const GList *           cafe_mixer_backend_list_devices              (CafeMixerBackend *backend);
const GList *           cafe_mixer_backend_list_streams              (CafeMixerBackend *backend);
const GList *           cafe_mixer_backend_list_stored_controls      (CafeMixerBackend *backend);

CafeMixerStream *       cafe_mixer_backend_get_default_input_stream  (CafeMixerBackend *backend);
gboolean                cafe_mixer_backend_set_default_input_stream  (CafeMixerBackend *backend,
                                                                      CafeMixerStream  *stream);

CafeMixerStream *       cafe_mixer_backend_get_default_output_stream (CafeMixerBackend *backend);
gboolean                cafe_mixer_backend_set_default_output_stream (CafeMixerBackend *backend,
                                                                      CafeMixerStream  *stream);

/* Protected functions */
void                   _cafe_mixer_backend_set_state                 (CafeMixerBackend *backend,
                                                                      CafeMixerState    state);

void                   _cafe_mixer_backend_set_default_input_stream  (CafeMixerBackend *backend,
                                                                      CafeMixerStream  *stream);

void                   _cafe_mixer_backend_set_default_output_stream (CafeMixerBackend *backend,
                                                                      CafeMixerStream  *stream);

G_END_DECLS

#endif /* CAFEMIXER_BACKEND_H */
