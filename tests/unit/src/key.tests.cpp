// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/key.hpp"

using fcn::Key;

// ============================================================================
// Key constructor
// ============================================================================

TEST_CASE("Key constructor sets value", "[unit][key]")
{
    Key key(65);
    REQUIRE(key.getValue() == 65);
}

TEST_CASE("Key default constructor sets zero", "[unit][key]")
{
    Key key;
    REQUIRE(key.getValue() == 0);
}

TEST_CASE("Key constructor with negative value", "[unit][key]")
{
    Key key(-100);
    REQUIRE(key.getValue() == -100);
}

// ============================================================================
// Key::isCharacter - happy path
// ============================================================================

TEST_CASE("Key isCharacter identifies standard ASCII", "[unit][key]")
{
    // Space
    REQUIRE(Key(' ').isCharacter() == true);
    // Numbers
    REQUIRE(Key('0').isCharacter() == true);
    REQUIRE(Key('5').isCharacter() == true);
    REQUIRE(Key('9').isCharacter() == true);
    // Uppercase letters
    REQUIRE(Key('A').isCharacter() == true);
    REQUIRE(Key('M').isCharacter() == true);
    REQUIRE(Key('Z').isCharacter() == true);
    // Lowercase letters
    REQUIRE(Key('a').isCharacter() == true);
    REQUIRE(Key('m').isCharacter() == true);
    REQUIRE(Key('z').isCharacter() == true);
    // Special characters
    REQUIRE(Key('!').isCharacter() == true);
    REQUIRE(Key('@').isCharacter() == true);
    // Tab is 9 (in isCharacter range)
    REQUIRE(Key(9).isCharacter() == true);
}

TEST_CASE("Key isCharacter identifies extended ASCII", "[unit][key]")
{
    // Extended ASCII range 162-255
    REQUIRE(Key(162).isCharacter() == true);
    REQUIRE(Key(200).isCharacter() == true);
    REQUIRE(Key(255).isCharacter() == true);
}

TEST_CASE("Key isCharacter rejects non-characters", "[unit][key]")
{
    // Control characters below 32
    REQUIRE(Key(0).isCharacter() == false);
    REQUIRE(Key(10).isCharacter() == false); // Newline
    REQUIRE(Key(31).isCharacter() == false);

    // DEL and beyond 126 but below 162
    REQUIRE(Key(127).isCharacter() == false);
    REQUIRE(Key(150).isCharacter() == false);
    REQUIRE(Key(161).isCharacter() == false);

    // Negative values (function keys)
    REQUIRE(Key(-1000).isCharacter() == false);
}

// ============================================================================
// Key::isNumber - happy path
// ============================================================================

TEST_CASE("Key isNumber identifies digits", "[unit][key]")
{
    REQUIRE(Key('0').isNumber() == true);
    REQUIRE(Key('5').isNumber() == true);
    REQUIRE(Key('9').isNumber() == true);
}

TEST_CASE("Key isNumber rejects non-digits", "[unit][key]")
{
    // Letters
    REQUIRE(Key('A').isNumber() == false);
    REQUIRE(Key('a').isNumber() == false);
    // Special characters
    REQUIRE(Key('!').isNumber() == false);
    REQUIRE(Key(' ').isNumber() == false);
    // Out of range
    REQUIRE(Key(0).isNumber() == false);
    REQUIRE(Key(47).isNumber() == false); // Before '0'
    REQUIRE(Key(58).isNumber() == false); // After '9'
    REQUIRE(Key(-1000).isNumber() == false);
}

// ============================================================================
// Key::isLetter - happy path
// ============================================================================

TEST_CASE("Key isLetter identifies uppercase letters", "[unit][key]")
{
    REQUIRE(Key('A').isLetter() == true);
    REQUIRE(Key('M').isLetter() == true);
    REQUIRE(Key('Z').isLetter() == true);
}

TEST_CASE("Key isLetter identifies lowercase letters", "[unit][key]")
{
    REQUIRE(Key('a').isLetter() == true);
    REQUIRE(Key('m').isLetter() == true);
    REQUIRE(Key('z').isLetter() == true);
}

TEST_CASE("Key isLetter identifies extended letters", "[unit][key]")
{
    // Extended ASCII letters (192-214, 216-246, 248-255)
    REQUIRE(Key(192).isLetter() == true); // À
    REQUIRE(Key(200).isLetter() == true); // È
    REQUIRE(Key(255).isLetter() == true); // ÿ
}

