// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_KEY_HPP_
#define INCLUDE_FIFECHAN_KEY_HPP_

// Standard library includes
#include <cstdint>

// Platform config include
#include "fifechan/platform.hpp"

// Project headers (subdirs before local)
#include "fifechan/key_gen.h"

namespace fcn
{
    /**
     * Represents a keyboard key or character code.
     *
     * Key values are defined by the auto-generated key_gen.h as
     * fcn::KeyType (uint32_t), mirroring SDL3 SDLK_* constants 1:1.
     *
     * @section key_semantic Semantic Key Model
     *
     * Keycodes are SEMANTIC SDLK_a is the "a" key on any keyboard layout
     * (QWERTY, AZERTY, Dvorak). The OS handles scancode-to-keycode mapping.
     * Scancodes are PHYSICAL raw SDL_Scancode values are used internally
     * by the backend for physical-position queries (e.g. WASD game controls),
     * but never exposed to widget or game code.
     *
     * The key_gen.h header is automatically generated from SDL3's
     * SDL_keycode.h at configure time by tools/keygen/. To regenerate
     * after an SDL3 update: cmake --build . --target sync_keys_with_SDL3
     *
     * @ingroup input
     */
    class FIFEGUI_API Key
    {
        public:
            /**
             * Constructor.
             *
             * @param value The keycode value (SDL3 SDLK_* or 0).
             */
            explicit Key(int value = 0);

            /**
             * Checks if a key is a character.
             *
             * @return True if the key is a letter, number or whitespace,
             *         false otherwise.
             */
            bool isCharacter() const;

            /**
             * Checks if a key is a number.
             *
             * @return True if the key is a number (0-9),
             *         false otherwise.
             */
            bool isNumber() const;

            /**
             * Checks if a key is a letter.
             *
             * @return True if the key is a letter (a-z,A-Z),
             *         false otherwise.
             */
            bool isLetter() const;

            /**
             * Gets the value of the key. If an ascii value exists it
             * will be returned. Otherwise the SDLK_* keycode is returned.
             *
             * @return the value of the key.
             */
            int getValue() const;

            /**
             * Compares two keys.
             *
             * @param key The key to compare this key with.
             * @return True if the keys are equal, false otherwise.
             */
            bool operator==(Key const & key) const;

            /**
             * Compares two keys.
             *
             * @param key The key to compare this key with.
             * @return True if the keys are not equal, false otherwise.
             */
            bool operator!=(Key const & key) const;

        protected:
            /**
             * Holds the value of the key. It may be an ascii value
             * or a uint32_t SDLK_* keycode.
             */
            int mValue;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_KEY_HPP_
