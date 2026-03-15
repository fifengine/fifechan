// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include <fifechan/utf8stringeditor.hpp>
#include <fifechan/util/utf8/utf8.hpp>

#include <string>

namespace fcn
{

    int UTF8StringEditor::nextChar(std::string const & text, int byteOffset)
    {
        std::string::const_iterator c;
        std::string::const_iterator e;

        c = text.begin() + byteOffset;
        e = text.end();

        utf8::next(c, e);
        return std::string(text.begin(), c).size();
    }

    int UTF8StringEditor::prevChar(std::string const & text, int byteOffset)
    {
        std::string::const_iterator c;
        std::string::const_iterator b;

        c = text.begin() + byteOffset;
        b = text.begin();

        utf8::prior(c, b);
        return std::string(b, c).size();
    }

    int UTF8StringEditor::eraseChar(std::string& text, int byteOffset)
    {
        std::string::iterator begin;
        std::string::iterator cur;
        begin = text.begin() + byteOffset;
        cur   = begin;
        utf8::next(cur, text.end());

        text = std::string(text.begin(), begin) + std::string(cur, text.end());
        return byteOffset; // this shouldn't change!
    }

    int UTF8StringEditor::insertChar(std::string& text, int byteOffset, int ch)
    {
        std::string newText;
        std::string::iterator cut;
        int newOffset = 0;

        // make a temp string from left part of the caret (+6 extra chars)
        newText = text.substr(0, byteOffset) + "        ";
        // append character
        utf8::append(ch, newText.begin() + byteOffset);
        // calculate newText real length
        cut = newText.begin() + byteOffset;
        utf8::next(cut, newText.end());
        // cut the string to real length
        newText   = std::string(newText.begin(), cut);
        newOffset = newText.size();
        // make new text
        text = newText + text.substr(byteOffset);

        return newOffset;
    }

    int UTF8StringEditor::countChars(std::string const & text, int byteOffset)
    {
        return utf8::distance(text.begin(), text.begin() + byteOffset);
    }

    int UTF8StringEditor::getOffset(std::string const & text, int charIndex)
    {
        std::string::const_iterator cur;
        std::string::const_iterator end;
        int i = 0;

        if (charIndex < 0) {
            return 0;
        }

        cur = text.begin();
        end = text.end();

        for (i = 0; i < charIndex && cur != end; i++) {
            utf8::next(cur, end);
        }

        return std::string(text.begin(), cur).size();
    }
}; // namespace fcn
