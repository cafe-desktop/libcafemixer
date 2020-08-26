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

#ifndef CAFEMIXER_H
#define CAFEMIXER_H

#include <glib.h>
#include <glib-object.h>

#include <libcafemixer/cafemixer-types.h>

#include <libcafemixer/cafemixer-app-info.h>
#include <libcafemixer/cafemixer-context.h>
#include <libcafemixer/cafemixer-device.h>
#include <libcafemixer/cafemixer-device-switch.h>
#include <libcafemixer/cafemixer-enums.h>
#include <libcafemixer/cafemixer-enum-types.h>
#include <libcafemixer/cafemixer-stored-control.h>
#include <libcafemixer/cafemixer-stream.h>
#include <libcafemixer/cafemixer-stream-control.h>
#include <libcafemixer/cafemixer-stream-switch.h>
#include <libcafemixer/cafemixer-stream-toggle.h>
#include <libcafemixer/cafemixer-switch.h>
#include <libcafemixer/cafemixer-switch-option.h>
#include <libcafemixer/cafemixer-version.h>

G_BEGIN_DECLS

gboolean cafe_mixer_init           (void);
gboolean cafe_mixer_is_initialized (void);

G_END_DECLS

#endif /* CAFEMIXER_H */
