// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Standard library includes
#include <string>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/events/textinputevent.hpp"
#include "fifechan/widgets/label.hpp"

using fcn::Label;
using fcn::TextInputEvent;

TEST_CASE("TextInputEvent default constructor", "[unit][textinputevent]")
{
    Label source;
    TextInputEvent event(&source, &source, false, false, false, false, "hello", TextInputEvent::Type::Input);

    REQUIRE(event.getSource() == &source);
    REQUIRE(event.getDistributor() == &source);
    REQUIRE(event.getText() == "hello");
    REQUIRE(event.getType() == TextInputEvent::Type::Input);
}

TEST_CASE("TextInputEvent with empty text", "[unit][textinputevent]")
{
    Label source;
    TextInputEvent event(&source, nullptr, false, false, false, false, "", TextInputEvent::Type::Input);

    REQUIRE(event.getText().empty());
    REQUIRE(event.getDistributor() == nullptr);
}

TEST_CASE("TextInputEvent with modifiers", "[unit][textinputevent]")
{
    Label source;
    TextInputEvent event(&source, &source, true, true, false, false, "A", TextInputEvent::Type::Input);

    REQUIRE(event.isShiftPressed());
    REQUIRE(event.isControlPressed());
    REQUIRE(!event.isAltPressed());
    REQUIRE(!event.isMetaPressed());
}

TEST_CASE("TextInputEvent with unicode text", "[unit][textinputevent]")
{
    Label source;
    std::string const unicode =
        "\xc3\xa9"
        "foobar";
    TextInputEvent event(&source, &source, false, false, false, false, unicode, TextInputEvent::Type::Input);

    REQUIRE(event.getText() == unicode);
}

TEST_CASE("TextInputEvent consume works via inheritance", "[unit][textinputevent]")
{
    Label source;
    TextInputEvent event(&source, &source, false, false, false, false, "x", TextInputEvent::Type::Input);

    REQUIRE(!event.isConsumed());
    event.consume();
    REQUIRE(event.isConsumed());
}

TEST_CASE("TextInputEvent with long text", "[unit][textinputevent]")
{
    Label source;
    std::string const longText(1000, 'a');
    TextInputEvent event(&source, &source, false, false, false, false, longText, TextInputEvent::Type::Input);

    REQUIRE(event.getText() == longText);
    REQUIRE(event.getText().size() == 1000);
}
