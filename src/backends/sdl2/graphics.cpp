// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2016 - 2019 Gwilherm Baudic
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/backends/sdl2/graphics.hpp"

#include <SDL2/SDL_render.h>

#include <algorithm>
#include <cstdio>
#include <memory>
#include <numbers>
#include <string>
#include <utility>
#include <vector>

#include "fifechan/backends/sdl2/image.hpp"
#include "fifechan/backends/sdl2/truetypefont.hpp"
#include "fifechan/exception.hpp"
#include "fifechan/font.hpp"
#include "fifechan/image.hpp"

namespace fcn::sdl2
{

    Graphics::Graphics() : mAlpha(false) { }

    Graphics::~Graphics() = default;

    void Graphics::_beginDraw()
    {
        Rectangle area;
        area.x      = 0;
        area.y      = 0;
        area.width  = mWidth;
        area.height = mHeight;
        pushClipArea(area);
    }

    void Graphics::_endDraw()
    {
        popClipArea();
    }

    void Graphics::setTarget(SDL_Renderer* renderer, int width, int height)
    {
        mRenderTarget = renderer;
        mWidth        = width;
        mHeight       = height;
    }

    bool Graphics::pushClipArea(Rectangle area)
    {
        bool const result = fcn::Graphics::pushClipArea(area);

        if (result) {
            ClipRectangle const & clip_rect = mClipStack.top();

            SDL_Rect rect;
            rect.x = clip_rect.x;
            rect.y = clip_rect.y;
            rect.w = clip_rect.width;
            rect.h = clip_rect.height;
            SDL_RenderSetClipRect(mRenderTarget, &rect);
        }

        return result;
    }

