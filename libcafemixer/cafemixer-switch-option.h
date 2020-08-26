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

#ifndef CAFEMIXER_SWITCH_OPTION_H
#define CAFEMIXER_SWITCH_OPTION_H

#include <glib.h>
#include <glib-object.h>

#include <libcafemixer/cafemixer-types.h>

G_BEGIN_DECLS

#define CAFE_MIXER_TYPE_SWITCH_OPTION           \
        (cafe_mixer_switch_option_get_type ())
#define CAFE_MIXER_SWITCH_OPTION(o)             \
        (G_TYPE_CHECK_INSTANCE_CAST ((o), CAFE_MIXER_TYPE_SWITCH_OPTION, CafeMixerSwitchOption))
#define CAFE_MIXER_IS_SWITCH_OPTION(o)          \
        (G_TYPE_CHECK_INSTANCE_TYPE ((o), CAFE_MIXER_TYPE_SWITCH_OPTION))
#define CAFE_MIXER_SWITCH_OPTION_CLASS(k)       \
        (G_TYPE_CHECK_CLASS_CAST ((k), CAFE_MIXER_TYPE_SWITCH_OPTION, CafeMixerSwitchOptionClass))
#define CAFE_MIXER_IS_SWITCH_OPTION_CLASS(k)    \
        (G_TYPE_CHECK_CLASS_TYPE ((k), CAFE_MIXER_TYPE_SWITCH_OPTION))
#define CAFE_MIXER_SWITCH_OPTION_GET_CLASS(o)   \
        (G_TYPE_INSTANCE_GET_CLASS ((o), CAFE_MIXER_TYPE_SWITCH_OPTION, CafeMixerSwitchOptionClass))

typedef struct _CafeMixerSwitchOptionClass    CafeMixerSwitchOptionClass;
typedef struct _CafeMixerSwitchOptionPrivate  CafeMixerSwitchOptionPrivate;

/**
 * CafeMixerSwitchOption:
 *
 * The #CafeMixerSwitchOption structure contains only private data and should only
 * be accessed using the provided API.
 */
struct _CafeMixerSwitchOption
{
    GObject parent;

    /*< private >*/
    CafeMixerSwitchOptionPrivate *priv;
};

/**
 * CafeMixerSwitchOptionClass:
 * @parent_class: The parent class.
 *
 * The class structure for #CafeMixerSwitchOption.
 */
struct _CafeMixerSwitchOptionClass
{
    GObjectClass parent_class;
};

GType        cafe_mixer_switch_option_get_type  (void) G_GNUC_CONST;

const gchar *cafe_mixer_switch_option_get_name  (CafeMixerSwitchOption *option);
const gchar *cafe_mixer_switch_option_get_label (CafeMixerSwitchOption *option);
const gchar *cafe_mixer_switch_option_get_icon  (CafeMixerSwitchOption *option);

G_END_DECLS

#endif /* CAFEMIXER_SWITCH_OPTION_H */
