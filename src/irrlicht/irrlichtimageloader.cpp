// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/irrlicht/irrlichtimageloader.hpp"

#include "fifechan/exception.hpp"
#include "fifechan/irrlicht/irrlichtimage.hpp"

namespace fcn
{
    IrrlichtImageLoader::IrrlichtImageLoader(irr::video::IVideoDriver* driver) : mDriver(driver)
    {
        if (mDriver) {
            mDriver->grab();
        }
    }

    IrrlichtImageLoader::~IrrlichtImageLoader()
    {
        if (mDriver) {
            mDriver->drop();
        }
    }

    Image* IrrlichtImageLoader::load(std::string const & filename, bool convertToDisplayFormat)
    {
        irr::video::IImage* image = mDriver->createImageFromFile(filename.c_str());

        if (image == NULL) {
            fcn::throwException(
                (std::string("Unable to load image file: ") + filename),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        return new IrrlichtImage(image, mDriver, filename, true, convertToDisplayFormat);
    }
} // namespace fcn
