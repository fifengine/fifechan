// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Corresponding header include
#include <fifechan/shortcut.hpp>

// Standard library includes
#include <string>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include <fifechan/widgets/label.hpp>

#include <fifechan/events/keyevent.hpp>

using fcn::Key;
using fcn::Shortcut;

TEST_CASE("Shortcut default constructor", "[unit][shortcut]")
{
    Shortcut sc;
    REQUIRE(sc.getKey().getValue() == 0);
    REQUIRE(sc.getModMask() == Shortcut::Modifier::None);
}

TEST_CASE("Shortcut key+modifier constructor", "[unit][shortcut]")
{
    SECTION("key only")
    {
        Shortcut sc(Key('A'));
        REQUIRE(sc.getKey().getValue() == 'A');
        REQUIRE(sc.getModMask() == Shortcut::Modifier::None);
    }

    SECTION("key with modifier")
    {
        Shortcut sc(Key('A'), Shortcut::Modifier::Control);
        REQUIRE(sc.getKey().getValue() == 'A');
        REQUIRE(sc.getModMask() == Shortcut::Modifier::Control);
    }

    SECTION("key with combined modifiers")
    {
        uint16_t mods = Shortcut::Modifier::Control | Shortcut::Modifier::Shift;
        Shortcut sc(Key('A'), mods);
        REQUIRE(sc.getKey().getValue() == 'A');
        REQUIRE(sc.getModMask() == mods);
    }
}

TEST_CASE("Shortcut equality", "[unit][shortcut]")
{
    Shortcut a(Key('X'), Shortcut::Modifier::Alt);
    Shortcut b(Key('X'), Shortcut::Modifier::Alt);

    REQUIRE(a == b);
    REQUIRE(!(a != b));
}

TEST_CASE("Shortcut inequality", "[unit][shortcut]")
{
    SECTION("different key")
    {
        Shortcut a(Key('X'), Shortcut::Modifier::Alt);
        Shortcut b(Key('Y'), Shortcut::Modifier::Alt);
        REQUIRE(a != b);
    }

    SECTION("different modifier")
    {
        Shortcut a(Key('X'), Shortcut::Modifier::Alt);
        Shortcut b(Key('X'), Shortcut::Modifier::Control);
        REQUIRE(a != b);
    }
}

TEST_CASE("Shortcut matches KeyEvent", "[unit][shortcut]")
{
    fcn::Label source;
    fcn::Label distributor;

    SECTION("matching key and modifiers")
    {
        Shortcut sc(Key(fcn::Key::KEY_RETURN), Shortcut::Modifier::None);
        fcn::KeyEvent event(
            &source,
            &distributor,
            false,
            false,
            false,
            false,
            fcn::KeyEvent::Type::Pressed,
            false,
            Key(fcn::Key::KEY_RETURN));

        REQUIRE(sc.matches(event));
    }

    SECTION("matching with control")
    {
        Shortcut sc(Key('C'), Shortcut::Modifier::Control);
        fcn::KeyEvent event(
            &source, &distributor, false, true, false, false, fcn::KeyEvent::Type::Pressed, false, Key('C'));

        REQUIRE(sc.matches(event));
    }

    SECTION("wrong key does not match")
    {
        Shortcut sc(Key('A'), Shortcut::Modifier::None);
        fcn::KeyEvent event(
            &source, &distributor, false, false, false, false, fcn::KeyEvent::Type::Pressed, false, Key('B'));

        REQUIRE(!sc.matches(event));
    }

    SECTION("wrong modifier does not match")
    {
        Shortcut sc(Key('A'), Shortcut::Modifier::Control);
        fcn::KeyEvent event(
            &source, &distributor, false, false, false, false, fcn::KeyEvent::Type::Pressed, false, Key('A'));

        REQUIRE(!sc.matches(event));
    }
}

TEST_CASE("Shortcut conflict detection", "[unit][shortcut]")
{
    SECTION("same key and overlapping mods conflicts")
    {
        Shortcut a(Key('Q'), Shortcut::Modifier::Control);
        Shortcut b(Key('Q'), Shortcut::Modifier::Control | Shortcut::Modifier::Shift);

        REQUIRE(a.conflicts(b));
        REQUIRE(b.conflicts(a));
    }

    SECTION("same key, no overlapping mods does not conflict")
    {
        Shortcut a(Key('Q'), Shortcut::Modifier::Control);
        Shortcut b(Key('Q'), Shortcut::Modifier::Alt);

        REQUIRE(!a.conflicts(b));
    }

    SECTION("different key does not conflict")
    {
        Shortcut a(Key('Q'), Shortcut::Modifier::Control);
        Shortcut b(Key('W'), Shortcut::Modifier::Control);

        REQUIRE(!a.conflicts(b));
    }

    SECTION("same key and same mods conflicts (identical)")
    {
        Shortcut a(Key('Q'), Shortcut::Modifier::Control);
        Shortcut b(Key('Q'), Shortcut::Modifier::Control);

        REQUIRE(a.conflicts(b));
    }
}

