// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_CONTRIB_ALLEGROTRUETYPEFONT_HPP
#define FCN_CONTRIB_ALLEGROTRUETYPEFONT_HPP

#include "fifechan/font.hpp"
#include "fifechan/platform.hpp"

struct GLYPH_FACE;
struct GLYPH_REND;
struct GLYPH_KEEP;

namespace fcn
{
    class Graphics;

    namespace contrib
    {
        /**
         * True type font support using Allegro and GlyphKeeper.
         *
         * Note: GlyphKeeper must be compiled for allegro (define
         *       GLYPH_TARGET=GLYPH_TARGET_ALLEGRO) and must be
         *       initialized before using this class.
         *
         * There's only basic functionality implemented here, so
         * you can add more and advanced functionality by deriving
         * from this class.
         */
        class FCN_EXTENSION_DECLSPEC AllegroGlyphKeeperFont : public Font
        {
        public:
            /**
             * Constructor.
             *
             * @param filename the filename of the True Type Font.
             * @param size the size the font should be in.
             */
            AllegroGlyphKeeperFont(std::string const & filename, int size);

            /**
             * Constructor.
             *
             * @param filename the filename of the True Type Font.
             * @param width the width the font should be in.
             * @param height the height the font should be in.
             */
            AllegroGlyphKeeperFont(std::string const & filename, int width, int height);

            /**
             * Destructor.
             */
            ~AllegroGlyphKeeperFont();

            // Inherited from Font

            virtual int getWidth(std::string const & text) const;

            virtual int getHeight() const;

            virtual void drawString(Graphics* graphics, std::string const & text, int x, int y);

        protected:
            void load(std::string const & filename, int w, int h);
            GLYPH_FACE* mFace;
            GLYPH_REND* mRend;
            GLYPH_KEEP* mKeeper;
        };
    } // namespace contrib
} // namespace fcn
#endif
