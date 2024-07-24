// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_SDL_SDLGRAPHICS_HPP_
#define INCLUDE_FIFECHAN_SDL_SDLGRAPHICS_HPP_

#include <SDL2/SDL.h>

#include "fifechan/color.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    class Image;
    class Rectangle;

    /**
     * SDL implementation of the Graphics.
     */
    class FIFEGUI_EXT_API SDLGraphics : public Graphics
    {
    public:
        // Needed so that drawImage(fcn::Image *, int, int) is visible.
        using Graphics::drawImage;

        /**
         * Constructor.
         */
        SDLGraphics();

        /**
         * Sets the target SDL_Surface to draw to. The target can be any
         * SDL_Surface. This function also pushes a clip areas corresponding to
         * the dimension of the target.
         *
         * @param target the target to draw to.
         */
        virtual void setTarget(SDL_Surface* target);

        /**
         * Gets the target SDL_Surface.
         *
         * @return the target SDL_Surface.
         */
        virtual SDL_Surface* getTarget() const;

        /**
         * Draws an SDL_Surface on the target surface.
         * Normally, you'll use drawImage, but if you want to write
         * SDL specific code this function might come in handy.
         *
         * NOTE: The clip areas will be taken into account.
         */
        virtual void drawSDLSurface(SDL_Surface* surface, SDL_Rect source, SDL_Rect destination);

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

    protected:
        /**
         * Draws a horizontal line.
         *
         * @param x1 the start coordinate of the line.
         * @param y the y coordinate of the line.
         * @param x2 the end coordinate of the line.
         */
        virtual void drawHLine(int x1, int y, int x2);

        /**
         * Draws a vertical line.
         *
         * @param x the x coordinate of the line.
         * @param y1 the start coordinate of the line.
         * @param y2 the end coordinate of the line.
         */
        virtual void drawVLine(int x, int y1, int y2);

        SDL_Surface* mTarget;
        Color mColor;
        bool mAlpha;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_SDL_SDLGRAPHICS_HPP_
