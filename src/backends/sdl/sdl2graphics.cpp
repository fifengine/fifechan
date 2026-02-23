// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2016, 2018, 2019 Gwilherm Baudic
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/backends/sdl/sdl2graphics.hpp"

#include "fifechan/backends/sdl/sdlimage.hpp"
#include "fifechan/backends/sdl/sdlpixel.hpp"
#include "fifechan/exception.hpp"
#include "fifechan/font.hpp"
#include "fifechan/image.hpp"

// For some reason an old version of MSVC did not like std::abs,
// so we added this macro.
#ifndef ABS
    #define ABS(x) ((x) < 0 ? -(x) : (x))
#endif

namespace fcn
{

    SDL2Graphics::SDL2Graphics()
    {
        mAlpha = false;
    }

    SDL2Graphics::~SDL2Graphics()
    {
        if (mRenderTarget != nullptr) {
            SDL_DestroyTexture(mTexture);
            SDL_FreeSurface(mTarget);
        }
    }

    void SDL2Graphics::_beginDraw()
    {
        Rectangle area;
        area.x      = 0;
        area.y      = 0;
        area.width  = mTarget->w;
        area.height = mTarget->h;
        pushClipArea(area);
    }

    void SDL2Graphics::_endDraw()
    {
        popClipArea();
    }

