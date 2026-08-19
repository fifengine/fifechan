// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2016 - 2019 Gwilherm Baudic
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/backends/sdl3/graphics.hpp"

// Standard library includes
#include <algorithm>
#include <array>
#include <iterator>
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

    namespace
    {
        constexpr float kColorNormalizationFactor = 1.0F / 255.0F;

        void setRenderDrawColor(SDL_Renderer* renderer, fcn::Color const & color)
        {
            SDL_SetRenderDrawColorFloat(
                renderer,
                static_cast<float>(color.r) * kColorNormalizationFactor,
                static_cast<float>(color.g) * kColorNormalizationFactor,
                static_cast<float>(color.b) * kColorNormalizationFactor,
                static_cast<float>(color.a) * kColorNormalizationFactor);
        }

        SDL_FColor toSDLVertexColor(fcn::Color const & color)
        {
            return SDL_FColor{
                .r = static_cast<float>(color.r) * kColorNormalizationFactor,
                .g = static_cast<float>(color.g) * kColorNormalizationFactor,
                .b = static_cast<float>(color.b) * kColorNormalizationFactor,
                .a = static_cast<float>(color.a) * kColorNormalizationFactor};
        }

        SDL_Vertex makeSolidVertex(float x, float y, SDL_FColor const & color)
        {
            return SDL_Vertex{.position = {.x = x, .y = y}, .color = color};
        }
    } // namespace

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
        setRenderDrawColor(mRenderTarget, mColor);
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
        setRenderDrawColor(mRenderTarget, mColor);
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
        setRenderDrawColor(mRenderTarget, mColor);
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
        setRenderDrawColor(mRenderTarget, mColor);
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
        setRenderDrawColor(mRenderTarget, mColor);
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
        setRenderDrawColor(mRenderTarget, mColor);

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
        // TODO: remove format off. this formatting is inconsistent between clang-format versions
        // clang-format off
        std::vector<SDL_Vertex> vertices = {
            SDL_Vertex{
                .position = {.x = static_cast<float>(x1) - offsetX, .y = static_cast<float>(y1) + offsetY},
                .color    = {
                       .r = static_cast<float>(mColor.r) / 255.0F,
                       .g = static_cast<float>(mColor.g) / 255.0F,
                       .b = static_cast<float>(mColor.b) / 255.0F,
                       .a = static_cast<float>(mColor.a) / 255.0F}},
            SDL_Vertex{
                .position = {.x = static_cast<float>(x1) + offsetX, .y = static_cast<float>(y1) - offsetY},
                .color    = {
                       .r = static_cast<float>(mColor.r) / 255.0F,
                       .g = static_cast<float>(mColor.g) / 255.0F,
                       .b = static_cast<float>(mColor.b) / 255.0F,
                       .a = static_cast<float>(mColor.a) / 255.0F}},
            SDL_Vertex{
                .position = {.x = static_cast<float>(x2) + offsetX, .y = static_cast<float>(y2) - offsetY},
                .color    = {
                       .r = static_cast<float>(mColor.r) / 255.0F,
                       .g = static_cast<float>(mColor.g) / 255.0F,
                       .b = static_cast<float>(mColor.b) / 255.0F,
                       .a = static_cast<float>(mColor.a) / 255.0F}},
            SDL_Vertex{
                .position = {.x = static_cast<float>(x2) - offsetX, .y = static_cast<float>(y2) + offsetY},
                .color    = {
                       .r = static_cast<float>(mColor.r) / 255.0F,
                       .g = static_cast<float>(mColor.g) / 255.0F,
                       .b = static_cast<float>(mColor.b) / 255.0F,
                       .a = static_cast<float>(mColor.a) / 255.0F}}};
        // clang-format on

        // Two triangles forming a quad
        std::array<int, 6> const indices = {0, 1, 2, 0, 2, 3};

        SDL_RenderGeometry(
            mRenderTarget,
            nullptr,
            vertices.data(),
            static_cast<int>(vertices.size()),
            indices.data(),
            static_cast<int>(indices.size()));

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
        setRenderDrawColor(mRenderTarget, mColor);

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
            // TODO: remove format off. this formatting is inconsistent between clang-format versions
            // clang-format off
            vertices.push_back(
                SDL_Vertex{
                    .position = {.x = static_cast<float>(centerX), .y = static_cast<float>(centerY)},
                    .color    = {
                           .r = static_cast<float>(mColor.r) / 255.0F,
                           .g = static_cast<float>(mColor.g) / 255.0F,
                           .b = static_cast<float>(mColor.b) / 255.0F,
                           .a = static_cast<float>(mColor.a) / 255.0F}});
            // clang-format on

            // Circle edge vertices
            for (int i = 0; i <= circleSegments; ++i) {
                float const angle =
                    2.0F * std::numbers::pi_v<float> * static_cast<float>(i) / static_cast<float>(circleSegments);
                float const x = static_cast<float>(centerX) + (radius * std::cos(angle));
                float const y = static_cast<float>(centerY) + (radius * std::sin(angle));
                // TODO: remove format off. this formatting is inconsistent between clang-format versions
                // clang-format off
                vertices.push_back(
                    SDL_Vertex{
                        .position = {.x = x, .y = y},
                        .color    = {
                               .r = static_cast<float>(mColor.r) / 255.0F,
                               .g = static_cast<float>(mColor.g) / 255.0F,
                               .b = static_cast<float>(mColor.b) / 255.0F,
                               .a = static_cast<float>(mColor.a) / 255.0F}});
                // clang-format on
            }

            // Generate indices for triangle fan
            std::vector<int> indices;
            indices.reserve(static_cast<std::size_t>(circleSegments) * 3);
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
        SDL_FColor const vertexColor = toSDLVertexColor(mColor);

        // Center vertex
        vertices.push_back(makeSolidVertex(static_cast<float>(x0), static_cast<float>(y0), vertexColor));

        // Circle edge vertices
        for (int i = 0; i <= numSegments; ++i) {
            float const angle =
                2.0F * std::numbers::pi_v<float> * static_cast<float>(i) / static_cast<float>(numSegments);
            float const x = static_cast<float>(x0) + (radius * std::cos(angle));
            float const y = static_cast<float>(y0) + (radius * std::sin(angle));
            vertices.push_back(makeSolidVertex(x, y, vertexColor));
        }

        // Generate indices for triangle fan
        std::vector<int> indices;
        indices.reserve(static_cast<std::size_t>(numSegments) * 3);
        for (int i = 1; i <= numSegments; ++i) {
            indices.push_back(0); // Center
            indices.push_back(i);
            indices.push_back(i + 1);
        }

        saveRenderColor();
        setRenderDrawColor(mRenderTarget, mColor);
        SDL_RenderGeometry(
            mRenderTarget,
            nullptr,
            vertices.data(),
            static_cast<int>(vertices.size()),
            indices.data(),
            static_cast<int>(indices.size()));
        restoreRenderColor();
    }

    void Graphics::fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
    {
        if (mClipStack.empty()) {
            throwException(
                "Clip stack is empty, perhaps you"
                "called a draw function outside of _beginDraw() and _endDraw()?");
        }
        ClipRectangle const & top = mClipStack.top();

        int const ax = x1 + top.xOffset;
        int const ay = y1 + top.yOffset;
        int const bx = x2 + top.xOffset;
        int const by = y2 + top.yOffset;
        int const cx = x3 + top.xOffset;
        int const cy = y3 + top.yOffset;

        std::vector<SDL_Vertex> vertices(3);
        SDL_FColor const vertexColor = toSDLVertexColor(mColor);
        vertices[0]                  = makeSolidVertex(static_cast<float>(ax), static_cast<float>(ay), vertexColor);
        vertices[1]                  = makeSolidVertex(static_cast<float>(bx), static_cast<float>(by), vertexColor);
        vertices[2]                  = makeSolidVertex(static_cast<float>(cx), static_cast<float>(cy), vertexColor);

        std::vector<int> indices = {0, 1, 2};

        saveRenderColor();
        setRenderDrawColor(mRenderTarget, mColor);
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

        fcn::Point bezierPoint(fcn::PointVector const & controlPoints, float t)
        {
            // De Casteljau's algorithm for Bezier curves
            std::vector<fcn::Point> points = controlPoints;
            while (points.size() > 1) {
                std::vector<fcn::Point> next;
                for (size_t i = 0; i + 1 < points.size(); ++i) {
                    float const x = ((1.0F - t) * static_cast<float>(points.at(i).x)) +
                                    (t * static_cast<float>(points.at(i + 1).x));
                    float const y = ((1.0F - t) * static_cast<float>(points.at(i).y)) +
                                    (t * static_cast<float>(points.at(i + 1).y));
                    next.emplace_back(static_cast<int>(x), static_cast<int>(y));
                }
                points = std::move(next);
            }
            return points.empty() ? fcn::Point{} : points.at(0);
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
        SDL_FColor const vertexColor = toSDLVertexColor(mColor);

        for (int i = 0; i <= numSegments; ++i) {
            float const angle =
                2.0F * std::numbers::pi_v<float> * static_cast<float>(i) / static_cast<float>(numSegments);
            float const x = static_cast<float>(x0) + (radius * std::cos(angle));
            float const y = static_cast<float>(y0) + (radius * std::sin(angle));
            vertices.push_back(makeSolidVertex(x, y, vertexColor));
        }

        // Generate indices for line loop
        std::vector<int> indices;
        indices.reserve(static_cast<std::size_t>(numSegments) * 2);
        for (int i = 0; i < numSegments; ++i) {
            indices.push_back(i);
            indices.push_back(i + 1);
        }

        saveRenderColor();
        setRenderDrawColor(mRenderTarget, mColor);
        SDL_RenderGeometry(
            mRenderTarget,
            nullptr,
            vertices.data(),
            static_cast<int>(vertices.size()),
            indices.data(),
            static_cast<int>(indices.size()));
        restoreRenderColor();
    }

    void Graphics::drawCircleSegment(
        Point const & /*center*/, unsigned int /*radius*/, int /*startAngle*/, int /*endAngle*/)
    {
        // TODO: Implement this function
    }

    void Graphics::drawFillCircleSegment(
        Point const & /*center*/, unsigned int /*radius*/, int /*startAngle*/, int /*endAngle*/)
    {
        // TODO: Implement this function
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
                SDL_FPoint{
                    .x = static_cast<float>(point.x + top.xOffset), .y = static_cast<float>(point.y + top.yOffset)});
        }

        if (width <= 1) {
            // Use SDL_RenderLines for thin lines (batched rendering)
            saveRenderColor();
            setRenderDrawColor(mRenderTarget, mColor);
            SDL_RenderLines(mRenderTarget, points.data(), static_cast<int>(points.size()));
            restoreRenderColor();
        } else {
            // For thick lines, draw multiple quads along the path using SDL_RenderGeometry
            saveRenderColor();
            setRenderDrawColor(mRenderTarget, mColor);

            float const halfWidth = static_cast<float>(width) / 2.0F;

            for (size_t i = 0; i < points.size() - 1; ++i) {
                float const x1 = points.at(i).x;
                float const y1 = points.at(i).y;
                float const x2 = points.at(i + 1).x;
                float const y2 = points.at(i + 1).y;

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
                // TODO: remove format off. this formatting is inconsistent between clang-format versions
                // clang-format off
                std::vector<SDL_Vertex> vertices = {
                    SDL_Vertex{
                        .position = {.x = x1 - offsetX, .y = y1 + offsetY},
                        .color =
                            {.r = static_cast<float>(mColor.r) / 255.0F,
                             .g = static_cast<float>(mColor.g) / 255.0F,
                             .b = static_cast<float>(mColor.b) / 255.0F,
                             .a = static_cast<float>(mColor.a) / 255.0F}},
                    SDL_Vertex{
                        .position = {.x = x1 + offsetX, .y = y1 - offsetY},
                        .color =
                            {.r = static_cast<float>(mColor.r) / 255.0F,
                             .g = static_cast<float>(mColor.g) / 255.0F,
                             .b = static_cast<float>(mColor.b) / 255.0F,
                             .a = static_cast<float>(mColor.a) / 255.0F}},
                    SDL_Vertex{
                        .position = {.x = x2 + offsetX, .y = y2 - offsetY},
                        .color =
                            {.r = static_cast<float>(mColor.r) / 255.0F,
                             .g = static_cast<float>(mColor.g) / 255.0F,
                             .b = static_cast<float>(mColor.b) / 255.0F,
                             .a = static_cast<float>(mColor.a) / 255.0F}},
                    SDL_Vertex{
                        .position = {.x = x2 - offsetX, .y = y2 + offsetY},
                        .color    = {
                               .r = static_cast<float>(mColor.r) / 255.0F,
                               .g = static_cast<float>(mColor.g) / 255.0F,
                               .b = static_cast<float>(mColor.b) / 255.0F,
                               .a = static_cast<float>(mColor.a) / 255.0F}}};
                // clang-format on

                std::array<int, 6> const indices = {0, 1, 2, 0, 2, 3};
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
        std::ranges::transform(points, std::back_inserter(sdlPoints), [&top](auto const & point) {
            return SDL_FPoint{
                .x = static_cast<float>(point.x + top.xOffset), .y = static_cast<float>(point.y + top.yOffset)};
        });

        if (width <= 1) {
            // Use SDL_RenderLines for batched rendering of thin lines
            saveRenderColor();
            setRenderDrawColor(mRenderTarget, mColor);
            SDL_RenderLines(mRenderTarget, sdlPoints.data(), static_cast<int>(sdlPoints.size()));
            restoreRenderColor();
        } else {
            // For thick lines, draw quads between each pair of points
            saveRenderColor();
            setRenderDrawColor(mRenderTarget, mColor);

            float const halfWidth = static_cast<float>(width) / 2.0F;

            for (size_t i = 0; i < sdlPoints.size() - 1; ++i) {
                float const x1 = sdlPoints.at(i).x;
                float const y1 = sdlPoints.at(i).y;
                float const x2 = sdlPoints.at(i + 1).x;
                float const y2 = sdlPoints.at(i + 1).y;

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
                // TODO: remove format off. this formatting is inconsistent between clang-format versions
                // clang-format off
                std::vector<SDL_Vertex> vertices = {
                    SDL_Vertex{
                        .position = {.x = x1 - offsetX, .y = y1 + offsetY},
                        .color =
                            {.r = static_cast<float>(mColor.r) / 255.0F,
                             .g = static_cast<float>(mColor.g) / 255.0F,
                             .b = static_cast<float>(mColor.b) / 255.0F,
                             .a = static_cast<float>(mColor.a) / 255.0F}},
                    SDL_Vertex{
                        .position = {.x = x1 + offsetX, .y = y1 - offsetY},
                        .color =
                            {.r = static_cast<float>(mColor.r) / 255.0F,
                             .g = static_cast<float>(mColor.g) / 255.0F,
                             .b = static_cast<float>(mColor.b) / 255.0F,
                             .a = static_cast<float>(mColor.a) / 255.0F}},
                    SDL_Vertex{
                        .position = {.x = x2 + offsetX, .y = y2 - offsetY},
                        .color =
                            {.r = static_cast<float>(mColor.r) / 255.0F,
                             .g = static_cast<float>(mColor.g) / 255.0F,
                             .b = static_cast<float>(mColor.b) / 255.0F,
                             .a = static_cast<float>(mColor.a) / 255.0F}},
                    SDL_Vertex{
                        .position = {.x = x2 - offsetX, .y = y2 + offsetY},
                        .color    = {
                               .r = static_cast<float>(mColor.r) / 255.0F,
                               .g = static_cast<float>(mColor.g) / 255.0F,
                               .b = static_cast<float>(mColor.b) / 255.0F,
                               .a = static_cast<float>(mColor.a) / 255.0F}}};
                // clang-format on

                std::array<int, 6> const indices = {0, 1, 2, 0, 2, 3};
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

    void Graphics::drawSurface(SDL_Surface* surface, int dstX, int dstY)
    {
        if (surface == nullptr) {
            return;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderTarget, surface);
        if (texture == nullptr) {
            throwException(std::string("Graphics::drawSurface – ") + SDL_GetError());
        }

        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

        // Apply the current Graphics colour as a modulation so that fonts
        // rendered in white (e.g. TrueTypeFont renderToSurface) appear in
        // the correct colour.
        SDL_SetTextureColorMod(texture, mColor.r, mColor.g, mColor.b);
        SDL_SetTextureAlphaMod(texture, mColor.a);

        SDL_FRect src;
        src.x = 0.0F;
        src.y = 0.0F;
        src.w = static_cast<float>(surface->w);
        src.h = static_cast<float>(surface->h);

        SDL_FRect dst;
        dst.x = static_cast<float>(dstX);
        dst.y = static_cast<float>(dstY);
        dst.w = src.w;
        dst.h = src.h;

        drawSDLTexture(texture, src, dst);

        SDL_DestroyTexture(texture);
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
