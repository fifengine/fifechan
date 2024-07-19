// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_OPENLAYERFONT_HPP
#define FCN_OPENLAYERFONT_HPP

#include <OpenLayer.hpp>
#if defined(DELETE)
#undef DELETE
#endif
#include "fifechan/font.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * OpenLayer font implementation of Font that uses the native
     * OpenLayer font.
     */
    class FIFEGUI_EXT_API OpenLayerTTFont : public Font
    {
    public:
        /**
         * Constructor. The font loaded with this constructor will be freed in the destructor.
         *
         * @param filename The filename of the font to load.
         * @param size The size of the font.
         */
        OpenLayerTTFont(std::string const & filename, int size);

        /**
         * Constructor. The font loaded with this constructor will be freed in the destructor.
         *
         * @param filename The filename of the font to load.
         * @param width The width of the font.
         * @param height The height of the font.
         */
        OpenLayerTTFont(std::string const & filename, int width, int height);

        /**
         * Destructor.
         */
        ~OpenLayerTTFont();

        // Inherited from Font

        virtual int getWidth(std::string const & text) const;

        virtual int getHeight() const;

        virtual void drawString(Graphics* graphics, std::string const & text, int x, int y);

    protected:
        ol::TextRenderer mTextRenderer;
    };
} // namespace fcn

#endif // FCN_OPENLAYERFONT_HPP