    void SDL2Graphics::setTarget(SDL_Renderer* renderer, int width, int height)
    {
        mRenderTarget = renderer;

        // An internal surface is still required to be able to handle surfaces and colorkeys
        mTarget = SDL_CreateRGBSurface(0, width, height, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
        SDL_FillRect(mTarget, NULL, SDL_MapRGB(mTarget->format, 0xff, 0, 0xff));
        SDL_SetColorKey(mTarget, SDL_TRUE, SDL_MapRGB(mTarget->format, 0xff, 0, 0xff)); // magenta
        SDL_SetSurfaceBlendMode(mTarget, SDL_BLENDMODE_NONE); // needed to cleanup temp data properly
        mTexture = SDL_CreateTextureFromSurface(mRenderTarget, mTarget);
        SDL_SetTextureBlendMode(mTexture, SDL_BLENDMODE_BLEND);
    }

    bool SDL2Graphics::pushClipArea(Rectangle area)
    {
        SDL_Rect rect;
        bool result = Graphics::pushClipArea(area);

        ClipRectangle const & clip_rect = mClipStack.top();
        rect.x                          = clip_rect.x;
        rect.y                          = clip_rect.y;
        rect.w                          = clip_rect.width;
        rect.h                          = clip_rect.height;

        SDL_RenderSetClipRect(mRenderTarget, &rect);

        return result;
    }

    void SDL2Graphics::popClipArea()
    {
        Graphics::popClipArea();

        if (mClipStack.empty()) {
            return;
        }

        ClipRectangle const & clip_rect = mClipStack.top();
        SDL_Rect rect;
        rect.x = clip_rect.x;
        rect.y = clip_rect.y;
        rect.w = clip_rect.width;
        rect.h = clip_rect.height;

        SDL_RenderSetClipRect(mRenderTarget, &rect);
    }

    SDL_Renderer* SDL2Graphics::getTarget() const
    {
        return mRenderTarget;
    }

    void SDL2Graphics::drawImage(Image const * image, int srcX, int srcY, int dstX, int dstY, int width, int height)
    {
        if (mClipStack.empty()) {
            fcn::throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        ClipRectangle const & top = mClipStack.top();
        SDL_Rect src;
        SDL_Rect dst;
        SDL_Rect temp;
        src.x  = srcX;
        src.y  = srcY;
        src.w  = width;
        src.h  = height;
        dst.x  = dstX + top.xOffset;
        dst.y  = dstY + top.yOffset;
        dst.w  = width;
        dst.h  = height;
        temp.x = 0;
        temp.y = 0;
        temp.w = width;
        temp.h = height;

        SDLImage const * srcImage = dynamic_cast<SDLImage const *>(image);

        if (srcImage == nullptr) {
            fcn::throwException(
                "Trying to draw an image of unknown format, must be an SDLImage.",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        if (srcImage->getTexture() == nullptr) {
            SDL_FillRect(mTarget, &temp, SDL_MapRGBA(mTarget->format, 0xff, 0, 0xff, 0));
            SDL_BlitSurface(srcImage->getSurface(), &src, mTarget, &temp);
            SDL_UpdateTexture(mTexture, &temp, mTarget->pixels, mTarget->pitch);
            SDL_RenderCopy(mRenderTarget, mTexture, &temp, &dst);
        } else {
            SDL_RenderCopy(mRenderTarget, srcImage->getTexture(), &src, &dst);
        }
    }

    void SDL2Graphics::fillRectangle(Rectangle const & rectangle)
    {
        if (mClipStack.empty()) {
            fcn::throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        ClipRectangle const & top = mClipStack.top();

        Rectangle area = rectangle;
        area.x += top.xOffset;
        area.y += top.yOffset;

        if (!area.isIntersecting(top)) {
            return;
        }

        if (mAlpha) {
            int x1 = area.x > top.x ? area.x : top.x;
            int y1 = area.y > top.y ? area.y : top.y;
            int x2 = area.x + area.width < top.x + top.width ? area.x + area.width : top.x + top.width;
            int y2 = area.y + area.height < top.y + top.height ? area.y + area.height : top.y + top.height;
            SDL_Rect rect;
            rect.x = x1;
            rect.y = y1;
            rect.w = x2 - x1;
            rect.h = y2 - y1;

            saveRenderColor();
            SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);
            SDL_RenderFillRect(mRenderTarget, &rect);
            restoreRenderColor();

        } else {
            SDL_Rect rect;
            rect.x = area.x;
            rect.y = area.y;
            rect.w = area.width;
            rect.h = area.height;

            saveRenderColor();
            SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);
            SDL_RenderFillRect(mRenderTarget, &rect);
            restoreRenderColor();
        }
    }

    void SDL2Graphics::drawPoint(int x, int y)
    {
        if (mClipStack.empty()) {
            fcn::throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        ClipRectangle const & top = mClipStack.top();

        x += top.xOffset;
        y += top.yOffset;

        if (!top.isContaining(x, y))
            return;

        saveRenderColor();
        SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);
        /*if (mAlpha)
        {
            SDLputPixelAlpha(mTarget, x, y, mColor);

        }
        else
        {
            SDLputPixel(mTarget, x, y, mColor);
        }*/
        SDL_RenderDrawPoint(mRenderTarget, x, y);
        restoreRenderColor();
    }

    void SDL2Graphics::drawHLine(int x1, int y, int x2)
    {
        if (mClipStack.empty()) {
            fcn::throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }
        ClipRectangle const & top = mClipStack.top();

        x1 += top.xOffset;
        y += top.yOffset;
        x2 += top.xOffset;

        if (y < top.y || y >= top.y + top.height)
            return;

        if (x1 > x2) {
            x1 ^= x2;
            x2 ^= x1;
            x1 ^= x2;
        }

        if (top.x > x1) {
            if (top.x > x2) {
                return;
            }
            x1 = top.x;
        }

        if (top.x + top.width <= x2) {
            if (top.x + top.width <= x1) {
                return;
            }
            x2 = top.x + top.width - 1;
        }

        saveRenderColor();
        SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);
        SDL_RenderDrawLine(mRenderTarget, x1, y, x2, y);
        restoreRenderColor();
    }

    void SDL2Graphics::drawVLine(int x, int y1, int y2)
    {
        if (mClipStack.empty()) {
            fcn::throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }
        ClipRectangle const & top = mClipStack.top();

        x += top.xOffset;
        y1 += top.yOffset;
        y2 += top.yOffset;

        if (x < top.x || x >= top.x + top.width)
            return;

        if (y1 > y2) {
            y1 ^= y2;
            y2 ^= y1;
            y1 ^= y2;
        }

        if (top.y > y1) {
            if (top.y > y2) {
                return;
            }
            y1 = top.y;
        }

        if (top.y + top.height <= y2) {
            if (top.y + top.height <= y1) {
                return;
            }
            y2 = top.y + top.height - 1;
        }

        saveRenderColor();
        SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);
        SDL_RenderDrawLine(mRenderTarget, x, y1, x, y2);
        restoreRenderColor();
    }

    void SDL2Graphics::drawRectangle(Rectangle const & rectangle)
    {
        int x1 = rectangle.x;
        int x2 = rectangle.x + rectangle.width - 1;
        int y1 = rectangle.y;
        int y2 = rectangle.y + rectangle.height - 1;

        drawHLine(x1, y1, x2);
        drawHLine(x1, y2, x2);

        drawVLine(x1, y1, y2);
        drawVLine(x2, y1, y2);
    }

    void SDL2Graphics::drawLine(int x1, int y1, int x2, int y2)
    {

        if (mClipStack.empty()) {
            fcn::throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }
        ClipRectangle const & top = mClipStack.top();

        x1 += top.xOffset;
        y1 += top.yOffset;
        x2 += top.xOffset;
        y2 += top.yOffset;

        saveRenderColor();
        SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);
        SDL_RenderDrawLine(mRenderTarget, x1, y1, x2, y2);
        restoreRenderColor();
    }

