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
        (G_TYPE_CHECK_INSTANCE_CAST ((o), CAFE_MIXER_TYPE_CONTEXT, CafeMixerContext))
#define CAFE_MIXER_IS_CONTEXT(o)                \
        (G_TYPE_CHECK_INSTANCE_TYPE ((o), CAFE_MIXER_TYPE_CONTEXT))
#define CAFE_MIXER_CONTEXT_CLASS(k)             \
        (G_TYPE_CHECK_CLASS_CAST ((k), CAFE_MIXER_TYPE_CONTEXT, CafeMixerContextClass))
#define CAFE_MIXER_IS_CONTEXT_CLASS(k)          \
        (G_TYPE_CHECK_CLASS_TYPE ((k), CAFE_MIXER_TYPE_CONTEXT))
#define CAFE_MIXER_CONTEXT_GET_CLASS(o)         \
        (G_TYPE_INSTANCE_GET_CLASS ((o), CAFE_MIXER_TYPE_CONTEXT, CafeMixerContextClass))

typedef struct _CafeMixerContextClass    CafeMixerContextClass;
typedef struct _CafeMixerContextPrivate  CafeMixerContextPrivate;

/**
 * CafeMixerContext:
 *
 * The #CafeMixerContext structure contains only private data and should only
 * be accessed using the provided API.
 */
struct _CafeMixerContext
{
    GObject parent;

    /*< private >*/
    CafeMixerContextPrivate *priv;
};

/**
 * CafeMixerContextClass:
 * @parent_class: The parent class.
 *
 * The class structure for #CafeMixerContext.
 */
struct _CafeMixerContextClass
{
    GObjectClass parent_class;

    /*< private >*/
    void (*device_added)           (CafeMixerContext *context,
                                    const gchar      *name);
    void (*device_removed)         (CafeMixerContext *context,
                                    const gchar      *name);
    void (*stream_added)           (CafeMixerContext *context,
                                    const gchar      *name);
    void (*stream_removed)         (CafeMixerContext *context,
                                    const gchar      *name);
    void (*stored_control_added)   (CafeMixerContext *context,
                                    const gchar      *name);
    void (*stored_control_removed) (CafeMixerContext *context,
                                    const gchar      *name);
};

GType                   cafe_mixer_context_get_type                  (void) G_GNUC_CONST;

CafeMixerContext *      cafe_mixer_context_new                       (void);

gboolean                cafe_mixer_context_set_backend_type          (CafeMixerContext     *context,
                                                                      CafeMixerBackendType  backend_type);
gboolean                cafe_mixer_context_set_app_name              (CafeMixerContext     *context,
                                                                      const gchar          *app_name);
gboolean                cafe_mixer_context_set_app_id                (CafeMixerContext     *context,
                                                                      const gchar          *app_id);
gboolean                cafe_mixer_context_set_app_version           (CafeMixerContext     *context,
                                                                      const gchar          *app_version);
gboolean                cafe_mixer_context_set_app_icon              (CafeMixerContext     *context,
                                                                      const gchar          *app_icon);
gboolean                cafe_mixer_context_set_server_address        (CafeMixerContext     *context,
                                                                      const gchar          *address);

gboolean                cafe_mixer_context_open                      (CafeMixerContext     *context);
void                    cafe_mixer_context_close                     (CafeMixerContext     *context);

CafeMixerState          cafe_mixer_context_get_state                 (CafeMixerContext     *context);

CafeMixerDevice *       cafe_mixer_context_get_device                (CafeMixerContext     *context,
                                                                      const gchar          *name);
CafeMixerStream *       cafe_mixer_context_get_stream                (CafeMixerContext     *context,
                                                                      const gchar          *name);
CafeMixerStoredControl *cafe_mixer_context_get_stored_control        (CafeMixerContext     *context,
                                                                      const gchar          *name);

const GList *           cafe_mixer_context_list_devices              (CafeMixerContext     *context);
const GList *           cafe_mixer_context_list_streams              (CafeMixerContext     *context);
const GList *           cafe_mixer_context_list_stored_controls      (CafeMixerContext     *context);

CafeMixerStream *       cafe_mixer_context_get_default_input_stream  (CafeMixerContext     *context);
gboolean                cafe_mixer_context_set_default_input_stream  (CafeMixerContext     *context,
                                                                      CafeMixerStream      *stream);

CafeMixerStream *       cafe_mixer_context_get_default_output_stream (CafeMixerContext     *context);
gboolean                cafe_mixer_context_set_default_output_stream (CafeMixerContext     *context,
                                                                      CafeMixerStream      *stream);

const gchar *           cafe_mixer_context_get_backend_name          (CafeMixerContext     *context);
CafeMixerBackendType    cafe_mixer_context_get_backend_type          (CafeMixerContext     *context);
CafeMixerBackendFlags   cafe_mixer_context_get_backend_flags         (CafeMixerContext     *context);

G_END_DECLS

#endif /* CAFEMIXER_CONTEXT_H */
