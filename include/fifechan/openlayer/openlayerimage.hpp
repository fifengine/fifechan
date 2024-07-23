// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_OPENLAYER_OPENLAYERIMAGE_HPP_
#define INCLUDE_FIFECHAN_OPENLAYER_OPENLAYERIMAGE_HPP_

#include <OpenLayer.hpp>
#if defined(DELETE)
#    undef DELETE
#endif
#include <string>

#include "fifechan/color.hpp"
#include "fifechan/image.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * OpenLayer implementation of Image.
     */
    class FIFEGUI_EXT_API OpenLayerImage : public Image
    {
    public:
        /**
         * Constructor. Load an image from an Allegro BITMAP.
         *
         * @param bitmap the bitmap from which to load.
         * @param autoFree true if the bitmap should automatically be deleted.
         */
        explicit OpenLayerImage(BITMAP* bitmap, bool autoFree = true);

        virtual ~OpenLayerImage();

        /**
         * Gets the OpenLayer bitmap for the image.
         *
         * @return the OpenLayer bitmap for the image.
         */
        virtual ol::Bitmap* getBitmap() const;

        // Inherited from Image

        virtual void free();

        virtual int getWidth() const;

        virtual int getHeight() const;

        virtual Color getPixel(int x, int y);

        virtual void putPixel(int x, int y, Color const & color);

        virtual void convertToDisplayFormat();

    protected:
        ol::Bitmap* mOpenLayerBitmap;
        BITMAP* mAllegroBitmap;
        bool mAutoFree;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_OPENLAYER_OPENLAYERIMAGE_HPP_
