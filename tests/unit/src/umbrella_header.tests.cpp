// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Standard library includes
#include <type_traits>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
// Deliberately the umbrella header only: this test file must not include any
// other fifechan header, so it fails to compile when include/fifechan.hpp stops
// covering the installed header list (CMake/HeaderDriftGuard.cmake checks the
// same drift at configure time).
#include "fifechan.hpp"

namespace
{
    // Naming a type only compiles if the umbrella header pulled in its header.
    template <typename T, typename = void>
    struct is_complete : std::false_type
    {
    };

    template <typename T>
    struct is_complete<T, decltype(void(sizeof(T)))> : std::true_type
    {
    };

    template <typename... Ts>
    inline constexpr bool all_complete = (is_complete<Ts>::value && ...);
} // namespace

TEST_CASE("umbrella header exposes every public widget type", "[unit][fifechan]")
{
    STATIC_REQUIRE(
        all_complete<
            fcn::ActivityBar,
            fcn::ActivityBarItem,
            fcn::BarSection,
            fcn::HorizontalBar,
            fcn::MenuBar,
            fcn::MenuItem,
            fcn::MenuPopup,
            fcn::PrimaryPanel,
            fcn::SecondaryPanel,
            fcn::SpeechBubble,
            fcn::StatusBar,
            fcn::Tooltip>);

    STATIC_REQUIRE(
        all_complete<
            fcn::AdjustingContainer,
            fcn::BarGraph,
            fcn::Button,
            fcn::CheckBox,
            fcn::Container,
            fcn::CurveGraph,
            fcn::DropDown,
            fcn::FlowContainer,
            fcn::Icon,
            fcn::IconProgressBar,
            fcn::ImageButton,
            fcn::ImageProgressBar,
            fcn::Label,
            fcn::LineGraph,
            fcn::ListBox,
            fcn::PasswordField,
            fcn::PieGraph,
            fcn::PointGraph,
            fcn::RadioButton,
            fcn::ScrollArea,
            fcn::Slider,
            fcn::Spacer,
            fcn::Tab,
            fcn::TabbedArea,
            fcn::TextBox,
            fcn::TextField,
            fcn::ToggleButton,
            fcn::Window>);
}

TEST_CASE("umbrella header exposes every public core type", "[unit][fifechan]")
{
    STATIC_REQUIRE(
        all_complete<
            fcn::ActionEvent,
            fcn::ClipRectangle,
            fcn::Color,
            fcn::ContainerEvent,
            fcn::ContainerListener,
            fcn::DefaultFont,
            fcn::DragEvent,
            fcn::DropTargetListener,
            fcn::Event,
            fcn::FocusHandler,
            fcn::Font,
            fcn::font::FontLoader,
            fcn::GenericInput,
            fcn::Graphics,
            fcn::Gui,
            fcn::Image,
            fcn::ImageFont,
            fcn::ImageLoader,
            fcn::InputEvent,
            fcn::Input,
            fcn::Key,
            fcn::KeyEvent,
            fcn::KeyInput,
            fcn::KeyListener,
            fcn::ListModel,
            fcn::MouseEvent,
            fcn::MouseInput,
            fcn::MouseListener,
            fcn::Point,
            fcn::Rectangle,
            fcn::SelectionEvent,
            fcn::SelectionListener,
            fcn::Shortcut,
            fcn::Size,
            fcn::Text,
            fcn::TextInputEvent,
            fcn::UTF8StringEditor,
            fcn::VisibilityEventHandler,
            fcn::Widget,
            fcn::WidgetListener>);
}
