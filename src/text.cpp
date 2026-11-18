// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/text.hpp"

// Standard library includes
#include <algorithm>
#include <cassert>
#include <numeric>
#include <string>
#include <utility>

// Third-party library includes
#include <utf8cpp/utf8.h>

// Project headers
#include "fifechan/exception.hpp"
#include "fifechan/font.hpp"
#include "fifechan/rectangle.hpp"

namespace fcn
{
    Text::Text()
    {
        // Ensure at least one empty row exists for default constructed Text
        mRows.emplace_back();
    }

    Text::Text(std::string const & content)
    {
        std::string::size_type lastPos = 0;
        std::string::size_type pos     = content.find('\n', lastPos);

        for (; pos != std::string::npos; pos = content.find('\n', lastPos)) {
            std::string::size_type const length = pos - lastPos;
            std::string const sub               = content.substr(lastPos, length);
            mRows.push_back(sub);
            lastPos = pos + 1;
        }

        // Add the last part after the last newline or the entire string if no newline is found
        std::string const sub = content.substr(lastPos);
        if (!sub.empty()) {
            mRows.push_back(sub);
        }

        // Ensure at least one row exists
        if (mRows.empty()) {
            mRows.emplace_back();
        }
    }

    Text::~Text() = default;

    void Text::setContent(std::string const & content)
    {
        // Reset caret
        mCaretPosition = 0;
        mCaretRow      = 0;
        mCaretColumn   = 0;

        mRows.clear();
        std::string::size_type lastPos = 0;
        std::string::size_type pos     = 0;

        for (; (pos = content.find('\n', lastPos)) != std::string::npos; lastPos = pos + 1) {
            std::string::size_type const length = pos - lastPos;
            std::string const sub               = content.substr(lastPos, length);
            mRows.push_back(sub);
        }

        // Add the last part after the last newline or the entire string if no newline is found
        std::string const sub = content.substr(lastPos);
        if (!sub.empty()) {
            mRows.push_back(sub);
        }

        // Ensure at least one row exists
        if (mRows.empty()) {
            mRows.emplace_back();
        }
    }

    std::string Text::getContent() const
    {
        if (mRows.empty()) {
            return {};
        }

        std::string result;
        for (size_t i = 0; i < mRows.size() - 1; ++i) {
            result.append(mRows.at(i)).append("\n");
        }

        result.append(mRows.back());

        return result;
    }

    void Text::setRow(unsigned int row, std::string const & content)
    {
        if (row >= mRows.size()) {
            throwException("Row out of bounds!");
        }
        assert("content is valid utf8" && utf8::is_valid(content.begin(), content.end()));

        mRows.at(row) = content;
    }

    void Text::addRow(std::string const & row)
    {
        for (char const i : row) {
            if (i == '\n') {
                throwException("Line feed not allowed in the row to be added!");
            }
        }
        assert("row is valid utf8" && utf8::is_valid(row.begin(), row.end()));

        mRows.push_back(row);
    }

    void Text::insertRow(std::string const & row, int position)
    {
        int const totalRows = static_cast<int>(mRows.size());

        if (position >= totalRows) {
            if (position == totalRows) {
                addRow(row);
                return;
            }
            throwException("Position out of bounds!");
        }

        for (char const i : row) {
            if (i == '\n') {
                throwException("Line feed not allowed in the row to be inserted!");
            }
        }
        assert("row is valid utf8" && utf8::is_valid(row.begin(), row.end()));

        mRows.insert(mRows.begin() + position, row);
    }

    void Text::eraseRow(int row)
    {
        if (row < 0 || static_cast<std::size_t>(row) >= mRows.size()) {
            throwException("Row to be erased out of bounds!");
        }

        mRows.erase(mRows.begin() + row);
    }

    std::string& Text::getRow(int row)
    {
        if (row < 0 || static_cast<std::size_t>(row) >= mRows.size()) {
            throwException("Row out of bounds!");
        }

        return mRows.at(static_cast<std::size_t>(row));
    }

    void Text::insert(int character)
    {
        assert("character is valid unicode" && character >= 0 && character <= 0x10FFFF);
        assert("caret row is within bounds" && static_cast<std::size_t>(mCaretRow) < mRows.size());

        char const c = static_cast<char>(character);

        if (mRows.empty()) {
            if (c == '\n') {
                mRows.emplace_back("");
            } else {
                mRows.emplace_back(1, c);
            }
        } else {
            if (c == '\n') {
                std::string const tail = mRows.at(static_cast<std::size_t>(mCaretRow))
                                             .substr(
                                                 static_cast<std::size_t>(mCaretColumn),
                                                 mRows.at(static_cast<std::size_t>(mCaretRow)).size() -
                                                     static_cast<std::size_t>(mCaretColumn));
                mRows.at(static_cast<std::size_t>(mCaretRow)).resize(static_cast<std::size_t>(mCaretColumn));
                mRows.insert(
                    mRows.begin() +
                        static_cast<std::vector<std::string>::difference_type>(static_cast<std::size_t>(mCaretRow) + 1),
                    tail);
            } else {
                mRows.at(static_cast<std::size_t>(mCaretRow))
                    .insert(static_cast<std::size_t>(mCaretColumn), std::string(1, c));
            }
        }

        setCaretPosition(getCaretPosition() + 1);
    }

