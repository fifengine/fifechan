// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Standard library includes
#include <memory>
#include <string>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/events/actionevent.hpp"
#include "fifechan/listeners/actionlistener.hpp"
#include "fifechan/widgets/radiobutton.hpp"

using fcn::ActionEvent;
using fcn::ActionListener;
using fcn::RadioButton;

TEST_CASE("RadioButton default constructor", "[unit][radiobutton]")
{
    RadioButton rb;
    REQUIRE(!rb.isSelected());
    REQUIRE(rb.getGroup().empty());
}

TEST_CASE("RadioButton parameterized constructor", "[unit][radiobutton]")
{
    RadioButton rb("Choice 1", "group1", true);
    REQUIRE(rb.getCaption() == "Choice 1");
    REQUIRE(rb.getGroup() == "group1");
    REQUIRE(rb.isSelected());
}

TEST_CASE("RadioButton group membership mutual exclusion", "[unit][radiobutton]")
{
    RadioButton rb1("A", "g");
    RadioButton rb2("B", "g");
    RadioButton rb3("C", "g");

    REQUIRE((!rb1.isSelected() && !rb2.isSelected() && !rb3.isSelected()));

    rb1.setSelected(true);
    REQUIRE(rb1.isSelected());
    REQUIRE(!rb2.isSelected());
    REQUIRE(!rb3.isSelected());

    rb2.setSelected(true);
    REQUIRE(!rb1.isSelected());
    REQUIRE(rb2.isSelected());
    REQUIRE(!rb3.isSelected());
}

TEST_CASE("RadioButton different groups do not interfere", "[unit][radiobutton]")
{
    RadioButton rb1("A", "group1", true);
    RadioButton rb2("B", "group2", true);

    REQUIRE((rb1.isSelected() && rb2.isSelected()));
}

TEST_CASE("RadioButton setGroup changes group", "[unit][radiobutton]")
{
    RadioButton rb("Item", "oldGroup");
    REQUIRE(rb.getGroup() == "oldGroup");

    rb.setGroup("newGroup");
    REQUIRE(rb.getGroup() == "newGroup");
}

TEST_CASE("RadioButton setSelected(false) does not affect group", "[unit][radiobutton]")
{
    RadioButton rb1("A", "groupUnselect");
    RadioButton rb2("B", "groupUnselect");

    rb1.setSelected(true);
    REQUIRE(rb1.isSelected());

    rb1.setSelected(false);
    REQUIRE(!rb1.isSelected());
    REQUIRE(!rb2.isSelected());
}

TEST_CASE("RadioButton toggleSelected", "[unit][radiobutton]")
{
    RadioButton rb("Tog", "togGroup");
    REQUIRE(!rb.isSelected());

    rb.toggleSelected();
    REQUIRE(rb.isSelected());

    rb.toggleSelected();
    REQUIRE(!rb.isSelected());
}

TEST_CASE("RadioButton destructor removes from group", "[unit][radiobutton]")
{
    auto rb1 = std::make_unique<RadioButton>("A", "groupDel");
    {
        RadioButton rb2("B", "groupDel", true);
        REQUIRE(rb2.isSelected());
    }
    // rb2 destroyed
    rb1->setSelected(true);
    REQUIRE(rb1->isSelected());
}
