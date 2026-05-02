// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Corresponding header include
#include "fifechan.hpp"

// Standard library includes
#include <cstring>
#include <string>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

using fcn::fifechanVersion;

TEST_CASE("fifechanVersion returns valid version string", "[fifechan]")
{
    char const * version = fifechanVersion();

    // Version should not be nullptr
    REQUIRE(version != nullptr);

    // Version should be a non-empty string
    REQUIRE(std::strlen(version) > 0);

    // Version should contain the version number
    std::string versionStr(version);
    REQUIRE(versionStr.find("0.2.0") != std::string::npos);
}

TEST_CASE("fifechanVersion returns consistent value", "[fifechan]")
{
    char const * version1 = fifechanVersion();
    char const * version2 = fifechanVersion();

    // Should return same pointer (or at least same string)
    REQUIRE(version1 == version2);
    REQUIRE(std::strcmp(version1, version2) == 0);
}