TEST_CASE("Shortcut fromKeycode factory", "[unit][shortcut]")
{
    SECTION("keycode only, no mods")
    {
        Shortcut sc = Shortcut::fromKeycode(0x00000061u); // SDLK_A = 'a'
        REQUIRE(sc.getKey().getValue() == 0x61);
        REQUIRE(sc.getModMask() == Shortcut::Modifier::None);
    }

    SECTION("keycode with SDL modifier mask")
    {
        // SDL_KMOD_CTRL = 64
        Shortcut sc = Shortcut::fromKeycode(0x00000063u, 64); // SDLK_C
        REQUIRE(sc.getKey().getValue() == 0x63);
        REQUIRE(sc.getModMask() == Shortcut::Modifier::Control);
    }

    SECTION("SDL_KMOD_GUI maps to Meta")
    {
        // SDL_KMOD_GUI = 1024
        Shortcut sc = Shortcut::fromKeycode(0x00000071u, 1024); // SDLK_Q
        REQUIRE(sc.getModMask() == Shortcut::Modifier::Meta);
    }
}

// ============================================================================
// Shortcut edge cases
// ============================================================================

TEST_CASE("Shortcut unbound (key=0)", "[unit][shortcut][edge]")
{
    Shortcut sc;
    REQUIRE(sc.getKey().getValue() == 0);
    REQUIRE(sc.getModMask() == Shortcut::Modifier::None);

    // An unbound shortcut should not match any real key
    fcn::Label source;
    fcn::Label distributor;
    fcn::KeyEvent event(
        &source,
        &distributor,
        false,
        false,
        false,
        false,
        fcn::KeyEvent::Type::Pressed,
        false,
        Key(fcn::Key::KEY_RETURN));

    REQUIRE(sc.matches(event) == false);
}

TEST_CASE("Shortcut all modifiers simultaneously", "[unit][shortcut][edge]")
{
    uint16_t allMods =
        Shortcut::Modifier::Shift | Shortcut::Modifier::Control | Shortcut::Modifier::Alt | Shortcut::Modifier::Meta;
    Shortcut sc(Key('P'), allMods);

    fcn::Label source;
    fcn::Label distributor;
    fcn::KeyEvent event(&source, &distributor, true, true, true, true, fcn::KeyEvent::Type::Pressed, false, Key('P'));

    REQUIRE(sc.matches(event));
}

TEST_CASE("Shortcut fromKeycode with all SDL modifiers", "[unit][shortcut][edge]")
{
    // SDL_KMOD_SHIFT=1, SDL_KMOD_CTRL=64, SDL_KMOD_ALT=256, SDL_KMOD_GUI=1024
    auto sc = Shortcut::fromKeycode(0x00000070u, 1 | 64 | 256 | 1024);
    REQUIRE(
        sc.getModMask() ==
        (Shortcut::Modifier::Shift | Shortcut::Modifier::Control | Shortcut::Modifier::Alt | Shortcut::Modifier::Meta));
}

TEST_CASE("Shortcut fromKeycode with no SDL modifiers", "[unit][shortcut][edge]")
{
    auto sc = Shortcut::fromKeycode(0x00000061u, 0);
    REQUIRE(sc.getKey().getValue() == 0x61);
    REQUIRE(sc.getModMask() == Shortcut::Modifier::None);
}

TEST_CASE("Shortcut conflict with identical shortcut", "[unit][shortcut][edge]")
{
    Shortcut a(Key('Z'), Shortcut::Modifier::Control);
    Shortcut b(Key('Z'), Shortcut::Modifier::Control);
    REQUIRE(a.conflicts(b));
}

TEST_CASE("Shortcut to_string edge cases", "[unit][shortcut][edge]")
{
    SECTION("unbound")
    {
        Shortcut sc;
        REQUIRE(sc.to_string() == "Key(0)");
    }

    SECTION("keycode zero")
    {
        Shortcut sc(Key(0));
        REQUIRE(sc.to_string() == "Key(0)");
    }

    SECTION("control char")
    {
        Shortcut sc(Key(1));
        REQUIRE(sc.to_string() == "Key(1)");
    }

    SECTION("all modifiers")
    {
        uint16_t allMods = Shortcut::Modifier::Control | Shortcut::Modifier::Shift | Shortcut::Modifier::Alt |
                           Shortcut::Modifier::Meta;
        Shortcut sc(Key('X'), allMods);
        // Order is always Ctrl+Shift+Alt+Meta+X
        REQUIRE(sc.to_string() == "Ctrl+Shift+Alt+Meta+X");
    }
}

// ============================================================================
// Shortcut layout independence semantic keycodes vs physical scancodes
// ============================================================================
//
// Layout independence is a design guarantee:
//   - Keycodes (SDLK_*) are SEMANTIC pressing the "A" key produces SDLK_a
//     on QWERTY, AZERTY, AND Dvorak (the OS handles scancode→keycode mapping).
//   - Scancodes (SDL_SCANCODE_*) are PHYSICAL SDL_SCANCODE_A is always the
//     same physical key position regardless of layout label.
//
// Shortcut stores semantic keycodes via fromKeycode(). When the backend
// receives a key event, it uses event.key.key (already an SDL keycode), so
// matching is a direct comparison layout-independent by construction.
//
// For physical-position bindings (e.g. WASD game controls), the backend
// calls SDL_GetKeyFromScancode() to convert the scancode to a keycode, then
// compares. This conversion is OS layout-aware, so it works on any layout.
//
// These tests verify the linkage contract without requiring a real keyboard
// or OS layout change.
// ============================================================================

