// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_HGE_HGEIMAGEFONT_HPP_
#define INCLUDE_FIFECHAN_HGE_HGEIMAGEFONT_HPP_

#include <string>

#include "fifechan/font.hpp"
#include "fifechan/platform.hpp"
#include <hgefont.h>

namespace fcn
{
    class Graphics;

    /**
     * Font to be used with HGE.
     * As HGE does not support colorkey blitting of images it's recommended
     * to use HGE's internal font in FifeGUI.
     */
    class FIFEGUI_EXT_API HGEImageFont : public Font
    {
    public:
        /**
         * Constructor. Creates a font from a HGE font file.
         *
         * @param filename the filename of the HGE font.
         */
        explicit HGEImageFont(std::string const & filename);

        ~HGEImageFont();

        /**
         * Gets the loaded HGE font.
         * The HGE font can be used to set certain properties like setting the
         * blend mode. The Properties of the HGE font will affect the font.
         *
         * @return the loaded HGE font.
         */
        virtual hgeFont* getFont() const;

        /**
         * Gets the width from a character.
         *
         * @param character the character to get the width from.
         */
        int getWidth(char character) const;

        // Inherited from Font

        int getWidth(std::string const & text) const;

        int getHeight() const;

        int getStringIndexAt(std::string const & text, int x) const;

        void drawString(Graphics* graphics, std::string const & text, int x, int y);

    protected:
        hgeFont* mHGEFont;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_HGE_HGEIMAGEFONT_HPP_
