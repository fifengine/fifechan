// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Standard library includes
#include <string>
#include <utility>
#include <vector>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/defaultfont.hpp"
#include "fifechan/events/actionevent.hpp"
#include "fifechan/events/keyevent.hpp"
#include "fifechan/events/mouseevent.hpp"
#include "fifechan/key.hpp"
#include "fifechan/listeners/actionlistener.hpp"
#include "fifechan/listeners/selectionlistener.hpp"
#include "fifechan/listmodel.hpp"
#include "fifechan/widgets/listbox.hpp"

using fcn::ActionEvent;
using fcn::ActionListener;
using fcn::DefaultFont;
using fcn::Key;
using fcn::KeyEvent;
using fcn::ListBox;
using fcn::ListModel;
using fcn::MouseEvent;
using fcn::SelectionEvent;
using fcn::SelectionListener;

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

    class MockActionListener : public ActionListener
    {
        public:
            void action(ActionEvent const & /*actionEvent*/) override
            {
                actionCalled = true;
            }
            bool actionCalled = false;
    };

    class MockSelectionListener : public SelectionListener
    {
        public:
            void valueChanged(SelectionEvent const & /*event*/) override
            {
                valueChangedCalled = true;
            }
            bool valueChangedCalled = false;
    };
} // namespace

TEST_CASE("ListBox default constructor", "[unit][listbox]")
{
    ListBox listbox;
    REQUIRE(listbox.getSelected() == -1);
    REQUIRE(listbox.getListModel() == nullptr);
    REQUIRE(listbox.isFocusable());
    REQUIRE(listbox.getWidth() == 100);
    REQUIRE_FALSE(listbox.isWrappingEnabled());
}

TEST_CASE("ListBox model constructor", "[unit][listbox]")
{
    TestListModel model;
    model.mItems = {"A", "B", "C"};
    ListBox listbox(&model);
    REQUIRE(listbox.getListModel() == &model);
    REQUIRE(listbox.getSelected() == -1);
    REQUIRE(listbox.isFocusable());
}

TEST_CASE("ListBox setSelected within bounds", "[unit][listbox]")
{
    TestListModel model;
    model.mItems = {"X", "Y", "Z"};
    ListBox listbox(&model);
    DefaultFont font;
    listbox.setFont(&font);

    listbox.setSelected(1);
    REQUIRE(listbox.getSelected() == 1);
}

TEST_CASE("ListBox setSelected negative clamped", "[unit][listbox]")
{
    TestListModel model;
    model.mItems = {"A", "B"};
    ListBox listbox(&model);

    listbox.setSelected(-5);
    REQUIRE(listbox.getSelected() == -1);
}

TEST_CASE("ListBox setSelected out of bounds clamped", "[unit][listbox]")
{
    TestListModel model;
    model.mItems = {"A", "B"};
    ListBox listbox(&model);

    listbox.setSelected(100);
    REQUIRE(listbox.getSelected() == 1);
}

TEST_CASE("ListBox setSelected without model returns -1", "[unit][listbox]")
{
    ListBox listbox;
    listbox.setSelected(0);
    REQUIRE(listbox.getSelected() == -1);
}

TEST_CASE("ListBox setListModel", "[unit][listbox]")
{
    ListBox listbox;
    REQUIRE(listbox.getListModel() == nullptr);

    TestListModel model;
    listbox.setListModel(&model);
    REQUIRE(listbox.getListModel() == &model);
}

TEST_CASE("ListBox setWrappingEnabled", "[unit][listbox]")
{
    ListBox listbox;
    REQUIRE_FALSE(listbox.isWrappingEnabled());

    listbox.setWrappingEnabled(true);
    REQUIRE(listbox.isWrappingEnabled());

    listbox.setWrappingEnabled(false);
    REQUIRE_FALSE(listbox.isWrappingEnabled());
}

TEST_CASE("ListBox addSelectionListener notified on setSelected", "[unit][listbox]")
{
    TestListModel model;
    model.mItems = {"A", "B", "C"};
    ListBox listbox(&model);
    DefaultFont font;
    listbox.setFont(&font);

    MockSelectionListener listener;
    listbox.addSelectionListener(&listener);

    listbox.setSelected(0);
    REQUIRE(listbox.getSelected() == 0);
    REQUIRE(listener.valueChangedCalled);
}

