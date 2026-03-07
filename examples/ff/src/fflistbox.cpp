// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fflistbox.hpp"

#include <SDL2/SDL.h>

#include <fifechan.hpp>

int FFListBox::mInstances    = 0;
std::unique_ptr<fcn::Image> FFListBox::mHand = nullptr;

FFListBox::FFListBox()
{
    if (mInstances == 0) {
        mHand = std::unique_ptr<fcn::Image>(fcn::Image::load("images/hand.png"));
    }

    mInstances++;
    setBorderSize(0);
    setWrappingEnabled(true);
}

FFListBox::~FFListBox()
{
    mInstances--;

    if (mInstances == 0) {
        mHand.reset();
    }
}

void FFListBox::draw(fcn::Graphics* graphics)
{
    if (mListModel == nullptr) {
        return;
    }

    graphics->setColor(getForegroundColor());
    graphics->setFont(getFont());

    int i, fontHeight;
    int y = 0;

    fontHeight = getFont()->getHeight();

    /**
     * @todo Check cliprects so we do not have to iterate over elements in the list model
     */
    for (i = 0; i < mListModel->getNumberOfElements(); ++i) {
        graphics->drawText(mListModel->getElementAt(i), 16, y);

        if (i == mSelected) {
            if (isFocused()) {
                graphics->drawImage(mHand.get(), 0, y);
            } else if ((SDL_GetTicks() / 100) & 1) {
                graphics->drawImage(mHand.get(), 0, y);
            }
        }

        y += fontHeight;
    }
}

void FFListBox::setSelected(int i)
{
    if (i >= 0 && i < getListModel()->getNumberOfElements() && getListModel()->getElementAt(i) == "") {
        if (i < getSelected()) {
            i--;
        } else {
            i++;
        }
    }

    ListBox::setSelected(i);
}
