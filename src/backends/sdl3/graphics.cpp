// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2016 - 2019 Gwilherm Baudic
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/backends/sdl3/graphics.hpp"

// Standard library includes
#include <algorithm>
#include <cstdio>
#include <memory>
#include <numbers>
#include <string>
#include <utility>
#include <vector>

// Third-party library includes
#include <SDL3/SDL.h>

// Project headers (subdirs before local)
#include "fifechan/backends/sdl3/image.hpp"
#include "fifechan/backends/sdl3/truetypefont.hpp"
#include "fifechan/exception.hpp"
#include "fifechan/font.hpp"
#include "fifechan/image.hpp"

namespace fcn::sdl3
{

    Graphics::Graphics() : mAlpha(false)
    {
    }

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
            SDL_SetRenderClipRect(mRenderTarget, &rect);
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

        SDL_SetRenderClipRect(mRenderTarget, &rect);
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
        SDL_FRect src;
        SDL_FRect dst;
        src.x = static_cast<float>(srcX);
        src.y = static_cast<float>(srcY);
        src.w = static_cast<float>(width);
        src.h = static_cast<float>(height);
        dst.x = static_cast<float>(dstX + top.xOffset);
        dst.y = static_cast<float>(dstY + top.yOffset);
        dst.w = static_cast<float>(width);
        dst.h = static_cast<float>(height);

        auto const * srcImage = dynamic_cast<Image const *>(image);

        if (srcImage == nullptr) {
            throwException("Trying to draw an image of unknown format, must be an Image.");
        }

        SDL_Texture* texture = srcImage->getTexture();
        if (texture != nullptr) {
            SDL_RenderTexture(mRenderTarget, texture, &src, &dst);
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

        SDL_FRect rect;
        rect.x = static_cast<float>(area.x);
        rect.y = static_cast<float>(area.y);
        rect.w = static_cast<float>(area.width);
        rect.h = static_cast<float>(area.height);

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
        SDL_RenderPoint(mRenderTarget, x, y);
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
        SDL_RenderLine(mRenderTarget, x1, y, x2, y);
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
        SDL_RenderLine(mRenderTarget, x, y1, x, y2);
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
        SDL_RenderLine(mRenderTarget, x1, y1, x2, y2);
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
            // For a single point with width, draw a filled circle
            drawFillCircle(fcn::Point{x1 - top.xOffset, y1 - top.yOffset}, width / 2);
            return;
        }

