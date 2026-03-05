// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "ffcharacterchooser.hpp"

FFCharacterChooser::FFCharacterChooser()
{
    setWidth(20);
    setHeight(240);
    mSelected = 0;
    mDistance = 76;
    mHand     = fcn::Image::load("images/hand.png");
    setFocusable(true);
    addKeyListener(this);
    setBorderSize(0);
}

FFCharacterChooser::~FFCharacterChooser()
{
    delete mHand;
}

void FFCharacterChooser::draw(fcn::Graphics* graphics)
{
    if (isFocused()) {
        graphics->drawImage(mHand, 0, mDistance * mSelected);
    }
}

int FFCharacterChooser::getSelected()
{
    return mSelected;
}

void FFCharacterChooser::setSelected(int selected)
{
    mSelected = selected;
}

void FFCharacterChooser::setDistance(int distance)
{
    mDistance = distance;
}

void FFCharacterChooser::keyPressed(fcn::KeyEvent& keyEvent)
{
    if (keyEvent.getKey().getValue() == fcn::Key::Up) {
        mSelected--;
        if (mSelected < 0) {
            mSelected = 0;
        }
    } else if (keyEvent.getKey().getValue() == fcn::Key::Down) {
        mSelected++;
        if (mSelected > 2) {
            mSelected = 2;
        }
    } else if (keyEvent.getKey().getValue() == fcn::Key::Enter) {
        distributeActionEvent();
    }
}
