// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_ALLEGRO_ALLEGROIMAGELOADER_HPP_
#define INCLUDE_FIFECHAN_ALLEGRO_ALLEGROIMAGELOADER_HPP_

#include <allegro.h>

#include <string>

#include "fifechan/image.hpp"
#include "fifechan/imageloader.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{

    /**
     * Allegro implementation of ImageLoader.
     */
    class FIFEGUI_EXT_API AllegroImageLoader : public ImageLoader
    {
    public:
        // Inherited from ImageLoader

        virtual Image* load(std::string const & filename, bool convertToDisplayFormat = true);

    protected:
        virtual BITMAP* loadBitmap(std::string const & filename, PALETTE pal);
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_ALLEGRO_ALLEGROIMAGELOADER_HPP_
