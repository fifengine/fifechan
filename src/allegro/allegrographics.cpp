// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/allegro/allegrographics.hpp"

#include "fifechan/allegro/allegroimage.hpp"
#include "fifechan/cliprectangle.hpp"
#include "fifechan/color.hpp"
#include "fifechan/exception.hpp"
#include "fifechan/rectangle.hpp"

namespace fcn
{
    AllegroGraphics::AllegroGraphics()
    {
        mTarget   = NULL;
        mClipNull = false;
    }

    AllegroGraphics::AllegroGraphics(BITMAP* target)
    {
        mTarget = target;
    }

    AllegroGraphics::~AllegroGraphics() { }

    void AllegroGraphics::setTarget(BITMAP* target)
    {
        mTarget = target;
    }

    BITMAP* AllegroGraphics::getTarget()
    {
        return mTarget;
    }

    void AllegroGraphics::_beginDraw()
    {
        if (mTarget == NULL) {
            fcn::throwException(
                "Target BITMAP is null, set it with setTarget first.",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        // push a clip area the size of the target bitmap
        pushClipArea(Rectangle(0, 0, mTarget->w, mTarget->h));
    }

    void AllegroGraphics::_endDraw()
    {
        // pop the clip area pushed in _beginDraw
        popClipArea();
    }

    bool AllegroGraphics::pushClipArea(Rectangle area)
    {
        bool result = Graphics::pushClipArea(area);

        ClipRectangle const & cr = mClipStack.top();

        // Allegro won't let you set clip areas
        // that have zero width or height
        // so we have to check for that.
        if (cr.width == 0 || cr.height == 0) {
            mClipNull = true;
        } else {
            mClipNull = false;
#if ALLEGRO_VERSION == 4 && ALLEGRO_SUB_VERSION == 0
            set_clip(mTarget, cr.x, cr.y, cr.x + cr.width - 1, cr.y + cr.height - 1);
#else
            set_clip_rect(mTarget, cr.x, cr.y, cr.x + cr.width - 1, cr.y + cr.height - 1);
#endif
        }

        return result;
    }

    void AllegroGraphics::popClipArea()
    {
        Graphics::popClipArea();

        if (mClipStack.empty()) {
            return;
        }

        ClipRectangle const & cr = mClipStack.top();

        // Allegro won't let you set clip areas
        // that have zero width or height
        // so we have to check for that.
        if (cr.width == 0 || cr.height == 0) {
            mClipNull = true;
        } else {
            mClipNull = false;
#if ALLEGRO_VERSION == 4 && ALLEGRO_SUB_VERSION == 0
            set_clip(mTarget, cr.x, cr.y, cr.x + cr.width - 1, cr.y + cr.height - 1);
#else
            set_clip_rect(mTarget, cr.x, cr.y, cr.x + cr.width - 1, cr.y + cr.height - 1);
#endif
        }
    }

    void AllegroGraphics::drawImage(Image const * image, int srcX, int srcY, int dstX, int dstY, int width, int height)
    {
        if (mClipNull) {
            return;
        }

        if (mClipStack.empty()) {
            fcn::throwException(
                "The clip stack is empty, perhaps you called a draw function "
                "outside of _beginDraw() and _endDraw()?",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        int const xOffset = mClipStack.top().xOffset;
        int const yOffset = mClipStack.top().yOffset;

        AllegroImage const * srcImage = dynamic_cast<AllegroImage const *>(image);

        if (srcImage == NULL) {
            fcn::throwException(
                ("Trying to draw an image of unknown format, must be an AllegroImage."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        masked_blit(srcImage->getBitmap(), mTarget, srcX, srcY, dstX + xOffset, dstY + yOffset, width, height);
    }

    void AllegroGraphics::drawPoint(int x, int y)
    {
        if (mClipNull) {
            return;
        }

        if (mClipStack.empty()) {
            fcn::throwException(
                "The clip stack is empty, perhaps you called a draw function "
                "outside of _beginDraw() and _endDraw()?",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        int const xOffset = mClipStack.top().xOffset;
        int const yOffset = mClipStack.top().yOffset;

        putpixel(mTarget, x + xOffset, y + yOffset, mAllegroColor);
    }

    void AllegroGraphics::drawLine(int x1, int y1, int x2, int y2)
    {
        if (mClipNull) {
            return;
        }

        if (mClipStack.empty()) {
            fcn::throwException(
                "The clip stack is empty, perhaps you called a draw function "
                "outside of _beginDraw() and _endDraw()?",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        int const xOffset = mClipStack.top().xOffset;
        int const yOffset = mClipStack.top().yOffset;

        line(mTarget, x1 + xOffset, y1 + yOffset, x2 + xOffset, y2 + yOffset, mAllegroColor);
    }

    void AllegroGraphics::drawRectangle(Rectangle const & rectangle)
    {
        if (mClipNull) {
            return;
        }

        if (mClipStack.empty()) {
            fcn::throwException(
                "The clip stack is empty, perhaps you called a draw function "
                "outside of _beginDraw() and _endDraw()?",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        int const xOffset = mClipStack.top().xOffset;
        int const yOffset = mClipStack.top().yOffset;

        rect(
            mTarget,
            rectangle.x + xOffset,
            rectangle.y + yOffset,
            rectangle.x + rectangle.width - 1 + xOffset,
            rectangle.y + rectangle.height - 1 + yOffset,
            mAllegroColor);
    }

    void AllegroGraphics::fillRectangle(Rectangle const & rectangle)
    {
        if (mClipNull) {
            return;
        }

        if (mClipStack.empty()) {
            fcn::throwException(
                "The clip stack is empty, perhaps you called a draw function "
                "outside of _beginDraw() and _endDraw()?",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        int const xOffset = mClipStack.top().xOffset;
        int const yOffset = mClipStack.top().yOffset;

        rectfill(
            mTarget,
            rectangle.x + xOffset,
            rectangle.y + yOffset,
            rectangle.x + rectangle.width - 1 + xOffset,
            rectangle.y + rectangle.height - 1 + yOffset,
            mAllegroColor);
    }

    void AllegroGraphics::setColor(Color const & color)
    {
        mColor        = color;
        mAllegroColor = makecol(color.r, color.g, color.b);

        if (color.a != 255) {
            set_trans_blender(255, 255, 255, color.a);
            drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
        } else {
            solid_mode();
        }
    }

    Color const & AllegroGraphics::getColor() const
    {
        return mColor;
    }

    int AllegroGraphics::getAllegroColor() const
    {
        return mAllegroColor;
    }

    void AllegroGraphics::drawBitmap(BITMAP* bitmap, int dstX, int dstY)
    {
        int const xOffset = mClipStack.top().xOffset;
        int const yOffset = mClipStack.top().yOffset;

        masked_blit(bitmap, mTarget, 0, 0, dstX + xOffset, dstY + yOffset, bitmap->w, bitmap->h);
    }
} // namespace fcn
