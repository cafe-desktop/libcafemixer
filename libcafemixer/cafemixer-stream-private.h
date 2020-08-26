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

#ifndef CAFEMIXER_STREAM_PRIVATE_H
#define CAFEMIXER_STREAM_PRIVATE_H

#include <glib.h>

#include "cafemixer-types.h"

G_BEGIN_DECLS

void _cafe_mixer_stream_set_default_control (CafeMixerStream        *stream,
                                             CafeMixerStreamControl *control);

G_END_DECLS

#endif /* CAFEMIXER_STREAM_PRIVATE_H */