    void Text::remove(int numberOfCharacters)
    {
        // cppcheck-suppress-begin knownConditionTrueFalse
        // False positive: numberOfCharacters and branch logic is correct.

        if (mRows.empty() || numberOfCharacters == 0) {
            return;
        }

        // We should remove characters left of the caret position.
        if (numberOfCharacters < 0) {
            while (numberOfCharacters != 0) {
                // If the caret position is zero there is nothing
                // more to do.
                if (mCaretPosition == 0) {
                    break;
                }

                // If we are at the end of the row
                // and the row is not the first row we
                // need to merge two rows.
                if (mCaretColumn == 0 && mCaretRow != 0) {
                    mRows.at(static_cast<std::size_t>(mCaretRow - 1)) += mRows.at(static_cast<std::size_t>(mCaretRow));
                    mRows.erase(
                        mRows.begin() +
                        static_cast<std::vector<std::string>::difference_type>(static_cast<std::size_t>(mCaretRow)));
                    setCaretRow(mCaretRow - 1);
                    setCaretColumn(getNumberOfCharacters(mCaretRow));
                } else {
                    mRows.at(static_cast<std::size_t>(mCaretRow)).erase(static_cast<std::size_t>(mCaretColumn) - 1, 1);
                    setCaretPosition(mCaretPosition - 1);
                }

                numberOfCharacters++;
            }
        } else if (numberOfCharacters > 0) {
            // We should remove characters right of the caret position.
            while (numberOfCharacters != 0) {
                // If all rows have been removed or
                // caret is out of bounds there is nothing more to do.
                if (mRows.empty() || static_cast<std::size_t>(mCaretRow) >= mRows.size()) {
                    break;
                }

                // If we are at the end of row and the row
                // is not the last row we need to merge two rows.
                if (static_cast<std::size_t>(mCaretRow) < mRows.size() &&
                    static_cast<std::size_t>(mCaretColumn) == mRows.at(static_cast<std::size_t>(mCaretRow)).size() &&
                    static_cast<std::size_t>(mCaretRow) < (mRows.size() - 1)) {
                    mRows.at(static_cast<std::size_t>(mCaretRow)) += mRows.at(static_cast<std::size_t>(mCaretRow) + 1);
                    mRows.erase(
                        mRows.begin() + static_cast<std::vector<std::string>::difference_type>(
                                            static_cast<std::size_t>(mCaretRow) + 1));
                } else {
                    mRows.at(static_cast<std::size_t>(mCaretRow)).erase(static_cast<std::size_t>(mCaretColumn), 1);
                }

                numberOfCharacters--;
            }
        }

        // cppcheck-suppress-end knownConditionTrueFalse
    }

    int Text::getCaretPosition() const
    {
        return mCaretPosition;
    }

    void Text::setCaretPosition(int position)
    {
        if (mRows.empty() || position < 0) {
            mCaretPosition = 0;
            mCaretRow      = 0;
            mCaretColumn   = 0;
            return;
        }

        int const pos = position;
        int total     = 0;

        for (std::size_t i = 0; i < mRows.size(); ++i) {
            int const rowLen = static_cast<int>(mRows.at(i).size());

            if (pos < total + rowLen) {
                mCaretRow      = static_cast<int>(i);
                mCaretColumn   = pos - total;
                mCaretPosition = position;
                return;
            }

            if (pos == total + rowLen) {
                // Caret at the boundary between this row and the next row.
                if (i + 1 < mRows.size()) {
                    mCaretRow      = static_cast<int>(i) + 1;
                    mCaretColumn   = 0;
                    mCaretPosition = position;
                    return;
                }

                // At end of content (last row), place caret at end of last row.
                mCaretRow      = static_cast<int>(i);
                mCaretColumn   = rowLen;
                mCaretPosition = position;
                return;
            }

            // Move to the start of the next row (include newline)
            total += rowLen + 1;
        }

        // Position is beyond the content: clamp to end.
        int const contentChars = getNumberOfCharacters();
        if (contentChars == 0) {
            mCaretPosition = 0;
            mCaretRow      = 0;
            mCaretColumn   = 0;
            return;
        }

        mCaretPosition = contentChars - 1;
        mCaretRow      = static_cast<int>(mRows.size()) - 1;
        mCaretColumn   = static_cast<int>(mRows.back().size());
    }

