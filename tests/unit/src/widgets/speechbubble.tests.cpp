// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Standard library includes
#include <type_traits>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/widgets/speechbubble.hpp"

using fcn::SpeechBubble;
using fcn::TailProfile;

TEST_CASE("TailProfile is an aggregate with documented defaults", "[unit][speechbubble]")
{
    TailProfile p;
    REQUIRE(p.tipWidthRatio == 0.0f);
    REQUIRE(p.curvature == 0.0f);
    REQUIRE(p.hook == 0.0f);
    REQUIRE(p.taperExponent == 1.0f);
    REQUIRE(p.jaggedness == 0.0f);
    REQUIRE(p.teeth == 5);
    REQUIRE(p.prongDepth == 0.0f);
    REQUIRE(p.segments == 6);

    STATIC_REQUIRE(std::is_aggregate_v<TailProfile>);
}

TEST_CASE("TailProfile static factories", "[unit][speechbubble]")
{
    REQUIRE(TailProfile::sharp().tipWidthRatio == 0.0f);

    REQUIRE(TailProfile::rounded().tipWidthRatio == 0.3f);

    REQUIRE(TailProfile::curved().curvature == 0.3f);

    TailProfile const wide = TailProfile::wide();
    REQUIRE(wide.tipWidthRatio == 0.55f);
    REQUIRE(wide.taperExponent == 0.8f);

    TailProfile const jagged = TailProfile::jagged();
    REQUIRE(jagged.jaggedness == 0.5f);
    REQUIRE(jagged.taperExponent == 1.0f);

    TailProfile const pronged = TailProfile::pronged();
    REQUIRE(pronged.teeth == 5);
    REQUIRE(pronged.prongDepth == 0.5f);
}

TEST_CASE("TailProfile is reachable as SpeechBubble::TailProfile", "[unit][speechbubble]")
{
    // The nested spelling stays valid for C++ consumers even though the type now
    // lives at namespace scope, which is what SWIG needs in order to wrap it.
    STATIC_REQUIRE(std::is_same_v<SpeechBubble::TailProfile, TailProfile>);

    SpeechBubble::TailProfile p = SpeechBubble::TailProfile::rounded();
    REQUIRE(p.tipWidthRatio == 0.3f);
}

TEST_CASE("SpeechBubble setTailProfile and getTailProfile", "[unit][speechbubble]")
{
    SpeechBubble bubble;

    TailProfile p;
    p.tipWidthRatio = 0.42f;
    p.teeth         = 9;
    bubble.setTailProfile(p);

    TailProfile const got = bubble.getTailProfile();
    REQUIRE(got.tipWidthRatio == 0.42f);
    REQUIRE(got.teeth == 9);
}

TEST_CASE("SpeechBubble defaults", "[unit][speechbubble]")
{
    SpeechBubble bubble;
    REQUIRE(bubble.getCornerRadius() == 10);
    REQUIRE(bubble.getTailWidth() == 16);
    REQUIRE(bubble.getTailHeight() == 12);
    REQUIRE(bubble.getBubbleStyle() == SpeechBubble::BubbleStyle::Classic);
    REQUIRE(bubble.getTailDirection() == SpeechBubble::TailDirection::Auto);
}
