/*
 * Copyright 2005 James Bursa <bursa@users.sourceforge.net>
 *           2008 Vincent Sanders <vince@simtec.co.uk>
 *
 * This file is part of NetSurf, http://www.netsurf-browser.org/
 *
 * NetSurf is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * NetSurf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <inttypes.h>

#include <assert.h>
#include "css/css.h"
#include "render/font.h"
#include "desktop/options.h"
#include "utils/utf8.h"

#include "framebuffer/fb_gui.h"
#include "framebuffer/fb_font.h"

const struct fb_font_desc*
fb_get_font(const struct css_style *style)
{
        return &font_vga_8x16;
}

utf8_convert_ret utf8_to_font_encoding(const struct fb_font_desc* font,
				       const char *string, 
				       size_t len,
				       char **result)
{
	return utf8_to_enc(string, font->encoding, len, result);

}

utf8_convert_ret utf8_to_local_encoding(const char *string, 
				       size_t len,
				       char **result)
{
	return utf8_to_enc(string, "CP437", len, result);

}

static bool nsfont_width(const struct css_style *style,
                         const char *string, size_t length,
                         int *width)
{
        const struct fb_font_desc* fb_font = fb_get_font(style);
        *width = fb_font->width * length;
	return true;
}

/**
 * Find the position in a string where an x coordinate falls.
 *
 * \param  style        css_style for this text, with style->font_size.size ==
 *                      CSS_FONT_SIZE_LENGTH
 * \param  string       UTF-8 string to measure
 * \param  length       length of string
 * \param  x            x coordinate to search for
 * \param  char_offset  updated to offset in string of actual_x, [0..length]
 * \param  actual_x     updated to x coordinate of character closest to x
 * \return  true on success, false on error and error reported
 */

static bool nsfont_position_in_string(const struct css_style *style,
		const char *string, size_t length,
		int x, size_t *char_offset, int *actual_x)
{
        const struct fb_font_desc* fb_font = fb_get_font(style);
        *char_offset = x / fb_font->width;
        if (*char_offset > length)
                *char_offset = length;
        *actual_x = *char_offset * fb_font->width;
	return true;
}


/**
 * Find where to split a string to make it fit a width.
 *
 * \param  style        css_style for this text, with style->font_size.size ==
 *                      CSS_FONT_SIZE_LENGTH
 * \param  string       UTF-8 string to measure
 * \param  length       length of string
 * \param  x            width available
 * \param  char_offset  updated to offset in string of actual_x, [0..length]
 * \param  actual_x     updated to x coordinate of character closest to x
 * \return  true on success, false on error and error reported
 *
 * On exit, [char_offset == 0 ||
 *           string[char_offset] == ' ' ||
 *           char_offset == length]
 */

static bool nsfont_split(const struct css_style *style,
		const char *string, size_t length,
		int x, size_t *char_offset, int *actual_x)
{

        const struct fb_font_desc* fb_font = fb_get_font(style);
        *char_offset = x / fb_font->width;
        if (*char_offset > length) {
                *char_offset = length;
        } else {
                while (*char_offset > 0) {
                        if (string[*char_offset] == ' ')
                                break;
                        (*char_offset)--;
                }
        }
        *actual_x = *char_offset * fb_font->width;
	return true;
}

const struct font_functions nsfont = {
	nsfont_width,
	nsfont_position_in_string,
	nsfont_split
};

/*
 * Local Variables:
 * c-basic-offset:8
 * End:
 */
