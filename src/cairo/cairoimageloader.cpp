// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/cairo/cairoimageloader.hpp"

#include "fifechan/cairo/cairoimage.hpp"
#include "fifechan/exception.hpp"

namespace fcn
{
    Image* CairoImageLoader::load(std::string const & filename, bool convertToDisplayFormat)
    {
        cairo_surface_t* loadedSurface = cairo_image_surface_create_from_png(filename.c_str());

        if (loadedSurface == NULL) {
            fcn::throwException(
                (std::string("Unable to load image file: ") + filename),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        Image* image = new CairoImage(loadedSurface);
        if (convertToDisplayFormat) {
            image->convertToDisplayFormat();
        }
        return image;
    }
} // namespace fcn
