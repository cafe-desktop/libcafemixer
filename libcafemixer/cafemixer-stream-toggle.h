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

#ifndef CAFEMIXER_STREAM_TOGGLE_H
#define CAFEMIXER_STREAM_TOGGLE_H

#include <glib.h>
#include <glib-object.h>

#include <libcafemixer/cafemixer-stream-switch.h>
#include <libcafemixer/cafemixer-types.h>

G_BEGIN_DECLS

#define CAFE_MIXER_TYPE_STREAM_TOGGLE           \
        (cafe_mixer_stream_toggle_get_type ())
#define CAFE_MIXER_STREAM_TOGGLE(o)             \
        (G_TYPE_CHECK_INSTANCE_CAST ((o), CAFE_MIXER_TYPE_STREAM_TOGGLE, CafeMixerStreamToggle))
#define CAFE_MIXER_IS_STREAM_TOGGLE(o)          \
        (G_TYPE_CHECK_INSTANCE_TYPE ((o), CAFE_MIXER_TYPE_STREAM_TOGGLE))
#define CAFE_MIXER_STREAM_TOGGLE_CLASS(k)       \
        (G_TYPE_CHECK_CLASS_CAST ((k), CAFE_MIXER_TYPE_STREAM_TOGGLE, CafeMixerStreamToggleClass))
#define CAFE_MIXER_IS_STREAM_TOGGLE_CLASS(k)    \
        (G_TYPE_CHECK_CLASS_TYPE ((k), CAFE_MIXER_TYPE_STREAM_TOGGLE))
#define CAFE_MIXER_STREAM_TOGGLE_GET_CLASS(o)   \
        (G_TYPE_INSTANCE_GET_CLASS ((o), CAFE_MIXER_TYPE_STREAM_TOGGLE, CafeMixerStreamToggleClass))

typedef struct _CafeMixerStreamToggleClass    CafeMixerStreamToggleClass;
typedef struct _CafeMixerStreamTogglePrivate  CafeMixerStreamTogglePrivate;

/**
 * CafeMixerStreamToggle:
 *
 * The #CafeMixerStreamToggle structure contains only private data and should only
 * be accessed using the provided API.
 */
struct _CafeMixerStreamToggle
{
    CafeMixerStreamSwitch object;

    /*< private >*/
    CafeMixerStreamTogglePrivate *priv;
};

/**
 * CafeMixerStreamToggleClass:
 * @parent_class: The parent class.
 *
 * The class structure for #CafeMixerStreamToggle.
 */
struct _CafeMixerStreamToggleClass
{
    CafeMixerStreamSwitchClass parent_class;
};

GType                  cafe_mixer_stream_toggle_get_type         (void) G_GNUC_CONST;

gboolean               cafe_mixer_stream_toggle_get_state        (CafeMixerStreamToggle *toggle);
gboolean               cafe_mixer_stream_toggle_set_state        (CafeMixerStreamToggle *toggle,
                                                                  gboolean               state);

CafeMixerSwitchOption *cafe_mixer_stream_toggle_get_state_option (CafeMixerStreamToggle *toggle,
                                                                  gboolean               state);

G_END_DECLS

#endif /* CAFEMIXER_STREAM_TOGGLE_H */
