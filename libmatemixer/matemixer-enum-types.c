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

#include "matemixer-enum-types.h"
#include "matemixer-enums.h"

/*
 * GTypes are not generated by glib-mkenums, see:
 * https://bugzilla.gnome.org/show_bug.cgi?id=621942
 */

GType
mate_mixer_state_get_type (void)
{
    static GType etype = 0;

    if (etype == 0) {
        static const GEnumValue values[] = {
            { CAFE_MIXER_STATE_IDLE, "CAFE_MIXER_STATE_IDLE", "idle" },
            { CAFE_MIXER_STATE_CONNECTING, "CAFE_MIXER_STATE_CONNECTING", "connecting" },
            { CAFE_MIXER_STATE_READY, "CAFE_MIXER_STATE_READY", "ready" },
            { CAFE_MIXER_STATE_FAILED, "CAFE_MIXER_STATE_FAILED", "failed" },
            { CAFE_MIXER_STATE_UNKNOWN, "CAFE_MIXER_STATE_UNKNOWN", "unknown" },
            { 0, NULL, NULL }
        };
        etype = g_enum_register_static (
            g_intern_static_string ("MateMixerState"),
            values);
    }
    return etype;
}

GType
mate_mixer_backend_type_get_type (void)
{
    static GType etype = 0;

    if (etype == 0) {
        static const GEnumValue values[] = {
            { CAFE_MIXER_BACKEND_UNKNOWN, "CAFE_MIXER_BACKEND_UNKNOWN", "unknown" },
            { CAFE_MIXER_BACKEND_PULSEAUDIO, "CAFE_MIXER_BACKEND_PULSEAUDIO", "pulseaudio" },
            { CAFE_MIXER_BACKEND_ALSA, "CAFE_MIXER_BACKEND_ALSA", "alsa" },
            { CAFE_MIXER_BACKEND_OSS, "CAFE_MIXER_BACKEND_OSS", "oss" },
            { CAFE_MIXER_BACKEND_NULL, "CAFE_MIXER_BACKEND_NULL", "null" },
            { 0, NULL, NULL }
        };
        etype = g_enum_register_static (
            g_intern_static_string ("MateMixerBackendType"),
            values);
    }
    return etype;
}

GType
mate_mixer_backend_flags_get_type (void)
{
    static GType etype = 0;

    if (etype == 0) {
        static const GFlagsValue values[] = {
            { CAFE_MIXER_BACKEND_NO_FLAGS, "CAFE_MIXER_STREAM_NO_FLAGS", "no-flags" },
            { CAFE_MIXER_BACKEND_HAS_APPLICATION_CONTROLS, "CAFE_MIXER_BACKEND_HAS_APPLICATION_CONTROLS", "has-application-controls" },
            { CAFE_MIXER_BACKEND_HAS_STORED_CONTROLS, "CAFE_MIXER_BACKEND_HAS_STORED_CONTROLS", "has-stored-controls" },
            { CAFE_MIXER_BACKEND_CAN_SET_DEFAULT_INPUT_STREAM, "CAFE_MIXER_BACKEND_CAN_SET_DEFAULT_INPUT_STREAM", "can-set-default-input-stream" },
            { CAFE_MIXER_BACKEND_CAN_SET_DEFAULT_OUTPUT_STREAM, "CAFE_MIXER_BACKEND_CAN_SET_DEFAULT_OUTPUT_STREAM", "can-set-default-output-stream" },
            { 0, NULL, NULL }
        };
        etype = g_flags_register_static (
            g_intern_static_string ("MateMixerBackendFlags"),
            values);
    }
    return etype;
}

GType
mate_mixer_direction_get_type (void)
{
    static GType etype = 0;

    if (etype == 0) {
        static const GEnumValue values[] = {
            { CAFE_MIXER_DIRECTION_UNKNOWN, "CAFE_MIXER_DIRECTION_UNKNOWN", "unknown" },
            { CAFE_MIXER_DIRECTION_INPUT, "CAFE_MIXER_DIRECTION_INPUT", "input" },
            { CAFE_MIXER_DIRECTION_OUTPUT, "CAFE_MIXER_DIRECTION_OUTPUT", "output" },
            { 0, NULL, NULL }
        };
        etype = g_enum_register_static (
            g_intern_static_string ("MateMixerDirection"),
            values);
    }
    return etype;
}

