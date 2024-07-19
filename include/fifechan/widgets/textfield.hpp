// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_TEXTFIELD_HPP
#define FCN_TEXTFIELD_HPP

#include "fifechan/keylistener.hpp"
#include "fifechan/mouselistener.hpp"
#include "fifechan/platform.hpp"
#include "fifechan/widget.hpp"

#include <string>

namespace fcn
{
    class Text;
    class UTF8StringEditor;

    /**
     * An implementation of a text field where a user can enter a line of text.
     */
    class FIFEGUI_API TextField : public Widget, public MouseListener, public KeyListener
    {
    public:
        /**
         * Constructor.
         */
        TextField();

        /**
         * Constructor. The text field will be automatically resized
         * to fit the text.
         *
         * @param text The default text of the text field.
         */
        TextField(std::string const & text);

        /**
         * Destructor.
         */
        ~TextField();

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

        virtual void resizeToContent(bool recursiv = true);

        /**
         * Adjusts the size of the text field to fit the text.
         */
        virtual void adjustSize();

        virtual void draw(Graphics* graphics);

        // Inherited from MouseListener

        virtual void mousePressed(MouseEvent& mouseEvent);

        virtual void mouseDragged(MouseEvent& mouseEvent);

        // Inherited from KeyListener

        virtual void keyPressed(KeyEvent& keyEvent);

    protected:
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
        bool mEditable;

        /**
         * Holds the text of the text field.
         */
        Text* mText;

        /**
         * Holds the amount scrolled in x. If a user types more characters than
         * the text field can display, due to the text field being to small, the
         * text needs to scroll in order to show the last type character.
         */
        int mXScroll;

        /**
         * String editor for UTF8 support.
         */
        UTF8StringEditor* mStringEditor;
    };
} // namespace fcn

#endif // end FCN_TEXTFIELD_HPP
