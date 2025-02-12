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
#include <gmodule.h>

#include "cafemixer-backend-module.h"

/* Initialize backend */
typedef void (*BackendInit) (GTypeModule *type_module);

/* Return information about backend */
typedef const CafeMixerBackendInfo *(*BackendGetInfo) (void);

struct _CafeMixerBackendModulePrivate
{
    GModule       *gmodule;
    gchar         *path;
    gboolean       loaded;
    BackendInit    init;
    BackendGetInfo get_info;
};

enum {
    PROP_0,
    PROP_PATH
};

static void cafe_mixer_backend_module_get_property (GObject                     *object,
                                                    guint                        param_id,
                                                    GValue                      *value,
                                                    GParamSpec                  *pspec);
static void cafe_mixer_backend_module_set_property (GObject                     *object,
                                                    guint                        param_id,
                                                    const GValue                *value,
                                                    GParamSpec                  *pspec);

static void cafe_mixer_backend_module_dispose      (GObject                     *object);
static void cafe_mixer_backend_module_finalize     (GObject                     *object);

G_DEFINE_TYPE_WITH_PRIVATE (CafeMixerBackendModule, cafe_mixer_backend_module, G_TYPE_TYPE_MODULE);

static gboolean backend_module_load   (GTypeModule *gmodule);
static void     backend_module_unload (GTypeModule *gmodule);

static void
cafe_mixer_backend_module_class_init (CafeMixerBackendModuleClass *klass)
{
    GObjectClass     *object_class;
    GTypeModuleClass *module_class;

    object_class = G_OBJECT_CLASS (klass);
    object_class->dispose      = cafe_mixer_backend_module_dispose;
    object_class->finalize     = cafe_mixer_backend_module_finalize;
    object_class->get_property = cafe_mixer_backend_module_get_property;
    object_class->set_property = cafe_mixer_backend_module_set_property;

    g_object_class_install_property (object_class,
                                     PROP_PATH,
                                     g_param_spec_string ("path",
                                                          "Path",
                                                          "File path to the module",
                                                          NULL,
                                                          G_PARAM_CONSTRUCT_ONLY |
                                                          G_PARAM_READWRITE |
                                                          G_PARAM_STATIC_STRINGS));

    module_class = G_TYPE_MODULE_CLASS (klass);
    module_class->load   = backend_module_load;
    module_class->unload = backend_module_unload;
}

static void
cafe_mixer_backend_module_get_property (GObject    *object,
                                        guint       param_id,
                                        GValue     *value,
                                        GParamSpec *pspec)
{
    CafeMixerBackendModule *module;

    module = CAFE_MIXER_BACKEND_MODULE (object);

    switch (param_id) {
    case PROP_PATH:
        g_value_set_string (value, module->priv->path);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
        break;
    }
}

static void
cafe_mixer_backend_module_set_property (GObject      *object,
                                        guint         param_id,
                                        const GValue *value,
                                        GParamSpec   *pspec)
{
    CafeMixerBackendModule *module;

    module = CAFE_MIXER_BACKEND_MODULE (object);

    switch (param_id) {
    case PROP_PATH:
        /* Construct-only string */
        module->priv->path = g_value_dup_string (value);

        g_type_module_set_name (G_TYPE_MODULE (object), module->priv->path);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
        break;
    }
}

static void
cafe_mixer_backend_module_init (CafeMixerBackendModule *module)
{
    module->priv = cafe_mixer_backend_module_get_instance_private (module);
}

static void
cafe_mixer_backend_module_dispose (GObject *object)
{
    CafeMixerBackendModule *module;

    module = CAFE_MIXER_BACKEND_MODULE (object);

    if (module->priv->loaded == TRUE) {
        /* Keep the module alive and avoid calling the parent dispose, which
         * would do the same thing and also produce a warning */
        g_object_ref (object);
        return;
    }

    G_OBJECT_CLASS (cafe_mixer_backend_module_parent_class)->dispose (object);
}

static void
cafe_mixer_backend_module_finalize (GObject *object)
{
    CafeMixerBackendModule *module;

    module = CAFE_MIXER_BACKEND_MODULE (object);

    g_free (module->priv->path);

    G_OBJECT_CLASS (cafe_mixer_backend_module_parent_class)->finalize (object);
}

/**
 * cafe_mixer_backend_module_new:
 * @path: path to a backend module
 *
 * Creates a new #CafeMixerBackendModule instance.
 *
 * Returns: a new #CafeMixerBackendModule instance.
 */
CafeMixerBackendModule *
cafe_mixer_backend_module_new (const gchar *path)
{
    g_return_val_if_fail (path != NULL, NULL);

    return g_object_new (CAFE_MIXER_TYPE_BACKEND_MODULE,
                         "path", path,
                         NULL);
}

/**
 * cafe_mixer_backend_module_get_info:
 * @module: a #CafeMixerBackendModule
 *
 * Gets information about the loaded backend.
 *
 * Returns: a #CafeMixerBackendInfo.
 */
const CafeMixerBackendInfo *
cafe_mixer_backend_module_get_info (CafeMixerBackendModule *module)
{
    g_return_val_if_fail (CAFE_MIXER_IS_BACKEND_MODULE (module), NULL);
    g_return_val_if_fail (module->priv->loaded == TRUE, NULL);

    return module->priv->get_info ();
}

/**
 * cafe_mixer_backend_module_get_path:
 * @module: a #CafeMixerBackendModule
 *
 * Gets file path to the backend module.
 *
 * Returns: the file path.
 */
const gchar *
cafe_mixer_backend_module_get_path (CafeMixerBackendModule *module)
{
    g_return_val_if_fail (CAFE_MIXER_IS_BACKEND_MODULE (module), NULL);

    return module->priv->path;
}

static gboolean
backend_module_load (GTypeModule *type_module)
{
    CafeMixerBackendModule *module;

    module = CAFE_MIXER_BACKEND_MODULE (type_module);

    if (module->priv->loaded == TRUE)
        return TRUE;

    module->priv->gmodule = g_module_open (module->priv->path,
                                           G_MODULE_BIND_LAZY |
                                           G_MODULE_BIND_LOCAL);
    if (module->priv->gmodule == NULL) {
        g_warning ("Failed to load backend module %s: %s",
                   module->priv->path,
                   g_module_error ());

        return FALSE;
    }

    /* Validate library symbols that each backend module must provide */
    if (g_module_symbol (module->priv->gmodule,
                         "backend_module_init",
                         (gpointer *) &module->priv->init) == FALSE ||
        g_module_symbol (module->priv->gmodule,
                         "backend_module_get_info",
                         (gpointer *) &module->priv->get_info) == FALSE) {
        g_warning ("Failed to load backend module %s: %s",
                   module->priv->path,
                   g_module_error ());

        g_module_close (module->priv->gmodule);
        return FALSE;
    }

    module->priv->init (type_module);
    module->priv->loaded = TRUE;

    /* Make sure get_info() returns something, so we can avoid checking it
     * in other parts of the library */
    if (G_UNLIKELY (module->priv->get_info () == NULL)) {
        g_critical ("Backend module %s does not provide module information",
                    module->priv->path);

        /* Close the module but keep the loaded flag to avoid unreffing
         * this instance as the GType has most likely been registered */
        g_module_close (module->priv->gmodule);
        return FALSE;
    }

    /* It is not possible to unref this instance, so keep the module alive */
    g_module_make_resident (module->priv->gmodule);

    g_debug ("Loaded backend module %s", module->priv->path);
    return TRUE;
}

static void
backend_module_unload (GTypeModule *type_module G_GNUC_UNUSED)
{
}
