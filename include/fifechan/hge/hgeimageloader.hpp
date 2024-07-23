// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_HGE_HGEIMAGELOADER_HPP_
#define INCLUDE_FIFECHAN_HGE_HGEIMAGELOADER_HPP_

#include <string>

#include "fifechan/imageloader.hpp"
#include "fifechan/platform.hpp"
#include <hge.h>
#include <hgeresource.h>

namespace fcn
{
    class Image;

    /**
     * HGE implementation of ImageLoader.
     */
    class FIFEGUI_EXT_API HGEImageLoader : public ImageLoader
    {
    public:
        HGEImageLoader();

        ~HGEImageLoader();

        /**
         * Loads an image with the HGE resource manager.
         *
         * NOTE: The functions Image::getPixel and Image::putPixel
         *       are only guaranteed to work before an image has
         *       been converted to display format.
         *
         * @param manager The manager to use.
         * @param name The name of the image.
         * @param resourceGroup The resource group.
         * @param convertToDisplayFormat True if the image should be converted
         *                               to display, false otherwise.
         */
        virtual Image* load(hgeResourceManager* manager, char* name, int resourceGroup, bool convertToDisplayFormat);

        // Inherited from ImageLoader

        virtual Image* load(std::string const & filename, bool convertToDisplayFormat = true);

    protected:
        static HGE* mHGE;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_HGE_HGEIMAGELOADER_HPP_
