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

#include "cafemixer-enums.h"

G_BEGIN_DECLS

#define CAFE_MIXER_TYPE_BACKEND_MODULE          \
        (cafe_mixer_backend_module_get_type ())
#define CAFE_MIXER_BACKEND_MODULE(o)            \
        (G_TYPE_CHECK_INSTANCE_CAST ((o), CAFE_MIXER_TYPE_BACKEND_MODULE, CafeMixerBackendModule))
#define CAFE_MIXER_IS_BACKEND_MODULE(o)         \
        (G_TYPE_CHECK_INSTANCE_TYPE ((o), CAFE_MIXER_TYPE_BACKEND_MODULE))
#define CAFE_MIXER_BACKEND_MODULE_CLASS(k)      \
        (G_TYPE_CHECK_CLASS_CAST ((k), CAFE_MIXER_TYPE_BACKEND_MODULE, CafeMixerBackendModuleClass))
#define CAFE_MIXER_IS_BACKEND_MODULE_CLASS(k)   \
        (G_TYPE_CHECK_CLASS_TYPE ((k), CAFE_MIXER_TYPE_BACKEND_MODULE))
#define CAFE_MIXER_BACKEND_MODULE_GET_CLASS(o)  \
        (G_TYPE_INSTANCE_GET_CLASS ((o), CAFE_MIXER_TYPE_BACKEND_MODULE, CafeMixerBackendModuleClass))

typedef struct _CafeMixerBackendInfo           CafeMixerBackendInfo;
typedef struct _CafeMixerBackendModule         CafeMixerBackendModule;
typedef struct _CafeMixerBackendModuleClass    CafeMixerBackendModuleClass;
typedef struct _CafeMixerBackendModulePrivate  CafeMixerBackendModulePrivate;

struct _CafeMixerBackendModule
{
    GTypeModule parent;

    /*< private >*/
    CafeMixerBackendModulePrivate *priv;
};

struct _CafeMixerBackendModuleClass
{
    GTypeModuleClass parent_class;
};

struct _CafeMixerBackendInfo
{
    gchar                *name;
    guint                 priority;
    GType                 g_type;
    CafeMixerBackendFlags backend_flags;
    CafeMixerBackendType  backend_type;
};

GType                       cafe_mixer_backend_module_get_type (void) G_GNUC_CONST;

CafeMixerBackendModule *    cafe_mixer_backend_module_new      (const gchar            *path);

const CafeMixerBackendInfo *cafe_mixer_backend_module_get_info (CafeMixerBackendModule *module);
const gchar *               cafe_mixer_backend_module_get_path (CafeMixerBackendModule *module);

G_END_DECLS

#endif /* CAFEMIXER_BACKEND_MODULE_H */
