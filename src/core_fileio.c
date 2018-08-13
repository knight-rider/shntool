/*  core_fileio.c - file I/O functions
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

#include <string.h>
#include "shntool.h"

CVSID("$Id: core_fileio.c,v 1.44 2009/03/11 17:18:01 jason Exp $")

int read_n_bytes(FILE *in,uint8_t *buf,int num,progress_info *proginfo)
/* reads the specified number of bytes from the file descriptor 'in' into buf */
{
  int read;

  if ((read = fread(buf,1,num,in)) != num) {
    st_debug1("tried to read %d bytes, but only read %d -- possible truncated/corrupt file",num,read);
  }

  if (proginfo) {
    proginfo->bytes_written += read;
    prog_update(proginfo);
  }

  return read;
}

int write_n_bytes(FILE *out,uint8_t *buf,int num,progress_info *proginfo)
/* writes the specified number of bytes from buf into the file descriptor 'out' */
{
  int wrote;

  if ((wrote = fwrite(buf,1,num,out)) != num) {
    st_debug1("tried to write %d bytes, but only wrote %d -- make sure that:\n"
               "+ there is enough disk space\n"
               "+ the specified output directory exists\n"
               "+ you have permission to create files in the output directory\n"
               "+ the output format's encoder is installed and in your PATH",num,wrote);
  }

  if (proginfo) {
    proginfo->bytes_written += wrote;
    prog_update(proginfo);
  }

  return wrote;
}

uint32_t transfer_n_bytes_internal(FILE *in, FILE *out1, FILE *out2, uint32_t bytes, progress_info *proginfo)
/* transfers 'bytes' bytes from file descriptor 'in' to file descriptor 'out' */
{
  uint8_t buf[XFER_SIZE];
  int bytes_to_xfer,
      actual_bytes_read,
      actual_bytes_written1,
      actual_bytes_written2;
  uint32_t total_bytes_to_xfer = bytes,
                total_bytes_xfered = 0;

  while (total_bytes_to_xfer > 0) {
    bytes_to_xfer = min(total_bytes_to_xfer,XFER_SIZE);
    actual_bytes_read = read_n_bytes(in,buf,bytes_to_xfer,NULL);
    actual_bytes_written1 = write_n_bytes(out1,buf,actual_bytes_read,proginfo);
    actual_bytes_written2 = (out2) ? write_n_bytes(out2,buf,actual_bytes_read,NULL) : 0;
    total_bytes_xfered += (uint32_t)actual_bytes_written1;
    if (actual_bytes_read != bytes_to_xfer || actual_bytes_written1 != bytes_to_xfer || (out2 && actual_bytes_written2 != bytes_to_xfer))
      break;
    total_bytes_to_xfer -= bytes_to_xfer;
  }

  return total_bytes_xfered;
}

int write_padding(FILE *out,int bytes,progress_info *proginfo)
/* writes the specified number of zero bytes to the file descriptor given */
{
  uint8_t silence[CD_BLOCK_SIZE];

  if (bytes >= CD_BLOCK_SIZE) {
    /* padding should always be less than the size of the block being padded */
    return 0;
  }

  memset((void *)silence,0,CD_BLOCK_SIZE);

  return write_n_bytes(out,silence,bytes,proginfo);
}

bool read_value_u32(FILE *file, uint32_t *be_val, uint32_t *le_val, uint8_t *tag_val)
/* reads an uint32_t in big- and/or little-endian format from a file descriptor */
{
  uint8_t buf[4];

  if (fread(buf, 1, 4, file) != 4)
    return FALSE;

  if (be_val)
    *be_val = (uint32_t)buf[0] << 24 | (uint32_t)buf[1] << 16 | (uint32_t)buf[2] << 8 | buf[3];

  if (le_val)
    *le_val = (uint32_t)buf[3] << 24 | (uint32_t)buf[2] << 16 | (uint32_t)buf[1] << 8 | buf[0];

  if (tag_val)
    tagcpy(tag_val,buf);

  return TRUE;
}

bool read_value_u16(FILE *file, uint16_t *be_val, uint16_t *le_val)
/* reads an uint16_t in big- and/or little-endian format from a file descriptor */
{
  uint8_t buf[2];

  if (fread(buf, 1, 2, file) != 2)
    return FALSE;

  if (be_val)
    *be_val = (uint16_t)buf[0] << 8 | buf[1];

  if (le_val)
    *le_val = (uint16_t)buf[1] << 8 | buf[0];

  return TRUE;
}
