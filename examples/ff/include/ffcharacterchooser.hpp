// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef __FFCHARACTERCHOOSER_HPP
#define __FFCHARACTERCHOOSER_HPP

#include <fifechan.hpp>

class FFCharacterChooser : public fcn::Widget, fcn::KeyListener
{
public:
    FFCharacterChooser();
    ~FFCharacterChooser();
    void draw(fcn::Graphics* graphics);
    int getSelected();
    void setSelected(int selected);
    void setDistance(int distance);
    void keyPressed(fcn::KeyEvent& keyEvent);

private:
    int mSelected;
    int mDistance;
    fcn::Image* mHand;
};

#endif
