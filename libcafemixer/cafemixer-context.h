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

#ifndef CAFEMIXER_CONTEXT_H
#define CAFEMIXER_CONTEXT_H

#include <glib.h>
#include <glib-object.h>

#include <libcafemixer/cafemixer-enums.h>
#include <libcafemixer/cafemixer-types.h>

G_BEGIN_DECLS

#define CAFE_MIXER_TYPE_CONTEXT                 \
        (cafe_mixer_context_get_type ())
#define CAFE_MIXER_CONTEXT(o)                   \
        (G_TYPE_CHECK_INSTANCE_CAST ((o), CAFE_MIXER_TYPE_CONTEXT, MateMixerContext))
#define CAFE_MIXER_IS_CONTEXT(o)                \
        (G_TYPE_CHECK_INSTANCE_TYPE ((o), CAFE_MIXER_TYPE_CONTEXT))
#define CAFE_MIXER_CONTEXT_CLASS(k)             \
        (G_TYPE_CHECK_CLASS_CAST ((k), CAFE_MIXER_TYPE_CONTEXT, MateMixerContextClass))
#define CAFE_MIXER_IS_CONTEXT_CLASS(k)          \
        (G_TYPE_CHECK_CLASS_TYPE ((k), CAFE_MIXER_TYPE_CONTEXT))
#define CAFE_MIXER_CONTEXT_GET_CLASS(o)         \
        (G_TYPE_INSTANCE_GET_CLASS ((o), CAFE_MIXER_TYPE_CONTEXT, MateMixerContextClass))

typedef struct _MateMixerContextClass    MateMixerContextClass;
typedef struct _MateMixerContextPrivate  MateMixerContextPrivate;

/**
 * MateMixerContext:
 *
 * The #MateMixerContext structure contains only private data and should only
 * be accessed using the provided API.
 */
struct _MateMixerContext
{
    GObject parent;

    /*< private >*/
    MateMixerContextPrivate *priv;
};

/**
 * MateMixerContextClass:
 * @parent_class: The parent class.
 *
 * The class structure for #MateMixerContext.
 */
struct _MateMixerContextClass
{
    GObjectClass parent_class;

    /*< private >*/
    void (*device_added)           (MateMixerContext *context,
                                    const gchar      *name);
    void (*device_removed)         (MateMixerContext *context,
                                    const gchar      *name);
    void (*stream_added)           (MateMixerContext *context,
                                    const gchar      *name);
    void (*stream_removed)         (MateMixerContext *context,
                                    const gchar      *name);
    void (*stored_control_added)   (MateMixerContext *context,
                                    const gchar      *name);
    void (*stored_control_removed) (MateMixerContext *context,
                                    const gchar      *name);
};

GType                   cafe_mixer_context_get_type                  (void) G_GNUC_CONST;

MateMixerContext *      cafe_mixer_context_new                       (void);

gboolean                cafe_mixer_context_set_backend_type          (MateMixerContext     *context,
                                                                      MateMixerBackendType  backend_type);
gboolean                cafe_mixer_context_set_app_name              (MateMixerContext     *context,
                                                                      const gchar          *app_name);
gboolean                cafe_mixer_context_set_app_id                (MateMixerContext     *context,
                                                                      const gchar          *app_id);
gboolean                cafe_mixer_context_set_app_version           (MateMixerContext     *context,
                                                                      const gchar          *app_version);
gboolean                cafe_mixer_context_set_app_icon              (MateMixerContext     *context,
                                                                      const gchar          *app_icon);
gboolean                cafe_mixer_context_set_server_address        (MateMixerContext     *context,
                                                                      const gchar          *address);

gboolean                cafe_mixer_context_open                      (MateMixerContext     *context);
void                    cafe_mixer_context_close                     (MateMixerContext     *context);

MateMixerState          cafe_mixer_context_get_state                 (MateMixerContext     *context);

MateMixerDevice *       cafe_mixer_context_get_device                (MateMixerContext     *context,
                                                                      const gchar          *name);
MateMixerStream *       cafe_mixer_context_get_stream                (MateMixerContext     *context,
                                                                      const gchar          *name);
MateMixerStoredControl *cafe_mixer_context_get_stored_control        (MateMixerContext     *context,
                                                                      const gchar          *name);

const GList *           cafe_mixer_context_list_devices              (MateMixerContext     *context);
const GList *           cafe_mixer_context_list_streams              (MateMixerContext     *context);
const GList *           cafe_mixer_context_list_stored_controls      (MateMixerContext     *context);

MateMixerStream *       cafe_mixer_context_get_default_input_stream  (MateMixerContext     *context);
gboolean                cafe_mixer_context_set_default_input_stream  (MateMixerContext     *context,
                                                                      MateMixerStream      *stream);

MateMixerStream *       cafe_mixer_context_get_default_output_stream (MateMixerContext     *context);
gboolean                cafe_mixer_context_set_default_output_stream (MateMixerContext     *context,
                                                                      MateMixerStream      *stream);

const gchar *           cafe_mixer_context_get_backend_name          (MateMixerContext     *context);
MateMixerBackendType    cafe_mixer_context_get_backend_type          (MateMixerContext     *context);
MateMixerBackendFlags   cafe_mixer_context_get_backend_flags         (MateMixerContext     *context);

G_END_DECLS

#endif /* CAFEMIXER_CONTEXT_H */