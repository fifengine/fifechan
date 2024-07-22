// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_ALLEGROIMAGE_HPP
#define FCN_ALLEGROIMAGE_HPP

#include <allegro.h>

#include <string>

#include "fifechan/color.hpp"
#include "fifechan/image.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * Allegro implementation of Image.
     */
    class FIFEGUI_EXT_API AllegroImage : public Image
    {
    public:
        /**
         * Constructor. Load an image from an Allegro BITMAP.
         *
         * NOTE: The functions getPixel and putPixel are only guaranteed to work
         *       before an image has been converted to display format.
         *
         * @param bitmap the bitmap from which to load.
         * @param autoFree true if the surface should automatically be deleted.
         */
        AllegroImage(BITMAP* bitmap, bool autoFree);

        /**
         * Destructor.
         */
        virtual ~AllegroImage();

        /**
         * Gets the Allegro bitmap for the image.
         *
         * @return the Allegro bitmap for the image.
         */
        virtual BITMAP* getBitmap() const;

        // Inherited from Image

        virtual void free();

        virtual int getWidth() const;

        virtual int getHeight() const;

        virtual Color getPixel(int x, int y);

        virtual void putPixel(int x, int y, Color const & color);

        virtual void convertToDisplayFormat();

    protected:
        BITMAP* mBitmap;
        bool mAutoFree;
    };
} // namespace fcn

#endif // end FCN_ALLEGROIMAGE_HPP
