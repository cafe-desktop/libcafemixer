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

#ifndef CAFEMIXER_PRIVATE_H
#define CAFEMIXER_PRIVATE_H

#include <glib.h>

#include "matemixer-enums.h"

#include "matemixer-app-info-private.h"
#include "matemixer-backend.h"
#include "matemixer-backend-module.h"
#include "matemixer-stream-private.h"
#include "matemixer-stream-control-private.h"
#include "matemixer-switch-private.h"
#include "matemixer-switch-option-private.h"

G_BEGIN_DECLS

#define CAFE_MIXER_IS_LEFT_CHANNEL(c)                   \
    ((c) == CAFE_MIXER_CHANNEL_FRONT_LEFT ||            \
     (c) == CAFE_MIXER_CHANNEL_BACK_LEFT ||             \
     (c) == CAFE_MIXER_CHANNEL_FRONT_LEFT_CENTER ||     \
     (c) == CAFE_MIXER_CHANNEL_SIDE_LEFT ||             \
     (c) == CAFE_MIXER_CHANNEL_TOP_FRONT_LEFT ||        \
     (c) == CAFE_MIXER_CHANNEL_TOP_BACK_LEFT)

#define CAFE_MIXER_IS_RIGHT_CHANNEL(c)                  \
    ((c) == CAFE_MIXER_CHANNEL_FRONT_RIGHT ||           \
     (c) == CAFE_MIXER_CHANNEL_BACK_RIGHT ||            \
     (c) == CAFE_MIXER_CHANNEL_FRONT_RIGHT_CENTER ||    \
     (c) == CAFE_MIXER_CHANNEL_SIDE_RIGHT ||            \
     (c) == CAFE_MIXER_CHANNEL_TOP_FRONT_RIGHT ||       \
     (c) == CAFE_MIXER_CHANNEL_TOP_BACK_RIGHT)

#define CAFE_MIXER_IS_FRONT_CHANNEL(c)                  \
    ((c) == CAFE_MIXER_CHANNEL_FRONT_LEFT ||            \
     (c) == CAFE_MIXER_CHANNEL_FRONT_RIGHT ||           \
     (c) == CAFE_MIXER_CHANNEL_FRONT_CENTER ||          \
     (c) == CAFE_MIXER_CHANNEL_FRONT_LEFT_CENTER ||     \
     (c) == CAFE_MIXER_CHANNEL_FRONT_RIGHT_CENTER ||    \
     (c) == CAFE_MIXER_CHANNEL_TOP_FRONT_LEFT ||        \
     (c) == CAFE_MIXER_CHANNEL_TOP_FRONT_RIGHT ||       \
     (c) == CAFE_MIXER_CHANNEL_TOP_FRONT_CENTER)

#define CAFE_MIXER_IS_BACK_CHANNEL(c)                   \
    ((c) == CAFE_MIXER_CHANNEL_BACK_LEFT ||             \
     (c) == CAFE_MIXER_CHANNEL_BACK_RIGHT ||            \
     (c) == CAFE_MIXER_CHANNEL_BACK_CENTER ||           \
     (c) == CAFE_MIXER_CHANNEL_TOP_BACK_LEFT ||         \
     (c) == CAFE_MIXER_CHANNEL_TOP_BACK_RIGHT ||        \
     (c) == CAFE_MIXER_CHANNEL_TOP_BACK_CENTER)

#define CAFE_MIXER_CHANNEL_MASK_LEFT                    \
    ((1 << CAFE_MIXER_CHANNEL_FRONT_LEFT) |             \
     (1 << CAFE_MIXER_CHANNEL_BACK_LEFT) |              \
     (1 << CAFE_MIXER_CHANNEL_FRONT_LEFT_CENTER) |      \
     (1 << CAFE_MIXER_CHANNEL_SIDE_LEFT) |              \
     (1 << CAFE_MIXER_CHANNEL_TOP_FRONT_LEFT) |         \
     (1 << CAFE_MIXER_CHANNEL_TOP_BACK_LEFT))

#define CAFE_MIXER_CHANNEL_MASK_RIGHT                   \
    ((1 << CAFE_MIXER_CHANNEL_FRONT_RIGHT) |            \
     (1 << CAFE_MIXER_CHANNEL_BACK_RIGHT) |             \
     (1 << CAFE_MIXER_CHANNEL_FRONT_RIGHT_CENTER) |     \
     (1 << CAFE_MIXER_CHANNEL_SIDE_RIGHT) |             \
     (1 << CAFE_MIXER_CHANNEL_TOP_FRONT_RIGHT) |        \
     (1 << CAFE_MIXER_CHANNEL_TOP_BACK_RIGHT))

#define CAFE_MIXER_CHANNEL_MASK_FRONT                   \
    ((1 << CAFE_MIXER_CHANNEL_FRONT_LEFT) |             \
     (1 << CAFE_MIXER_CHANNEL_FRONT_RIGHT) |            \
     (1 << CAFE_MIXER_CHANNEL_FRONT_CENTER) |           \
     (1 << CAFE_MIXER_CHANNEL_FRONT_LEFT_CENTER) |      \
     (1 << CAFE_MIXER_CHANNEL_FRONT_RIGHT_CENTER) |     \
     (1 << CAFE_MIXER_CHANNEL_TOP_FRONT_LEFT) |         \
     (1 << CAFE_MIXER_CHANNEL_TOP_FRONT_RIGHT) |        \
     (1 << CAFE_MIXER_CHANNEL_TOP_FRONT_CENTER))

#define CAFE_MIXER_CHANNEL_MASK_BACK                    \
    ((1 << CAFE_MIXER_CHANNEL_BACK_LEFT) |              \
     (1 << CAFE_MIXER_CHANNEL_BACK_RIGHT) |             \
     (1 << CAFE_MIXER_CHANNEL_BACK_CENTER) |            \
     (1 << CAFE_MIXER_CHANNEL_TOP_BACK_LEFT) |          \
     (1 << CAFE_MIXER_CHANNEL_TOP_BACK_RIGHT) |         \
     (1 << CAFE_MIXER_CHANNEL_TOP_BACK_CENTER))

#define CAFE_MIXER_CHANNEL_MASK_HAS_CHANNEL(m,c)    ((m) & (1 << (c)))
#define CAFE_MIXER_CHANNEL_MASK_HAS_LEFT(m)         ((m) & CAFE_MIXER_CHANNEL_MASK_LEFT)
#define CAFE_MIXER_CHANNEL_MASK_HAS_RIGHT(m)        ((m) & CAFE_MIXER_CHANNEL_MASK_RIGHT)
#define CAFE_MIXER_CHANNEL_MASK_HAS_FRONT(m)        ((m) & CAFE_MIXER_CHANNEL_MASK_FRONT)
#define CAFE_MIXER_CHANNEL_MASK_HAS_BACK(m)         ((m) & CAFE_MIXER_CHANNEL_MASK_BACK)

const GList *_mate_mixer_list_modules        (void);

guint32      _mate_mixer_create_channel_mask (MateMixerChannelPosition *positions,
                                              guint                     n) G_GNUC_PURE;

G_END_DECLS

#endif /* CAFEMIXER_PRIVATE_H */