    void Graphics::popClipArea()
    {
        ::fcn::Graphics::popClipArea();

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

    SDL_Renderer* Graphics::getRenderTarget() const
    {
        return mRenderTarget;
    }

    void Graphics::drawImage(fcn::Image const * image, int srcX, int srcY, int dstX, int dstY, int width, int height)
    {
        if (mClipStack.empty()) {
            throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?");
        }

        ClipRectangle const & top = mClipStack.top();
        SDL_Rect src;
        SDL_Rect dst;
        src.x = srcX;
        src.y = srcY;
        src.w = width;
        src.h = height;
        dst.x = dstX + top.xOffset;
        dst.y = dstY + top.yOffset;
        dst.w = width;
        dst.h = height;

        auto const * srcImage = dynamic_cast<Image const *>(image);

        if (srcImage == nullptr) {
            throwException("Trying to draw an image of unknown format, must be an Image.");
        }

        SDL_Texture* texture = srcImage->getTexture();
        if (texture != nullptr) {
            SDL_RenderCopy(mRenderTarget, texture, &src, &dst);
        }
    }

    void Graphics::fillRectangle(Rectangle const & rectangle)
    {
        if (mClipStack.empty()) {
            throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?");
        }

        ClipRectangle const & top = mClipStack.top();

        Rectangle area = rectangle;
        area.x += top.xOffset;
        area.y += top.yOffset;

        if (!area.isIntersecting(top)) {
            return;
        }

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

    void Graphics::drawPoint(int x, int y)
    {
        if (mClipStack.empty()) {
            throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?");
        }

        ClipRectangle const & top = mClipStack.top();

        x += top.xOffset;
        y += top.yOffset;

        if (!top.isContaining(x, y)) {
            return;
        }

        saveRenderColor();
        SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);
        SDL_RenderDrawPoint(mRenderTarget, x, y);
        restoreRenderColor();
    }

    void Graphics::drawHorizontalLine(int x1, int y, int x2)
    {
        if (mClipStack.empty()) {
            throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?");
        }
        ClipRectangle const & top = mClipStack.top();

        x1 += top.xOffset;
        y += top.yOffset;
        x2 += top.xOffset;

        if (y < top.y || y >= top.y + top.height) {
            return;
        }

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

    void Graphics::drawVerticalLine(int x, int y1, int y2)
    {
        if (mClipStack.empty()) {
            throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?");
        }
        ClipRectangle const & top = mClipStack.top();

        x += top.xOffset;
        y1 += top.yOffset;
        y2 += top.yOffset;

        if (x < top.x || x >= top.x + top.width) {
            return;
        }

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

    void Graphics::drawRectangle(Rectangle const & rectangle)
    {
        int const x1 = rectangle.x;
        int const x2 = rectangle.x + rectangle.width - 1;
        int const y1 = rectangle.y;
        int const y2 = rectangle.y + rectangle.height - 1;

        drawHorizontalLine(x1, y1, x2);
        drawHorizontalLine(x1, y2, x2);

        drawVerticalLine(x1, y1, y2);
        drawVerticalLine(x2, y1, y2);
    }

    void Graphics::drawLine(int x1, int y1, int x2, int y2)
    {

        if (mClipStack.empty()) {
            throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?");
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

    void Graphics::drawLine(int x1, int y1, int x2, int y2, unsigned int width)
    {
        if (mClipStack.empty()) {
            throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?");
        }
        ClipRectangle const & top = mClipStack.top();

        x1 += top.xOffset;
        y1 += top.yOffset;
        x2 += top.xOffset;
        y2 += top.yOffset;

        if (width <= 1) {
            drawLine(x1 - top.xOffset, y1 - top.yOffset, x2 - top.xOffset, y2 - top.yOffset);
            return;
        }

        if (x1 == x2 && y1 == y2) {
            drawRoundStroke(x1 - top.xOffset, y1 - top.yOffset, x2 - top.xOffset, y2 - top.yOffset, width);
            return;
        }

        saveRenderColor();
        SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);

        auto const dx       = static_cast<float>(x2 - x1);
        auto const dy       = static_cast<float>(y2 - y1);
        float const length  = std::sqrt((dx * dx) + (dy * dy));
        float const offsetX = (dy / length) * (static_cast<float>(width) / 2.0F);
        float const offsetY = (dx / length) * (static_cast<float>(width) / 2.0F);

        for (int i = -static_cast<int>(width) / 2; i <= static_cast<int>(width) / 2; ++i) {
            int const startX = static_cast<int>(x1 + (static_cast<float>(i) * offsetX));
            int const startY = static_cast<int>(y1 - (static_cast<float>(i) * offsetY));
            int const endX   = static_cast<int>(x2 + (static_cast<float>(i) * offsetX));
            int const endY   = static_cast<int>(y2 - (static_cast<float>(i) * offsetY));
            SDL_RenderDrawLine(mRenderTarget, startX, startY, endX, endY);
        }

        restoreRenderColor();
    }

    void Graphics::drawRoundStroke(int x1, int y1, int x2, int y2, unsigned int width)
    {
        if (mClipStack.empty()) {
            throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?");
        }
        ClipRectangle const & top = mClipStack.top();

        x1 += top.xOffset;
        y1 += top.yOffset;
        x2 += top.xOffset;
        y2 += top.yOffset;

        if (width <= 1) {
            drawLine(x1 - top.xOffset, y1 - top.yOffset, x2 - top.xOffset, y2 - top.yOffset);
            return;
        }

        saveRenderColor();
        SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);

        int const dx        = x2 - x1;
        int const dy        = y2 - y1;
        int const radius    = std::max(1, static_cast<int>(width) / 2);
        int const stepCount = std::max(std::abs(dx), std::abs(dy));

        auto drawFilledDisk = [&](int centerX, int centerY) {
            for (int offsetY = -radius; offsetY <= radius; ++offsetY) {
                for (int offsetX = -radius; offsetX <= radius; ++offsetX) {
                    if ((offsetX * offsetX) + (offsetY * offsetY) <= (radius * radius)) {
                        SDL_RenderDrawPoint(mRenderTarget, centerX + offsetX, centerY + offsetY);
                    }
                }
            }
        };

        if (stepCount == 0) {
            drawFilledDisk(x1, y1);
            restoreRenderColor();
            return;
        }

        for (int step = 0; step <= stepCount; ++step) {
            int const centerX = x1 + ((dx * step) / stepCount);
            int const centerY = y1 + ((dy * step) / stepCount);
            drawFilledDisk(centerX, centerY);
        }

        restoreRenderColor();
    }

    void Graphics::drawFillCircle(fcn::Point const & center, unsigned int radius)
    {
        if (mClipStack.empty()) {
            throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?");
        }
        ClipRectangle const & top = mClipStack.top();

        int const x0 = center.x + top.xOffset;
        int const y0 = center.y + top.yOffset;

        saveRenderColor();
        SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);

        for (int y = -radius; std::cmp_less_equal(y, radius); y++) {
            for (int x = -radius; std::cmp_less_equal(x, radius); x++) {
                if (x * x + y * y <= radius * radius) {
                    SDL_RenderDrawPoint(mRenderTarget, x0 + x, y0 + y);
                }
            }
        }

        restoreRenderColor();
    }

    namespace
    {
        fcn::Point bezierPoint(std::vector<fcn::Point> const & controlPoints, float t)
        {
            std::vector<fcn::Point> points = controlPoints;
            while (points.size() > 1) {
                std::vector<fcn::Point> nextPoints;
                for (size_t i = 0; i < points.size() - 1; ++i) {
                    int const x = static_cast<int>(((1 - t) * points[i].x) + (t * points[i + 1].x));
                    int const y = static_cast<int>(((1 - t) * points[i].y) + (t * points[i + 1].y));
                    nextPoints.emplace_back(x, y);
                }
                points = nextPoints;
            }
            return points[0];
        }

        constexpr float degToRad(float degrees)
        {
            return degrees * std::numbers::pi_v<float> / 180.0F;
        }
    } // namespace

    void Graphics::drawFillCircleSegment(fcn::Point const & center, unsigned int radius, int startAngle, int endAngle)
    {
        if (mClipStack.empty()) {
            throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?");
        }
        ClipRectangle const & top = mClipStack.top();

        int const x0 = center.x + top.xOffset;
        int const y0 = center.y + top.yOffset;

        saveRenderColor();
        SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);

        startAngle = startAngle % 360;
        endAngle   = endAngle % 360;

        if (endAngle < startAngle) {
            endAngle += 360;
        }

        for (int y = -radius; std::cmp_less_equal(y, radius); y++) {
            for (int x = -radius; std::cmp_less_equal(x, radius); x++) {
                if (x * x + y * y <= radius * radius) {
                    float angle = std::atan2(y, x) * 180.0F / std::numbers::pi_v<float>;
                    if (angle < 0) {
                        angle += 360;
                    }

                    if (angle >= startAngle && angle <= endAngle) {
                        SDL_RenderDrawPoint(mRenderTarget, x0 + x, y0 + y);
                    }
                }
            }
        }

        restoreRenderColor();
    }

    namespace
    {
        int normalizeAngle(int angle)
        {
            angle %= 360;
            if (angle < 0) {
                angle += 360;
            }
            return angle;
        }
    } // namespace

    void Graphics::drawCircle(fcn::Point const & center, unsigned int radius)
    {
        if (mClipStack.empty()) {
            throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?");
        }
        ClipRectangle const & top = mClipStack.top();

        int const x0 = center.x + top.xOffset;
        int const y0 = center.y + top.yOffset;

        saveRenderColor();
        SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);

        int x = radius;
        int y = 0;
        int p = 1 - radius;

        while (x >= y) {
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

    void Graphics::drawCircleSegment(fcn::Point const & center, unsigned int radius, int startAngle, int endAngle)
    {
        if (mClipStack.empty()) {
            throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?");
        }
        ClipRectangle const & top = mClipStack.top();

        int const x0 = center.x + top.xOffset;
        int const y0 = center.y + top.yOffset;

        saveRenderColor();
        SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);

        startAngle = normalizeAngle(startAngle);
        endAngle   = normalizeAngle(endAngle);

        if (endAngle < startAngle) {
            endAngle += 360;
        }

        int x = radius;
        int y = 0;
        int p = 1 - radius;

        auto isInSegment = [&](int x, int y) {
            float angle = std::atan2(static_cast<float>(y), static_cast<float>(x)) * 180.0F / std::numbers::pi_v<float>;
            if (angle < 0) {
                angle += 360;
            }
            return angle >= startAngle && angle <= endAngle;
        };

        while (x >= y) {
            if (isInSegment(x, y)) {
                SDL_RenderDrawPoint(mRenderTarget, x0 + x, y0 + y);
            }
            if (isInSegment(-x, y)) {
                SDL_RenderDrawPoint(mRenderTarget, x0 - x, y0 + y);
            }
            if (isInSegment(x, -y)) {
                SDL_RenderDrawPoint(mRenderTarget, x0 + x, y0 - y);
            }
            if (isInSegment(-x, -y)) {
                SDL_RenderDrawPoint(mRenderTarget, x0 - x, y0 - y);
            }
            if (isInSegment(y, x)) {
                SDL_RenderDrawPoint(mRenderTarget, x0 + y, y0 + x);
            }
            if (isInSegment(-y, x)) {
                SDL_RenderDrawPoint(mRenderTarget, x0 - y, y0 + x);
            }
            if (isInSegment(y, -x)) {
                SDL_RenderDrawPoint(mRenderTarget, x0 + y, y0 - x);
            }
            if (isInSegment(-y, -x)) {
                SDL_RenderDrawPoint(mRenderTarget, x0 - y, y0 - x);
            }

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

    void Graphics::drawBezier(PointVector const & controlPoints, int segments, unsigned int width)
    {
        if (mClipStack.empty()) {
            throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?");
        }
        saveRenderColor();
        SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);

        fcn::Point previousPoint = bezierPoint(controlPoints, 0.0F);

        for (int i = 1; i <= segments; ++i) {
            float const t                 = static_cast<float>(i) / static_cast<float>(segments);
            fcn::Point const currentPoint = bezierPoint(controlPoints, t);

            drawLine(previousPoint.x, previousPoint.y, currentPoint.x, currentPoint.y, width);

            previousPoint = currentPoint;
        }

        restoreRenderColor();
    }

    void Graphics::drawPolyLine(PointVector const & points, unsigned int width)
    {
        if (mClipStack.empty()) {
            throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?");
        }

        if (points.size() < 2) {
            return;
        }

        saveRenderColor();
        SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);

        for (size_t i = 0; i < points.size() - 1; ++i) {
            drawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, width);
        }

        restoreRenderColor();
    }

    void Graphics::setColor(Color const & color)
    {
        mColor = color;

        mAlpha = color.a != 255;
    }

    Color const & Graphics::getColor() const
    {
        return mColor;
    }

    std::shared_ptr<Font> Graphics::createFont(std::string const & filename, int size)
    {
        return std::make_shared<TrueTypeFont>(filename, size);
    }

    void Graphics::drawSDLTexture(SDL_Texture* texture, SDL_Rect source, SDL_Rect destination)
    {
        if (mClipStack.empty()) {
            throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?");
        }

        ClipRectangle const & top = mClipStack.top();

        destination.x += top.xOffset;
        destination.y += top.yOffset;
        destination.w = source.w;
        destination.h = source.h;

        SDL_RenderCopy(mRenderTarget, texture, &source, &destination);
    }

    void Graphics::saveRenderColor()
    {
        SDL_GetRenderDrawColor(mRenderTarget, &r, &g, &b, &a);
    }

    void Graphics::restoreRenderColor()
    {
        SDL_SetRenderDrawColor(mRenderTarget, r, g, b, a);
    }

} // namespace fcn::sdl2
