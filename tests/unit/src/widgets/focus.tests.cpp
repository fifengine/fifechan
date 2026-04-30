// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/focushandler.hpp"
#include "fifechan/gui.hpp"
#include "fifechan/mouseinput.hpp"
#include "fifechan/widgets/textfield.hpp"

TEST_CASE("Clicking TextField focuses and sets caret", "[integration][focus]")
{
    class TestGui : public fcn::Gui
    {
        public:
            void callHandleMousePressed(fcn::MouseInput const & mi)
            {
                handleMousePressed(mi);
            }
    };

    // Create GUI
    TestGui gui;
    fcn::TextField tf("Hello");

    // Place the widget at origin so a click at (5,5) is inside it.
    tf.setPosition(0, 0);

    // Register widget as top so focus handling is wired.
    gui.setTop(&tf);

    REQUIRE(tf.isFocusable());
    REQUIRE(!tf.isFocused());
    REQUIRE(tf.getCaretPosition() == 0);

    // TODO MouseAutomation
    // Simulate a left mouse press inside the textfield.
    // With DefaultFont (8px per char), x=13 maps to character index 1 in "Hello".
    fcn::MouseInput mi(fcn::MouseInput::Button::Left, fcn::MouseInput::Type::Pressed, 13, 5, 100);

    gui.callHandleMousePressed(mi);

    // After clicking the TextField it should have focus and the caret moved.
    REQUIRE(tf.isFocused());
    REQUIRE(gui.getFocusHandler()->getFocused() == &tf);
    REQUIRE(tf.getCaretPosition() > 0);
}
