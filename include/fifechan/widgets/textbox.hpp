// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_TEXTBOX_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_TEXTBOX_HPP_

#include <ctime>
#include <string>
#include <vector>

#include "fifechan/listeners/keylistener.hpp"
#include "fifechan/listeners/mouselistener.hpp"
#include "fifechan/platform.hpp"
#include "fifechan/widget.hpp"

namespace fcn
{
    class Text;
    class UTF8StringEditor;

    /**
     * A multi-line text input field.
     *
     * @ingroup widgets
     */
    class FIFEGUI_API TextBox : public Widget, public MouseListener, public KeyListener
    {
    public:
        /**
         * Constructor.
         *
         * @param text The default text of the text box.
         */
        explicit TextBox(std::string const & text = "");

        TextBox(TextBox const &)            = delete;
        TextBox& operator=(TextBox const &) = delete;
        TextBox(TextBox&&)                  = delete;
        TextBox& operator=(TextBox&&)       = delete;

        ~TextBox() override;

        /**
         * Sets the text of the text box.
         *
         * @param text The text of the text box.
         * @see getText
         */
        void setText(std::string const & text);

        /**
         * Gets the text of the text box.
         *
         * @return The text of the text box.
         * @see setText
         */
        std::string getText() const;

        /**
         * Gets a certain row from the text.
         *
         * @param row The number of the row to get from the text.
         * @return A row from the text of the text box.
         * @see setTextRow
         */
        std::string getTextRow(int row) const;

        /**
         * Sets the text of a certain row of the text.
         *
         * @param row The number of the row to set in the text.
         * @param text The text to set in the given row number.
         * @see getTextRow
         */
        void setTextRow(int row, std::string const & text);

        /**
         * Gets the number of rows in the text.
         *
         * @return The number of rows in the text.
         */
        unsigned int getNumberOfRows() const;

        /**
         * Gets the caret position in the text.
         *
         * @return The caret position in the text.
         * @see setCaretPosition
         */
        unsigned int getCaretPosition() const;

        /**
         * Sets the position of the caret in the text.
         *
         * @param position the position of the caret.
         * @see getCaretPosition
         */
        void setCaretPosition(unsigned int position);

        /**
         * Gets the row number where the caret is currently located.
         *
         * @return The row number where the caret is currently located.
         * @see setCaretRow
         */
        unsigned int getCaretRow() const;

        /**
         * Sets the row where the caret should be currently located.
         *
         * @param row The row where the caret should be currently located.
         * @see getCaretRow
         */
        void setCaretRow(int row);

        /**
         * Gets the column where the caret is currently located.
         *
         * @return The column where the caret is currently located.
         * @see setCaretColumn
         */
        unsigned int getCaretColumn() const;

        /**
         * Sets the column where the caret should be currently located.
         *
         * @param column The column where the caret should be currently located.
         * @see getCaretColumn
         */
        void setCaretColumn(int column);

        /**
         * Sets the row and the column where the caret should be currently
         * located.
         *
         * @param row The row where the caret should be currently located.
         * @param column The column where the caret should be currently located.
         * @see getCaretRow, getCaretColumn
         */
        void setCaretRowColumn(int row, int column);

        /**
         * Scrolls the text to the caret if the text box is in a scroll area.
         *
         * @see ScrollArea
         */
        virtual void scrollToCaret();

        /**
         * Checks if the text box is editable.
         *
         * @return True it the text box is editable, false otherwise.
         * @see setEditable
         */
        bool isEditable() const;

        /**
         * Sets the text box to be editable or not.
         *
         * @param editable True if the text box should be editable, false otherwise.
         */
        void setEditable(bool editable);

        /**
         * Adds a row of text to the end of the text.
         *
         * @param row The row to add.
         */
        virtual void addRow(std::string const & row);

        /**
         * Checks if the text box is opaque. An opaque text box will draw
         * it's background and it's text. A non opaque text box only draw it's
         * text making it transparent.
         *
         * @return True if the text box is opaque, false otherwise.
         * @see setOpaque
         */
        bool isOpaque() const;

        /**
         * Sets the text box to be opaque or not. An opaque text box will draw
         * it's background and it's text. A non opaque text box only draw it's
         * text making it transparent.
         *
         * @param opaque True if the text box should be opaque, false otherwise.
         * @see isOpaque
         */
        void setOpaque(bool opaque);

        // Inherited from Widget

        void draw(Graphics* graphics) override;

        void fontChanged() override;

        using Widget::resizeToContent;

        void resizeToContent(bool recursion) override;
        /**
         * Adjusts the text box's size to fit the text.
         */
        void adjustSize() override;

        // Inherited from KeyListener

        void keyPressed(KeyEvent& keyEvent) override;

        // Inherited from MouseListener

        void mousePressed(MouseEvent& mouseEvent) override;

        void mouseDragged(MouseEvent& mouseEvent) override;

    protected:
        /**
         * Adjusts the size of the button to fit the caption.
         *
         * The public `adjustSize()` method serves as a virtual entry point
         * for polymorphism, while this `adjustSizeImpl()` method contains
         * the concrete implementation of the resizing logic.
         *
         * @see adjustSize (virtual entry point for polymorphism)
         */
        void adjustSizeImpl();

        /**
         * Sets caret column (UTF-8 aware).
         * @param column Caret column.
         */
        void setCaretColumnUTF8(int column);

        /**
         * Sets caret row (UTF-8 aware).
         * @param row Caret row.
         */
        void setCaretRowUTF8(int row);

        /**
         * Sets the caret row and column (UTF-8 aware).
         *
         * @param row Caret row (UTF-8 aware index).
         * @param column Caret column (UTF-8 aware index).
         */
        void setCaretRowColumnUTF8(int row, int column);

        /**
         * Draws the caret. Overloaded this method if you want to
         * change the style of the caret.
         *
         * @param graphics a Graphics object to draw with.
         * @param x the x position.
         * @param y the y position.
         */
        virtual void drawCaret(Graphics* graphics, int x, int y);

        /**
         * Holds the text of the text box.
         */
        Text* mText;

        /**
         * True if the text box is editable, false otherwise.
         */
        bool mEditable{true};

        /**
         * True if the text box is opaque, false otherwise.
         */
        bool mOpaque{true};

        /**
         * UTF8StringEditor for UTF8 support.
         */
        UTF8StringEditor* mStringEditor;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_TEXTBOX_HPP_