        // Use SDL_RenderGeometry with quad primitives for thick line
        saveRenderColor();
        SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);

        auto const dx      = static_cast<float>(x2 - x1);
        auto const dy      = static_cast<float>(y2 - y1);
        float const length = std::sqrt((dx * dx) + (dy * dy));

        if (length < 0.001F) {
            restoreRenderColor();
            return;
        }

        // Calculate perpendicular offset for line width
        float const offsetX = (dy / length) * (static_cast<float>(width) / 2.0F);
        float const offsetY = (dx / length) * (static_cast<float>(width) / 2.0F);

        // Create quad vertices (two triangles)
        std::vector<SDL_Vertex> vertices = {
            SDL_Vertex{
                .position = {static_cast<float>(x1) - offsetX, static_cast<float>(y1) + offsetY},
                .color =
                    {static_cast<float>(mColor.r),
                     static_cast<float>(mColor.g),
                     static_cast<float>(mColor.b),
                     static_cast<float>(mColor.a)}},
            SDL_Vertex{
                .position = {static_cast<float>(x1) + offsetX, static_cast<float>(y1) - offsetY},
                .color =
                    {static_cast<float>(mColor.r),
                     static_cast<float>(mColor.g),
                     static_cast<float>(mColor.b),
                     static_cast<float>(mColor.a)}},
            SDL_Vertex{
                .position = {static_cast<float>(x2) + offsetX, static_cast<float>(y2) - offsetY},
                .color =
                    {static_cast<float>(mColor.r),
                     static_cast<float>(mColor.g),
                     static_cast<float>(mColor.b),
                     static_cast<float>(mColor.a)}},
            SDL_Vertex{
                .position = {static_cast<float>(x2) - offsetX, static_cast<float>(y2) + offsetY},
                .color    = {
                    static_cast<float>(mColor.r),
                    static_cast<float>(mColor.g),
                    static_cast<float>(mColor.b),
                    static_cast<float>(mColor.a)}}};

        // Two triangles forming a quad
        int const indices[] = {0, 1, 2, 0, 2, 3};

        SDL_RenderGeometry(mRenderTarget, nullptr, vertices.data(), static_cast<int>(vertices.size()), indices, 6);

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

        int const radius = std::max(1, static_cast<int>(width) / 2);

        // For a single point, just draw a filled circle
        if (x1 == x2 && y1 == y2) {
            drawFillCircle(fcn::Point{x1 - top.xOffset, y1 - top.yOffset}, static_cast<unsigned int>(radius));
            return;
        }

        // Draw filled circles along the line path using SDL_RenderGeometry
        saveRenderColor();
        SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);

        int const dx        = x2 - x1;
        int const dy        = y2 - y1;
        int const stepCount = std::max(std::abs(dx), std::abs(dy));

        // For each step, draw a filled circle using triangle fan
        // Use adaptive segment count based on radius for quality
        int const circleSegments = std::max(8, radius * 2);

        for (int step = 0; step <= stepCount; ++step) {
            int const centerX = x1 + ((dx * step) / stepCount);
            int const centerY = y1 + ((dy * step) / stepCount);

            // Generate triangle fan vertices for filled circle
            std::vector<SDL_Vertex> vertices;
            vertices.reserve(circleSegments + 2);

            // Center vertex
            vertices.push_back(
                SDL_Vertex{
                    .position = {static_cast<float>(centerX), static_cast<float>(centerY)},
                    .color    = {
                        static_cast<float>(mColor.r),
                        static_cast<float>(mColor.g),
                        static_cast<float>(mColor.b),
                        static_cast<float>(mColor.a)}});

            // Circle edge vertices
            for (int i = 0; i <= circleSegments; ++i) {
                float const angle =
                    2.0f * std::numbers::pi_v<float> * static_cast<float>(i) / static_cast<float>(circleSegments);
                float const x = static_cast<float>(centerX) + (radius * std::cos(angle));
                float const y = static_cast<float>(centerY) + (radius * std::sin(angle));
                vertices.push_back(
                    SDL_Vertex{
                        .position = {x, y},
                        .color    = {
                            static_cast<float>(mColor.r),
                            static_cast<float>(mColor.g),
                            static_cast<float>(mColor.b),
                            static_cast<float>(mColor.a)}});
            }

            // Generate indices for triangle fan
            std::vector<int> indices;
            indices.reserve(circleSegments * 3);
            for (int i = 1; i <= circleSegments; ++i) {
                indices.push_back(0); // Center
                indices.push_back(i);
                indices.push_back(i + 1 <= circleSegments ? i + 1 : 1);
            }

            SDL_RenderGeometry(
                mRenderTarget,
                nullptr,
                vertices.data(),
                static_cast<int>(vertices.size()),
                indices.data(),
                static_cast<int>(indices.size()));
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

        // Use SDL_RenderGeometry for hardware-accelerated circle rendering
        // Generate triangle fan vertices for the circle
        int const numSegments = std::max(8, static_cast<int>(radius / 2));
        std::vector<SDL_Vertex> vertices;
        vertices.reserve(numSegments + 2);

        // Center vertex
        vertices.push_back(
            SDL_Vertex{
                .position = {static_cast<float>(x0), static_cast<float>(y0)},
                .color    = {
                    static_cast<float>(mColor.r),
                    static_cast<float>(mColor.g),
                    static_cast<float>(mColor.b),
                    static_cast<float>(mColor.a)}});

        // Circle edge vertices
        for (int i = 0; i <= numSegments; ++i) {
            float const angle =
                2.0f * std::numbers::pi_v<float> * static_cast<float>(i) / static_cast<float>(numSegments);
            float const x = static_cast<float>(x0) + (radius * std::cos(angle));
            float const y = static_cast<float>(y0) + (radius * std::sin(angle));
            vertices.push_back(
                SDL_Vertex{
                    .position = {x, y},
                    .color    = {
                        static_cast<float>(mColor.r),
                        static_cast<float>(mColor.g),
                        static_cast<float>(mColor.b),
                        static_cast<float>(mColor.a)}});
        }

        // Generate indices for triangle fan
        std::vector<int> indices;
        indices.reserve(numSegments * 3);
        for (int i = 1; i <= numSegments; ++i) {
            indices.push_back(0); // Center
            indices.push_back(i);
            indices.push_back(i + 1 <= numSegments ? i + 1 : 1);
        }

        saveRenderColor();
        SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);
        SDL_RenderGeometry(
            mRenderTarget,
            nullptr,
            vertices.data(),
            static_cast<int>(vertices.size()),
            indices.data(),
            static_cast<int>(indices.size()));
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

        // Normalize angles
        startAngle = startAngle % 360;
        endAngle   = endAngle % 360;

        if (endAngle < startAngle) {
            endAngle += 360;
        }

        // Use SDL_RenderGeometry for hardware-accelerated filled circle segment
        // Generate triangle fan vertices for the circle segment
        int const numSegments = std::max(8, static_cast<int>(radius / 2));
        std::vector<SDL_Vertex> vertices;
        vertices.reserve(numSegments + 2);

        // Center vertex
        vertices.push_back(
            SDL_Vertex{
                .position = {static_cast<float>(x0), static_cast<float>(y0)},
                .color    = {
                    static_cast<float>(mColor.r),
                    static_cast<float>(mColor.g),
                    static_cast<float>(mColor.b),
                    static_cast<float>(mColor.a)}});

        float const startRad = static_cast<float>(startAngle) * std::numbers::pi_v<float> / 180.0F;
        float const endRad   = static_cast<float>(endAngle) * std::numbers::pi_v<float> / 180.0F;

        for (int i = 0; i <= numSegments; ++i) {
            float const t     = static_cast<float>(i) / static_cast<float>(numSegments);
            float const angle = startRad + (t * (endRad - startRad));
            float const x     = static_cast<float>(x0) + (radius * std::cos(angle));
            float const y     = static_cast<float>(y0) + (radius * std::sin(angle));
            vertices.push_back(
                SDL_Vertex{
                    .position = {x, y},
                    .color    = {
                        static_cast<float>(mColor.r),
                        static_cast<float>(mColor.g),
                        static_cast<float>(mColor.b),
                        static_cast<float>(mColor.a)}});
        }

        // Generate indices for triangle fan
        std::vector<int> indices;
        indices.reserve(numSegments * 3);
        for (int i = 1; i <= numSegments; ++i) {
            indices.push_back(0); // Center
            indices.push_back(i);
            indices.push_back(i + 1 <= numSegments ? i + 1 : 1);
        }

        saveRenderColor();
        SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);
        SDL_RenderGeometry(
            mRenderTarget,
            nullptr,
            vertices.data(),
            static_cast<int>(vertices.size()),
            indices.data(),
            static_cast<int>(indices.size()));
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

        // Use SDL_RenderGeometry for hardware-accelerated circle outline
        // Generate vertices along the circle and connect with line segments
        int const numSegments = std::max(8, static_cast<int>(radius / 2));
        std::vector<SDL_Vertex> vertices;
        vertices.reserve(numSegments + 1);

        for (int i = 0; i <= numSegments; ++i) {
            float const angle =
                2.0f * std::numbers::pi_v<float> * static_cast<float>(i) / static_cast<float>(numSegments);
            float const x = static_cast<float>(x0) + (radius * std::cos(angle));
            float const y = static_cast<float>(y0) + (radius * std::sin(angle));
            vertices.push_back(
                SDL_Vertex{
                    .position = {x, y},
                    .color    = {
                        static_cast<float>(mColor.r),
                        static_cast<float>(mColor.g),
                        static_cast<float>(mColor.b),
                        static_cast<float>(mColor.a)}});
        }

        // Generate indices for line loop (connect consecutive vertices)
        std::vector<int> indices;
        indices.reserve(numSegments * 2);
        for (int i = 0; i < numSegments; ++i) {
            indices.push_back(i);
            indices.push_back(i + 1);
        }

        saveRenderColor();
        SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);
        SDL_RenderGeometry(
            mRenderTarget,
            nullptr,
            vertices.data(),
            static_cast<int>(vertices.size()),
            indices.data(),
            static_cast<int>(indices.size()));
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

        // Normalize angles
        startAngle = normalizeAngle(startAngle);
        endAngle   = normalizeAngle(endAngle);

        if (endAngle < startAngle) {
            endAngle += 360;
        }

        // Use SDL_RenderGeometry for hardware-accelerated circle segment outline
        // Generate vertices along the arc from startAngle to endAngle
        int const numSegments = std::max(8, static_cast<int>(radius / 2));
        std::vector<SDL_Vertex> vertices;
        vertices.reserve(numSegments + 1);

        float const startRad = static_cast<float>(startAngle) * std::numbers::pi_v<float> / 180.0F;
        float const endRad   = static_cast<float>(endAngle) * std::numbers::pi_v<float> / 180.0F;

        for (int i = 0; i <= numSegments; ++i) {
            float const t     = static_cast<float>(i) / static_cast<float>(numSegments);
            float const angle = startRad + (t * (endRad - startRad));
            float const x     = static_cast<float>(x0) + (radius * std::cos(angle));
            float const y     = static_cast<float>(y0) + (radius * std::sin(angle));
            vertices.push_back(
                SDL_Vertex{
                    .position = {x, y},
                    .color    = {
                        static_cast<float>(mColor.r),
                        static_cast<float>(mColor.g),
                        static_cast<float>(mColor.b),
                        static_cast<float>(mColor.a)}});
        }

        // Generate indices for line loop
        std::vector<int> indices;
        indices.reserve(numSegments * 2);
        for (int i = 0; i < numSegments; ++i) {
            indices.push_back(i);
            indices.push_back(i + 1);
        }

        saveRenderColor();
        SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);
        SDL_RenderGeometry(
            mRenderTarget,
            nullptr,
            vertices.data(),
            static_cast<int>(vertices.size()),
            indices.data(),
            static_cast<int>(indices.size()));
        restoreRenderColor();
    }

    void Graphics::drawBezier(PointVector const & controlPoints, int segments, unsigned int width)
    {
        if (mClipStack.empty()) {
            throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?");
        }
        ClipRectangle const & top = mClipStack.top();

        if (segments < 1) {
            return;
        }

        // Generate all points along the Bezier curve
        std::vector<SDL_FPoint> points;
        points.reserve(segments + 1);

        for (int i = 0; i <= segments; ++i) {
            float const t          = static_cast<float>(i) / static_cast<float>(segments);
            fcn::Point const point = bezierPoint(controlPoints, t);
            points.push_back(
                SDL_FPoint{static_cast<float>(point.x + top.xOffset), static_cast<float>(point.y + top.yOffset)});
        }

        if (width <= 1) {
            // Use SDL_RenderLines for thin lines (batched rendering)
            saveRenderColor();
            SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);
            SDL_RenderLines(mRenderTarget, points.data(), static_cast<int>(points.size()));
            restoreRenderColor();
        } else {
            // For thick lines, draw multiple quads along the path using SDL_RenderGeometry
            saveRenderColor();
            SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);

            float const halfWidth = static_cast<float>(width) / 2.0F;

            for (size_t i = 0; i < points.size() - 1; ++i) {
                float const x1 = points[i].x;
                float const y1 = points[i].y;
                float const x2 = points[i + 1].x;
                float const y2 = points[i + 1].y;

                float const dx     = x2 - x1;
                float const dy     = y2 - y1;
                float const length = std::sqrt((dx * dx) + (dy * dy));

                if (length < 0.001F) {
                    continue;
                }

                // Calculate perpendicular offset for line width
                float const offsetX = (dy / length) * halfWidth;
                float const offsetY = (dx / length) * halfWidth;

                // Create quad vertices (two triangles)
                std::vector<SDL_Vertex> vertices = {
                    SDL_Vertex{
                        .position = {x1 - offsetX, y1 + offsetY},
                        .color =
                            {static_cast<float>(mColor.r),
                             static_cast<float>(mColor.g),
                             static_cast<float>(mColor.b),
                             static_cast<float>(mColor.a)}},
                    SDL_Vertex{
                        .position = {x1 + offsetX, y1 - offsetY},
                        .color =
                            {static_cast<float>(mColor.r),
                             static_cast<float>(mColor.g),
                             static_cast<float>(mColor.b),
                             static_cast<float>(mColor.a)}},
                    SDL_Vertex{
                        .position = {x2 + offsetX, y2 - offsetY},
                        .color =
                            {static_cast<float>(mColor.r),
                             static_cast<float>(mColor.g),
                             static_cast<float>(mColor.b),
                             static_cast<float>(mColor.a)}},
                    SDL_Vertex{
                        .position = {x2 - offsetX, y2 + offsetY},
                        .color    = {
                            static_cast<float>(mColor.r),
                            static_cast<float>(mColor.g),
                            static_cast<float>(mColor.b),
                            static_cast<float>(mColor.a)}}};

                int const indices[] = {0, 1, 2, 0, 2, 3};
                SDL_RenderGeometry(
                    mRenderTarget, nullptr, vertices.data(), static_cast<int>(vertices.size()), indices, 6);
            }

            restoreRenderColor();
        }
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

        ClipRectangle const & top = mClipStack.top();

        // Convert points to SDL_FPoint array with offset applied
        std::vector<SDL_FPoint> sdlPoints;
        sdlPoints.reserve(points.size());
        for (auto const & point : points) {
            sdlPoints.push_back(
                SDL_FPoint{static_cast<float>(point.x + top.xOffset), static_cast<float>(point.y + top.yOffset)});
        }

        if (width <= 1) {
            // Use SDL_RenderLines for batched rendering of thin lines
            saveRenderColor();
            SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);
            SDL_RenderLines(mRenderTarget, sdlPoints.data(), static_cast<int>(sdlPoints.size()));
            restoreRenderColor();
        } else {
            // For thick lines, draw quads between each pair of points
            saveRenderColor();
            SDL_SetRenderDrawColor(mRenderTarget, mColor.r, mColor.g, mColor.b, mColor.a);

            float const halfWidth = static_cast<float>(width) / 2.0F;

            for (size_t i = 0; i < sdlPoints.size() - 1; ++i) {
                float const x1 = sdlPoints[i].x;
                float const y1 = sdlPoints[i].y;
                float const x2 = sdlPoints[i + 1].x;
                float const y2 = sdlPoints[i + 1].y;

                float const dx     = x2 - x1;
                float const dy     = y2 - y1;
                float const length = std::sqrt((dx * dx) + (dy * dy));

                if (length < 0.001F) {
                    continue;
                }

                // Calculate perpendicular offset for line width
                float const offsetX = (dy / length) * halfWidth;
                float const offsetY = (dx / length) * halfWidth;

                // Create quad vertices (two triangles)
                std::vector<SDL_Vertex> vertices = {
                    SDL_Vertex{
                        .position = {x1 - offsetX, y1 + offsetY},
                        .color =
                            {static_cast<float>(mColor.r),
                             static_cast<float>(mColor.g),
                             static_cast<float>(mColor.b),
                             static_cast<float>(mColor.a)}},
                    SDL_Vertex{
                        .position = {x1 + offsetX, y1 - offsetY},
                        .color =
                            {static_cast<float>(mColor.r),
                             static_cast<float>(mColor.g),
                             static_cast<float>(mColor.b),
                             static_cast<float>(mColor.a)}},
                    SDL_Vertex{
                        .position = {x2 + offsetX, y2 - offsetY},
                        .color =
                            {static_cast<float>(mColor.r),
                             static_cast<float>(mColor.g),
                             static_cast<float>(mColor.b),
                             static_cast<float>(mColor.a)}},
                    SDL_Vertex{
                        .position = {x2 - offsetX, y2 + offsetY},
                        .color    = {
                            static_cast<float>(mColor.r),
                            static_cast<float>(mColor.g),
                            static_cast<float>(mColor.b),
                            static_cast<float>(mColor.a)}}};

                int const indices[] = {0, 1, 2, 0, 2, 3};
                SDL_RenderGeometry(
                    mRenderTarget, nullptr, vertices.data(), static_cast<int>(vertices.size()), indices, 6);
            }

            restoreRenderColor();
        }
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

    void Graphics::drawSDLTexture(SDL_Texture* texture, SDL_FRect source, SDL_FRect destination)
    {
        if (mClipStack.empty()) {
            throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?");
        }

        ClipRectangle const & top = mClipStack.top();

        destination.x += static_cast<float>(top.xOffset);
        destination.y += static_cast<float>(top.yOffset);
        destination.w = source.w;
        destination.h = source.h;

        SDL_RenderTexture(mRenderTarget, texture, &source, &destination);
    }

    void Graphics::saveRenderColor()
    {
        SDL_GetRenderDrawColor(mRenderTarget, &r, &g, &b, &a);
    }

    void Graphics::restoreRenderColor()
    {
        SDL_SetRenderDrawColor(mRenderTarget, r, g, b, a);
    }

} // namespace fcn::sdl3
