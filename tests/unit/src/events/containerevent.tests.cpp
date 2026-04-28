// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include <fifechan/widgets/container.hpp>

#include <fifechan/events/containerevent.hpp>
#include <fifechan/widget.hpp>

TEST_CASE("ContainerEvent constructors initialize properly", "[unit][containerevent]")
{
    SECTION("empty constructor")
    {
        fcn::ContainerEvent event(nullptr, nullptr);
        REQUIRE(event.getSource() == nullptr);
        REQUIRE(event.getContainer() == nullptr);
    }

    SECTION("with source and container")
    {
        fcn::Widget* source       = reinterpret_cast<fcn::Widget*>(0x1000);
        fcn::Container* container = reinterpret_cast<fcn::Container*>(0x2000);

        fcn::ContainerEvent event(source, container);

        REQUIRE(event.getSource() == source);
        REQUIRE(event.getContainer() == container);
    }

    SECTION("null source with valid container")
    {
        fcn::Container* container = reinterpret_cast<fcn::Container*>(0x3000);
        fcn::ContainerEvent event(nullptr, container);

        REQUIRE(event.getSource() == nullptr);
        REQUIRE(event.getContainer() == container);
    }

    SECTION("valid source with null container")
    {
        fcn::Widget* source = reinterpret_cast<fcn::Widget*>(0x4000);
        fcn::ContainerEvent event(source, nullptr);

        REQUIRE(event.getSource() == source);
        REQUIRE(event.getContainer() == nullptr);
    }
}

TEST_CASE("ContainerEvent getContainer returns correct container", "[unit][containerevent]")
{
    SECTION("null container")
    {
        fcn::ContainerEvent event(nullptr, nullptr);
        REQUIRE(event.getContainer() == nullptr);
    }

    SECTION("valid container")
    {
        fcn::Container* container = reinterpret_cast<fcn::Container*>(0x5678);
        fcn::ContainerEvent event(nullptr, container);
        REQUIRE(event.getContainer() == container);
    }

    SECTION("different containers")
    {
        fcn::Container* container1 = reinterpret_cast<fcn::Container*>(0x1111);
        fcn::Container* container2 = reinterpret_cast<fcn::Container*>(0x2222);
        fcn::Container* container3 = reinterpret_cast<fcn::Container*>(0x3333);

        fcn::ContainerEvent event1(nullptr, container1);
        fcn::ContainerEvent event2(nullptr, container2);
        fcn::ContainerEvent event3(nullptr, container3);

        REQUIRE(event1.getContainer() == container1);
        REQUIRE(event2.getContainer() == container2);
        REQUIRE(event3.getContainer() == container3);
    }
}

TEST_CASE("ContainerEvent inherits from Event", "[unit][containerevent]")
{
    SECTION("getSource works")
    {
        fcn::Widget* source       = reinterpret_cast<fcn::Widget*>(0x9999);
        fcn::Container* container = reinterpret_cast<fcn::Container*>(0xAAAA);

        fcn::ContainerEvent event(source, container);

        // getSource is inherited from Event
        REQUIRE(event.getSource() == source);
    }
}

TEST_CASE("ContainerEvent edge cases", "[unit][containerevent]")
{
    SECTION("same source and container")
    {
        fcn::Widget* widget       = reinterpret_cast<fcn::Widget*>(0x1234);
        fcn::Container* container = reinterpret_cast<fcn::Container*>(0x1234);

        fcn::ContainerEvent event(widget, container);

        REQUIRE(event.getSource() == widget);
        REQUIRE(event.getContainer() == container);
    }

    SECTION("zero address is valid")
    {
        fcn::Container* zeroContainer = reinterpret_cast<fcn::Container*>(0);
        fcn::ContainerEvent event(nullptr, zeroContainer);

        REQUIRE(event.getContainer() == nullptr);
    }

    SECTION("max address is valid")
    {
        fcn::Container* maxContainer = reinterpret_cast<fcn::Container*>(UINTPTR_MAX);
        fcn::ContainerEvent event(nullptr, maxContainer);

        REQUIRE(event.getContainer() == maxContainer);
    }

    SECTION("container and source are different")
    {
        fcn::Widget* source       = reinterpret_cast<fcn::Widget*>(0x5000);
        fcn::Container* container = reinterpret_cast<fcn::Container*>(0x6000);

        fcn::ContainerEvent event(source, container);

        REQUIRE(event.getSource() != event.getContainer());
        REQUIRE(event.getSource() == source);
        REQUIRE(event.getContainer() == container);
    }
}
