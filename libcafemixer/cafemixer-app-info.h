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

#ifndef CAFEMIXER_APP_INFO_H
#define CAFEMIXER_APP_INFO_H

#include <glib.h>
#include <glib-object.h>

#include "cafemixer-types.h"

G_BEGIN_DECLS

#define CAFE_MIXER_TYPE_APP_INFO (cafe_mixer_app_info_get_type ())

GType        cafe_mixer_app_info_get_type    (void) G_GNUC_CONST;

const gchar *cafe_mixer_app_info_get_name    (CafeMixerAppInfo *info);
const gchar *cafe_mixer_app_info_get_id      (CafeMixerAppInfo *info);
const gchar *cafe_mixer_app_info_get_version (CafeMixerAppInfo *info);
const gchar *cafe_mixer_app_info_get_icon    (CafeMixerAppInfo *info);

G_END_DECLS

#endif /* CAFEMIXER_APP_INFO_H */
