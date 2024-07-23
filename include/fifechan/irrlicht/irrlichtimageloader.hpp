// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_IRRLICHT_IRRLICHTIMAGELOADER_HPP_
#define INCLUDE_FIFECHAN_IRRLICHT_IRRLICHTIMAGELOADER_HPP_

#include <string>

#include "fifechan/imageloader.hpp"
#include "fifechan/platform.hpp"
#include "irrlicht.h"

namespace fcn
{
    class Image;

    /**
     * Irrlicht implementation of ImageLoader.
     */
    class FIFEGUI_EXT_API IrrlichtImageLoader : public ImageLoader
    {
    public:
        /**
         * Constructor.
         *
         * @param device The Irrlicht IVideoDriver device to use when loading images.
         */
        explicit IrrlichtImageLoader(irr::video::IVideoDriver* driver);

        ~IrrlichtImageLoader();

        // Inherited from ImageLoader

        virtual Image* load(std::string const & filename, bool convertToDisplayFormat = true);

    protected:
        /**
         * Holds the Irrlicht IVideoDriver to use when loading images.
         */
        irr::video::IVideoDriver* mDriver;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_IRRLICHT_IRRLICHTIMAGELOADER_HPP_
