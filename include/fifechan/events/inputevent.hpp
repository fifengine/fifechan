// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_INPUTEVENT_HPP_
#define INCLUDE_FIFECHAN_INPUTEVENT_HPP_

// Standard library includes
#include <cstdint>

// Platform config include
#include "fifechan/platform.hpp"

// Project headers (subdirs before local)
#include "fifechan/events/event.hpp"

namespace fcn
{
    /**
     * Base class for all input-related events (keyboard, mouse).
     *
     * Modifier state is stored as a bitmask. The public query methods
     * (isShiftPressed(), etc.) are retained for backward compatibility.
     *
     * @ingroup events
     */
    class FIFEGUI_API InputEvent : public Event
    {
        public:
            /// Modifier bitmask flags, matching Shortcut::Modifier positions.
            /// @see getModMask
            static constexpr uint16_t ModShift   = 1 << 0; ///< Shift key.
            static constexpr uint16_t ModControl = 1 << 1; ///< Control key.
            static constexpr uint16_t ModAlt     = 1 << 2; ///< Alt key.
            static constexpr uint16_t ModMeta    = 1 << 3; ///< Meta (GUI/Command) key.

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
             * Gets the modifier bitmask.
             *
             * @return The modifier bitmask.
             */
            uint16_t getModMask() const;

            /**
             * Marks this event as consumed.
             *
             * Consumed events may be ignored by some listeners or handled differently.
             * For example, a ScrollArea may discard consumed mouse wheel events to
             * prevent scrolling when a child widget (e.g., a slider) has already
             * handled the input.
             *
             * @see isConsumed()
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
             * Returns the widget that distributed this event.
             *
             * The distributor indicates which widget originally propagated the event.
             * This may differ from the event source due to event bubbling through
             * the widget hierarchy.
             *
             * @return Pointer to the distributing widget, or nullptr if none.
             */
            Widget* getDistributor() const;

        protected:
            /**
             * Modifier bitmask (ModShift | ModControl | ModAlt | ModMeta).
             */
            uint16_t mModMask{0};

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
             * Grants Gui privileged access to internal state.
             *
             * Gui is declared as a friend in order to be able to manipulate
             * the protected member variables of this class and at the same time
             * keep the MouseEvent class as const as possible. Gui needs to
             * update the distributer of this class whenever the distributer
             * changes as events bubble up.
             */
            friend class Gui;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_INPUTEVENT_HPP_
