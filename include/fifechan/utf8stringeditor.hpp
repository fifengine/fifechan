// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_UTF8STRINGEDITOR_HPP_
#define INCLUDE_FIFECHAN_UTF8STRINGEDITOR_HPP_

// Standard C++ library includes
#include <string>

namespace fcn
{

    /**
     * Utility for editing and handling UTF-8 encoded strings.
     *
     * This is a helper class which allows to use UTF-8 strings in
     * your application.
     *
     * @ingroup utils
     */
    class UTF8StringEditor
    {
    public:
        /**
         * Returns byte offset of the next character.
         *
         * @param text UTF-8 text to navigate.
         * @param byteOffset Byte offset of current character.
         * @return Byte offset of the next character.
         */
        static int nextChar(std::string const & text, int byteOffset);

        /**
         * Returns byte offset of the previous character.
         *
         * @param text UTF-8 text to navigate.
         * @param byteOffset Byte offset of current character.
         * @return Byte offset of the previous character.
         */
        static int prevChar(std::string const & text, int byteOffset);

        /**
         * Erase character at specified byte offset.
         *
         * @param text UTF-8 text to modify.
         * @param byteOffset Byte offset of the character to erase.
         * @return New byte offset (is equal to byteOffset).
         */
        static int eraseChar(std::string& text, int byteOffset);

        /**
         * Insert a character at specified byte offset.
         *
         * @param text UTF-8 text to modify.
         * @param byteOffset Byte offset where character will be inserted.
         * @param ch Unicode character to insert.
         * @return New byte offset (after the new character).
         */
        static int insertChar(std::string& text, int byteOffset, int ch);

        /**
         * Counts characters up to byteOffset.
         *
         * @param text UTF-8 text to navigate.
         * @param byteOffset Byte offset inside the text.
         * @return Number of characters.
         */
        static int countChars(std::string const & text, int byteOffset);

        /**
         * Gets byte offset for character index.
         *
         * This method automatically clips charIndex to be inside
         * the string + EOF
         *
         * @param text UTF-8 text to navigate.
         * @param charIndex Character index to move to.
         * @return Byte offset of character at charIndex.
         */
        static int getOffset(std::string const & text, int charIndex);
    };

}; // namespace fcn

#endif // INCLUDE_FIFECHAN_UTF8STRINGEDITOR_HPP_