    void Text::setCaretPosition(int x, int y, Font* font)
    {
        assert("font is not null" && font != nullptr);
        assert("font height is positive" && font->getHeight() > 0);

        if (mRows.empty()) {
            return;
        }

        setCaretRow(y / font->getHeight());
        setCaretColumn(font->getStringIndexAt(mRows.at(static_cast<std::size_t>(mCaretRow)), x));
    }

    int Text::getCaretColumn() const
    {
        return mCaretColumn;
    }

    int Text::getCaretRow() const
    {
        return mCaretRow;
    }

    void Text::setCaretColumn(int column)
    {
        if (mRows.empty() || column < 0) {
            mCaretColumn = 0;
        } else if (std::cmp_greater(column, static_cast<int>(mRows.at(static_cast<std::size_t>(mCaretRow)).size()))) {
            mCaretColumn = static_cast<int>(mRows.at(static_cast<std::size_t>(mCaretRow)).size());
        } else {
            mCaretColumn = column;
        }

        calculateCaretPositionFromRowAndColumn();
    }

    void Text::setCaretRow(int row)
    {
        if (mRows.empty() || row < 0) {
            mCaretRow = 0;
        } else if (std::cmp_greater_equal(row, static_cast<int>(mRows.size()))) {
            mCaretRow = static_cast<int>(mRows.size()) - 1;
        } else {
            mCaretRow = row;
        }

        setCaretColumn(mCaretColumn);
    }

    int Text::getCaretX(Font* font) const
    {
        assert("font is not null" && font != nullptr);

        if (mRows.empty()) {
            return 0;
        }

        return font->getWidth(
            mRows.at(static_cast<std::size_t>(mCaretRow)).substr(0, static_cast<std::size_t>(mCaretColumn)));
    }

    int Text::getCaretY(Font* font) const
    {
        assert("font is not null" && font != nullptr);
        assert("font height is positive" && font->getHeight() > 0);

        return mCaretRow * font->getHeight();
    }

    Rectangle Text::getDimension(Font* font) const
    {
        assert("font is not null" && font != nullptr);
        assert("font height is positive" && font->getHeight() > 0);

        if (mRows.empty()) {
            return {0, 0, font->getWidth(" "), font->getHeight()};
        }

        int width = 0;
        for (auto const & mRow : mRows) {
            int const w = font->getWidth(mRow);
            width       = std::max(width, w);
        }

        int const h = font->getHeight() * static_cast<int>(mRows.size());
        int const w = width + font->getWidth(" ");

        return {0, 0, w, h};
    }

    Rectangle Text::getCaretDimension(Font* font) const
    {
        assert("font is not null" && font != nullptr);
        assert("font height is positive" && font->getHeight() > 0);

        Rectangle dim;
        dim.x =
            !mRows.empty() ?
                font->getWidth(
                    mRows.at(static_cast<std::size_t>(mCaretRow)).substr(0, static_cast<std::size_t>(mCaretColumn))) :
                0;
        dim.y      = font->getHeight() * mCaretRow;
        dim.width  = font->getWidth(" ");
        dim.height = font->getHeight() + 2;
        return dim;
    }

    int Text::getWidth(int /*row*/, Font* /*font*/) const
    {
        return 0;
    }

    int Text::getMaximumCaretRow() const
    {
        return 0;
    }

    int Text::getMaximumCaretRow(int /*row*/) const
    {
        return 0;
    }

    int Text::getNumberOfCharacters() const
    {
        return static_cast<int>(
            std::accumulate(mRows.begin(), mRows.end(), size_t{0}, [](size_t sum, auto const & row) {
                return sum + row.size() + 1;
            }));
    }

    int Text::getNumberOfRows() const
    {
        return static_cast<int>(mRows.size());
    }

    int Text::getNumberOfCharacters(int row) const
    {
        if (row < 0 || static_cast<std::size_t>(row) >= mRows.size()) {
            return 0;
        }

        return static_cast<int>(mRows.at(static_cast<std::size_t>(row)).size());
    }

    void Text::calculateCaretPositionFromRowAndColumn()
    {
        int total = 0;
        for (int i = 0; i < mCaretRow; ++i) {
            // Add one for the line feed.
            total += static_cast<int>(mRows.at(static_cast<std::size_t>(i)).size()) + 1;
        }

        mCaretPosition = total + mCaretColumn;
    }
} // namespace fcn
