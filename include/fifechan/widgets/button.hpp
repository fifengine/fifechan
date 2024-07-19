// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_BUTTON_HPP
#define FCN_BUTTON_HPP

#include <string>

#include "fifechan/focuslistener.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/keylistener.hpp"
#include "fifechan/mouseevent.hpp"
#include "fifechan/mouselistener.hpp"
#include "fifechan/platform.hpp"
#include "fifechan/widget.hpp"
#include "fifechan/widgetlistener.hpp"

namespace fcn
{
    /**
     * An implementation of a regular clickable button. A button is capable of
     * displaying a caption.
     *
     * If a button is clicked an action event will be sent to all action listener's
     * of the button.
     *
     * @see ImageButton
     */
    class FIFEGUI_API Button :
        public Widget,
        public MouseListener,
        public KeyListener,
        public FocusListener,
        public WidgetListener
    {
    public:
        /**
         * Constructor.
         */
        Button();

        /**
         * Constructor. The button will be automatically resized
         * to fit the caption.
         *
         * @param caption The caption of the button.
         */
        Button(std::string const & caption);

        /**
         * Destructor.
         */
        virtual ~Button();

        /**
         * Sets the caption of the button.
         *
         * @param caption The caption of the button.
         * @see getCaption, adjustSize
         */
        void setCaption(std::string const & caption);

        /**
         * Gets the caption of the button.
         *
         * @return The caption of the button.
         */
        std::string const & getCaption() const;

        /**
         * Sets the button state.
         * If the button is inactiv it looks gray.
         *
         * @param state True to activate the button or false to deactivate it.
         */
        void setActive(bool state);

        /**
         * Returns the button state.
         *
         * @return True if the button is active otherwise false.
         */
        bool isActive() const;

        /**
         * Sets the alignment of the caption. The alignment is relative
         * to the center of the button.
         *
         * @param alignment The alignment of the caption.
         * @see getAlignment, Graphics
         */
        void setAlignment(Graphics::Alignment alignment);

        /**
         * Gets the alignment of the caption.
         *
         * @return The alignment of the caption.
         * @see setAlignment, Graphics
         */
        Graphics::Alignment getAlignment() const;

        /**
         * Sets the number of pixels the image or text will be offset from
         * the top left corner of button when the button is pressed or toggled.
         *
         * @param offset The x offset.
         */
        void setDownXOffset(int offset);

        /**
         * Gets the number of pixels the image or text will be offset.
         *
         * @return The x offset.
         */
        int getDownXOffset() const;

        /**
         * Sets the number of pixels the image or text will be offset from
         * the top left corner of button when the button is pressed or toggled.
         *
         * @param offset The y offset.
         */
        void setDownYOffset(int offset);

        /**
         * Gets the number of pixels the image or text will be offset.
         *
         * @return The y offset.
         */
        int getDownYOffset() const;

        /**
         * Sets the number of pixels the image or text will be offset from
         * the top left corner of button when the button is pressed or toggled.
         *
         * @param x The x offset.
         * @param y The y offset.
         */
        void setDownOffset(int x, int y);

        // Inherited from Widget

        virtual void resizeToContent(bool recursiv = true);
        virtual void adjustSize();
        virtual void draw(Graphics* graphics);
        virtual void fontChanged();

        // Inherited from FocusListener

        virtual void focusLost(Event const & event);

        // Inherited from MouseListener

        virtual void mousePressed(MouseEvent& mouseEvent);
        virtual void mouseReleased(MouseEvent& mouseEvent);
        virtual void mouseEntered(MouseEvent& mouseEvent);
        virtual void mouseExited(MouseEvent& mouseEvent);
        virtual void mouseDragged(MouseEvent& mouseEvent);

        // Inherited from KeyListener

        virtual void keyPressed(KeyEvent& keyEvent);
        virtual void keyReleased(KeyEvent& keyEvent);

        // Inherited from WidgetListener

        virtual void ancestorHidden(Event const & e);

    protected:
        /**
         * Checks if the button is pressed. Convenient method to use
         * when overloading the draw method of the button.
         *
         * @return True if the button is pressed, false otherwise.
         */
        virtual bool isPressed() const;

        /**
         * Holds the caption of the button.
         */
        std::string mCaption;

        /**
         * True if the mouse is ontop of the button, false otherwise.
         */
        bool mHasMouse;

        /**
         * True if a key has been pressed, false otherwise.
         */
        bool mKeyPressed;

        /**
         * True if a mouse has been pressed, false otherwise.
         */
        bool mMousePressed;

        /**
         * True if the button is active.
         */
        bool mState;

        /**
         * Holds the alignment of the caption.
         */
        Graphics::Alignment mAlignment;

        /**
         * Holds the x down offset of the caption.
         */
        int mXOffset;

        /**
         * Holds the y down offset of the caption.
         */
        int mYOffset;
    };
} // namespace fcn

#endif // end FCN_BUTTON_HPP
