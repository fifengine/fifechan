// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "ffcharacterchooser.hpp"

// Standard library includes
#include <algorithm>

FFCharacterChooser::FFCharacterChooser() : mSelected(0), mDistance(76), mHand(fcn::Image::load("images/hand.png"))
{
    setWidth(20);
    setHeight(240);
    setFocusable(true);
    addKeyListener(this);
    setBorderSize(0);
}

void FFCharacterChooser::draw(fcn::Graphics* graphics)
{
    if (isFocused()) {
        graphics->drawImage(mHand.get(), 0, mDistance * mSelected);
    }
}

int FFCharacterChooser::getSelected() const
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
    if (keyEvent.getKey().getValue() == fcn::UP) {
        --mSelected;
        mSelected = std::max(mSelected, 0);
    } else if (keyEvent.getKey().getValue() == fcn::DOWN) {
        ++mSelected;
        mSelected = std::min(mSelected, 2);
    } else if (keyEvent.getKey().getValue() == fcn::RETURN) {
        distributeActionEvent();
    }
}
