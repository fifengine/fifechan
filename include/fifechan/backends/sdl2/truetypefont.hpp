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

namespace fcn::sdl2
{

    /**
     * SDL2/FreeType implementation for rendering TrueType fonts.
     *
     *  It uses the SDL_ttf library to display True Type Fonts with SDL.
     *
     * Note: initialize the SDL_ttf library (TTF_Init) before using this
     * class and call TTF_Quit when finished.
     *
     * @ingroup fonts
     */
    class FIFEGUI_EXT_API TrueTypeFont : public Font
    {
    public:
        /**
         * Constructor.
         *
         * @param filename the filename of the True Type Font.
         * @param size the size the font should be in.
         */
        TrueTypeFont(std::string const & filename, int size);

        ~TrueTypeFont() override;

        TrueTypeFont(TrueTypeFont const &)            = delete;
        TrueTypeFont& operator=(TrueTypeFont const &) = delete;
        TrueTypeFont(TrueTypeFont&&)                  = delete;
        TrueTypeFont& operator=(TrueTypeFont&&)       = delete;

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
         * Enable or disable anti-aliasing for rendered glyphs.
         *
         * @param antiAlias True to enable anti-aliasing, false to disable.
         */
        virtual void setAntiAlias(bool antiAlias);

        /**
         * Checks if anti aliasing is used.
         *
         * @return true if anti aliasing is used.
         */
        virtual bool isAntiAlias();

        // Inherited from Font

        int getWidth(std::string const & text) const override;

        int getHeight() const override;

        void drawString(fcn::Graphics* graphics, std::string const & text, int x, int y) override;

    protected:
        /** Underlying TTF_Font pointer from SDL_ttf. */
        TTF_Font* mFont;

        /** Cached font height in pixels. */
        int mHeight{};

        /** Additional spacing between glyphs in pixels. */
        int mGlyphSpacing;

        /** Additional spacing between rows in pixels. */
        int mRowSpacing;

        /** Filename of the font used to create mFont. */
        std::string mFilename;

        /** Whether anti-aliasing is enabled for rendering. */
        bool mAntiAlias;
    };
} // namespace fcn::sdl2

#endif // INCLUDE_FIFECHAN_BACKENDS_SDL_SDLTRUETYPEFONT_HPP_
