/*  format_kxs.c - kxs format module
 *  Copyright (C) 2000-2009  Jason Jordan <shnutils@freeshell.org>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "format.h"

CVSID("$Id: format_kxs.c,v 1.20 2009/03/11 17:18:01 jason Exp $")

#define KEXIS "kexis"

#define KEXIS_MAGIC "KEXS"

static char default_decoder_args[] = "-x -s " FILENAME_PLACEHOLDER;

format_module format_kxs = {
  "kxs",
  "Kexis lossless WAV file compressor",
  CVSIDSTR,
  TRUE,
  FALSE,
  FALSE,
  TRUE,
  TRUE,
  FALSE,
  NULL,
  KEXIS_MAGIC,
  0,
  "kxs",
  KEXIS,
  default_decoder_args,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL
};
