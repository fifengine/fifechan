// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_BACKENDS_SDL_SDLTRUETYPEFONT_HPP_
#define INCLUDE_FIFECHAN_BACKENDS_SDL_SDLTRUETYPEFONT_HPP_

#include <SDL_ttf.h>

#include <map>
#include <string>

#include "fifechan/font.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    class Graphics;

    /**
     * SDL True Type Font implementation of Font. It uses the SDL_ttf library
     * to display True Type Fonts with SDL.
     *
     * NOTE: You must initialize the SDL_ttf library before using this
     *       class. Also, remember to call the SDL_ttf libraries quit
     *       function.
     */
    class FIFEGUI_EXT_API SDLTrueTypeFont : public Font
    {
    public:
        /**
         * Constructor.
         *
         * @param filename the filename of the True Type Font.
         * @param size the size the font should be in.
         */
        SDLTrueTypeFont(std::string const & filename, int size);

        virtual ~SDLTrueTypeFont();

        /**
         * Sets the spacing between rows in pixels. Default is 0 pixels.
         * The spacing can be negative.
         *
         * @param spacing the spacing in pixels.
         */
        virtual void setRowSpacing(int spacing);

        /**
         * Gets the spacing between rows in pixels.
         *
         * @return the spacing.
         */
        virtual int getRowSpacing();

        /**
         * Sets the spacing between letters in pixels. Default is 0 pixels.
         * The spacing can be negative.
         *
         * @param spacing the spacing in pixels.
         */
        virtual void setGlyphSpacing(int spacing);

        /**
         * Gets the spacing between letters in pixels.
         *
         * @return the spacing.
         */
        virtual int getGlyphSpacing();

        /**
         * Sets the use of anti aliasing..
         *
         * @param antiAlias true for use of anti aliasing.
         */
        virtual void setAntiAlias(bool antiAlias);

        /**
         * Checks if anti aliasing is used.
         *
         * @return true if anti aliasing is used.
         */
        virtual bool isAntiAlias();

        // Inherited from Font

        virtual int getWidth(std::string const & text) const;

        virtual int getHeight() const;

        virtual void drawString(Graphics* graphics, std::string const & text, int x, int y);

    protected:
        TTF_Font* mFont;

        int mHeight;
        int mGlyphSpacing;
        int mRowSpacing;

        std::string mFilename;
        bool mAntiAlias;
    };
} // namespace fcn

namespace fcn::sdl2
{
    using TrueTypeFont = fcn::SDLTrueTypeFont;
}

#endif // INCLUDE_FIFECHAN_BACKENDS_SDL_SDLTRUETYPEFONT_HPP_
