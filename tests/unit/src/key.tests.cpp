// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include <fifechan/key.hpp>

TEST_CASE("Key constructors initialize properly", "[unit][key]")
{
    SECTION("default constructor initializes to zero")
    {
        fcn::Key key;
        REQUIRE(key.getValue() == 0);
    }

    SECTION("value constructor sets the value")
    {
        fcn::Key key(65);
        REQUIRE(key.getValue() == 65);
    }
}

TEST_CASE("Key isCharacter identifies character keys", "[unit][key]")
{
    SECTION("printable ASCII characters return true")
    {
        REQUIRE(fcn::Key(32).isCharacter() == true);  // space
        REQUIRE(fcn::Key(65).isCharacter() == true);  // A
        REQUIRE(fcn::Key(97).isCharacter() == true);  // a
        REQUIRE(fcn::Key(126).isCharacter() == true); // ~
    }

    SECTION("tab returns true")
    {
        REQUIRE(fcn::Key(9).isCharacter() == true); // tab
    }

    SECTION("extended ASCII characters return true")
    {
        REQUIRE(fcn::Key(162).isCharacter() == true); // cent
        REQUIRE(fcn::Key(255).isCharacter() == true); // yuml
    }

    SECTION("control characters return false")
    {
        REQUIRE(fcn::Key(0).isCharacter() == false);  // null
        REQUIRE(fcn::Key(31).isCharacter() == false); // unit separator
    }

    SECTION("non-printable ASCII returns false")
    {
        REQUIRE(fcn::Key(127).isCharacter() == false); // del
    }

    SECTION("extended ASCII out of range returns false")
    {
        REQUIRE(fcn::Key(161).isCharacter() == false);
        REQUIRE(fcn::Key(256).isCharacter() == false);
    }

    SECTION("negative key values return false")
    {
        REQUIRE(fcn::Key(-1000).isCharacter() == false); // LeftAlt
        REQUIRE(fcn::Key(-1).isCharacter() == false);
    }
}

TEST_CASE("Key isNumber identifies digit keys", "[unit][key]")
{
    SECTION("digit keys return true")
    {
        REQUIRE(fcn::Key(48).isNumber() == true); // 0
        REQUIRE(fcn::Key(49).isNumber() == true); // 1
        REQUIRE(fcn::Key(50).isNumber() == true); // 2
        REQUIRE(fcn::Key(51).isNumber() == true); // 3
        REQUIRE(fcn::Key(52).isNumber() == true); // 4
        REQUIRE(fcn::Key(53).isNumber() == true); // 5
        REQUIRE(fcn::Key(54).isNumber() == true); // 6
        REQUIRE(fcn::Key(55).isNumber() == true); // 7
        REQUIRE(fcn::Key(56).isNumber() == true); // 8
        REQUIRE(fcn::Key(57).isNumber() == true); // 9
    }

    SECTION("non-digit keys return false")
    {
        REQUIRE(fcn::Key(47).isNumber() == false); // /
        REQUIRE(fcn::Key(65).isNumber() == false); // A
    }

    SECTION("negative key values return false")
    {
        REQUIRE(fcn::Key(-1).isNumber() == false);
    }

    SECTION("boundary values return false")
    {
        REQUIRE(fcn::Key(0).isNumber() == false);
        REQUIRE(fcn::Key(58).isNumber() == false);
    }
}

TEST_CASE("Key isLetter identifies alphabetic keys", "[unit][key]")
{
    SECTION("uppercase letters return true")
    {
        REQUIRE(fcn::Key(65).isLetter() == true); // A
        REQUIRE(fcn::Key(90).isLetter() == true); // Z
    }

    SECTION("lowercase letters return true")
    {
        REQUIRE(fcn::Key(97).isLetter() == true);  // a
        REQUIRE(fcn::Key(122).isLetter() == true); // z
    }

    SECTION("extended ASCII letters return true")
    {
        REQUIRE(fcn::Key(192).isLetter() == true); // Agrave
        REQUIRE(fcn::Key(255).isLetter() == true); // yuml
    }

    SECTION("non-letter characters return false")
    {
        REQUIRE(fcn::Key(48).isLetter() == false); // 0
        REQUIRE(fcn::Key(91).isLetter() == false); // [
    }

    SECTION("multiplication sign returns false")
    {
        REQUIRE(fcn::Key(215).isLetter() == false); // multiplication
    }

    SECTION("division sign returns false")
    {
        REQUIRE(fcn::Key(247).isLetter() == false); // division
    }

    SECTION("negative key values return false")
    {
        REQUIRE(fcn::Key(-1000).isLetter() == false); // LeftAlt
    }

    SECTION("boundary values return false")
    {
        REQUIRE(fcn::Key(64).isLetter() == false);  // @
        REQUIRE(fcn::Key(91).isLetter() == false);  // [
        REQUIRE(fcn::Key(96).isLetter() == false);  // `
        REQUIRE(fcn::Key(123).isLetter() == false); // {
    }
}

TEST_CASE("Key equality operators", "[unit][key]")
{
    SECTION("equal keys are equal")
    {
        fcn::Key key1(65);
        fcn::Key key2(65);
        REQUIRE(key1 == key2);
    }

    SECTION("different keys are not equal")
    {
        fcn::Key key1(65);
        fcn::Key key2(66);
        REQUIRE(key1 != key2);
    }

    SECTION("default keys are equal")
    {
        fcn::Key key1;
        fcn::Key key2;
        REQUIRE(key1 == key2);
    }

    SECTION("zero and non-zero are not equal")
    {
        fcn::Key key1;
        fcn::Key key2(65);
        REQUIRE(key1 != key2);
    }
}

TEST_CASE("Key enum values", "[unit][key]")
{
    SECTION("Space enum value")
    {
        REQUIRE(fcn::Key::Space == 32);
    }

    SECTION("Tab enum value")
    {
        REQUIRE(fcn::Key::Tab == 9);
    }

    SECTION("Enter enum value")
    {
        REQUIRE(fcn::Key::Enter == 10);
    }

    SECTION("Escape enum value")
    {
        REQUIRE(fcn::Key::Escape == -1012);
    }

    SECTION("Arrow keys have negative values")
    {
        REQUIRE(fcn::Key::Left == -1026);
        REQUIRE(fcn::Key::Right == -1027);
        REQUIRE(fcn::Key::Up == -1028);
        REQUIRE(fcn::Key::Down == -1029);
    }
}
