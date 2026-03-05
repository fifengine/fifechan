// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2016, 2018, 2019 Gwilherm Baudic
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_BACKENDS_SDL_SDL2GRAPHICS_HPP_
#define INCLUDE_FIFECHAN_BACKENDS_SDL_SDL2GRAPHICS_HPP_

#include <SDL2/SDL.h>

#include "fifechan/color.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/platform.hpp"

#include <memory>
#include <string>

namespace fcn
{
    class Image;
    class Rectangle;

    /**
     * SDL implementation of the Graphics using SDL_Surface.
     */
    class FIFEGUI_EXT_API SDLGraphics : public Graphics
    {
    public:
        using Graphics::drawImage;

        SDLGraphics();

        virtual void setTarget(SDL_Surface* target);
        virtual SDL_Surface* getTarget() const;
        virtual void drawSDLSurface(SDL_Surface* surface, SDL_Rect source, SDL_Rect destination);

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

    protected:
        virtual void drawHLine(int x1, int y, int x2);
        virtual void drawVLine(int x, int y1, int y2);

        SDL_Surface* mTarget;
        Color mColor;
        bool mAlpha;
    };

    /**
     * SDL implementation of the Graphics.
     */
    class FIFEGUI_EXT_API SDL2Graphics : public Graphics
    {
    public:
        // Needed so that drawImage(fcn::Image *, int, int) is visible.
        using Graphics::drawImage;

        SDL2Graphics();

        ~SDL2Graphics();

        /**
         *  Sets the target SDL_Renderer to use for drawing. Preferably done only once.
         *
         *  @param renderer the SDL_Renderer to use for drawing.
         *  @param width screen width
         *  @param height screen height
         */
        virtual void setTarget(SDL_Renderer* renderer, int width, int height);

        /**
         * Gets the target SDL_Renderer.
         *
         * @return the target SDL_Renderer.
         */
        virtual SDL_Renderer* getTarget() const;

        /**
         * Draws an SDL_Surface on the target surface. Normally you'll
         * use drawImage, but if you want to write SDL specific code
         * this function might come in handy.
         *
         * NOTE: The clip areas will be taken into account.
         */
        virtual void drawSDLSurface(SDL_Surface* surface, SDL_Rect source, SDL_Rect destination);

        /**
         * Draws an SDL_Texture on the target surface. Normally you'll
         * use drawImage, but if you want to write SDL specific code
         * this function might come in handy.
         *
         * NOTE: The clip areas will be taken into account.
         */
        virtual void drawSDLTexture(SDL_Texture* texture, SDL_Rect source, SDL_Rect destination);

        // Inherited from Graphics

        void _beginDraw() override;
        void _endDraw() override;
        bool pushClipArea(fcn::Rectangle area) override;
        void popClipArea() override;
        void drawImage(
            fcn::Image const * image, int srcX, int srcY, int dstX, int dstY, int width, int height) override;
        void drawPoint(int x, int y) override;
        void drawLine(int x1, int y1, int x2, int y2) override;
        void drawLine(int x1, int y1, int x2, int y2, unsigned int width) override;

        void drawRectangle(Rectangle const & rectangle) override;
        void fillRectangle(Rectangle const & rectangle) override;

        void drawPolyLine(PointVector const & points, unsigned int width) override;
        void drawBezier(PointVector const & points, int steps, unsigned int width) override;

        void drawCircle(Point const & p, unsigned int radius) override;
        void drawFillCircle(Point const & p, unsigned int radius) override;
        void drawCircleSegment(Point const & p, unsigned int radius, int sangle, int eangle) override;
        void drawFillCircleSegment(Point const & p, unsigned int radius, int sangle, int eangle) override;

        void setColor(Color const & color) override;
        Color const & getColor() const override;
        std::shared_ptr<Font> createFont(std::string const & filename, int size) override;

    protected:
        /**
         * Draws a horizontal line.
         *
         * @param x1 the start coordinate of the line.
         * @param y the y coordinate of the line.
         * @param x2 the end coordinate of the line.
         */
        void drawHLine(int x1, int y, int x2);

        /**
         * Draws a vertical line.
         *
         * @param x the x coordinate of the line.
         * @param y1 the start coordinate of the line.
         * @param y2 the end coordinate of the line.
         */
        void drawVLine(int x, int y1, int y2);

        /**
         *  Save the current rendering color before drawing.
         *  Does not affect the mColor attribute.
         */
        void saveRenderColor();

        /**
         *  Restore the rendering color after drawing
         */
        void restoreRenderColor();

        SDL_Surface* mTarget;
        SDL_Renderer* mRenderTarget;
        SDL_Texture* mTexture;
        Color mColor;
        Uint8 r, g, b, a; //! to store previous color from renderer
        bool mAlpha;
    };
} // namespace fcn

namespace fcn::sdl2
{
    using Graphics        = fcn::SDL2Graphics;
    using SurfaceGraphics = fcn::SDLGraphics;
} // namespace fcn::sdl2

#endif // INCLUDE_FIFECHAN_BACKENDS_SDL_SDL2GRAPHICS_HPP_
