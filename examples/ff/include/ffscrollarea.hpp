// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef FFSCROLLAREA_HPP
#define FFSCROLLAREA_HPP

#include <fifechan.hpp>

class FFScrollArea : public fcn::ScrollArea, public fcn::KeyListener
{
public:
    FFScrollArea();
    ~FFScrollArea() override                      = default;
    FFScrollArea(FFScrollArea const &)            = delete;
    FFScrollArea& operator=(FFScrollArea const &) = delete;
    FFScrollArea(FFScrollArea&&)                  = delete;
    FFScrollArea& operator=(FFScrollArea&&)       = delete;

    void draw(fcn::Graphics* graphics) override;
    void keyPressed(fcn::KeyEvent& keyEvent) override;
};

#endif
