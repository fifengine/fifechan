// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "ffcontainer.hpp"

// Standard library includes
#include <algorithm>
#include <cmath>
#include <memory>

// Third-party library includes
#include <SDL3/SDL.h>

int FFContainer::mInstances                          = 0;
std::unique_ptr<fcn::Image> FFContainer::mCornerUL   = nullptr;
std::unique_ptr<fcn::Image> FFContainer::mCornerUR   = nullptr;
std::unique_ptr<fcn::Image> FFContainer::mCornerDL   = nullptr;
std::unique_ptr<fcn::Image> FFContainer::mCornerDR   = nullptr;
std::unique_ptr<fcn::Image> FFContainer::mHorizontal = nullptr;
std::unique_ptr<fcn::Image> FFContainer::mVertical   = nullptr;

FFContainer::FFContainer() : mRealWidth(0), mRealHeight(0), mSlideTarget(0), mCurrentSlide(0), mTime(-1), mShow(true)
{
    if (mInstances == 0) {
        mCornerUL   = std::unique_ptr<fcn::Image>(fcn::Image::load("images/cornerul.png"));
        mCornerUR   = std::unique_ptr<fcn::Image>(fcn::Image::load("images/cornerur.png"));
        mCornerDL   = std::unique_ptr<fcn::Image>(fcn::Image::load("images/cornerdl.png"));
        mCornerDR   = std::unique_ptr<fcn::Image>(fcn::Image::load("images/cornerdr.png"));
        mHorizontal = std::unique_ptr<fcn::Image>(fcn::Image::load("images/horizontal.png"));
        mVertical   = std::unique_ptr<fcn::Image>(fcn::Image::load("images/vertical.png"));
    }

    mInstances++;

    Container::setWidth(0);
    Container::setHeight(0);
    setBorderSize(0);
}

FFContainer::~FFContainer()
{
    mInstances--;

    if (mInstances == 0) {
        mCornerUL.reset();
        mCornerUR.reset();
        mCornerDL.reset();
        mCornerDR.reset();
        mHorizontal.reset();
        mVertical.reset();
    }
}

void FFContainer::draw(fcn::Graphics* graphics)
{
    int i       = 0;
    int const w = getWidth();
    int const h = getHeight();

    if (w <= 0 || h <= 0) {
        return;
    }

    if (isOpaque() && w > 8 && h > 8) {
        double const height = (h - 8) / 16.0;
        fcn::Color const c(0x7070FF);

        for (i = 0; i < 16; ++i) {
            graphics->setColor(c * (1.0 - (i / 18.0)));
            graphics->fillRectangle(
                4,
                static_cast<int>((i * height) + 4),
                std::max(0, w - 8),
                std::max(0, static_cast<int>((i * height) + height)));
        }
    }

    graphics->pushClipArea(fcn::Rectangle(0, mCurrentSlide, w, h));
    for (auto* child : getChildren()) {
        child->_draw(graphics);
    }
    graphics->popClipArea();

    for (i = 5; i < h - 10; i += 5) {
        graphics->drawImage(mVertical.get(), 0, i);
        graphics->drawImage(mVertical.get(), w - 4, i);
    }
    int const remainingVertical = std::max(0, h - 5 - i);
    if (remainingVertical > 0) {
        graphics->drawImage(mVertical.get(), 0, 0, 0, i, 4, remainingVertical);
        graphics->drawImage(mVertical.get(), 0, 0, w - 4, i, 4, remainingVertical);
    }

    for (i = 5; i < w - 10; i += 5) {
        graphics->drawImage(mHorizontal.get(), i, 0);
        graphics->drawImage(mHorizontal.get(), i, h - 4);
    }
    int const remainingHorizontal = std::max(0, w - 5 - i);
    if (remainingHorizontal > 0) {
        graphics->drawImage(mHorizontal.get(), 0, 0, i, 0, remainingHorizontal, 4);
        graphics->drawImage(mHorizontal.get(), 0, 0, i, h - 4, remainingHorizontal, 4);
    }

    graphics->drawImage(mCornerUL.get(), 0, 0);
    graphics->drawImage(mCornerUR.get(), w - 5, 0);
    graphics->drawImage(mCornerDL.get(), 0, h - 5);
    graphics->drawImage(mCornerDR.get(), w - 5, h - 5);
}

