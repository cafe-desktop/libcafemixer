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
        (G_TYPE_CHECK_INSTANCE_CAST ((o), CAFE_MIXER_TYPE_BACKEND, MateMixerBackend))
#define CAFE_MIXER_IS_BACKEND(o)                \
        (G_TYPE_CHECK_INSTANCE_TYPE ((o), CAFE_MIXER_TYPE_BACKEND))
#define CAFE_MIXER_BACKEND_CLASS(k)             \
        (G_TYPE_CHECK_CLASS_CAST ((k), CAFE_MIXER_TYPE_BACKEND, MateMixerBackendClass))
#define CAFE_MIXER_IS_BACKEND_CLASS(k)          \
        (G_TYPE_CHECK_CLASS_TYPE ((k), CAFE_MIXER_TYPE_BACKEND))
#define CAFE_MIXER_BACKEND_GET_CLASS(o)         \
        (G_TYPE_INSTANCE_GET_CLASS ((o), CAFE_MIXER_TYPE_BACKEND, MateMixerBackendClass))

typedef struct _MateMixerBackend         MateMixerBackend;
typedef struct _MateMixerBackendClass    MateMixerBackendClass;
typedef struct _MateMixerBackendPrivate  MateMixerBackendPrivate;

struct _MateMixerBackend
{
    GObject object;

    /*< private >*/
    MateMixerBackendPrivate *priv;
};

struct _MateMixerBackendClass
{
    GObjectClass parent_class;

    /*< private >*/
    void         (*set_app_info)              (MateMixerBackend *backend,
                                               MateMixerAppInfo *info);
    void         (*set_server_address)        (MateMixerBackend *backend,
                                               const gchar      *address);

    gboolean     (*open)                      (MateMixerBackend *backend);
    void         (*close)                     (MateMixerBackend *backend);

    const GList *(*list_devices)              (MateMixerBackend *backend);
    const GList *(*list_streams)              (MateMixerBackend *backend);
    const GList *(*list_stored_controls)      (MateMixerBackend *backend);

    gboolean     (*set_default_input_stream)  (MateMixerBackend *backend,
                                               MateMixerStream  *stream);
    gboolean     (*set_default_output_stream) (MateMixerBackend *backend,
                                               MateMixerStream  *stream);

    /* Signals */
    void         (*device_added)              (MateMixerBackend *backend,
                                               const gchar      *name);
    void         (*device_removed)            (MateMixerBackend *backend,
                                               const gchar      *name);
    void         (*stream_added)              (MateMixerBackend *backend,
                                               const gchar      *name);
    void         (*stream_removed)            (MateMixerBackend *backend,
                                               const gchar      *name);
    void         (*stored_control_added)      (MateMixerBackend *backend,
                                               const gchar      *name);
    void         (*stored_control_removed)    (MateMixerBackend *backend,
                                               const gchar      *name);
};

GType                   cafe_mixer_backend_get_type                  (void) G_GNUC_CONST;

void                    cafe_mixer_backend_set_app_info              (MateMixerBackend *backend,
                                                                      MateMixerAppInfo *info);
void                    cafe_mixer_backend_set_server_address        (MateMixerBackend *backend,
                                                                      const gchar      *address);

gboolean                cafe_mixer_backend_open                      (MateMixerBackend *backend);
void                    cafe_mixer_backend_close                     (MateMixerBackend *backend);

MateMixerState          cafe_mixer_backend_get_state                 (MateMixerBackend *backend);

MateMixerDevice *       cafe_mixer_backend_get_device                (MateMixerBackend *backend,
                                                                      const gchar      *name);
MateMixerStream *       cafe_mixer_backend_get_stream                (MateMixerBackend *backend,
                                                                      const gchar      *name);
MateMixerStoredControl *cafe_mixer_backend_get_stored_control        (MateMixerBackend *backend,
                                                                      const gchar      *name);

const GList *           cafe_mixer_backend_list_devices              (MateMixerBackend *backend);
const GList *           cafe_mixer_backend_list_streams              (MateMixerBackend *backend);
const GList *           cafe_mixer_backend_list_stored_controls      (MateMixerBackend *backend);

MateMixerStream *       cafe_mixer_backend_get_default_input_stream  (MateMixerBackend *backend);
gboolean                cafe_mixer_backend_set_default_input_stream  (MateMixerBackend *backend,
                                                                      MateMixerStream  *stream);

MateMixerStream *       cafe_mixer_backend_get_default_output_stream (MateMixerBackend *backend);
gboolean                cafe_mixer_backend_set_default_output_stream (MateMixerBackend *backend,
                                                                      MateMixerStream  *stream);

/* Protected functions */
void                   _cafe_mixer_backend_set_state                 (MateMixerBackend *backend,
                                                                      MateMixerState    state);

void                   _cafe_mixer_backend_set_default_input_stream  (MateMixerBackend *backend,
                                                                      MateMixerStream  *stream);

void                   _cafe_mixer_backend_set_default_output_stream (MateMixerBackend *backend,
                                                                      MateMixerStream  *stream);

G_END_DECLS

#endif /* CAFEMIXER_BACKEND_H */
