// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_INPUTEVENT_HPP_
#define INCLUDE_FIFECHAN_INPUTEVENT_HPP_

#include "fifechan/events/event.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * Base class for all input-related events (keyboard, mouse).
     *
     * @ingroup events
     */
    class FIFEGUI_API InputEvent : public Event
    {
    public:
        /**
         * Constructor.
         *
         * @param source The widget the event concerns.
         * @param distributor The distributor of the event.
         * @param isShiftPressed True if shift is pressed, false otherwise.
         * @param isControlPressed True if control is pressed, false otherwise.
         * @param isAltPressed True if alt is pressed, false otherwise.
         * @param isMetaPressed True if meta is pressed, false otherwise.
         */
        InputEvent(
            Widget* source,
            Widget* distributor,
            bool isShiftPressed,
            bool isControlPressed,
            bool isAltPressed,
            bool isMetaPressed);

        /**
         * Checks if shift is pressed.
         *
         * @return True if shift was pressed at the same time as the key,
         *         false otherwise.
         */
        bool isShiftPressed() const;

        /**
         * Checks if control is pressed.
         *
         * @return True if control was pressed at the same time as the key,
         *         false otherwise.
         */
        bool isControlPressed() const;

        /**
         * Checks if alt is pressed.
         *
         * @return True if alt was pressed at the same time as the key,
         *         false otherwise.
         */
        bool isAltPressed() const;

        /**
         * Checks whether meta is pressed.
         *
         * @return True if meta was pressed at the same time as the key,
         *         false otherwise.
         */
        bool isMetaPressed() const;

        /**
         * Marks the event as consumed. Input event listeners may discard
         * consumed input or act on consumed input. An example of a widget
         * that discards consumed input is the ScrollArea widget that
         * discards consumed mouse wheel events so the ScrollArea will not
         * scroll if for instance a Slider's value inside the ScrollArea was
         * changed with the mouse wheel.
         *
         * @see isConsumed
         */
        void consume();

        /**
         * Checks if the input event is consumed.
         *
         * @return True if the input event is consumed,
         *         false otherwise.
         * @see consume
         */
        bool isConsumed() const;

        /**
         * Gets the distributor of the event. The function is
         * used to tell which widget actually distributed the
         * event. As input events bubbles up, the source of the event
         * may not be the same as the distributor of the event.
         */
        Widget* getDistributor() const;

    protected:
        /**
         * True if shift is pressed, false otherwise.
         */
        bool mShiftPressed;

        /**
         * True if control is pressed, false otherwise.
         */
        bool mControlPressed;

        /**
         * True if alt is pressed, false otherwise.
         */
        bool mAltPressed;

        /**
         * True if meta is pressed, false otherwise.
         */
        bool mMetaPressed;

        /**
         * True if the input event is consumed,
         * false otherwise.
         */
        bool mIsConsumed;

        /**
         * Holds the distributor of the event.
         */
        Widget* mDistributor;

        /**
         * Gui is a friend of this class in order to be able to manipulate
         * the protected member variables of this class and at the same time
         * keep the MouseEvent class as const as possible. Gui needs to
         * update the distributer of this class whenever the distributer
         * changes as events bubble up.
         */
        friend class Gui;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_INPUTEVENT_HPP_
