// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

#pragma once

#include <string>

#include "fifechan/font.hpp"
#include "fifechan/graphics.hpp"

// Shared mock font used by multiple unit tests to avoid ODR violations.
class MockFont : public fcn::Font
{
    public:
        bool drawStringCalled = false;
        explicit MockFont(int charWidth = 10, int height = 16) : m_charWidth(charWidth), m_height(height)
        {
        }

        int getWidth(std::string const & text) const override
        {
            return static_cast<int>(text.size()) * m_charWidth;
        }

        int getHeight() const override
        {
            return m_height;
        }

        void drawString(fcn::Graphics* /*graphics*/, std::string const & /*text*/, int /*x*/, int /*y*/) override
        {
            drawStringCalled = true;
        }

    private:
        int m_charWidth;
        int m_height;
};
