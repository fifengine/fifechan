// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/cairo/cairographics.hpp"

#include "fifechan/exception.hpp"
#include "fifechan/cairo/cairofont.hpp"
#include "fifechan/cairo/cairoimage.hpp"

namespace fcn
{

    CairoGraphics::CairoGraphics(cairo_surface_t* TargetSurface, int Width, int Height)
    {
        if (!TargetSurface) {
            fcn::throwException(
                "Specified reference to target cairo surface is null!",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }
        mCairoContext  = cairo_create(TargetSurface);
        mTargetSurface = TargetSurface;
        mHeight        = Height;
        mWidth         = Width;
    }

    CairoGraphics::~CairoGraphics()
    {
        cairo_destroy(mCairoContext);
    }
    void CairoGraphics::_beginDraw()
    {
        pushClipArea(Rectangle(0, 0, mWidth, mHeight));
        cairo_push_group(mCairoContext);
    }

    void CairoGraphics::_endDraw()
    {
        cairo_pop_group_to_source(mCairoContext);
        cairo_paint(mCairoContext);
        popClipArea();
    }

    bool CairoGraphics::pushClipArea(Rectangle area)
    {
        bool result                        = Graphics::pushClipArea(area);
        ClipRectangle& mCurrentDrawingArea = mClipStack.top();

        // saves the current context options including clipping area
        //  ! Actualy saves all drawing options.
        cairo_save(mCairoContext);
        // create a rectangle corresponding to the clipping area
        cairo_rectangle(
            mCairoContext, mCurrentDrawingArea.xOffset, mCurrentDrawingArea.yOffset, area.width, area.height);
        cairo_clip(mCairoContext);
        return result;
    }

    void CairoGraphics::popClipArea()
    {
        Graphics::popClipArea();
        // restore drawing options
        cairo_restore(mCairoContext);
    }

    void CairoGraphics::drawImage(Image const * image, int srcX, int srcY, int dstX, int dstY, int width, int height)
    {
        CairoImage const * srcImage = dynamic_cast<CairoImage const *>(image);
        if (!srcImage) {
            fcn::throwException(
                ("Passed image reference is null or not of type fcn::CairoImage*."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }
        ClipRectangle const & top = mClipStack.top();

        cairo_save(mCairoContext);

        cairo_set_source_surface(
            mCairoContext, srcImage->mCairoSurface, top.xOffset + (dstX - srcX), top.yOffset + (dstY - srcY));
        // sets the clipping area
        cairo_rectangle(mCairoContext, top.xOffset + dstX, top.yOffset + dstY, width, height);
        cairo_clip(mCairoContext);

        // paint and restore the context
        cairo_paint(mCairoContext);
        cairo_restore(mCairoContext);
    }

    void CairoGraphics::drawRectangle(Rectangle const & rectangle)
    {
        ClipRectangle& mCurrentDrawingArea = mClipStack.top();
        cairo_rectangle(
            mCairoContext,
            mCurrentDrawingArea.xOffset + rectangle.x,
            mCurrentDrawingArea.yOffset + rectangle.y,
            rectangle.width,
            rectangle.height);
        SetCurrentColorAsSource();
        cairo_stroke(mCairoContext);
    }

    void CairoGraphics::fillRectangle(Rectangle const & rectangle)
    {
        ClipRectangle& mCurrentDrawingArea = mClipStack.top();
        cairo_rectangle(
            mCairoContext,
            mCurrentDrawingArea.xOffset + rectangle.x,
            mCurrentDrawingArea.yOffset + rectangle.y,
            rectangle.width,
            rectangle.height);
        SetCurrentColorAsSource();
        cairo_fill(mCairoContext);
    }

    void CairoGraphics::drawPoint(int x, int y)
    {
        ClipRectangle& mCurrentDrawingArea = mClipStack.top();
        cairo_rectangle(mCairoContext, mCurrentDrawingArea.xOffset + x, mCurrentDrawingArea.yOffset + y, 1, 1);
        SetCurrentColorAsSource();
        cairo_fill(mCairoContext);
    }

    void CairoGraphics::drawLine(int x1, int y1, int x2, int y2)
    {
        ClipRectangle& mCurrentDrawingArea = mClipStack.top();
        cairo_move_to(mCairoContext, mCurrentDrawingArea.xOffset + x1, mCurrentDrawingArea.yOffset + y1);

        cairo_line_to(mCairoContext, mCurrentDrawingArea.xOffset + x2, mCurrentDrawingArea.yOffset + y2);
        SetCurrentColorAsSource();
        cairo_set_line_width(mCairoContext, 1.0f);
        cairo_set_line_cap(mCairoContext, CAIRO_LINE_CAP_SQUARE);
        cairo_stroke(mCairoContext);
    }

    void CairoGraphics::setColor(Color const & color)
    {
        mColor  = color;
        mColorR = mColor.r / 255.0;
        mColorG = mColor.g / 255.0;
        mColorB = mColor.b / 255.0;
        mColorA = mColor.a / 255.0;
    }

    Color const & CairoGraphics::getColor() const
    {
        return mColor;
    }

    void CairoGraphics::SetCurrentColorAsSource()
    {
        cairo_set_source_rgba(mCairoContext, mColorR, mColorG, mColorB, mColorA);
    }

    cairo_t* CairoGraphics::GetContext()
    {
        return mCairoContext;
    }
} // namespace fcn
