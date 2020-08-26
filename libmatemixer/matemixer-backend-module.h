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

#ifndef CAFEMIXER_BACKEND_MODULE_H
#define CAFEMIXER_BACKEND_MODULE_H

#include <glib.h>
#include <glib-object.h>

#include "matemixer-enums.h"

G_BEGIN_DECLS

#define CAFE_MIXER_TYPE_BACKEND_MODULE          \
        (mate_mixer_backend_module_get_type ())
#define CAFE_MIXER_BACKEND_MODULE(o)            \
        (G_TYPE_CHECK_INSTANCE_CAST ((o), CAFE_MIXER_TYPE_BACKEND_MODULE, MateMixerBackendModule))
#define CAFE_MIXER_IS_BACKEND_MODULE(o)         \
        (G_TYPE_CHECK_INSTANCE_TYPE ((o), CAFE_MIXER_TYPE_BACKEND_MODULE))
#define CAFE_MIXER_BACKEND_MODULE_CLASS(k)      \
        (G_TYPE_CHECK_CLASS_CAST ((k), CAFE_MIXER_TYPE_BACKEND_MODULE, MateMixerBackendModuleClass))
#define CAFE_MIXER_IS_BACKEND_MODULE_CLASS(k)   \
        (G_TYPE_CHECK_CLASS_TYPE ((k), CAFE_MIXER_TYPE_BACKEND_MODULE))
#define CAFE_MIXER_BACKEND_MODULE_GET_CLASS(o)  \
        (G_TYPE_INSTANCE_GET_CLASS ((o), CAFE_MIXER_TYPE_BACKEND_MODULE, MateMixerBackendModuleClass))

typedef struct _MateMixerBackendInfo           MateMixerBackendInfo;
typedef struct _MateMixerBackendModule         MateMixerBackendModule;
typedef struct _MateMixerBackendModuleClass    MateMixerBackendModuleClass;
typedef struct _MateMixerBackendModulePrivate  MateMixerBackendModulePrivate;

struct _MateMixerBackendModule
{
    GTypeModule parent;

    /*< private >*/
    MateMixerBackendModulePrivate *priv;
};

struct _MateMixerBackendModuleClass
{
    GTypeModuleClass parent_class;
};

struct _MateMixerBackendInfo
{
    gchar                *name;
    guint                 priority;
    GType                 g_type;
    MateMixerBackendFlags backend_flags;
    MateMixerBackendType  backend_type;
};

GType                       mate_mixer_backend_module_get_type (void) G_GNUC_CONST;

MateMixerBackendModule *    mate_mixer_backend_module_new      (const gchar            *path);

const MateMixerBackendInfo *mate_mixer_backend_module_get_info (MateMixerBackendModule *module);
const gchar *               mate_mixer_backend_module_get_path (MateMixerBackendModule *module);

G_END_DECLS

#endif /* CAFEMIXER_BACKEND_MODULE_H */
