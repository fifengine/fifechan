// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_TEXTINPUTEVENT_HPP_
#define INCLUDE_FIFECHAN_TEXTINPUTEVENT_HPP_

// Standard library includes
#include <cstdint>
#include <string>
#include <utility>

// Platform config include
#include "fifechan/platform.hpp"

// Project headers (subdirs before local)
#include "fifechan/events/inputevent.hpp"

namespace fcn
{
    /**
     * Text input event for IME (input method editor) composition,
     * dead keys, and pasted text.
     *
     * Fired on `SDL_EVENT_TEXT_INPUT`. Carries UTF-8 text and remains
     * separate from `KeyEvent` so composed input is not collapsed into
     * a single keycode.
     *
     * Inherits `InputEvent`, including modifier-state queries
     * (`isShiftPressed()`, etc.) and the `consume()` mechanism. This allows
     * IME-aware widgets such as `TextField` to consume text input before the
     * originating keystrokes are processed as hotkeys.
     *
     * @ingroup events
     */
    class FIFEGUI_API TextInputEvent : public InputEvent
    {
        public:
            /**
             * Text event types.
             */
            enum class Type : std::uint8_t
            {
                Input = 0, ///< Text was input (SDL_EVENT_TEXT_INPUT).
                Editing    ///< IME composition in progress (SDL_EVENT_TEXT_EDITING).
            };

            /**
             * Constructor.
             *
             * @param source The source widget.
             * @param distributor The distributor of the event.
             * @param isShiftPressed True if shift is pressed.
             * @param isControlPressed True if control is pressed.
             * @param isAltPressed True if alt is pressed.
             * @param isMetaPressed True if meta is pressed.
             * @param text The UTF-8 text that was input.
             * @param type The type of text event.
             */
            TextInputEvent(
                Widget* source,
                Widget* distributor,
                bool isShiftPressed,
                bool isControlPressed,
                bool isAltPressed,
                bool isMetaPressed,
                std::string text,
                Type type = Type::Input);

            /**
             * Gets the text of the event.
             *
             * @return The UTF-8 text.
             */
            std::string const & getText() const;

            /**
             * Gets the type of text event.
             *
             * @return The type.
             */
            Type getType() const;

        protected:
            /** The UTF-8 text. */
            std::string mText;

            /** The type of text event. */
            Type mType{Type::Input};
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_TEXTINPUTEVENT_HPP_
