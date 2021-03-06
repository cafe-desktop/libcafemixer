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

#include <string.h>
#include <glib.h>

#include <libcafemixer/cafemixer-enums.h>

#include <pulse/pulseaudio.h>

#include "pulse-helpers.h"

const CafeMixerChannelPosition pulse_channel_map_from[PA_CHANNEL_POSITION_MAX] =
{
    [PA_CHANNEL_POSITION_MONO]                  = CAFE_MIXER_CHANNEL_MONO,
    [PA_CHANNEL_POSITION_FRONT_LEFT]            = CAFE_MIXER_CHANNEL_FRONT_LEFT,
    [PA_CHANNEL_POSITION_FRONT_RIGHT]           = CAFE_MIXER_CHANNEL_FRONT_RIGHT,
    [PA_CHANNEL_POSITION_FRONT_CENTER]          = CAFE_MIXER_CHANNEL_FRONT_CENTER,
    [PA_CHANNEL_POSITION_LFE]                   = CAFE_MIXER_CHANNEL_LFE,
    [PA_CHANNEL_POSITION_REAR_LEFT]             = CAFE_MIXER_CHANNEL_BACK_LEFT,
    [PA_CHANNEL_POSITION_REAR_RIGHT]            = CAFE_MIXER_CHANNEL_BACK_RIGHT,
    [PA_CHANNEL_POSITION_FRONT_LEFT_OF_CENTER]  = CAFE_MIXER_CHANNEL_FRONT_LEFT_CENTER,
    [PA_CHANNEL_POSITION_FRONT_RIGHT_OF_CENTER] = CAFE_MIXER_CHANNEL_FRONT_RIGHT_CENTER,
    [PA_CHANNEL_POSITION_SIDE_LEFT]             = CAFE_MIXER_CHANNEL_SIDE_LEFT,
    [PA_CHANNEL_POSITION_SIDE_RIGHT]            = CAFE_MIXER_CHANNEL_SIDE_RIGHT,
    [PA_CHANNEL_POSITION_TOP_FRONT_LEFT]        = CAFE_MIXER_CHANNEL_TOP_FRONT_LEFT,
    [PA_CHANNEL_POSITION_TOP_FRONT_RIGHT]       = CAFE_MIXER_CHANNEL_TOP_FRONT_RIGHT,
    [PA_CHANNEL_POSITION_TOP_FRONT_CENTER]      = CAFE_MIXER_CHANNEL_TOP_FRONT_CENTER,
    [PA_CHANNEL_POSITION_TOP_CENTER]            = CAFE_MIXER_CHANNEL_TOP_CENTER,
    [PA_CHANNEL_POSITION_TOP_REAR_LEFT]         = CAFE_MIXER_CHANNEL_TOP_BACK_LEFT,
    [PA_CHANNEL_POSITION_TOP_REAR_RIGHT]        = CAFE_MIXER_CHANNEL_TOP_BACK_RIGHT,
    [PA_CHANNEL_POSITION_TOP_REAR_CENTER]       = CAFE_MIXER_CHANNEL_TOP_BACK_CENTER
};

const pa_channel_position_t pulse_channel_map_to[CAFE_MIXER_CHANNEL_MAX] =
{
    [CAFE_MIXER_CHANNEL_UNKNOWN]                = PA_CHANNEL_POSITION_INVALID,
    [CAFE_MIXER_CHANNEL_MONO]                   = PA_CHANNEL_POSITION_MONO,
    [CAFE_MIXER_CHANNEL_FRONT_LEFT]             = PA_CHANNEL_POSITION_FRONT_LEFT,
    [CAFE_MIXER_CHANNEL_FRONT_RIGHT]            = PA_CHANNEL_POSITION_FRONT_RIGHT,
    [CAFE_MIXER_CHANNEL_FRONT_CENTER]           = PA_CHANNEL_POSITION_FRONT_CENTER,
    [CAFE_MIXER_CHANNEL_LFE]                    = PA_CHANNEL_POSITION_LFE,
    [CAFE_MIXER_CHANNEL_BACK_LEFT]              = PA_CHANNEL_POSITION_REAR_LEFT,
    [CAFE_MIXER_CHANNEL_BACK_RIGHT]             = PA_CHANNEL_POSITION_REAR_RIGHT,
    [CAFE_MIXER_CHANNEL_BACK_CENTER]            = PA_CHANNEL_POSITION_REAR_CENTER,
    [CAFE_MIXER_CHANNEL_FRONT_LEFT_CENTER]      = PA_CHANNEL_POSITION_FRONT_LEFT_OF_CENTER,
    [CAFE_MIXER_CHANNEL_FRONT_RIGHT_CENTER]     = PA_CHANNEL_POSITION_FRONT_RIGHT_OF_CENTER,
    [CAFE_MIXER_CHANNEL_SIDE_LEFT]              = PA_CHANNEL_POSITION_SIDE_LEFT,
    [CAFE_MIXER_CHANNEL_SIDE_RIGHT]             = PA_CHANNEL_POSITION_SIDE_RIGHT,
    [CAFE_MIXER_CHANNEL_TOP_FRONT_LEFT]         = PA_CHANNEL_POSITION_TOP_FRONT_LEFT,
    [CAFE_MIXER_CHANNEL_TOP_FRONT_RIGHT]        = PA_CHANNEL_POSITION_TOP_FRONT_RIGHT,
    [CAFE_MIXER_CHANNEL_TOP_FRONT_CENTER]       = PA_CHANNEL_POSITION_TOP_FRONT_CENTER,
    [CAFE_MIXER_CHANNEL_TOP_CENTER]             = PA_CHANNEL_POSITION_TOP_CENTER,
    [CAFE_MIXER_CHANNEL_TOP_BACK_LEFT]          = PA_CHANNEL_POSITION_TOP_REAR_LEFT,
    [CAFE_MIXER_CHANNEL_TOP_BACK_RIGHT]         = PA_CHANNEL_POSITION_TOP_REAR_RIGHT,
    [CAFE_MIXER_CHANNEL_TOP_BACK_CENTER]        = PA_CHANNEL_POSITION_TOP_REAR_CENTER
};

CafeMixerStreamControlMediaRole
pulse_convert_media_role_name (const gchar *name)
{
    g_return_val_if_fail (name != NULL, CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_UNKNOWN);

    if (!strcmp (name, "video")) {
        return CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_VIDEO;
    }
    else if (!strcmp (name, "music")) {
        return CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_MUSIC;
    }
    else if (!strcmp (name, "game")) {
        return CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_GAME;
    }
    else if (!strcmp (name, "event")) {
        return CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_EVENT;
    }
    else if (!strcmp (name, "phone")) {
        return CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_PHONE;
    }
    else if (!strcmp (name, "animation")) {
        return CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_ANIMATION;
    }
    else if (!strcmp (name, "production")) {
        return CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_PRODUCTION;
    }
    else if (!strcmp (name, "a11y")) {
        return CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_A11Y;
    }
    else if (!strcmp (name, "test")) {
        return CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_TEST;
    }
    else if (!strcmp (name, "abstract")) {
        return CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_ABSTRACT;
    }
    else if (!strcmp (name, "filter")) {
        return CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_FILTER;
    }

    return CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_UNKNOWN;
}
