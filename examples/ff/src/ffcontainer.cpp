// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#include "ffcontainer.hpp"

#include <SDL2/SDL.h>

#include <algorithm>
#include <cmath>
#include <memory>

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
    int i = 0;

    if (isOpaque()) {
        double const height = (mRealHeight - 8) / 16.0;
        fcn::Color const c(0x7070FF);

        for (i = 0; i < 16; ++i) {
            graphics->setColor(c * (1.0 - (i / 18.0)));
            graphics->fillRectangle(
                4, static_cast<int>((i * height) + 4), getWidth() - 8, static_cast<int>((i * height) + height));
        }
    }

    graphics->pushClipArea(fcn::Rectangle(0, mCurrentSlide, getWidth(), getHeight()));
    for (auto* child : getChildren()) {
        child->_draw(graphics);
    }
    graphics->popClipArea();

    for (i = 5; i < getHeight() - 10; i += 5) {
        graphics->drawImage(mVertical.get(), 0, i);
        graphics->drawImage(mVertical.get(), getWidth() - 4, i);
    }
    graphics->drawImage(mVertical.get(), 0, 0, 0, i, 4, getHeight() - 5 - i);
    graphics->drawImage(mVertical.get(), 0, 0, getWidth() - 4, i, 4, getHeight() - 5 - i);

    for (i = 5; i < getWidth() - 10; i += 5) {
        graphics->drawImage(mHorizontal.get(), i, 0);
        graphics->drawImage(mHorizontal.get(), i, getHeight() - 4);
    }
    graphics->drawImage(mHorizontal.get(), 0, 0, i, 0, getWidth() - 5 - i, 4);
    graphics->drawImage(mHorizontal.get(), 0, 0, i, getHeight() - 4, getWidth() - 5 - i, 4);

    graphics->drawImage(mCornerUL.get(), 0, 0);
    graphics->drawImage(mCornerUR.get(), getWidth() - 5, 0);
    graphics->drawImage(mCornerDL.get(), 0, getHeight() - 5);
    graphics->drawImage(mCornerDR.get(), getWidth() - 5, getHeight() - 5);
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
        Container::setWidth(getWidth() - deltaTime);

        if (getWidth() < 0) {
            Container::setWidth(0);
        }

        Container::setHeight(getHeight() - deltaTime);

        if (getHeight() < 0) {
            Container::setHeight(0);
        }

        if (getHeight() == 0 && getWidth() == 0) {
            Container::setVisible(false);
        }
    } else {
        if (getWidth() < mRealWidth) {
            Container::setWidth(getWidth() + deltaTime);

            if (getWidth() > mRealWidth) {
                Container::setWidth(mRealWidth);
            }
        } else if (getWidth() > mRealWidth) {
            Container::setWidth(getWidth() - deltaTime);

            if (getWidth() < mRealWidth) {
                Container::setWidth(mRealWidth);
            }
        }

        if (getHeight() < mRealHeight) {
            Container::setHeight(getHeight() + deltaTime);

            if (getHeight() > mRealHeight) {
                Container::setHeight(mRealHeight);
            }
        } else if (getHeight() > mRealHeight) {
            Container::setHeight(getHeight() - deltaTime);

            if (getHeight() < mRealHeight) {
                Container::setHeight(mRealHeight);
            }
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
    setPosition(dimension.x, dimension.y);
    setWidth(dimension.width);
    setHeight(dimension.height);
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
    mRealWidth = width;
}

void FFContainer::setHeight(int height)
{
    mRealHeight = height;
}

void FFContainer::slideContentTo(int y)
{
    mSlideTarget = y;
}

fcn::Rectangle FFContainer::getChildrenArea()
{
    return {0, 0, mRealWidth, mRealHeight};
}
