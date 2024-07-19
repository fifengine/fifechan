// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_CAIROIMAGELOADER_HPP
#define FCN_CAIROIMAGELOADER_HPP

#include "fifechan/imageloader.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    class Image;

    /**
     * Cairo implementation of ImageLoader.
     */
    class FCN_EXTENSION_DECLSPEC CairoImageLoader : public ImageLoader
    {
    public:
        // Inherited from ImageLoader

        virtual Image* load(std::string const & filename, bool convertToDisplayFormat = true);
    };
} // namespace fcn

#endif