TEST_CASE("Key isLetter rejects non-letters", "[unit][key]")
{
    // Numbers
    REQUIRE(Key('0').isLetter() == false);
    REQUIRE(Key('9').isLetter() == false);
    // Special characters
    REQUIRE(Key('!').isLetter() == false);
    REQUIRE(Key(' ').isLetter() == false);
    // Non-letter extended ASCII
    REQUIRE(Key(215).isLetter() == false); // × (multiplication sign)
    REQUIRE(Key(247).isLetter() == false); // ÷ (division sign)
    // Control characters
    REQUIRE(Key(0).isLetter() == false);
    REQUIRE(Key(31).isLetter() == false);
    // Function keys
    REQUIRE(Key(-1000).isLetter() == false);
}

// ============================================================================
// Key::isLetter - edge cases
// ============================================================================

TEST_CASE("Key isLetter handles boundary values", "[unit][key]")
{
    // Boundaries for A-Z (65-90)
    REQUIRE(Key(64).isLetter() == false); // Before 'A'
    REQUIRE(Key(65).isLetter() == true);  // 'A'
    REQUIRE(Key(90).isLetter() == true);  // 'Z'
    REQUIRE(Key(91).isLetter() == false); // After 'Z'

    // Boundaries for a-z (97-122)
    REQUIRE(Key(96).isLetter() == false);  // Before 'a'
    REQUIRE(Key(97).isLetter() == true);   // 'a'
    REQUIRE(Key(122).isLetter() == true);  // 'z'
    REQUIRE(Key(123).isLetter() == false); // After 'z'

    // Boundaries for extended (192-255, excluding 215 and 247)
    REQUIRE(Key(191).isLetter() == false);
    REQUIRE(Key(192).isLetter() == true);
    REQUIRE(Key(214).isLetter() == true);
    REQUIRE(Key(215).isLetter() == false); // × (excluded)
    REQUIRE(Key(216).isLetter() == true);
    REQUIRE(Key(246).isLetter() == true);
    REQUIRE(Key(247).isLetter() == false); // ÷ (excluded)
    REQUIRE(Key(248).isLetter() == true);
    REQUIRE(Key(255).isLetter() == true);
    REQUIRE(Key(256).isLetter() == false);
}

// ============================================================================
// Key comparison operators
// ============================================================================

TEST_CASE("Key operator== compares values", "[unit][key]")
{
    Key key1(65);
    Key key2(65);
    Key key3(66);

    REQUIRE(key1 == key2);
    REQUIRE(!(key1 == key3));
}

TEST_CASE("Key operator!= compares values", "[unit][key]")
{
    Key key1(65);
    Key key2(65);
    Key key3(66);

    REQUIRE(!(key1 != key2));
    REQUIRE(key1 != key3);
}

TEST_CASE("Key comparison with same object", "[unit][key]")
{
    Key key(42);
    REQUIRE(key == key);
    REQUIRE(!(key != key));
}

// ============================================================================
// Key SDL3 keycode values (from generated key_gen.h)
// ============================================================================

TEST_CASE("Key SDL3 keycode values are uint32_t", "[unit][key]")
{
    REQUIRE(fcn::F1 == 0x4000003Au);
    REQUIRE(fcn::F12 == 0x40000045u);
    REQUIRE(fcn::RETURN == 0x0000000Du);
    REQUIRE(fcn::ESCAPE == 0x0000001Bu);
    REQUIRE(fcn::TAB == 0x00000009u);
}

TEST_CASE("Key SDL3 keycode function keys are not characters", "[unit][key]")
{
    REQUIRE(Key(fcn::F1).isCharacter() == false);
    REQUIRE(Key(fcn::F12).isCharacter() == false);
}

TEST_CASE("Key SDL3 keycode function keys are not numbers", "[unit][key]")
{
    REQUIRE(Key(fcn::F1).isNumber() == false);
    REQUIRE(Key(fcn::F5).isNumber() == false);
}

TEST_CASE("Key SDL3 keycode function keys are not letters", "[unit][key]")
{
    REQUIRE(Key(fcn::F1).isLetter() == false);
    REQUIRE(Key(fcn::F10).isLetter() == false);
}