GType
mate_mixer_stream_control_flags_get_type (void)
{
    static GType etype = 0;

    if (etype == 0) {
        static const GFlagsValue values[] = {
            { CAFE_MIXER_STREAM_CONTROL_NO_FLAGS, "CAFE_MIXER_STREAM_CONTROL_NO_FLAGS", "no-flags" },
            { CAFE_MIXER_STREAM_CONTROL_MUTE_READABLE, "CAFE_MIXER_STREAM_CONTROL_MUTE_READABLE", "mute-readable" },
            { CAFE_MIXER_STREAM_CONTROL_MUTE_WRITABLE, "CAFE_MIXER_STREAM_CONTROL_MUTE_WRITABLE", "mute-writable" },
            { CAFE_MIXER_STREAM_CONTROL_VOLUME_READABLE, "CAFE_MIXER_STREAM_CONTROL_VOLUME_READABLE", "volume-readable" },
            { CAFE_MIXER_STREAM_CONTROL_VOLUME_WRITABLE, "CAFE_MIXER_STREAM_CONTROL_VOLUME_WRITABLE", "volume-writable" },
            { CAFE_MIXER_STREAM_CONTROL_CAN_BALANCE, "CAFE_MIXER_STREAM_CONTROL_CAN_BALANCE", "can-balance" },
            { CAFE_MIXER_STREAM_CONTROL_CAN_FADE, "CAFE_MIXER_STREAM_CONTROL_CAN_FADE", "can-fade" },
            { CAFE_MIXER_STREAM_CONTROL_MOVABLE, "CAFE_MIXER_STREAM_CONTROL_MOVABLE", "movable" },
            { CAFE_MIXER_STREAM_CONTROL_HAS_DECIBEL, "CAFE_MIXER_STREAM_CONTROL_HAS_DECIBEL", "has-decibel" },
            { CAFE_MIXER_STREAM_CONTROL_HAS_MONITOR, "CAFE_MIXER_STREAM_CONTROL_HAS_MONITOR", "has-monitor" },
            { CAFE_MIXER_STREAM_CONTROL_STORED, "CAFE_MIXER_STREAM_CONTROL_STORED", "stored" },
            { 0, NULL, NULL }
        };
        etype = g_flags_register_static (
            g_intern_static_string ("MateMixerStreamControlFlags"),
            values);
    }
    return etype;
}

GType
mate_mixer_stream_control_role_get_type (void)
{
    static GType etype = 0;

    if (etype == 0) {
        static const GEnumValue values[] = {
            { CAFE_MIXER_STREAM_CONTROL_ROLE_UNKNOWN, "CAFE_MIXER_STREAM_CONTROL_ROLE_UNKNOWN", "unknown" },
            { CAFE_MIXER_STREAM_CONTROL_ROLE_MASTER, "CAFE_MIXER_STREAM_CONTROL_ROLE_MASTER", "master" },
            { CAFE_MIXER_STREAM_CONTROL_ROLE_APPLICATION, "CAFE_MIXER_STREAM_CONTROL_ROLE_APPLICATION", "application" },
            { CAFE_MIXER_STREAM_CONTROL_ROLE_PCM, "CAFE_MIXER_STREAM_CONTROL_ROLE_PCM", "pcm" },
            { CAFE_MIXER_STREAM_CONTROL_ROLE_SPEAKER, "CAFE_MIXER_STREAM_CONTROL_ROLE_SPEAKER", "speaker" },
            { CAFE_MIXER_STREAM_CONTROL_ROLE_MICROPHONE, "CAFE_MIXER_STREAM_CONTROL_ROLE_MICROPHONE", "microphone" },
            { CAFE_MIXER_STREAM_CONTROL_ROLE_PORT, "CAFE_MIXER_STREAM_CONTROL_ROLE_PORT", "port" },
            { CAFE_MIXER_STREAM_CONTROL_ROLE_BOOST, "CAFE_MIXER_STREAM_CONTROL_ROLE_BOOST", "boost" },
            { CAFE_MIXER_STREAM_CONTROL_ROLE_BASS, "CAFE_MIXER_STREAM_CONTROL_ROLE_BASS", "bass" },
            { CAFE_MIXER_STREAM_CONTROL_ROLE_TREBLE, "CAFE_MIXER_STREAM_CONTROL_ROLE_TREBLE", "treble" },
            { CAFE_MIXER_STREAM_CONTROL_ROLE_CD, "CAFE_MIXER_STREAM_CONTROL_ROLE_CD", "cd" },
            { CAFE_MIXER_STREAM_CONTROL_ROLE_VIDEO, "CAFE_MIXER_STREAM_CONTROL_ROLE_VIDEO", "video" },
            { CAFE_MIXER_STREAM_CONTROL_ROLE_MUSIC, "CAFE_MIXER_STREAM_CONTROL_ROLE_MUSIC", "music" },
            { 0, NULL, NULL }
        };
        etype = g_enum_register_static (
            g_intern_static_string ("MateMixerStreamControlRole"),
            values);
    }
    return etype;
}

