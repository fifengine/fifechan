// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fflistbox.hpp"

// Standard library includes
#include <memory>

// Third-party library includes
#include <SDL3/SDL.h>

#include <fifechan.hpp>

int FFListBox::mInstances                    = 0;
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

    int i                = 0;
    int y                = 0;
    int const fontHeight = getFont()->getHeight();

    /**
     * @todo Check cliprects so we do not have to iterate over elements in the list model
     */
    for (i = 0; i < mListModel->getNumberOfElements(); ++i) {
        graphics->drawText(mListModel->getElementAt(i), kHandIconOffset, y);

        bool const shouldDrawSelection = isFocused() || (((SDL_GetTicks() / 100U) & 1U) != 0U);
        if (i == mSelected && shouldDrawSelection) {
            graphics->drawImage(mHand.get(), 0, y);
        }

        y += fontHeight;
    }
}

void FFListBox::logic()
{
    ListBox::logic();

    // ListBox::logic() sizes the box to the widest label, but this class
    // draws its labels after the hand icon. Add that offset so the labels
    // are not clipped by the widget bounds. ListBox::logic() recomputes the
    // width from scratch on every call, so adding the offset here stays
    // stable even when the box is laid out more than once per frame.
    if (getListModel() != nullptr) {
        setWidth(getWidth() + kHandIconOffset);
    }
}

void FFListBox::setSelected(int i)
{
    auto* listModel = getListModel();
    if (listModel != nullptr && i >= 0 && i < listModel->getNumberOfElements() && listModel->getElementAt(i).empty()) {
        if (i < getSelected()) {
            i--;
        } else {
            i++;
        }
    }

    ListBox::setSelected(i);
}
