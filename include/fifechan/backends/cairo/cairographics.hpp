// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_BACKENDS_CAIRO_CAIROGRAPHICS_HPP_
#define INCLUDE_FIFECHAN_BACKENDS_CAIRO_CAIROGRAPHICS_HPP_

#include "cairo.h"
#include "fifechan/color.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    class Image;
    class Rectangle;

    /**
     * Cairo implementation of the Graphics.
     */
    class FIFEGUI_EXT_API CairoGraphics : public Graphics
    {
    public:
        // Needed so that drawImage(fcn::Image *, int, int) is visible.
        using Graphics::drawImage;

        /**
         * Constructor.
         * Create a CairoGraphics backend using a specific cairo target surface.
         *
         * @param TargetSurface a cairo_surface_t* where the drawings are made. It can
         * be any surface type (png, Xlib, Win32).
         */
        CairoGraphics(cairo_surface_t* TargetSurface, int Width, int Height);

        virtual ~CairoGraphics();

        /**
         * Returns the current cairo context encapsulated by the graphics object
         * @return A pointer to the associated cairo_t object.
         */
        cairo_t* GetContext();

        // Inherited from Graphics

        virtual void _beginDraw();

        virtual void _endDraw();

        virtual bool pushClipArea(Rectangle area);

        virtual void popClipArea();

        virtual void drawImage(Image const * image, int srcX, int srcY, int dstX, int dstY, int width, int height);

        virtual void drawPoint(int x, int y);

        virtual void drawLine(int x1, int y1, int x2, int y2);

        virtual void drawRectangle(Rectangle const & rectangle);

        virtual void fillRectangle(Rectangle const & rectangle);

        virtual void setColor(Color const & color);

        virtual Color const & getColor() const;

    private:
        void SetCurrentColorAsSource();

        cairo_t* mCairoContext;
        cairo_surface_t* mTargetSurface;
        cairo_surface_t* mBufferSurface;
        int mWidth, mHeight;

        bool FinalizeTargetSurface;
        Color mColor;
        double mColorR, mColorG, mColorB, mColorA;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_BACKENDS_CAIRO_CAIROGRAPHICS_HPP_
