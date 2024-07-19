// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_CONTRIB_OGLFTFONT_HPP
#define FCN_CONTRIB_OGLFTFONT_HPP

#include <string>

#include <OGLFT.h>

#include "fifechan/font.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    namespace contrib
    {
        /**
         * OpenGL True Type Font implementation of Font. It uses the OGLFT library
         * to display True Type Fonts with OpenGL.
         */
        class FCN_EXTENSION_DECLSPEC OGLFTFont : public Font
        {
        public:
            /**
             * Constructor.
             *
             * @param filename the filename of the True Type Font.
             * @param size the size the font should be in.
             */
            OGLFTFont(std::string const & filename, int size);

            /**
             * Destructor.
             */
            virtual ~OGLFTFont();

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

            // Inherited from Font

            virtual int getWidth(std::string const & text) const;

            virtual int getHeight() const;

            virtual void drawString(Graphics* graphics, std::string const & text, int x, int y);

        protected:
            OGLFT::TranslucentTexture* mFont;
            std::string mFilename;
            int mSize;
            int mRowSpacing;
        };
    } // namespace contrib
} // namespace fcn

#endif
