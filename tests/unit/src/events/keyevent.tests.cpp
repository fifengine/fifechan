// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include <fifechan/events/keyevent.hpp>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include <fifechan/widgets/label.hpp>

#include <fifechan/key.hpp>

TEST_CASE("KeyEvent constructors initialize properly", "[unit][keyevent]")
{
    fcn::Label source;
    fcn::Label distributor;

    SECTION("basic constructor")
    {
        fcn::KeyEvent event(
            &source, &distributor, false, false, false, false, fcn::KeyEvent::Type::Pressed, false, fcn::Key(65));

        REQUIRE(event.getType() == fcn::KeyEvent::Type::Pressed);
        REQUIRE(event.isNumericPad() == false);
        REQUIRE(event.getKey().getValue() == 65);
    }

    SECTION("released type with numpad")
    {
        fcn::KeyEvent event(
            &source, &distributor, false, false, false, false, fcn::KeyEvent::Type::Released, true, fcn::Key(65));

        REQUIRE(event.getType() == fcn::KeyEvent::Type::Released);
        REQUIRE(event.isNumericPad() == true);
    }

    SECTION("with source and distributor")
    {
        fcn::KeyEvent event(
            &source, &distributor, true, false, false, false, fcn::KeyEvent::Type::Pressed, false, fcn::Key(65));

        REQUIRE(event.getSource() == &source);
        REQUIRE(event.getDistributor() == &distributor);
    }

    SECTION("with modifiers pressed")
    {
        fcn::KeyEvent event(
            &source, &distributor, true, true, true, true, fcn::KeyEvent::Type::Pressed, false, fcn::Key(65));

        REQUIRE(event.isShiftPressed() == true);
        REQUIRE(event.isControlPressed() == true);
        REQUIRE(event.isAltPressed() == true);
        REQUIRE(event.isMetaPressed() == true);
    }
}

TEST_CASE("KeyEvent getType returns correct type", "[unit][keyevent]")
{
    fcn::Label source;
    fcn::Label distributor;

    SECTION("Pressed type")
    {
        fcn::KeyEvent event(
            &source, &distributor, false, false, false, false, fcn::KeyEvent::Type::Pressed, false, fcn::Key(65));

        REQUIRE(event.getType() == fcn::KeyEvent::Type::Pressed);
    }

    SECTION("Released type")
    {
        fcn::KeyEvent event(
            &source, &distributor, false, false, false, false, fcn::KeyEvent::Type::Released, false, fcn::Key(65));

        REQUIRE(event.getType() == fcn::KeyEvent::Type::Released);
    }
}

TEST_CASE("KeyEvent isNumericPad returns correct value", "[unit][keyevent]")
{
    fcn::Label source;
    fcn::Label distributor;

    SECTION("numeric pad false")
    {
        fcn::KeyEvent event(
            &source, &distributor, false, false, false, false, fcn::KeyEvent::Type::Pressed, false, fcn::Key(65));

        REQUIRE(event.isNumericPad() == false);
    }

    SECTION("numeric pad true")
    {
        fcn::KeyEvent event(
            &source, &distributor, false, false, false, false, fcn::KeyEvent::Type::Pressed, true, fcn::Key(65));

        REQUIRE(event.isNumericPad() == true);
    }
}

TEST_CASE("KeyEvent getKey returns correct key", "[unit][keyevent]")
{
    fcn::Label source;
    fcn::Label distributor;

    SECTION("letter key")
    {
        fcn::KeyEvent event(
            &source, &distributor, false, false, false, false, fcn::KeyEvent::Type::Pressed, false, fcn::Key(65));

        REQUIRE(event.getKey().getValue() == 65);
    }

    SECTION("number key")
    {
        fcn::KeyEvent event(
            &source, &distributor, false, false, false, false, fcn::KeyEvent::Type::Pressed, false, fcn::Key(48));

        REQUIRE(event.getKey().getValue() == 48);
    }

    SECTION("special key")
    {
        fcn::KeyEvent event(
            &source,
            &distributor,
            false,
            false,
            false,
            false,
            fcn::KeyEvent::Type::Pressed,
            false,
            fcn::Key(fcn::ESCAPE));

        REQUIRE(event.getKey().getValue() == fcn::ESCAPE);
    }

    SECTION("tab key")
    {
        fcn::KeyEvent event(
            &source, &distributor, false, false, false, false, fcn::KeyEvent::Type::Pressed, false, fcn::Key(fcn::TAB));

        REQUIRE(event.getKey().getValue() == fcn::TAB);
    }
}

TEST_CASE("KeyEvent inherits from InputEvent", "[unit][keyevent]")
{
    fcn::Label source;
    fcn::Label distributor;

    SECTION("consume works")
    {
        fcn::KeyEvent event(
            &source, &distributor, false, false, false, false, fcn::KeyEvent::Type::Pressed, false, fcn::Key(65));

        REQUIRE(event.isConsumed() == false);
        event.consume();
        REQUIRE(event.isConsumed() == true);
    }

    SECTION("modifier keys work")
    {
        fcn::KeyEvent event(
            &source, &distributor, true, false, false, false, fcn::KeyEvent::Type::Pressed, false, fcn::Key(65));

        REQUIRE(event.isShiftPressed() == true);
    }
}

TEST_CASE("KeyEvent Type enum values", "[unit][keyevent]")
{
    SECTION("enum ordering")
    {
        // cppcheck-suppress knownConditionTrueFalse
        REQUIRE(static_cast<int>(fcn::KeyEvent::Type::Pressed) == 0);
        // cppcheck-suppress knownConditionTrueFalse
        REQUIRE(static_cast<int>(fcn::KeyEvent::Type::Released) == 1);
    }
}

TEST_CASE("KeyEvent edge cases", "[unit][keyevent]")
{
    fcn::Label source;
    fcn::Label distributor;

    SECTION("zero key value")
    {
        fcn::KeyEvent event(
            &source, &distributor, false, false, false, false, fcn::KeyEvent::Type::Pressed, false, fcn::Key(0));

        REQUIRE(event.getKey().getValue() == 0);
    }

    SECTION("negative key value")
    {
        fcn::KeyEvent event(
            &source, &distributor, false, false, false, false, fcn::KeyEvent::Type::Pressed, false, fcn::Key(-1000));

        REQUIRE(event.getKey().getValue() == -1000);
    }

    SECTION("empty source works")
    {
        fcn::KeyEvent event(
            &source, &distributor, false, false, false, false, fcn::KeyEvent::Type::Pressed, false, fcn::Key(65));

        REQUIRE(event.getSource() == &source);
    }
}
