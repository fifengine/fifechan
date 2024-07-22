// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/text.hpp"

#include <string>

#include "fifechan/exception.hpp"
#include "fifechan/font.hpp"
#include "fifechan/rectangle.hpp"

namespace fcn
{
    Text::Text() : mCaretPosition(0), mCaretRow(0), mCaretColumn(0) { }

    Text::Text(std::string const & content) : mCaretPosition(0), mCaretRow(0), mCaretColumn(0)
    {
        std::string::size_type lastPos = 0;
        std::string::size_type pos     = content.find("\n", lastPos);

        for (; pos != std::string::npos; pos = content.find("\n", lastPos)) {
            int const length      = pos - lastPos;
            std::string const sub = content.substr(lastPos, length);
            mRows.push_back(sub);
            lastPos = pos + 1;
        }

        // Add the last part after the last newline or the entire string if no newline is found
        std::string const sub = content.substr(lastPos);
        if (!sub.empty()) {
            mRows.push_back(sub);
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

        for (; (pos = content.find("\n", lastPos)) != std::string::npos; lastPos = pos + 1) {
            int const length      = pos - lastPos;
            std::string const sub = content.substr(lastPos, length);
            mRows.push_back(sub);
        }

        // Add the last part after the last newline or the entire string if no newline is found
        std::string const sub = content.substr(lastPos);
        if (!sub.empty()) {
            mRows.push_back(sub);
        }
    }

    std::string Text::getContent() const
    {
        if (mRows.empty()) {
            return {};
        }

        std::string result;
        int i = 0;
        for (i = 0; i < mRows.size() - 1; ++i) {
            result = result + mRows[i] + "\n";
        }

        result = result + mRows[i];

        return result;
    }

    void Text::setRow(unsigned int row, std::string const & content)
    {
        if (row >= mRows.size()) {
            fcn::throwException("Row out of bounds!", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        mRows[row] = content;
    }

    void Text::addRow(std::string const & row)
    {
        unsigned int i = 0;
        for (i = 0; i < row.size(); i++) {
            if (row[i] == '\n') {
                fcn::throwException(
                    "Line feed not allowed in the row to be added!",
                    static_cast<char const *>(__FUNCTION__),
                    __FILE__,
                    __LINE__);
            }
        }

        mRows.push_back(row);
    }

    void Text::insertRow(std::string const & row, unsigned int position)
    {
        unsigned int const totalRows = mRows.size();

        if (position >= totalRows) {
            if (position == totalRows) {
                addRow(row);
                return;
            }
            fcn::throwException("Position out of bounds!", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        unsigned int i = 0;
        for (i = 0; i < row.size(); i++) {
            if (row[i] == '\n') {
                fcn::throwException(
                    "Line feed not allowed in the row to be inserted!",
                    static_cast<char const *>(__FUNCTION__),
                    __FILE__,
                    __LINE__);
            }
        }

        mRows.insert(mRows.begin() + position, row);
    }

    void Text::eraseRow(unsigned int row)
    {
        if (row >= mRows.size()) {
            fcn::throwException(
                "Row to be erased out of bounds!", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        mRows.erase(mRows.begin() + row);
    }

    std::string& Text::getRow(unsigned int row)
    {
        if (row >= mRows.size()) {
            fcn::throwException("Row out of bounds!", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        return mRows[row];
    }

    void Text::insert(int character)
    {
        char const c = (char)character;

        if (mRows.empty()) {
            if (c == '\n') {
                mRows.emplace_back("");
            } else {
                mRows.emplace_back(1, c);
            }
        } else {
            if (c == '\n') {
                mRows.insert(
                    mRows.begin() + mCaretRow + 1,
                    mRows[mCaretRow].substr(mCaretColumn, mRows[mCaretRow].size() - mCaretColumn));
                mRows[mCaretRow].resize(mCaretColumn);
            } else {
                mRows[mCaretRow].insert(mCaretColumn, std::string(1, c));
            }
        }

        setCaretPosition(getCaretPosition() + 1);
    }

    void Text::remove(int numberOfCharacters)
    {
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
                    mRows[mCaretRow - 1] += mRows[mCaretRow];
                    mRows.erase(mRows.begin() + mCaretRow);
                    setCaretRow(mCaretRow - 1);
                    setCaretColumn(getNumberOfCharacters(mCaretRow));
                } else {
                    mRows[mCaretRow].erase(mCaretColumn - 1, 1);
                    setCaretPosition(mCaretPosition - 1);
                }

                numberOfCharacters++;
            }
        }
        // We should remove characters right of the caret position.
        else if (numberOfCharacters > 0) {
            while (numberOfCharacters != 0) {
                // If all rows have been removed there is nothing
                // more to do.
                if (mRows.empty()) {
                    break;
                }

                // If we are at the end of row and the row
                // is not the last row we need to merge two
                // rows.
                if (mCaretColumn == mRows[mCaretRow].size() && mCaretRow < (mRows.size() - 1)) {
                    mRows[mCaretRow] += mRows[mCaretRow + 1];
                    mRows.erase(mRows.begin() + mCaretRow + 1);
                } else {
                    mRows[mCaretRow].erase(mCaretColumn, 1);
                }

                numberOfCharacters--;
            }
        }
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

        // Loop through all rows until we find the
        // position of the caret.
        unsigned int i     = 0;
        unsigned int total = 0;
        for (i = 0; i < mRows.size(); i++) {
            if (position <= (int)(total + mRows[i].size())) {
                mCaretRow      = i;
                mCaretColumn   = position - total;
                mCaretPosition = position;
                return;
            }

            // Add one for the line feed.
            total += mRows[i].size() + 1;
        }

        // The position is beyond the content.

        // Remove one as the last line doesn't have a line feed.
        mCaretPosition = total - 1;
        mCaretRow      = mRows.size() - 1;
        mCaretColumn   = mRows[mCaretRow].size();
    }

    void Text::setCaretPosition(int x, int y, Font* font)
    {
        if (mRows.empty()) {
            return;
        }

        setCaretRow(y / font->getHeight());
        setCaretColumn(font->getStringIndexAt(mRows[mCaretRow], x));
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
        } else if (column > (int)mRows[mCaretRow].size()) {
            mCaretColumn = mRows[mCaretRow].size();
        } else {
            mCaretColumn = column;
        }

        calculateCaretPositionFromRowAndColumn();
    }

    void Text::setCaretRow(int row)
    {
        if (mRows.empty() || row < 0) {
            mCaretRow = 0;
        } else if (row >= (int)mRows.size()) {
            mCaretRow = mRows.size() - 1;
        } else {
            mCaretRow = row;
        }

        setCaretColumn(mCaretColumn);
    }

    int Text::getCaretX(Font* font) const
    {
        if (mRows.empty()) {
            return 0;
        }

        return font->getWidth(mRows[mCaretRow].substr(0, mCaretColumn));
    }

    int Text::getCaretY(Font* font) const
    {
        return mCaretRow * font->getHeight();
        ;
    }

    Rectangle Text::getDimension(Font* font) const
    {
        if (mRows.empty()) {
            return {0, 0, font->getWidth(" "), font->getHeight()};
        }

        int width = 0;
        for (auto const & mRow : mRows) {
            int const w = font->getWidth(mRow);
            if (width < w) {
                width = w;
            }
        }

        auto h = static_cast<int>(font->getHeight() * mRows.size());
        auto w = width + font->getWidth(" ");

        return {0, 0, w, h};
    }

    Rectangle Text::getCaretDimension(Font* font) const
    {
        Rectangle dim;
        dim.x     = !mRows.empty() ? font->getWidth(mRows[mCaretRow].substr(0, mCaretColumn)) : 0;
        dim.y     = font->getHeight() * mCaretRow;
        dim.width = font->getWidth(" ");
        // We add two for some extra spacing to be sure the whole caret is visible.
        dim.height = font->getHeight() + 2;
        return dim;
    }

    int Text::getWidth(int /*row*/, Font* /*font*/) const
    {
        return 0;
    }

    unsigned int Text::getMaximumCaretRow() const
    {
        return 0;
    }

    unsigned int Text::getMaximumCaretRow(unsigned int /*row*/) const
    {
        return 0;
    }

    unsigned int Text::getNumberOfCharacters() const
    {
        unsigned int result = 0;
        for (auto const & mRow : mRows) {
            result += mRow.size() + 1;
        }

        return result;
    }

    unsigned int Text::getNumberOfRows() const
    {
        return mRows.size();
    }

    unsigned int Text::getNumberOfCharacters(unsigned int row) const
    {
        if (row >= mRows.size()) {
            return 0;
        }

        return mRows[row].size();
    }

    void Text::calculateCaretPositionFromRowAndColumn()
    {
        unsigned int total = 0;
        for (auto i = 0; i < mCaretRow; i++) {
            // Add one for the line feed.
            total += mRows[i].size() + 1;
        }

        mCaretPosition = total + mCaretColumn;
    }
} // namespace fcn
