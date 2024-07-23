// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_CAIRO_CAIROIMAGELOADER_HPP_
#define INCLUDE_FIFECHAN_CAIRO_CAIROIMAGELOADER_HPP_

#include <string>

#include "fifechan/imageloader.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    class Image;

    /**
     * Cairo implementation of ImageLoader.
     */
    class FIFEGUI_EXT_API CairoImageLoader : public ImageLoader
    {
    public:
        // Inherited from ImageLoader

        virtual Image* load(std::string const & filename, bool convertToDisplayFormat = true);
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_CAIRO_CAIROIMAGELOADER_HPP_
