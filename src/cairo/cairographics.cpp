/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2008 Mehdi Abbad a.k.a slyf0x
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "guichan/cairo/cairographics.hpp"

#include "guichan/exception.hpp"
#include "guichan/cairo/cairofont.hpp"
#include "guichan/cairo/cairoimage.hpp"

namespace gcn
{

    CairoGraphics::CairoGraphics(cairo_surface_t* TargetSurface,int Width,int Height)
    {
        if (!TargetSurface)
        {
            GCN_EXCEPTION("Specified reference to target cairo surface is null!");
        }
        mCairoContext=cairo_create(TargetSurface);
        mTargetSurface=TargetSurface;
        mHeight=Height;
        mWidth=Width;
    }

    CairoGraphics::~CairoGraphics()
    {
        cairo_destroy(mCairoContext);
    }
    void CairoGraphics::_beginDraw()
    {
        pushClipArea(Rectangle(0,0,mWidth,mHeight));
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
        bool result=Graphics::pushClipArea(area);
        ClipRectangle& mCurrentDrawingArea=mClipStack.top();

        //saves the current context options including clipping area
        // ! Actualy saves all drawing options.
        cairo_save(mCairoContext);
        //create a rectangle corresponding to the clipping area
        cairo_rectangle(mCairoContext,
                        mCurrentDrawingArea.xOffset,
                        mCurrentDrawingArea.yOffset,
                        area.width,
                        area.height);
        cairo_clip(mCairoContext);
        return result;
    }

    void CairoGraphics::popClipArea()
    {
        Graphics::popClipArea();
        //restore drawing options
        cairo_restore(mCairoContext);
    }

    void CairoGraphics::drawImage(const Image* image,
                                int srcX,
                                int srcY,
                                int dstX,
                                int dstY,
                                int width,
                                int height)
    {
        const CairoImage *srcImage=dynamic_cast<const CairoImage*>(image);
        if (!srcImage)
        {
            throw GCN_EXCEPTION("Passed image reference is null or not of type gcn::CairoImage*.");
        }
        const ClipRectangle& top=mClipStack.top();

        cairo_save(mCairoContext);

        cairo_set_source_surface(mCairoContext,
                                 srcImage->mCairoSurface,
                                 top.xOffset + (dstX - srcX),
                                 top.yOffset + (dstY - srcY));
        //sets the clipping area
        cairo_rectangle(mCairoContext,
                        top.xOffset + dstX,
                        top.yOffset + dstY,
                        width,height);
        cairo_clip(mCairoContext);

        //paint and restore the context
        cairo_paint(mCairoContext);
        cairo_restore (mCairoContext);
    }

    void CairoGraphics::drawRectangle(const Rectangle& rectangle)
    {
        ClipRectangle& mCurrentDrawingArea=mClipStack.top();
        cairo_rectangle(mCairoContext,
                        mCurrentDrawingArea.xOffset+rectangle.x,
                        mCurrentDrawingArea.yOffset+rectangle.y,
                        rectangle.width,
                        rectangle.height);
        SetCurrentColorAsSource();
        cairo_stroke(mCairoContext);
    }

    void CairoGraphics::fillRectangle(const Rectangle& rectangle)
    {
        ClipRectangle& mCurrentDrawingArea=mClipStack.top();
        cairo_rectangle(mCairoContext,
                        mCurrentDrawingArea.xOffset+rectangle.x,
                        mCurrentDrawingArea.yOffset+rectangle.y,
                        rectangle.width,
                        rectangle.height);
        SetCurrentColorAsSource();
        cairo_fill(mCairoContext);
    }

    void CairoGraphics::drawPoint(int x, int y)
    {
        ClipRectangle& mCurrentDrawingArea=mClipStack.top();
        cairo_rectangle(mCairoContext,
                        mCurrentDrawingArea.xOffset+x,
                        mCurrentDrawingArea.yOffset+y,
                        1,1);
        SetCurrentColorAsSource();
        cairo_fill(mCairoContext);
    }

    void CairoGraphics::drawLine(int x1, int y1, int x2, int y2)
    {
        ClipRectangle& mCurrentDrawingArea=mClipStack.top();
        cairo_move_to(mCairoContext,
                      mCurrentDrawingArea.xOffset+x1,
                      mCurrentDrawingArea.yOffset+y1);

        cairo_line_to(mCairoContext,
                      mCurrentDrawingArea.xOffset+x2,
                      mCurrentDrawingArea.yOffset+y2);
        SetCurrentColorAsSource();
        cairo_set_line_width(mCairoContext,1.0f);
        cairo_set_line_cap(mCairoContext, CAIRO_LINE_CAP_SQUARE);
        cairo_stroke(mCairoContext);
    }

    void CairoGraphics::setColor(const Color& color)
    {
        mColor = color;
        mColorR= mColor.r/255.0;
        mColorG= mColor.g/255.0;
        mColorB= mColor.b/255.0;
        mColorA= mColor.a/255.0;
    }

    const Color& CairoGraphics::getColor() const
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
}
