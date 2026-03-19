// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_TEXTFIELD_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_TEXTFIELD_HPP_

#include <string>

#include "fifechan/keylistener.hpp"
#include "fifechan/mouselistener.hpp"
#include "fifechan/platform.hpp"
#include "fifechan/widget.hpp"

namespace fcn
{
    class Text;
    class UTF8StringEditor;

    /**
     * A single-line text input field.
     *
     * @ingroup widgets
     */
    class FIFEGUI_API TextField : public Widget, public MouseListener, public KeyListener
    {
    public:
        TextField();

        /**
         * Constructor. The text field will be automatically resized
         * to fit the text.
         *
         * @param text The default text of the text field.
         */
        explicit TextField(std::string const & text);

        TextField(TextField const &)            = delete;
        TextField& operator=(TextField const &) = delete;
        TextField(TextField&&)                  = delete;
        TextField& operator=(TextField&&)       = delete;

        ~TextField() override;

        /**
         * Sets the text of the text field.
         *
         * @param text The text of the text field.
         * @see getText
         */
        virtual void setText(std::string const & text);

        /**
         * Gets the text of the text field.
         *
         * @return The text of the text field.
         * @see setText
         */
        virtual std::string getText() const;

        /**
         * Adjusts the height of the text field to fit caption.
         */
        void adjustHeight();

        /**
         * Checks if the text field is editable.
         *
         * @return True it the text field is editable, false otherwise.
         * @see setEditable
         */
        bool isEditable() const;

        /**
         * Sets the text field to be editable or not. A text field is editable
         * by default.
         *
         * @param editable True if the text field should be editable, false
         *                 otherwise.
         */
        void setEditable(bool editable);

        /**
         * Sets the caret position. As there is only one line of text
         * in a text field the position is the caret's x coordinate.
         *
         * @param position The caret position.
         * @see getCaretPosition
         */
        void setCaretPosition(unsigned int position);

        /**
         * Gets the caret position. As there is only one line of text
         * in a text field the position is the caret's x coordinate.
         *
         * @return The caret position.
         * @see setCaretPosition
         */
        unsigned int getCaretPosition() const;

        // Inherited from Widget

        using Widget::resizeToContent;

        void resizeToContent(bool recursion) override;

        /**
         * Adjusts the size of the text field to fit the text.
         */
        void adjustSize() override;

        void draw(Graphics* graphics) override;

        // Inherited from MouseListener

        void mousePressed(MouseEvent& mouseEvent) override;

        void mouseDragged(MouseEvent& mouseEvent) override;

        // Inherited from KeyListener

        void keyPressed(KeyEvent& keyEvent) override;

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
         * Draws the caret. Overloaded this method if you want to
         * change the style of the caret.
         *
         * @param graphics the Graphics object to draw with.
         * @param x the caret's x-position.
         */
        virtual void drawCaret(Graphics* graphics, int x);

        /**
         * Scrolls the text horizontally so that the caret shows if needed.
         * The method is used any time a user types in the text field so the
         * caret always will be shown.
         */
        void fixScroll();

        /**
         * True if the text field is editable, false otherwise.
         */
        bool mEditable{true};

        /**
         * Holds the text of the text field.
         */
        Text* mText;

        /**
         * Holds the amount scrolled in x. If a user types more characters than
         * the text field can display, due to the text field being to small, the
         * text needs to scroll in order to show the last type character.
         */
        int mXScroll{0};

        /**
         * String editor for UTF8 support.
         */
        UTF8StringEditor* mStringEditor;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_TEXTFIELD_HPP_
