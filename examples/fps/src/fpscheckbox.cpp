// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fpscheckbox.hpp"

void FPSCheckBox::draw(fcn::Graphics* graphics)
{
    graphics->setFont(getFont());
    graphics->setColor(getForegroundColor());

    // First draw the caption
    graphics->drawText(mCaption, 0, 0);

    // Calculate the x coordinate for the box
    int const x = getFont()->getWidth(mCaption) + (getHeight() / 2);

    // Push a clip area where the box should draw itself
    graphics->pushClipArea(fcn::Rectangle(x, 0, getWidth() - x, getHeight()));
    drawBox(graphics);
    // Pop the cliparea. VERY IMPORTANT!
    graphics->popClipArea();
}
