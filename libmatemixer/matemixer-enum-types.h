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

#ifndef CAFEMIXER_ENUM_TYPES_H
#define CAFEMIXER_ENUM_TYPES_H

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

/*
 * GTypes are not generated by glib-mkenums, see:
 * https://bugzilla.gnome.org/show_bug.cgi?id=621942
 */

#define CAFE_MIXER_TYPE_STATE (mate_mixer_state_get_type ())
GType mate_mixer_state_get_type (void) G_GNUC_CONST;

#define CAFE_MIXER_TYPE_BACKEND_TYPE (mate_mixer_backend_type_get_type ())
GType mate_mixer_backend_type_get_type (void) G_GNUC_CONST;

#define CAFE_MIXER_TYPE_BACKEND_FLAGS (mate_mixer_backend_flags_get_type ())
GType mate_mixer_backend_flags_get_type (void) G_GNUC_CONST;

#define CAFE_MIXER_TYPE_DIRECTION (mate_mixer_direction_get_type ())
GType mate_mixer_direction_get_type (void) G_GNUC_CONST;

#define CAFE_MIXER_TYPE_STREAM_CONTROL_FLAGS (mate_mixer_stream_control_flags_get_type ())
GType mate_mixer_stream_control_flags_get_type (void) G_GNUC_CONST;

#define CAFE_MIXER_TYPE_STREAM_CONTROL_ROLE (mate_mixer_stream_control_role_get_type ())
GType mate_mixer_stream_control_role_get_type (void) G_GNUC_CONST;

#define CAFE_MIXER_TYPE_STREAM_CONTROL_MEDIA_ROLE (mate_mixer_stream_control_media_role_get_type ())
GType mate_mixer_stream_control_media_role_get_type (void) G_GNUC_CONST;

#define CAFE_MIXER_TYPE_DEVICE_SWITCH_ROLE (mate_mixer_device_switch_role_get_type ())
GType mate_mixer_device_switch_role_get_type (void) G_GNUC_CONST;

#define CAFE_MIXER_TYPE_STREAM_SWITCH_FLAGS (mate_mixer_stream_switch_flags_get_type ())
GType mate_mixer_stream_switch_flags_get_type (void) G_GNUC_CONST;

#define CAFE_MIXER_TYPE_STREAM_SWITCH_ROLE (mate_mixer_stream_switch_role_get_type ())
GType mate_mixer_stream_switch_role_get_type (void) G_GNUC_CONST;

#define CAFE_MIXER_TYPE_CHANNEL_POSITION (mate_mixer_channel_position_get_type ())
GType mate_mixer_channel_position_get_type (void) G_GNUC_CONST;

G_END_DECLS

#endif /* CAFEMIXER_ENUM_TYPES_H */
