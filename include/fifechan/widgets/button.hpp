// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_BUTTON_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_BUTTON_HPP_

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
     * A standard clickable button widget.
     *
     * A button is capable of displaying a caption.
     * If a button is clicked an action event will be sent to all action listener's
     * of the button.
     *
     * @see ImageButton
     *
     * @ingroup widgets
     */
    class FIFEGUI_API Button :
        public Widget,
        public MouseListener,
        public KeyListener,
        public FocusListener,
        public WidgetListener
    {
    public:
        Button();

        /**
         * Constructor. The button will be automatically resized
         * to fit the caption.
         *
         * @param caption The caption of the button.
         */
        explicit Button(std::string caption);

        ~Button() override;

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
         * If the button is inactive it looks gray.
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

        using Widget::resizeToContent;

        void resizeToContent(bool recursion) override;
        void adjustSize() override;
        void draw(Graphics* graphics) override;
        void fontChanged() override;

        // Inherited from FocusListener

        void focusLost(Event const & event) override;

        // Inherited from MouseListener

        void mousePressed(MouseEvent& mouseEvent) override;
        void mouseReleased(MouseEvent& mouseEvent) override;
        void mouseEntered(MouseEvent& mouseEvent) override;
        void mouseExited(MouseEvent& mouseEvent) override;
        void mouseDragged(MouseEvent& mouseEvent) override;

        // Inherited from KeyListener

        void keyPressed(KeyEvent& keyEvent) override;
        void keyReleased(KeyEvent& keyEvent) override;

        // Inherited from WidgetListener

        void ancestorHidden(Event const & e) override;

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
        virtual void adjustSizeImpl();

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
         * True if the mouse is on top of the button, false otherwise.
         */
        bool mHasMouse{false};

        /**
         * True if a key has been pressed, false otherwise.
         */
        bool mKeyPressed{false};

        /**
         * True if a mouse has been pressed, false otherwise.
         */
        bool mMousePressed{false};

        /**
         * True if the button is active.
         */
        bool mState{true};

        /**
         * Holds the alignment of the caption.
         */
        Graphics::Alignment mAlignment{Graphics::Alignment::Center};

        /**
         * Holds the x down offset of the caption.
         */
        int mXOffset{1};

        /**
         * Holds the y down offset of the caption.
         */
        int mYOffset{1};
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_BUTTON_HPP_