GType
mate_mixer_stream_control_media_role_get_type (void)
{
    static GType etype = 0;

    if (etype == 0) {
        static const GEnumValue values[] = {
            { CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_UNKNOWN, "CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_UNKNOWN", "unknown" },
            { CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_VIDEO, "CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_VIDEO", "video" },
            { CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_MUSIC, "CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_MUSIC", "music" },
            { CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_GAME, "CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_GAME", "game" },
            { CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_EVENT, "CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_EVENT", "event" },
            { CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_PHONE, "CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_PHONE", "phone" },
            { CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_ANIMATION, "CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_ANIMATION", "animation" },
            { CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_PRODUCTION, "CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_PRODUCTION", "production" },
            { CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_A11Y, "CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_A11Y", "a11y" },
            { CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_TEST, "CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_TEST", "test" },
            { CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_ABSTRACT, "CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_ABSTRACT", "abstract" },
            { CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_FILTER, "CAFE_MIXER_STREAM_CONTROL_MEDIA_ROLE_FILTER", "filter" },
            { 0, NULL, NULL }
        };
        etype = g_enum_register_static (
            g_intern_static_string ("MateMixerStreamControlMediaRole"),
            values);
    }
    return etype;
}

GType
mate_mixer_device_switch_role_get_type (void)
{
    static GType etype = 0;

    if (etype == 0) {
        static const GEnumValue values[] = {
            { CAFE_MIXER_DEVICE_SWITCH_ROLE_UNKNOWN, "CAFE_MIXER_DEVICE_SWITCH_ROLE_UNKNOWN", "unknown" },
            { CAFE_MIXER_DEVICE_SWITCH_ROLE_PROFILE, "CAFE_MIXER_DEVICE_SWITCH_ROLE_PROFILE", "profile" },
            { 0, NULL, NULL }
        };
        etype = g_enum_register_static (
            g_intern_static_string ("MateMixerDeviceSwitchRole"),
            values);
    }
    return etype;
}

GType
mate_mixer_stream_switch_flags_get_type (void)
{
    static GType etype = 0;

    if (etype == 0) {
        static const GFlagsValue values[] = {
            { CAFE_MIXER_STREAM_SWITCH_NO_FLAGS, "CAFE_MIXER_STREAM_SWITCH_NO_FLAGS", "no-flags" },
            { CAFE_MIXER_STREAM_SWITCH_TOGGLE, "CAFE_MIXER_STREAM_SWITCH_TOGGLE", "toggle" },
            { 0, NULL, NULL }
        };
        etype = g_flags_register_static (
            g_intern_static_string ("MateMixerStreamSwitchFlags"),
            values);
    }
    return etype;
}

GType
mate_mixer_stream_switch_role_get_type (void)
{
    static GType etype = 0;

    if (etype == 0) {
        static const GEnumValue values[] = {
            { CAFE_MIXER_STREAM_SWITCH_ROLE_UNKNOWN, "CAFE_MIXER_STREAM_SWITCH_ROLE_UNKNOWN", "unknown" },
            { CAFE_MIXER_STREAM_SWITCH_ROLE_PORT, "CAFE_MIXER_STREAM_SWITCH_ROLE_PORT", "port" },
            { CAFE_MIXER_STREAM_SWITCH_ROLE_BOOST, "CAFE_MIXER_STREAM_SWITCH_ROLE_BOOST", "boost" },
            { 0, NULL, NULL }
        };
        etype = g_enum_register_static (
            g_intern_static_string ("MateMixerStreamSwitchRole"),
            values);
    }
    return etype;
}

