// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/events/containerevent.hpp"

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/widgets/container.hpp"
#include "fifechan/widgets/label.hpp" // Use concrete widget

using fcn::Container;
using fcn::ContainerEvent;
using fcn::Label;

// ============================================================================
// ContainerEvent constructor and getContainer
// ============================================================================

TEST_CASE("ContainerEvent constructor sets source and container", "[unit][containerevent]")
{
    Label sourceWidget;
    Container container;

    ContainerEvent event(&sourceWidget, &container);

    REQUIRE(event.getSource() == &sourceWidget);
    REQUIRE(event.getContainer() == &container);
}

TEST_CASE("ContainerEvent getContainer returns correct container", "[unit][containerevent]")
{
    Label sourceWidget;
    Container container1;
    Container container2;

    ContainerEvent event1(&sourceWidget, &container1);
    ContainerEvent event2(&sourceWidget, &container2);

    REQUIRE(event1.getContainer() == &container1);
    REQUIRE(event2.getContainer() == &container2);
}

// ============================================================================
// ContainerEvent - edge cases
// ============================================================================

TEST_CASE("ContainerEvent with different source widgets", "[unit][containerevent]")
{
    Label source1;
    Label source2;
    Container container;

    ContainerEvent event1(&source1, &container);
    ContainerEvent event2(&source2, &container);

    REQUIRE(event1.getSource() == &source1);
    REQUIRE(event2.getSource() == &source2);
    REQUIRE(event1.getContainer() == &container);
    REQUIRE(event2.getContainer() == &container);
}
