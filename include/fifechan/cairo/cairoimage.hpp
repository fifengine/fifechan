// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_CAIROIMAGE_HPP
#define FCN_CAIROIMAGE_HPP

#include <string>
#include <cairo.h>
#include "fifechan/color.hpp"
#include "fifechan/platform.hpp"
#include "fifechan/image.hpp"

namespace fcn
{

    /**
     * Cairo implementation of Image.
     */
    class FCN_EXTENSION_DECLSPEC CairoImage : public Image
    {

        friend class CairoGraphics;

    public:
        CairoImage(cairo_surface_t* surface);

        /**
         * Destructor.
         */
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
        unsigned long PrecomputeAlpha(Color const & color);
        unsigned long GetRGB(Color const & color);
        Color GetColorFromRGB(unsigned long color);
        Color GetColorFromARGB(unsigned long color);
        cairo_surface_t* mCairoSurface;
        bool mFinished;
    };
} // namespace fcn

#endif