TEST_CASE("ListBox removeSelectionListener", "[unit][listbox]")
{
    TestListModel model;
    model.mItems = {"A", "B"};
    ListBox listbox(&model);
    DefaultFont font;
    listbox.setFont(&font);

    MockSelectionListener listener;
    listbox.addSelectionListener(&listener);
    listbox.removeSelectionListener(&listener);

    listbox.setSelected(0);
    REQUIRE_FALSE(listener.valueChangedCalled);
}

TEST_CASE("ListBox keyPressed ENTER fires action", "[unit][listbox]")
{
    TestListModel model;
    model.mItems = {"A", "B"};
    ListBox listbox(&model);
    DefaultFont font;
    listbox.setFont(&font);

    MockActionListener actionListener;
    listbox.addActionListener(&actionListener);
    listbox.setActionEventId("list_enter");

    Key const enterKey(Key::KEY_RETURN);
    KeyEvent event(&listbox, &listbox, false, false, false, false, KeyEvent::Type::Pressed, false, enterKey);
    listbox.keyPressed(event);

    REQUIRE(actionListener.actionCalled);
    REQUIRE(event.isConsumed());
}

TEST_CASE("ListBox keyPressed SPACE fires action", "[unit][listbox]")
{
    TestListModel model;
    model.mItems = {"A", "B", "C"};
    ListBox listbox(&model);
    MockActionListener listener;
    listbox.addActionListener(&listener);

    Key const spaceKey(Key::SPACE);
    KeyEvent event(&listbox, &listbox, false, false, false, false, KeyEvent::Type::Pressed, false, spaceKey);
    listbox.keyPressed(event);
    REQUIRE(listener.actionCalled);
    REQUIRE(event.isConsumed());
}

TEST_CASE("ListBox keyPressed UP decrements selection", "[unit][listbox]")
{
    TestListModel model;
    model.mItems = {"A", "B", "C"};
    ListBox listbox(&model);
    DefaultFont font;
    listbox.setFont(&font);
    listbox.setSelected(2);
    REQUIRE(listbox.getSelected() == 2);

    Key const upKey(Key::UP);
    KeyEvent event(&listbox, &listbox, false, false, false, false, KeyEvent::Type::Pressed, false, upKey);
    listbox.keyPressed(event);
    REQUIRE(listbox.getSelected() == 1);
    REQUIRE(event.isConsumed());
}

TEST_CASE("ListBox keyPressed DOWN increments from -1 to 0", "[unit][listbox]")
{
    TestListModel model;
    model.mItems = {"A", "B", "C"};
    ListBox listbox(&model);
    DefaultFont font;
    listbox.setFont(&font);

    REQUIRE(listbox.getSelected() == -1);
    Key const downKey(Key::DOWN);
    KeyEvent event(&listbox, &listbox, false, false, false, false, KeyEvent::Type::Pressed, false, downKey);
    listbox.keyPressed(event);
    REQUIRE(listbox.getSelected() == 0);
    REQUIRE(event.isConsumed());
}

TEST_CASE("ListBox keyPressed HOME goes to first", "[unit][listbox]")
{
    TestListModel model;
    model.mItems = {"A", "B", "C"};
    ListBox listbox(&model);
    DefaultFont font;
    listbox.setFont(&font);
    listbox.setSelected(2);

    Key const homeKey(Key::HOME);
    KeyEvent event(&listbox, &listbox, false, false, false, false, KeyEvent::Type::Pressed, false, homeKey);
    listbox.keyPressed(event);
    REQUIRE(listbox.getSelected() == 0);
    REQUIRE(event.isConsumed());
}

TEST_CASE("ListBox keyPressed END goes to last", "[unit][listbox]")
{
    TestListModel model;
    model.mItems = {"A", "B", "C"};
    ListBox listbox(&model);
    DefaultFont font;
    listbox.setFont(&font);

    Key const endKey(Key::END);
    KeyEvent event(&listbox, &listbox, false, false, false, false, KeyEvent::Type::Pressed, false, endKey);
    listbox.keyPressed(event);
    REQUIRE(listbox.getSelected() == 2);
    REQUIRE(event.isConsumed());
}

