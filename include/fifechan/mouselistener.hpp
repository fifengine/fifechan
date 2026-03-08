// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_MOUSELISTENER_HPP_
#define INCLUDE_FIFECHAN_MOUSELISTENER_HPP_

#include "fifechan/mouseevent.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * Interface for listening for mouse events from widgets.
     *
     * @see Widget::addMouseListener, Widget::removeMouseListener
     */
    class FIFEGUI_API MouseListener
    {
    public:
        virtual ~MouseListener() = default;

        /**
         * Called when the mouse has entered into the widget area.
         *
         * @param mouseEvent Describes the event.
         */
        virtual void mouseEntered(MouseEvent& mouseEvent) { }

        /**
         * Called when the mouse has exited the widget area.
         *
         * @param mouseEvent Describes the event.
         */
        virtual void mouseExited(MouseEvent& mouseEvent) { }

        /**
         * Called when a mouse button has been pressed on the widget area.
         *
         * NOTE: A mouse press is NOT equal to a mouse click.
         *       Use mouseClickMessage to check for mouse clicks.
         *
         * @param mouseEvent Describes the event.
         */
        virtual void mousePressed(MouseEvent& mouseEvent) { }

        /**
         * Called when a mouse button has been released on the widget area.
         *
         * @param mouseEvent Describes the event.
         */
        virtual void mouseReleased(MouseEvent& mouseEvent) { }

        /**
         * Called when a mouse button is pressed and released (clicked) on
         * the widget area.
         *
         * @param mouseEvent Describes the event.
         */
        virtual void mouseClicked(MouseEvent& mouseEvent) { }

        /**
         * Called when the mouse wheel has moved up on the widget area.
         *
         * @param mouseEvent Describes the event.
         */
        virtual void mouseWheelMovedUp(MouseEvent& mouseEvent) { }

        /**
         * Called when the mouse wheel has moved down on the widget area.
         *
         * @param mouseEvent Describes the event.
         */
        virtual void mouseWheelMovedDown(MouseEvent& mouseEvent) { }

        /**
         * Called when the mouse wheel has moved right on the widget area.
         *
         * @param mouseEvent Describes the event.
         */
        virtual void mouseWheelMovedRight(MouseEvent& mouseEvent) { }

        /**
         * Called when the mouse wheel has moved left on the widget area.
         *
         * @param mouseEvent Describes the event.
         */
        virtual void mouseWheelMovedLeft(MouseEvent& mouseEvent) { }

        /**
         * Called when the mouse has moved in the widget area and no mouse button
         * has been pressed (i.e no widget is being dragged).
         *
         * @param mouseEvent Describes the event.
         */
        virtual void mouseMoved(MouseEvent& mouseEvent) { }

        /**
         * Called when the mouse has moved and the mouse has previously been
         * pressed on the widget.
         *
         * @param mouseEvent Describes the event.
         */
        virtual void mouseDragged(MouseEvent& mouseEvent) { }

    protected:
        /**
         * Constructor.
         *
         * You should not be able to make an instance of MouseListener,
         * therefore its constructor is protected.
         */
        MouseListener() = default;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_MOUSELISTENER_HPP_
