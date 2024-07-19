// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_OPENLAYERIMAGELOADER_HPP
#define FCN_OPENLAYERIMAGELOADER_HPP

#include "fifechan/image.hpp"
#include "fifechan/imageloader.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{

    /**
     * OpenLayer implementation of ImageLoader.
     */
    class FIFEGUI_EXT_API OpenLayerImageLoader : public ImageLoader
    {
    public:
        // Inherited from ImageLoader

        virtual Image* load(std::string const & filename, bool convertToDisplayFormat = true);
    };
} // namespace fcn

#endif // end FCN_OPENLAYERIMAGELOADER_HPP
