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

#ifndef CAFEMIXER_SWITCH_H
#define CAFEMIXER_SWITCH_H

#include <glib.h>
#include <glib-object.h>

#include <libcafemixer/cafemixer-enums.h>
#include <libcafemixer/cafemixer-types.h>

G_BEGIN_DECLS

#define CAFE_MIXER_TYPE_SWITCH                  \
        (cafe_mixer_switch_get_type ())
#define CAFE_MIXER_SWITCH(o)                    \
        (G_TYPE_CHECK_INSTANCE_CAST ((o), CAFE_MIXER_TYPE_SWITCH, CafeMixerSwitch))
#define CAFE_MIXER_IS_SWITCH(o)                 \
        (G_TYPE_CHECK_INSTANCE_TYPE ((o), CAFE_MIXER_TYPE_SWITCH))
#define CAFE_MIXER_SWITCH_CLASS(k)              \
        (G_TYPE_CHECK_CLASS_CAST ((k), CAFE_MIXER_TYPE_SWITCH, CafeMixerSwitchClass))
#define CAFE_MIXER_IS_SWITCH_CLASS(k)           \
        (G_TYPE_CHECK_CLASS_TYPE ((k), CAFE_MIXER_TYPE_SWITCH))
#define CAFE_MIXER_SWITCH_GET_CLASS(o)          \
        (G_TYPE_INSTANCE_GET_CLASS ((o), CAFE_MIXER_TYPE_SWITCH, CafeMixerSwitchClass))

typedef struct _CafeMixerSwitchClass    CafeMixerSwitchClass;
typedef struct _CafeMixerSwitchPrivate  CafeMixerSwitchPrivate;

/**
 * CafeMixerSwitch:
 *
 * The #CafeMixerSwitch structure contains only private data and should only
 * be accessed using the provided API.
 */
struct _CafeMixerSwitch
{
    GObject object;

    /*< private >*/
    CafeMixerSwitchPrivate *priv;
};

/**
 * CafeMixerSwitchClass:
 * @parent_class: The parent class.
 *
 * The class structure for #CafeMixerSwitch.
 */
struct _CafeMixerSwitchClass
{
    GObjectClass parent_class;

    /*< private >*/
    CafeMixerSwitchOption *(*get_option)        (CafeMixerSwitch       *swtch,
                                                 const gchar           *name);

    const GList           *(*list_options)      (CafeMixerSwitch       *swtch);

    gboolean               (*set_active_option) (CafeMixerSwitch       *swtch,
                                                 CafeMixerSwitchOption *option);
};

GType                  cafe_mixer_switch_get_type          (void) G_GNUC_CONST;

const gchar *          cafe_mixer_switch_get_name          (CafeMixerSwitch       *swtch);
const gchar *          cafe_mixer_switch_get_label         (CafeMixerSwitch       *swtch);

CafeMixerSwitchOption *cafe_mixer_switch_get_option        (CafeMixerSwitch       *swtch,
                                                            const gchar           *name);

const GList *          cafe_mixer_switch_list_options      (CafeMixerSwitch       *swtch);

CafeMixerSwitchOption *cafe_mixer_switch_get_active_option (CafeMixerSwitch       *swtch);
gboolean               cafe_mixer_switch_set_active_option (CafeMixerSwitch       *swtch,
                                                            CafeMixerSwitchOption *option);

G_END_DECLS

#endif /* CAFEMIXER_SWITCH_H */
