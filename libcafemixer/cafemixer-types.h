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

#ifndef CAFEMIXER_TYPES_H
#define CAFEMIXER_TYPES_H

G_BEGIN_DECLS

typedef struct _CafeMixerAppInfo        CafeMixerAppInfo;
typedef struct _CafeMixerContext        CafeMixerContext;
typedef struct _CafeMixerDevice         CafeMixerDevice;
typedef struct _CafeMixerDeviceSwitch   CafeMixerDeviceSwitch;
typedef struct _CafeMixerStoredControl  CafeMixerStoredControl;
typedef struct _CafeMixerStream         CafeMixerStream;
typedef struct _CafeMixerStreamControl  CafeMixerStreamControl;
typedef struct _CafeMixerStreamSwitch   CafeMixerStreamSwitch;
typedef struct _CafeMixerStreamToggle   CafeMixerStreamToggle;
typedef struct _CafeMixerSwitch         CafeMixerSwitch;
typedef struct _CafeMixerSwitchOption   CafeMixerSwitchOption;

G_END_DECLS

#endif /* CAFEMIXER_TYPES_H */
