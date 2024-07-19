// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/allegro/allegroimageloader.hpp"

#include "fifechan/allegro/allegroimage.hpp"
#include "fifechan/exception.hpp"

namespace fcn
{
    Image* AllegroImageLoader::load(std::string const & filename, bool convertToDisplayFormat)
    {
#if !(ALLEGRO_VERSION == 4 && ALLEGRO_SUB_VERSION == 0)
        int colconv = get_color_conversion();
#endif

        set_color_conversion(COLORCONV_NONE);

        PALETTE pal;
        BITMAP* bmp = loadBitmap(filename, pal);

        if (bmp == NULL) {
            fcn::throwException(
                (std::string("Unable to load: ") + filename),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        BITMAP* bmp2 = create_bitmap_ex(32, bmp->w, bmp->h);

        if (bmp2 == NULL) {
            fcn::throwException(
                (std::string("Not enough memory to load: ") + filename),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        set_palette(pal);
        blit(bmp, bmp2, 0, 0, 0, 0, bmp->w, bmp->h);
        destroy_bitmap(bmp);

#if (ALLEGRO_VERSION == 4 && ALLEGRO_SUB_VERSION == 0)
        set_color_conversion(COLORCONV_TOTAL);
#else
        set_color_conversion(colconv);
#endif

        Image* image = new AllegroImage(bmp2, true);

        if (convertToDisplayFormat) {
            image->convertToDisplayFormat();
        }

        return image;
    }

    BITMAP* AllegroImageLoader::loadBitmap(std::string const & filename, PALETTE pal)
    {
        return load_bitmap(filename.c_str(), pal);
    }
} // namespace fcn
