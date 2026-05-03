// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/image.hpp"

// Standard library includes
#include <cassert>
#include <string>

// Platform config include
#include "fifechan/platform.hpp"

// Project headers (subdirs before local)
#include "fifechan/exception.hpp"
#include "fifechan/imageloader.hpp"

namespace fcn
{

    ImageLoader* Image::mImageLoader = nullptr;

    Image::Image() = default;

    Image::~Image() = default;

    void Image::setImageLoader(ImageLoader* imageLoader)
    {
        assert("Image loader must not be null" && imageLoader != nullptr);
        mImageLoader = imageLoader;
    }

    ImageLoader* Image::getImageLoader()
    {
        return mImageLoader;
    }

    void Image::resetImageLoader()
    {
        mImageLoader = nullptr;
    }

    Image* Image::load(std::string const & filename, bool convertToDisplayFormat)
    {
        assert("Image loader must be set before loading" && mImageLoader != nullptr);
        assert("Filename must not be empty" && !filename.empty());

        return mImageLoader->load(filename, convertToDisplayFormat);
    }
} // namespace fcn