void FFContainer::logic()
{
    if (mTime < 0) {
        mTime = SDL_GetTicks();
    }

    int const currentTime = SDL_GetTicks();
    int const deltaTime   = currentTime - mTime;
    mTime                 = currentTime;

    if (!mShow) {
        // Update the *current* animated size via base setDimension to avoid
        // calling back into FFContainer::setDimension, which manages targets.
        fcn::Rectangle current = Container::getDimension();
        current.width          = std::max(0, getWidth() - deltaTime);
        current.height         = std::max(0, getHeight() - deltaTime);
        Container::setDimension(current);

        if (getHeight() == 0 && getWidth() == 0) {
            Container::setVisible(false);
        }
    } else {
        if (getWidth() < mRealWidth) {
            fcn::Rectangle current = Container::getDimension();
            current.width          = std::min(mRealWidth, getWidth() + deltaTime);
            Container::setDimension(current);
        } else if (getWidth() > mRealWidth) {
            fcn::Rectangle current = Container::getDimension();
            current.width          = std::max(mRealWidth, getWidth() - deltaTime);
            Container::setDimension(current);
        }

        if (getHeight() < mRealHeight) {
            fcn::Rectangle current = Container::getDimension();
            current.height         = std::min(mRealHeight, getHeight() + deltaTime);
            Container::setDimension(current);
        } else if (getHeight() > mRealHeight) {
            fcn::Rectangle current = Container::getDimension();
            current.height         = std::max(mRealHeight, getHeight() - deltaTime);
            Container::setDimension(current);
        }
    }

    if (mCurrentSlide < mSlideTarget) {
        mCurrentSlide += deltaTime;
        mCurrentSlide = std::min(mCurrentSlide, mSlideTarget);
    }

    if (mCurrentSlide > mSlideTarget) {
        mCurrentSlide -= deltaTime;
        mCurrentSlide = std::max(mCurrentSlide, mSlideTarget);
    }

    Container::logic();
}

void FFContainer::setDimension(fcn::Rectangle const & dimension)
{
    fcn::Rectangle current = fcn::Container::getDimension();

    // Only update animation targets when the requested size actually changes.
    // Position-only updates (e.g. setPosition/setX/setY) should not clobber
    // the intended target dimensions with current transient size.
    if (dimension.width != current.width || dimension.height != current.height) {
        mRealWidth  = std::max(0, dimension.width);
        mRealHeight = std::max(0, dimension.height);
    }

    current.x = dimension.x;
    current.y = dimension.y;
    fcn::Container::setDimension(current);
}

void FFContainer::setVisible(bool visible)
{
    mShow = visible;

    if (visible) {
        Container::setVisible(true);
    }
}

void FFContainer::setWidth(int width)
{
    // Width changes requested through the generic widget/layout API should
    // update the current size only. Bypass Widget::setWidth (which delegates
    // to virtual setDimension) to avoid rewriting animation targets.
    fcn::Rectangle current = Container::getDimension();
    current.width          = std::max(0, width);
    Container::setDimension(current);
}

void FFContainer::setHeight(int height)
{
    // See setWidth(). Keep target size management in setDimension().
    fcn::Rectangle current = Container::getDimension();
    current.height         = std::max(0, height);
    Container::setDimension(current);
}

void FFContainer::slideContentTo(int y)
{
    mSlideTarget = y;
}

fcn::Rectangle FFContainer::getChildrenArea()
{
    return {0, 0, mRealWidth, mRealHeight};
}
