// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/shortcut.hpp"

// Standard library includes
#include <cassert>
#include <string>

// Project headers (subdirs before local)
#include "fifechan/events/keyevent.hpp"
#include "fifechan/key.hpp"

namespace fcn
{

    Shortcut::Shortcut(Key key, uint16_t modMask) : mKey(key), mModMask(modMask)
    {
    }

    Shortcut Shortcut::fromKeycode(uint32_t keycode, uint16_t sdlMods)
    {
        // Convert SDL_Keymod bitmask to Shortcut::Modifier
        // SDL_KMOD_* map as: SHIFT=1, CTRL=64, ALT=256, GUI=1024
        uint16_t mods = 0;
        if ((sdlMods & 1) != 0) {
            mods |= Modifier::Shift; // SDL_KMOD_SHIFT
        }
        if ((sdlMods & 64) != 0) {
            mods |= Modifier::Control; // SDL_KMOD_CTRL
        }
        if ((sdlMods & 256) != 0) {
            mods |= Modifier::Alt; // SDL_KMOD_ALT
        }
        if ((sdlMods & 1024) != 0) {
            mods |= Modifier::Meta; // SDL_KMOD_GUI
        }

        return Shortcut(Key(static_cast<int>(keycode)), mods);
    }

    bool Shortcut::matches(KeyEvent const & event) const
    {
        assert("event key must be valid" && event.getKey().getValue() != 0);
        if (mKey.getValue() != event.getKey().getValue()) {
            return false;
        }

        uint16_t eventMods = 0;
        if (event.isShiftPressed()) {
            eventMods |= Modifier::Shift;
        }
        if (event.isControlPressed()) {
            eventMods |= Modifier::Control;
        }
        if (event.isAltPressed()) {
            eventMods |= Modifier::Alt;
        }
        if (event.isMetaPressed()) {
            eventMods |= Modifier::Meta;
        }

        return mModMask == eventMods;
    }

    bool Shortcut::conflicts(Shortcut const & other) const
    {
        // Same key and overlapping modifier masks
        if (mKey.getValue() != other.mKey.getValue()) {
            return false;
        }

        return (mModMask & other.mModMask) != 0;
    }

    std::string Shortcut::to_string() const
    {
        std::string result;

        if ((mModMask & Modifier::Control) != 0) {
            result += "Ctrl+";
        }
        if ((mModMask & Modifier::Shift) != 0) {
            result += "Shift+";
        }
        if ((mModMask & Modifier::Alt) != 0) {
            result += "Alt+";
        }
        if ((mModMask & Modifier::Meta) != 0) {
            result += "Meta+";
        }

        // Append key name using the value
        int const val = mKey.getValue();
        if (val >= 32 && val <= 126) {
            result += static_cast<char>(val);
        } else {
            result += "Key(" + std::to_string(val) + ")";
        }

        return result;
    }

    Key Shortcut::getKey() const
    {
        return mKey;
    }

    uint16_t Shortcut::getModMask() const
    {
        return mModMask;
    }

    bool Shortcut::operator==(Shortcut const & other) const
    {
        return mKey.getValue() == other.mKey.getValue() && mModMask == other.mModMask;
    }

    bool Shortcut::operator!=(Shortcut const & other) const
    {
        return !(*this == other);
    }

} // namespace fcn
