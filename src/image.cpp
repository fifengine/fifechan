// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include <string>

#include "fifechan/image.hpp"

#include "fifechan/exception.hpp"
#include "fifechan/imageloader.hpp"

namespace fcn
{

    ImageLoader* Image::mImageLoader = nullptr;

    Image::Image() = default;

    Image::~Image() = default;

    void Image::setImageLoader(ImageLoader* imageLoader)
    {
        mImageLoader = imageLoader;
    }

    ImageLoader* Image::getImageLoader()
    {
        return mImageLoader;
    }

    Image* Image::load(std::string const & filename, bool convertToDisplayFormat)
    {
        if (mImageLoader == nullptr) {
            fcn::throwException(
                "Trying to load an image but no image loader is set.",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        return mImageLoader->load(filename, convertToDisplayFormat);
    }
} // namespace fcn
