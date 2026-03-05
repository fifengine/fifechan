// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef __FFSCROLLAREA_HPP
#define __FFSCROLLAREA_HPP

#include <fifechan.hpp>

class FFScrollArea : public fcn::ScrollArea, public fcn::KeyListener
{
public:
    FFScrollArea();
    void draw(fcn::Graphics* graphics);
    void keyPressed(fcn::KeyEvent& keyEvent);
};

#endif
