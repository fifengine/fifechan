// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_HGE_HGEIMAGE_HPP_
#define INCLUDE_FIFECHAN_HGE_HGEIMAGE_HPP_

#include <string>

#include "fifechan/color.hpp"
#include "fifechan/image.hpp"
#include "fifechan/platform.hpp"
#include <hge.h>
#include <hgesprite.h>

namespace fcn
{
    /**
     * HGE implementation of Image.
     */
    class FIFEGUI_EXT_API HGEImage : public Image
    {
    public:
        /**
         * Constructor.
         *
         * Creates an image from a HGE texture.
         *
         * NOTE : The functions getPixel and putPixel are only guaranteed to work
         * with a 32bits image.
         *
         * @param sprite A HGE texture to create the image from.
         * @param autoFree True it the image should be automatically deleted by
         *                 the destructor, false otherwise.
         */
        explicit HGEImage(HTEXTURE texture, bool autoFree = false);

        ~HGEImage();

        /**
         * Gets the HGE sprite of the image.
         *
         * @return the HGE sprite of the image.
         */
        virtual hgeSprite* getSprite() const;

        // Inherited from Image

        virtual void free();

        virtual int getWidth() const;

        virtual int getHeight() const;

        virtual Color getPixel(int x, int y);

        virtual void putPixel(int x, int y, Color const & color);

        virtual void convertToDisplayFormat();

    protected:
        static HGE* mHGE;
        bool mAutoFree;
        hgeSprite* mHGESprite;
        HTEXTURE mTexture;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_HGE_HGEIMAGE_HPP_