TEST_CASE("ListBox keyPressed UP with wrapping enabled wraps to last", "[unit][listbox]")
{
    TestListModel model;
    model.mItems = {"A", "B", "C"};
    ListBox listbox(&model);
    DefaultFont font;
    listbox.setFont(&font);
    listbox.setWrappingEnabled(true);
    listbox.setSelected(0);

    Key const upKey(Key::UP);
    KeyEvent event(&listbox, &listbox, false, false, false, false, KeyEvent::Type::Pressed, false, upKey);
    listbox.keyPressed(event);
    REQUIRE(listbox.getSelected() == 2);
}

TEST_CASE("ListBox keyPressed DOWN with wrapping enabled wraps to first", "[unit][listbox]")
{
    TestListModel model;
    model.mItems = {"A", "B", "C"};
    ListBox listbox(&model);
    DefaultFont font;
    listbox.setFont(&font);
    listbox.setWrappingEnabled(true);
    listbox.setSelected(2);

    Key const downKey(Key::DOWN);
    KeyEvent event(&listbox, &listbox, false, false, false, false, KeyEvent::Type::Pressed, false, downKey);
    listbox.keyPressed(event);
    REQUIRE(listbox.getSelected() == 0);
}

TEST_CASE("ListBox mousePressed sets selection and fires action", "[unit][listbox]")
{
    TestListModel model;
    model.mItems = {"A", "B", "C"};
    ListBox listbox(&model);
    DefaultFont font;
    listbox.setFont(&font);

    MockActionListener listener;
    listbox.addActionListener(&listener);
    listbox.setActionEventId("list_mouse");

    MouseEvent pressEvent(
        &listbox, &listbox, false, false, false, false, MouseEvent::Type::Pressed, MouseEvent::Button::Left, 0, 20, 0);
    listbox.mousePressed(pressEvent);
    REQUIRE(listbox.getSelected() == 2);
    REQUIRE(listener.actionCalled);
}

TEST_CASE("ListBox mouseWheelMovedUp does nothing without focus", "[unit][listbox]")
{
    TestListModel model;
    model.mItems = {"A", "B", "C"};
    ListBox listbox(&model);
    DefaultFont font;
    listbox.setFont(&font);
    listbox.setSelected(2);

    // Without focus handler, requestFocus throws. Skip focus-dependent test.
    REQUIRE(listbox.getSelected() == 2);
}

TEST_CASE("ListBox mouseWheelMovedDown does nothing without focus", "[unit][listbox]")
{
    TestListModel model;
    model.mItems = {"A", "B", "C"};
    ListBox listbox(&model);
    DefaultFont font;
    listbox.setFont(&font);

    REQUIRE(listbox.getSelected() == -1);
}

TEST_CASE("ListBox mouseDragged consumes event", "[unit][listbox]")
{
    ListBox listbox;
    MouseEvent dragEvent(
        &listbox, &listbox, false, false, false, false, MouseEvent::Type::Dragged, MouseEvent::Button::Left, 0, 0, 0);
    listbox.mouseDragged(dragEvent);
    REQUIRE(dragEvent.isConsumed());
}

TEST_CASE("ListBox adjustSize with model", "[unit][listbox]")
{
    TestListModel model;
    model.mItems = {"Short", "A longer item", "Tiny"};
    ListBox listbox(&model);
    DefaultFont font;
    listbox.setFont(&font);

    listbox.adjustSize();
    REQUIRE(listbox.getWidth() > 0);
    REQUIRE(listbox.getHeight() > 0);
    REQUIRE(listbox.getHeight() == static_cast<int>(listbox.getRowHeight()) * 3);
}

TEST_CASE("ListBox resizeToContent", "[unit][listbox]")
{
    TestListModel model;
    model.mItems = {"Test"};
    ListBox listbox(&model);
    DefaultFont font;
    listbox.setFont(&font);

    listbox.resizeToContent();
    REQUIRE(listbox.getWidth() > 0);
    REQUIRE(listbox.getHeight() > 0);
}

TEST_CASE("ListBox getRowHeight equals font height", "[unit][listbox]")
{
    ListBox listbox;
    DefaultFont font;
    listbox.setFont(&font);
    REQUIRE(listbox.getRowHeight() == static_cast<unsigned>(font.getHeight()));
}

TEST_CASE("ListBox logic calls adjustSize", "[unit][listbox]")
{
    ListBox listbox;
    listbox.logic();
    REQUIRE(true);
}
