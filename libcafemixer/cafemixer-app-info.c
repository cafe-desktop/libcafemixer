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

#include "cafemixer-app-info.h"
#include "cafemixer-app-info-private.h"

/**
 * SECTION:cafemixer-app-info
 * @short_description: Application information
 * @include: libcafemixer/cafemixer.h
 * @see_also: #CafeMixerStreamControl
 *
 * The #CafeMixerAppInfo structure describes application properties.
 *
 * See #CafeMixerStreamControl and the cafe_mixer_stream_control_get_app_info()
 * function for more information.
 */

/**
 * CafeMixerAppInfo:
 *
 * The #CafeMixerAppInfo structure contains only private data and should only
 * be accessed using the provided API.
 */
G_DEFINE_BOXED_TYPE (CafeMixerAppInfo, cafe_mixer_app_info,
                     _cafe_mixer_app_info_copy,
                     _cafe_mixer_app_info_free)

/**
 * cafe_mixer_app_info_get_name:
 * @info: a #CafeMixerAppInfo
 *
 * Gets the name of the application described by @info.
 *
 * Returns: name of the application or %NULL if it is unknown.
 */
const gchar *
cafe_mixer_app_info_get_name (CafeMixerAppInfo *info)
{
    g_return_val_if_fail (info != NULL, NULL);

    return info->name;
}

/**
 * cafe_mixer_app_info_get_id:
 * @info: a #CafeMixerAppInfo
 *
 * Gets the identifier of the application described by @info
 * (e.g. org.example.app).
 *
 * Returns: identifier of the application or %NULL if it is unknown.
 */
const gchar *
cafe_mixer_app_info_get_id (CafeMixerAppInfo *info)
{
    g_return_val_if_fail (info != NULL, NULL);

    return info->id;
}

/**
 * cafe_mixer_app_info_get_version:
 * @info: a #CafeMixerAppInfo
 *
 * Gets the version of the application described by @info.
 *
 * Returns: version of the application or %NULL if it is unknown.
 */
const gchar *
cafe_mixer_app_info_get_version (CafeMixerAppInfo *info)
{
    g_return_val_if_fail (info != NULL, NULL);

    return info->version;
}

/**
 * cafe_mixer_app_info_get_icon:
 * @info: a #CafeMixerAppInfo
 *
 * Gets the XDG icon name of the application described by @info.
 *
 * Returns: icon name of the application or %NULL if it is unknown.
 */
const gchar *
cafe_mixer_app_info_get_icon (CafeMixerAppInfo *info)
{
    g_return_val_if_fail (info != NULL, NULL);

    return info->icon;
}

/**
 * _cafe_mixer_app_info_new:
 *
 * Creates a new empty #CafeMixerAppInfo structure.
 *
 * Returns: a new #CafeMixerAppInfo.
 */
CafeMixerAppInfo *
_cafe_mixer_app_info_new (void)
{
    return g_slice_new0 (CafeMixerAppInfo);
}

/**
 * _cafe_mixer_app_info_set_name:
 * @info: a #CafeMixerAppInfo
 * @name: the application name to set
 *
 * Sets the name of the application described by @info.
 */
void
_cafe_mixer_app_info_set_name (CafeMixerAppInfo *info, const gchar *name)
{
    g_return_if_fail (info != NULL);

    g_free (info->name);

    info->name = g_strdup (name);
}

/**
 * _cafe_mixer_app_info_set_id:
 * @info: a #CafeMixerAppInfo
 * @id: the application identifier to set
 *
 * Sets the identifier of the application described by @info.
 */
void
_cafe_mixer_app_info_set_id (CafeMixerAppInfo *info, const gchar *id)
{
    g_return_if_fail (info != NULL);

    g_free (info->id);

    info->id = g_strdup (id);
}

/**
 * _cafe_mixer_app_info_set_version:
 * @info: a #CafeMixerAppInfo
 * @version: the application version to set
 *
 * Sets the version of the application described by @info.
 */
void
_cafe_mixer_app_info_set_version (CafeMixerAppInfo *info, const gchar *version)
{
    g_return_if_fail (info != NULL);

    g_free (info->version);

    info->version = g_strdup (version);
}

/**
 * _cafe_mixer_app_info_set_version:
 * @info: a #CafeMixerAppInfo
 * @icon: the application icon name to set
 *
 * Sets the XDG icon name of the application described by @info.
 */
void
_cafe_mixer_app_info_set_icon (CafeMixerAppInfo *info, const gchar *icon)
{
    g_return_if_fail (info != NULL);

    g_free (info->icon);

    info->icon = g_strdup (icon);
}

/**
 * _cafe_mixer_app_info_copy:
 * @info: a #CafeMixerAppInfo
 *
 * Creates a copy of the #CafeMixerAppInfo.
 *
 * Returns: a copy of the given @info.
 */
CafeMixerAppInfo *
_cafe_mixer_app_info_copy (CafeMixerAppInfo *info)
{
    CafeMixerAppInfo *info2;

    g_return_val_if_fail (info != NULL, NULL);

    info2 = _cafe_mixer_app_info_new ();
    info2->name     = g_strdup (info->name);
    info2->id       = g_strdup (info->id);
    info2->version  = g_strdup (info->version);
    info2->icon     = g_strdup (info->icon);

    return info2;
}

/**
 * _cafe_mixer_app_info_free:
 * @info: a #CafeMixerAppInfo
 *
 * Frees the #CafeMixerAppInfo.
 */
void
_cafe_mixer_app_info_free (CafeMixerAppInfo *info)
{
    g_return_if_fail (info != NULL);

    g_free (info->name);
    g_free (info->id);
    g_free (info->version);
    g_free (info->icon);

    g_slice_free (CafeMixerAppInfo, info);
}
