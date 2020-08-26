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

#ifndef CAFEMIXER_APP_INFO_PRIVATE_H
#define CAFEMIXER_APP_INFO_PRIVATE_H

#include <glib.h>

#include "cafemixer-types.h"

G_BEGIN_DECLS

struct _CafeMixerAppInfo
{
    gchar *name;
    gchar *id;
    gchar *version;
    gchar *icon;
};

CafeMixerAppInfo *_cafe_mixer_app_info_new         (void);

void              _cafe_mixer_app_info_set_name    (CafeMixerAppInfo *info,
                                                    const gchar      *name);
void              _cafe_mixer_app_info_set_id      (CafeMixerAppInfo *info,
                                                    const gchar      *id);
void              _cafe_mixer_app_info_set_version (CafeMixerAppInfo *info,
                                                    const gchar      *version);
void              _cafe_mixer_app_info_set_icon    (CafeMixerAppInfo *info,
                                                    const gchar      *icon);

CafeMixerAppInfo *_cafe_mixer_app_info_copy        (CafeMixerAppInfo *info);
void              _cafe_mixer_app_info_free        (CafeMixerAppInfo *info);

G_END_DECLS

#endif /* CAFEMIXER_APP_INFO_PRIVATE_H */
