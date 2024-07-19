// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_ALLEGROFONT_HPP
#define FCN_ALLEGROFONT_HPP

#include <allegro.h>

#include "fifechan/font.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * Allegro font implementation of Font that uses the native
     * Allegro font.
     */
    class FIFEGUI_EXT_API AllegroFont : public Font
    {
    public:
        /**
         * Constructor.
         *
         * @param font An Allegro font.
         */
        AllegroFont(FONT* font);

        /**
         * Constructor. The font loaded with this constructor will be freed in the destructor.
         *
         * @param filename The filename of the Allegro font to load.
         */
        AllegroFont(std::string const & filename);

        /**
         * Destructor.
         */
        ~AllegroFont();

        // Inherited from Font

        virtual int getWidth(std::string const & text) const;

        virtual int getHeight() const;

        virtual void drawString(fcn::Graphics* graphics, std::string const & text, int x, int y);

    protected:
        bool mAutoFree;
        FONT* mAllegroFont;
    };
} // namespace fcn

#endif // FCN_ALLEGROFONT_HPP
