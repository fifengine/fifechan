// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "ffscrollarea.hpp"

#include <fifechan.hpp>

FFScrollArea::FFScrollArea()
{
    setScrollPolicy(ScrollPolicy::ShowNever, ScrollPolicy::ShowNever);
    addKeyListener(this);
    setFocusable(false);
    setBorderSize(0);
}

void FFScrollArea::draw(fcn::Graphics* graphics)
{
    graphics->pushClipArea(getContent()->getDimension());
    getContent()->draw(graphics);
    graphics->popClipArea();

    if (getVerticalMaxScroll() != 0) {
        int y = ((getHeight() - 32) * getVerticalScrollAmount()) / getVerticalMaxScroll();

        graphics->setColor(fcn::Color(0x000000));
        graphics->drawRectangle(getWidth() - 11, y, 8, 32);
        graphics->drawRectangle(getWidth() - 10, y + 1, 8, 32);

        graphics->setColor(fcn::Color(0xffffff));

        graphics->fillRectangle(getWidth() - 10, y + 1, 6, 30);
    }
}

void FFScrollArea::keyPressed(fcn::KeyEvent& keyEvent)
{
    if (keyEvent.isConsumed()) {
        return;
    }

    if (keyEvent.getKey().getValue() == fcn::Key::Down) {
        setVerticalScrollAmount(getVerticalScrollAmount() + 16);
    } else if (keyEvent.getKey().getValue() == fcn::Key::Up) {
        setVerticalScrollAmount(getVerticalScrollAmount() - 16);
    }
}
