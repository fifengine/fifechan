// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_CAIRO_CAIROIMAGE_HPP_
#define INCLUDE_FIFECHAN_CAIRO_CAIROIMAGE_HPP_

#include <string>

#include "fifechan/color.hpp"
#include "fifechan/image.hpp"
#include "fifechan/platform.hpp"
#include <cairo.h>

namespace fcn
{

    /**
     * Cairo implementation of Image.
     */
    class FIFEGUI_EXT_API CairoImage : public Image
    {

        friend class CairoGraphics;

    public:
        explicit CairoImage(cairo_surface_t* surface);

        virtual ~CairoImage();

        /**
         * Gets a pointer to the encapsulated cairo image surface.
         */
        cairo_surface_t* GetSurface();

        // Inherited from Image

        virtual void free();

        virtual int getWidth() const;

        virtual int getHeight() const;

        virtual Color getPixel(int x, int y);

        virtual void putPixel(int x, int y, Color const & color);

        virtual void convertToDisplayFormat();

    private:
        std::uint32_t PrecomputeAlpha(Color const & color);
        std::uint32_t GetRGB(Color const & color);
        Color GetColorFromRGB(std::uint32_t color);
        Color GetColorFromARGB(std::uint32_t color);
        cairo_surface_t* mCairoSurface;
        bool mFinished;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_CAIRO_CAIROIMAGE_HPP_