    // Function to draw a thick line
    void SDL2Graphics::drawLine(int x1, int y1, int x2, int y2, unsigned int width)
    {
        if (mClipStack.empty()) {
            fcn::throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }
        ClipRectangle const & top = mClipStack.top();

        x1 += top.xOffset;
        y1 += top.yOffset;
        x2 += top.xOffset;
        y2 += top.yOffset;

        saveRenderColor();
        SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);

        // Calculate the direction vector of the line
        float dx      = x2 - x1;
        float dy      = y2 - y1;
        float length  = std::sqrt(dx * dx + dy * dy);
        float offsetX = (dy / length) * (width / 2.0f);
        float offsetY = (dx / length) * (width / 2.0f);

        // Draw multiple parallel lines to simulate thickness
        for (int i = -width / 2; i <= width / 2; ++i) {
            int startX = static_cast<int>(x1 + i * offsetX);
            int startY = static_cast<int>(y1 - i * offsetY);
            int endX   = static_cast<int>(x2 + i * offsetX);
            int endY   = static_cast<int>(y2 - i * offsetY);
            SDL_RenderDrawLine(mRenderTarget, startX, startY, endX, endY);
        }

        restoreRenderColor();
    }

    // Function to draw a filled circle
    void SDL2Graphics::drawFillCircle(fcn::Point const & center, unsigned int radius)
    {
        if (mClipStack.empty()) {
            fcn::throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }
        ClipRectangle const & top = mClipStack.top();

        int x0 = center.x + top.xOffset;
        int y0 = center.y + top.yOffset;

        saveRenderColor();
        SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);

        // Use Midpoint Circle Algorithm to draw a filled circle
        for (int y = -radius; y <= radius; y++) {
            for (int x = -radius; x <= radius; x++) {
                if (x * x + y * y <= radius * radius) {
                    SDL_RenderDrawPoint(mRenderTarget, x0 + x, y0 + y);
                }
            }
        }

        restoreRenderColor();
    }

    // Function to compute a point on a Bézier curve
    fcn::Point bezierPoint(std::vector<fcn::Point> const & controlPoints, float t)
    {
        std::vector<fcn::Point> points = controlPoints;
        while (points.size() > 1) {
            std::vector<fcn::Point> nextPoints;
            for (size_t i = 0; i < points.size() - 1; ++i) {
                int x = static_cast<int>((1 - t) * points[i].x + t * points[i + 1].x);
                int y = static_cast<int>((1 - t) * points[i].y + t * points[i + 1].y);
                nextPoints.emplace_back(fcn::Point{x, y});
            }
            points = nextPoints;
        }
        return points[0];
    }

    // Function to convert degrees to radians
    constexpr float degToRad(float degrees)
    {
        return degrees * static_cast<float>(M_PI) / 180.0f;
    }

    // Function to draw a filled circle segment
    void SDL2Graphics::drawFillCircleSegment(
        fcn::Point const & center, unsigned int radius, int startAngle, int endAngle)
    {
        if (mClipStack.empty()) {
            fcn::throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }
        ClipRectangle const & top = mClipStack.top();

        int x0 = center.x + top.xOffset;
        int y0 = center.y + top.yOffset;

        saveRenderColor();
        SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);

        // Normalize angles to [0, 360)
        startAngle = startAngle % 360;
        endAngle   = endAngle % 360;

        // If endAngle < startAngle, it means the segment crosses the 0-degree line
        if (endAngle < startAngle) {
            endAngle += 360;
        }

        for (int y = -radius; y <= radius; y++) {
            for (int x = -radius; x <= radius; x++) {
                if (x * x + y * y <= radius * radius) {
                    float angle = std::atan2(y, x) * 180.0f / static_cast<float>(M_PI);
                    if (angle < 0)
                        angle += 360;

                    if (angle >= startAngle && angle <= endAngle) {
                        SDL_RenderDrawPoint(mRenderTarget, x0 + x, y0 + y);
                    }
                }
            }
        }

        restoreRenderColor();
    }

    // Function to draw a circle
    void SDL2Graphics::drawCircle(fcn::Point const & center, unsigned int radius)
    {
        if (mClipStack.empty()) {
            fcn::throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }
        ClipRectangle const & top = mClipStack.top();

        int x0 = center.x + top.xOffset;
        int y0 = center.y + top.yOffset;

        saveRenderColor();
        SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);

        // Midpoint Circle Algorithm
        int x = radius;
        int y = 0;
        int p = 1 - radius;

        while (x >= y) {
            // Draw the eight octants
            SDL_RenderDrawPoint(mRenderTarget, x0 + x, y0 + y);
            SDL_RenderDrawPoint(mRenderTarget, x0 - x, y0 + y);
            SDL_RenderDrawPoint(mRenderTarget, x0 + x, y0 - y);
            SDL_RenderDrawPoint(mRenderTarget, x0 - x, y0 - y);
            SDL_RenderDrawPoint(mRenderTarget, x0 + y, y0 + x);
            SDL_RenderDrawPoint(mRenderTarget, x0 - y, y0 + x);
            SDL_RenderDrawPoint(mRenderTarget, x0 + y, y0 - x);
            SDL_RenderDrawPoint(mRenderTarget, x0 - y, y0 - x);

            y++;

            if (p <= 0) {
                p = p + 2 * y + 1;
            } else {
                x--;
                p = p + 2 * y - 2 * x + 1;
            }
        }

        restoreRenderColor();
    }

    // normalize angles to [0, 360)
    int normalizeAngle(int angle)
    {
        angle %= 360;
        if (angle < 0) {
            angle += 360;
        }
        return angle;
    }

    // Function to draw a circle segment
    void SDL2Graphics::drawCircleSegment(fcn::Point const & center, unsigned int radius, int startAngle, int endAngle)
    {
        if (mClipStack.empty()) {
            fcn::throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }
        ClipRectangle const & top = mClipStack.top();

        int x0 = center.x + top.xOffset;
        int y0 = center.y + top.yOffset;

        saveRenderColor();
        SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);

        // Normalize angles to [0, 360)
        startAngle = normalizeAngle(startAngle);
        endAngle   = normalizeAngle(endAngle);

        // If endAngle < startAngle, it means the segment crosses the 0-degree line
        if (endAngle < startAngle) {
            endAngle += 360;
        }

        // Midpoint Circle Algorithm
        int x = radius;
        int y = 0;
        int p = 1 - radius;

        auto isInSegment = [&](int x, int y) {
            float angle = std::atan2(static_cast<float>(y), static_cast<float>(x)) * 180.0f / static_cast<float>(M_PI);
            if (angle < 0)
                angle += 360;
            return angle >= startAngle && angle <= endAngle;
        };

        while (x >= y) {
            // Draw points only if they are within the specified segment
            if (isInSegment(x, y))
                SDL_RenderDrawPoint(mRenderTarget, x0 + x, y0 + y);
            if (isInSegment(-x, y))
                SDL_RenderDrawPoint(mRenderTarget, x0 - x, y0 + y);
            if (isInSegment(x, -y))
                SDL_RenderDrawPoint(mRenderTarget, x0 + x, y0 - y);
            if (isInSegment(-x, -y))
                SDL_RenderDrawPoint(mRenderTarget, x0 - x, y0 - y);
            if (isInSegment(y, x))
                SDL_RenderDrawPoint(mRenderTarget, x0 + y, y0 + x);
            if (isInSegment(-y, x))
                SDL_RenderDrawPoint(mRenderTarget, x0 - y, y0 + x);
            if (isInSegment(y, -x))
                SDL_RenderDrawPoint(mRenderTarget, x0 + y, y0 - x);
            if (isInSegment(-y, -x))
                SDL_RenderDrawPoint(mRenderTarget, x0 - y, y0 - x);

            y++;
            if (p <= 0) {
                p = p + 2 * y + 1;
            } else {
                x--;
                p = p + 2 * y - 2 * x + 1;
            }
        }

        restoreRenderColor();
    }

    // Function to draw a Bézier curve
    void SDL2Graphics::drawBezier(std::vector<fcn::Point> const & controlPoints, int segments, unsigned int width)
    {
        if (mClipStack.empty()) {
            fcn::throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }
        ClipRectangle const & top = mClipStack.top();

        saveRenderColor();
        SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);

        fcn::Point previousPoint = bezierPoint(controlPoints, 0.0f);

        for (int i = 1; i <= segments; ++i) {
            float t                 = static_cast<float>(i) / static_cast<float>(segments);
            fcn::Point currentPoint = bezierPoint(controlPoints, t);

            // Draw thick line segment
            drawLine(
                previousPoint.x + top.xOffset,
                previousPoint.y + top.yOffset,
                currentPoint.x + top.xOffset,
                currentPoint.y + top.yOffset,
                width);

            previousPoint = currentPoint;
        }

        restoreRenderColor();
    }

    // Function to draw a polyline
    void SDL2Graphics::drawPolyLine(std::vector<fcn::Point> const & points, unsigned int width)
    {
        if (mClipStack.empty()) {
            fcn::throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        if (points.size() < 2) {
            return; // Not enough points to form a line
        }

        ClipRectangle const & top = mClipStack.top();

        saveRenderColor();
        SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);

        for (size_t i = 0; i < points.size() - 1; ++i) {
            int x1 = points[i].x + top.xOffset;
            int y1 = points[i].y + top.yOffset;
            int x2 = points[i + 1].x + top.xOffset;
            int y2 = points[i + 1].y + top.yOffset;

            drawLine(x1, y1, x2, y2, width);
        }

        restoreRenderColor();
    }

    void SDL2Graphics::setColor(Color const & color)
    {
        mColor = color;

        mAlpha = color.a != 255;
    }

    Color const & SDL2Graphics::getColor() const
    {
        return mColor;
    }

    void SDL2Graphics::drawSDLSurface(SDL_Surface* surface, SDL_Rect source, SDL_Rect destination)
    {
        if (mClipStack.empty()) {
            fcn::throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }
        ClipRectangle const & top = mClipStack.top();

        destination.x += top.xOffset;
        destination.y += top.yOffset;
        destination.w = source.w;
        destination.h = source.h;
        SDL_Rect temp;
        temp.x = 0;
        temp.y = 0;
        temp.w = source.w;
        temp.h = source.h;

        SDL_FillRect(mTarget, &temp, SDL_MapRGBA(mTarget->format, 0xff, 0, 0xff, 0));
        SDL_BlitSurface(surface, &source, mTarget, &temp);
        SDL_UpdateTexture(mTexture, &temp, mTarget->pixels, mTarget->pitch);
        SDL_RenderCopy(mRenderTarget, mTexture, &temp, &destination);
    }

    void SDL2Graphics::drawSDLTexture(SDL_Texture* texture, SDL_Rect source, SDL_Rect destination)
    {
        if (mClipStack.empty()) {
            fcn::throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }
        ClipRectangle const & top = mClipStack.top();

        destination.x += top.xOffset;
        destination.y += top.yOffset;
        destination.w = source.w;
        destination.h = source.h;

        SDL_RenderCopy(mRenderTarget, texture, &source, &destination);
    }

    void SDL2Graphics::saveRenderColor()
    {
        SDL_GetRenderDrawColor(mRenderTarget, &r, &g, &b, &a);
    }

    void SDL2Graphics::restoreRenderColor()
    {
        SDL_SetRenderDrawColor(mRenderTarget, r, g, b, a);
    }

} // namespace fcn
