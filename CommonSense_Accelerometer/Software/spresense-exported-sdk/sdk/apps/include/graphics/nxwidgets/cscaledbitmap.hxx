/****************************************************************************
 * apps/include/graphics/nxwidgets/cscaledbitmap.hxx
 *
 *   Copyright (C) 2013-2014 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX, NxWidgets, nor the names of its contributors
 *    me be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef __APPS_INCLUDE_GRAPHICS_NXWIDGETS_CSCALEDBITMAP_HXX
#define __APPS_INCLUDE_GRAPHICS_NXWIDGETS_CSCALEDBITMAP_HXX

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <fixedmath.h>
#include <debug.h>

#include <nuttx/video/rgbcolors.h>
#include <nuttx/nx/nxglib.h>

#include "graphics/nxwidgets/nxconfig.hxx"
#include "graphics/nxwidgets/ibitmap.hxx"

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Implementation Classes
 ****************************************************************************/

#if defined(__cplusplus)

namespace NXWidgets
{
  /**
   * Class for scaling layer for any bitmap that inherits from IBitMap
   */

  class CScaledBitmap : public IBitmap
  {
  protected:
    FAR IBitmap       *m_bitmap;      /**< The bitmap that is being scaled */
    struct nxgl_size_s m_size;        /**< Scaled size of the image */
    FAR uint8_t       *m_rowCache[2]; /**< Two cached rows of the image */
    unsigned int       m_row;         /**< Row number of the first cached row */
    b16_t              m_xScale;      /**< X scale factor */
    b16_t              m_yScale;      /**< Y scale factor */

    /**
     * Read two rows into the row cache
     *
     * @param row - The row number of the first row to cache
     */

    bool cacheRows(unsigned int row);

    /**
     * Given an two RGB colors and a fractional value, return the scaled
     * value between the two colors.
     *
     * @param incolor1 - The first color to be used
     * @param incolor2 - The second color to be used
     * @param fraction - The fractional value
     * @param outcolor - The returned, scaled color
     */

    bool scaleColor(FAR const struct rgbcolor_s &incolor1,
                    FAR const struct rgbcolor_s &incolor2,
                    b16_t fraction, FAR struct rgbcolor_s &outcolor);

    /**
     * Given an image row and a non-integer column offset, return the
     * interpolated RGB color value corresponding to that position
     *
     * @param row - The pointer to the row in the row cache to use
     * @param column - The non-integer column offset
     * @param outcolor - The returned, interpolated color
     *
     */

    bool rowColor(FAR uint8_t *row, b16_t column,
                  FAR struct rgbcolor_s &outcolor);

    /**
     * Copy constructor is protected to prevent usage.
     */

    inline CScaledBitmap(const CScaledBitmap &bitmap) { }

  public:

    /**
     * Constructor.
     *
     * @param bitmap The bitmap structure being scaled.
     * @newSize The new, scaled size of the image
     */

    CScaledBitmap(IBitmap *bitmap, struct nxgl_size_s &newSize);

    /**
     * Destructor.
     */

    ~CScaledBitmap(void);

    /**
     * Get the bitmap's color format.
     *
     * @return The bitmap's color format.
     */

    const uint8_t getColorFormat(void) const;

    /**
     * Get the bitmap's color format.
     *
     * @return The bitmap's pixel depth.
     */

    const uint8_t getBitsPerPixel(void) const;

    /**
     * Get the bitmap's width (in pixels/columns).
     *
     * @return The bitmap's width (in pixels/columns).
     */

    const nxgl_coord_t getWidth(void) const;

    /**
     * Get the bitmap's height (in rows).
     *
     * @return The bitmap's height (in rows).
     */

    const nxgl_coord_t getHeight(void) const;

    /**
     * Get the bitmap's width (in bytes).
     *
     * @return The bitmap's width (in bytes).
     */

    const size_t getStride(void) const;

    /**
     * Use the colors associated with a selected image.
     *
     * @param selected.  true: Use colors for a selected widget,
     *   false: Use normal (default) colors.
     */

    inline void setSelected(bool selected) {}

    /**
     * Get one row from the bit map image.
     *
     * @param x The offset into the row to get
     * @param y The row number to get
     * @param width The number of pixels to get from the row
     * @param data The memory location provided by the caller
     *   in which to return the data.  This should be at least
     *   (getWidth()*getBitsPerPixl() + 7)/8 bytes in length
     *   and properly aligned for the pixel color format.
     * @param True if the run was returned successfully.
     */

    bool getRun(nxgl_coord_t x, nxgl_coord_t y, nxgl_coord_t width,
                FAR void *data);
  };
}

#endif // __cplusplus

#endif // __APPS_INCLUDE_GRAPHICS_NXWIDGETS_CSCALEDBITMAP_HXX
