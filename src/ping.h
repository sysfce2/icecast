/* Icecast
 *
 *  Copyright 2023      Philipp "ph3-der-loewe" Schafft <lion@lion.leolix.org>
 *
 *  This program is distributed under the GNU General Public License, version 2.
 *  A copy of this license is included with this source.
 */

#ifndef __PING_H__
#define __PING_H__

#include "icecasttypes.h"

void ping_initialize(void);
void ping_shutdown(void);

void ping_simple(const char *url, const char *username, const char *password, string_renderer_t *data);

#endif  /* __PING_H__ */
