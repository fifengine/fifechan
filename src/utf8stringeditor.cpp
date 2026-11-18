// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include <fifechan/utf8stringeditor.hpp>

// Standard library includes
#include <cassert>
#include <string>
#include <utility>

// Third-party library includes
#include <utf8cpp/utf8.h>

namespace fcn
{

    int UTF8StringEditor::nextChar(std::string const & text, int byteOffset)
    {
        assert("byteOffset is non-negative" && byteOffset >= 0);
        assert("byteOffset is within text" && std::cmp_less(byteOffset, text.size()));
        assert("text is valid utf8" && utf8::is_valid(text.begin(), text.end()));

        std::string::const_iterator c;
        std::string::const_iterator e;

        c = text.begin() + byteOffset;
        e = text.end();

        utf8::next(c, e);
        return static_cast<int>(std::string(text.begin(), c).size());
    }

    int UTF8StringEditor::prevChar(std::string const & text, int byteOffset)
    {
        assert("byteOffset is positive" && byteOffset > 0);
        assert("byteOffset is within text" && std::cmp_less_equal(byteOffset, text.size()));
        assert("text is valid utf8" && utf8::is_valid(text.begin(), text.end()));

        std::string::const_iterator c;
        std::string::const_iterator b;

        c = text.begin() + static_cast<std::ptrdiff_t>(byteOffset);
        b = text.begin();

        utf8::prior(c, b);
        return static_cast<int>(std::string(b, c).size());
    }

    int UTF8StringEditor::eraseChar(std::string& text, int byteOffset)
    {
        assert("byteOffset is within text" && byteOffset >= 0 && std::cmp_less(byteOffset, text.size()));
        assert("text is valid utf8" && utf8::is_valid(text.begin(), text.end()));

        std::string::iterator begin;
        std::string::iterator cur;
        begin = text.begin() + static_cast<std::ptrdiff_t>(byteOffset);
        cur   = begin;
        utf8::next(cur, text.end());

        text = std::string(text.begin(), begin) + std::string(cur, text.end());

        return byteOffset;
    }

    int UTF8StringEditor::insertChar(std::string& text, int byteOffset, int ch)
    {
        assert("byteOffset is non-negative" && byteOffset >= 0);
        assert("byteOffset is within text" && std::cmp_less_equal(byteOffset, text.size()));
        assert("character is valid unicode code point" && ch >= 0 && ch <= 0x10FFFF && (ch < 0xD800 || ch > 0xDFFF));

        std::string newText;
        std::string::iterator cut;

        newText = text.substr(0, static_cast<std::size_t>(byteOffset)) + "        ";
        utf8::append(static_cast<utf8::utfchar32_t>(ch), newText.begin() + static_cast<std::ptrdiff_t>(byteOffset));
        cut = newText.begin() + static_cast<std::ptrdiff_t>(byteOffset);
        utf8::next(cut, newText.end());
        newText.erase(cut, newText.end());
        int const newOffset = static_cast<int>(newText.size());

        text = newText + text.substr(static_cast<std::size_t>(byteOffset));

        assert("result is valid utf8" && utf8::is_valid(text.begin(), text.end()));

        return newOffset;
    }

    int UTF8StringEditor::countChars(std::string const & text, int byteOffset)
    {
        assert("byteOffset is non-negative" && byteOffset >= 0);
        assert("byteOffset is within text bounds" && std::cmp_less_equal(byteOffset, text.size()));
        assert("text is valid utf8" && utf8::is_valid(text.begin(), text.end()));

        return static_cast<int>(utf8::distance(text.begin(), text.begin() + byteOffset));
    }

    int UTF8StringEditor::getOffset(std::string const & text, int charIndex)
    {
        assert("charIndex is non-negative" && charIndex >= 0);
        assert("text is valid utf8" && utf8::is_valid(text.begin(), text.end()));

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

        return static_cast<int>(std::string(text.begin(), cur).size());
    }
}; // namespace fcn
