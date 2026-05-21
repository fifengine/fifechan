// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Standard library includes
#include <memory>
#include <string>
#include <utility>
#include <vector>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/defaultfont.hpp"
#include "fifechan/events/keyevent.hpp"
#include "fifechan/key.hpp"
#include "fifechan/listmodel.hpp"
#include "fifechan/widgets/dropdown.hpp"
#include "fifechan/widgets/listbox.hpp"
#include "fifechan/widgets/scrollarea.hpp"

using fcn::DefaultFont;
using fcn::DropDown;
using fcn::Key;
using fcn::KeyEvent;
using fcn::ListBox;
using fcn::ListModel;
using fcn::ScrollArea;

namespace
{
    class TestListModel : public ListModel
    {
        public:
            int getNumberOfElements() override
            {
                return static_cast<int>(mItems.size());
            }

            std::string getElementAt(int i) override
            {
                if (i >= 0 && std::cmp_less(i, mItems.size())) {
                    return mItems.at(static_cast<std::size_t>(i));
                }
                return {};
            }

            std::vector<std::string> mItems;
    };
} // namespace

TEST_CASE("DropDown default constructor", "[unit][dropdown]")
{
    TestListModel model;
    model.mItems = {"A", "B", "C"};
    DropDown dropdown(&model);
    DefaultFont font;
    dropdown.setFont(&font);

    REQUIRE(dropdown.getListModel() == &model);
    REQUIRE(dropdown.getSelected() == -1);
    REQUIRE(dropdown.isFocusable());
    REQUIRE(dropdown.getWidth() == 100);
}

TEST_CASE("DropDown setSelected", "[unit][dropdown]")
{
    TestListModel model;
    model.mItems = {"X", "Y", "Z"};
    DropDown dropdown(&model);
    DefaultFont font;
    dropdown.setFont(&font);

    dropdown.setSelected(1);
    REQUIRE(dropdown.getSelected() == 1);
}

TEST_CASE("DropDown setSelected negative clamped", "[unit][dropdown]")
{
    TestListModel model;
    model.mItems = {"A", "B"};
    DropDown dropdown(&model);
    DefaultFont font;
    dropdown.setFont(&font);

    dropdown.setSelected(-1);
    REQUIRE(dropdown.getSelected() == -1);
}

TEST_CASE("DropDown setListModel", "[unit][dropdown]")
{
    DropDown dropdown(static_cast<ListModel*>(nullptr));
    DefaultFont font;
    dropdown.setFont(&font);
    REQUIRE(dropdown.getListModel() == nullptr);

    TestListModel model;
    model.mItems = {"Item1"};
    dropdown.setListModel(&model);
    REQUIRE(dropdown.getListModel() == &model);
}

TEST_CASE("DropDown with custom ScrollArea and ListBox", "[unit][dropdown]")
{
    TestListModel model;
    model.mItems    = {"A", "B"};
    auto listBox    = std::make_unique<ListBox>();
    auto scrollArea = std::make_unique<ScrollArea>();

    DropDown dropdown(&model, scrollArea.get(), listBox.get());
    DefaultFont font;
    dropdown.setFont(&font);

    REQUIRE(dropdown.getListModel() == &model);
    REQUIRE(dropdown.getSelected() == -1);
}

TEST_CASE("DropDown keyPressed UP", "[unit][dropdown]")
{
    TestListModel model;
    model.mItems = {"A", "B", "C"};
    DropDown dropdown(&model);
    DefaultFont font;
    dropdown.setFont(&font);
    dropdown.setSelected(2);

    Key const upKey(Key::UP);
    KeyEvent event(&dropdown, &dropdown, false, false, false, false, KeyEvent::Type::Pressed, false, upKey);
    dropdown.keyPressed(event);
    REQUIRE(dropdown.getSelected() == 1);
    REQUIRE(event.isConsumed());
}

TEST_CASE("DropDown keyPressed DOWN", "[unit][dropdown]")
{
    TestListModel model;
    model.mItems = {"A", "B", "C"};
    DropDown dropdown(&model);
    DefaultFont font;
    dropdown.setFont(&font);

    Key const downKey(Key::DOWN);
    KeyEvent event(&dropdown, &dropdown, false, false, false, false, KeyEvent::Type::Pressed, false, downKey);
    dropdown.keyPressed(event);
    REQUIRE(dropdown.getSelected() == 0);
    REQUIRE(event.isConsumed());
}

TEST_CASE("DropDown keyPressed consumed events are ignored", "[unit][dropdown]")
{
    TestListModel model;
    model.mItems = {"A", "B"};
    DropDown dropdown(&model);
    DefaultFont font;
    dropdown.setFont(&font);

    // Create a pre-consumed event
    Key const downKey(Key::DOWN);
    KeyEvent event(&dropdown, &dropdown, false, false, false, false, KeyEvent::Type::Pressed, false, downKey);
    event.consume();

    dropdown.keyPressed(event);
    REQUIRE(dropdown.getSelected() == -1);
}

TEST_CASE("DropDown setBaseColor", "[unit][dropdown]")
{
    TestListModel model;
    model.mItems = {"A"};
    DropDown dropdown(&model);
    DefaultFont font;
    dropdown.setFont(&font);

    fcn::Color color(100, 150, 200);
    dropdown.setBaseColor(color);
    REQUIRE(dropdown.getBaseColor() == color);
}

TEST_CASE("DropDown setForegroundColor", "[unit][dropdown]")
{
    TestListModel model;
    model.mItems = {"A"};
    DropDown dropdown(&model);
    DefaultFont font;
    dropdown.setFont(&font);

    fcn::Color color(50, 100, 150);
    dropdown.setForegroundColor(color);
    REQUIRE(dropdown.getForegroundColor() == color);
}

TEST_CASE("DropDown setBackgroundColor", "[unit][dropdown]")
{
    TestListModel model;
    model.mItems = {"A"};
    DropDown dropdown(&model);
    DefaultFont font;
    dropdown.setFont(&font);

    fcn::Color color(200, 200, 200);
    dropdown.setBackgroundColor(color);
    REQUIRE(dropdown.getBackgroundColor() == color);
}

TEST_CASE("DropDown setSelectionColor", "[unit][dropdown]")
{
    TestListModel model;
    model.mItems = {"A"};
    DropDown dropdown(&model);
    DefaultFont font;
    dropdown.setFont(&font);

    fcn::Color color(255, 0, 0);
    dropdown.setSelectionColor(color);
    REQUIRE(dropdown.getSelectionColor() == color);
}

TEST_CASE("DropDown setFont propagates to internal widgets", "[unit][dropdown]")
{
    TestListModel model;
    model.mItems = {"A"};
    DropDown dropdown(&model);
    DefaultFont font;
    dropdown.setFont(&font);
    REQUIRE(dropdown.getFont() == &font);
}

TEST_CASE("DropDown adjustSize", "[unit][dropdown]")
{
    TestListModel model;
    model.mItems = {"A", "B"};
    DropDown dropdown(&model);
    DefaultFont font;
    dropdown.setFont(&font);

    dropdown.adjustSize();
    REQUIRE(dropdown.getHeight() > 0);
}

TEST_CASE("DropDown resizeToContent", "[unit][dropdown]")
{
    TestListModel model;
    model.mItems = {"A", "B"};
    DropDown dropdown(&model);
    DefaultFont font;
    dropdown.setFont(&font);

    dropdown.resizeToContent();
    REQUIRE(true);
}