TEST_CASE("ShortcutLayoutIndependence: fromKeycode matches same keycode", "[unit][shortcut][layout]")
{
    // Register with a semantic keycode (as game code would).
    // The backend receives the same keycode from event.key.key.
    // Matching is a direct comparison → layout-independent.
    Shortcut sc = Shortcut::fromKeycode(0x00000077u); // SDLK_w

    fcn::Label source;
    fcn::Label distributor;
    fcn::KeyEvent event(
        &source, &distributor, false, false, false, false, fcn::KeyEvent::Type::Pressed, false, Key(0x00000077u));

    REQUIRE(sc.matches(event));
}

TEST_CASE("ShortcutLayoutIndependence: modifier flags survive round-trip", "[unit][shortcut][layout]")
{
    // SDL_KMOD_SHIFT=1, SDL_KMOD_CTRL=64 → Shortcut::Shift|Control
    auto sc = Shortcut::fromKeycode(0x00000063u, 1 | 64); // SDLK_c Ctrl+Shift

    fcn::Label source;
    fcn::Label distributor;
    fcn::KeyEvent event(
        &source, &distributor, true, true, false, false, fcn::KeyEvent::Type::Pressed, false, Key(0x00000063u));

    REQUIRE(sc.matches(event));
}

TEST_CASE("ShortcutLayoutIndependence: different layout same symbol same keycode", "[unit][shortcut][layout]")
{
    // On any layout, pressing the key labeled 'M' produces SDLK_m = 0x6D.
    // A shortcut registered with fromKeycode(SDLK_m) matches when the
    // backend receives an event with SDLK_m.
    Shortcut sc = Shortcut::fromKeycode(0x0000006Du);

    fcn::Label source;
    fcn::Label distributor;
    fcn::KeyEvent event(
        &source, &distributor, false, false, false, false, fcn::KeyEvent::Type::Pressed, false, Key(0x0000006Du));

    REQUIRE(sc.matches(event));
}

TEST_CASE("ShortcutLayoutIndependence: non-printable keycodes are layout-stable", "[unit][shortcut][layout]")
{
    // Function keys, arrows, etc. have the same SDLK_* value on all layouts.
    Shortcut sc{Key(fcn::Key::F1)};

    fcn::Label source;
    fcn::Label distributor;
    fcn::KeyEvent event(
        &source, &distributor, false, false, false, false, fcn::KeyEvent::Type::Pressed, false, Key(fcn::Key::F1));

    REQUIRE(sc.matches(event));
}

TEST_CASE("ShortcutLayoutIndependence: fromScancode registered shortcut matches keycode", "[unit][shortcut][layout]")
{
    // This simulates what the SDL backend does when a game registers a
    // scancode-based shortcut (e.g. WASD). The backend calls
    // SDL_GetKeyFromScancode() which returns a keycode based on the
    // current OS layout. We then store that keycode in the Shortcut.
    //
    // Since we can't change the OS layout, we test the identity: a shortcut
    // registered with a keycode matches a KeyEvent carrying the same keycode.
    //
    // On a real system, SDL_GetKeyFromScancode(SDL_SCANCODE_A) returns:
    //   QWERTY: SDLK_a (0x61)
    //   AZERTY: SDLK_q (0x71)
    //   Dvorak: SDLK_a (0x61)
    // The shortcut would match the corresponding keycode on each layout.
    int const scancodeResult = 0x00000061u; // what SDL_GetKeyFromScancode returns on THIS system
    Shortcut sc{Key(scancodeResult)};

    fcn::Label source;
    fcn::Label distributor;
    fcn::KeyEvent event(
        &source, &distributor, false, false, false, false, fcn::KeyEvent::Type::Pressed, false, Key(scancodeResult));

    REQUIRE(sc.matches(event));
}

// ============================================================================

TEST_CASE("Shortcut to_string", "[unit][shortcut]")
{
    SECTION("printable key, no modifier")
    {
        Shortcut sc(Key('A'));
        REQUIRE(sc.to_string() == "A");
    }

    SECTION("printable key with Ctrl")
    {
        Shortcut sc(Key('W'), Shortcut::Modifier::Control);
        REQUIRE(sc.to_string() == "Ctrl+W");
    }

    SECTION("combined modifiers")
    {
        uint16_t mods = Shortcut::Modifier::Control | Shortcut::Modifier::Shift;
        Shortcut sc(Key('W'), mods);
        REQUIRE(sc.to_string() == "Ctrl+Shift+W");
    }

    SECTION("non-printable key")
    {
        Shortcut sc{Key(fcn::Key::F1)};
        // Without SDL_GetKeyName(), falls back to "Key(<value>)" format.
        REQUIRE(sc.to_string().find("Key(") != std::string::npos);
    }
}
