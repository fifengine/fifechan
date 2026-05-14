// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_SHORTCUT_HPP_
#define INCLUDE_FIFECHAN_SHORTCUT_HPP_

// Standard library includes
#include <cstdint>
#include <string>

// Platform config include
#include "fifechan/platform.hpp"

// Project headers (subdirs before local)
#include "fifechan/key.hpp"

namespace fcn
{
    class KeyEvent;

    /**
     * High-level shortcut binding a semantic key with a modifier mask.
     *
     * `Shortcut` is the primary key representation exposed to widgets and
     * game code. It stores an `fcn::Key` semantic keycode and a `uint16_t`
     * modifier bitmask, hiding raw scancode details.
     *
     * Scancode-based construction and matching are handled internally by
     * the input backend. Widget code interacts only with the semantic API.
     *
     * @section shortcut_usage Usage
     *
     * @code{.cpp}
     * // Register a shortcut
     * fcn::Shortcut saveSc(fcn::Key('S'), fcn::Shortcut::Modifier::Control);
     *
     * // Match against a key event
     * if (saveSc.matches(keyEvent)) { }
     *
     * // Build from raw SDL keycode (backend use)
     * auto sc = fcn::Shortcut::fromKeycode(SDLK_s, SDL_KMOD_CTRL);
     * @endcode
     *
     * @section layout Layout Independence
     *
     * Shortcuts are layout-independent by design. A shortcut created with
     * `fromKeycode(SDLK_a)` matches the logical "A" key across keyboard
     * layouts because SDL keycodes are semantic and derived from the active
     * OS keyboard layout.
     *
     * Physical-position bindings such as WASD are resolved internally with
     * `SDL_GetKeyFromScancode()`, which maps a physical key position to the
     * correct semantic keycode for the current layout.
     *
     * @ingroup input
     */
    class FIFEGUI_API Shortcut
    {
        public:
            /**
             * Modifier flags.
             */
            enum Modifier : uint16_t
            {
                None    = 0,
                Shift   = 1 << 0,
                Control = 1 << 1,
                Alt     = 1 << 2,
                Meta    = 1 << 3
            };

            /**
             * Default constructor creates an unbound shortcut (key 0, no mods).
             */
            Shortcut() = default;

            /**
             * Constructor.
             *
             * @param key The semantic key.
             * @param modMask Bitmask of Modifier flags.
             */
            explicit Shortcut(Key key, uint16_t modMask = Modifier::None);

            /**
             * Creates a Shortcut from a raw SDL keycode + SDL modifier mask.
             *
             * Since fcn::KeyType mirrors SDL_Keycode 1:1, this is an inline
             * passthrough requiring no SDL headers.
             *
             * @param keycode SDL_Keycode value (uint32_t).
             * @param sdlMods SDL key modifier bitmask.
             * @return A Shortcut with the matching semantic key.
             */
            static Shortcut fromKeycode(uint32_t keycode, uint16_t sdlMods = 0);

            /**
             * Compares the shortcut to a KeyEvent.
             *
             * Both the semantic key value and modifier mask must match.
             *
             * @param event The key event to test against.
             * @return True if the event matches this shortcut.
             */
            bool matches(KeyEvent const & event) const;

            /**
             * Checks for collision with another shortcut.
             *
             * Two shortcuts conflict if they have the same key and share at
             * least one modifier flag (i.e. the modifier masks overlap).
             *
             * @param other The other shortcut to check against.
             * @return True if the shortcuts conflict.
             */
            bool conflicts(Shortcut const & other) const;

            /**
             * Returns a human-readable string, e.g. "Ctrl+Shift+W".
             *
             * Modifier names and key names are determined via SDL_GetKeyName()
             * at the backend level. This base implementation uses generic names.
             *
             * @return A human-readable representation.
             */
            std::string to_string() const;

            /**
             * Gets the semantic key.
             *
             * @return The key.
             */
            Key getKey() const;

            /**
             * Gets the modifier bitmask.
             *
             * @return The modifier mask.
             */
            uint16_t getModMask() const;

            /**
             * Compares two shortcuts for equality (same key + same mods).
             */
            bool operator==(Shortcut const & other) const;

            /**
             * Compares two shortcuts for inequality.
             */
            bool operator!=(Shortcut const & other) const;

        protected:
            /** The semantic key. */
            Key mKey;

            /** Bitmask of Modifier flags. */
            uint16_t mModMask{Modifier::None};
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_SHORTCUT_HPP_