GType
mate_mixer_channel_position_get_type (void)
{
    static GType etype = 0;

    if (etype == 0) {
        static const GEnumValue values[] = {
            { CAFE_MIXER_CHANNEL_UNKNOWN, "CAFE_MIXER_CHANNEL_UNKNOWN", "unknown" },
            { CAFE_MIXER_CHANNEL_MONO, "CAFE_MIXER_CHANNEL_MONO", "mono" },
            { CAFE_MIXER_CHANNEL_FRONT_LEFT, "CAFE_MIXER_CHANNEL_FRONT_LEFT", "front-left" },
            { CAFE_MIXER_CHANNEL_FRONT_RIGHT, "CAFE_MIXER_CHANNEL_FRONT_RIGHT", "front-right" },
            { CAFE_MIXER_CHANNEL_FRONT_CENTER, "CAFE_MIXER_CHANNEL_FRONT_CENTER", "front-center" },
            { CAFE_MIXER_CHANNEL_LFE, "CAFE_MIXER_CHANNEL_LFE", "lfe" },
            { CAFE_MIXER_CHANNEL_BACK_LEFT, "CAFE_MIXER_CHANNEL_BACK_LEFT", "back-left" },
            { CAFE_MIXER_CHANNEL_BACK_RIGHT, "CAFE_MIXER_CHANNEL_BACK_RIGHT", "back-right" },
            { CAFE_MIXER_CHANNEL_BACK_CENTER, "CAFE_MIXER_STREAM_BACK_CENTER", "back-center" },
            { CAFE_MIXER_CHANNEL_FRONT_LEFT_CENTER, "CAFE_MIXER_STREAM_FRONT_LEFT_CENTER", "front-left-center" },
            { CAFE_MIXER_CHANNEL_FRONT_RIGHT_CENTER, "CAFE_MIXER_STREAM_FRONT_RIGHT_CENTER", "front-right-center" },
            { CAFE_MIXER_CHANNEL_SIDE_LEFT, "CAFE_MIXER_CHANNEL_SIDE_LEFT", "side-left" },
            { CAFE_MIXER_CHANNEL_SIDE_RIGHT, "CAFE_MIXER_CHANNEL_SIDE_RIGHT", "side-right" },
            { CAFE_MIXER_CHANNEL_TOP_FRONT_LEFT, "CAFE_MIXER_CHANNEL_TOP_FRONT_LEFT", "top-front-left" },
            { CAFE_MIXER_CHANNEL_TOP_FRONT_RIGHT, "CAFE_MIXER_CHANNEL_TOP_FRONT_RIGHT", "top-front-right" },
            { CAFE_MIXER_CHANNEL_TOP_FRONT_CENTER, "CAFE_MIXER_CHANNEL_TOP_FRONT_CENTER", "top-front-center" },
            { CAFE_MIXER_CHANNEL_TOP_CENTER, "CAFE_MIXER_CHANNEL_TOP_CENTER", "top-center" },
            { CAFE_MIXER_CHANNEL_TOP_BACK_LEFT, "CAFE_MIXER_CHANNEL_TOP_BACK_LEFT", "top-back-left" },
            { CAFE_MIXER_CHANNEL_TOP_BACK_RIGHT, "CAFE_MIXER_CHANNEL_TOP_BACK_RIGHT", "top-back-right" },
            { CAFE_MIXER_CHANNEL_TOP_BACK_CENTER, "CAFE_MIXER_CHANNEL_TOP_BACK_CENTER", "top-back-center" },
            { CAFE_MIXER_CHANNEL_MAX, "CAFE_MIXER_CHANNEL_MAX", "max" },
            { 0, NULL, NULL }
        };
        etype = g_enum_register_static (
            g_intern_static_string ("MateMixerChannelPosition"),
            values);
    }
    return etype;
}
