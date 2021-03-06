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

#ifndef CAFEMIXER_STREAM_CONTROL_PRIVATE_H
#define CAFEMIXER_STREAM_CONTROL_PRIVATE_H

#include <glib.h>

#include "cafemixer-enums.h"
#include "cafemixer-types.h"

G_BEGIN_DECLS

void _cafe_mixer_stream_control_set_flags   (CafeMixerStreamControl     *control,
                                             CafeMixerStreamControlFlags flags);

void _cafe_mixer_stream_control_set_stream  (CafeMixerStreamControl     *control,
                                             CafeMixerStream            *stream);

void _cafe_mixer_stream_control_set_mute    (CafeMixerStreamControl     *control,
                                             gboolean                    mute);

void _cafe_mixer_stream_control_set_balance (CafeMixerStreamControl     *control,
                                             gfloat                      balance);

void _cafe_mixer_stream_control_set_fade    (CafeMixerStreamControl     *control,
                                             gfloat                      fade);

G_END_DECLS

#endif /* CAFEMIXER_STREAM_CONTROL_PRIVATE_H */
