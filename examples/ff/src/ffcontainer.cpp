// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "ffcontainer.hpp"

#include <SDL2/SDL.h>

#include <cmath>

int FFContainer::mInstances          = 0;
fcn::Image* FFContainer::mCornerUL   = nullptr;
fcn::Image* FFContainer::mCornerUR   = nullptr;
fcn::Image* FFContainer::mCornerDL   = nullptr;
fcn::Image* FFContainer::mCornerDR   = nullptr;
fcn::Image* FFContainer::mHorizontal = nullptr;
fcn::Image* FFContainer::mVertical   = nullptr;

FFContainer::FFContainer()
{
    if (mInstances == 0) {
        mCornerUL   = fcn::Image::load("images/cornerul.png");
        mCornerUR   = fcn::Image::load("images/cornerur.png");
        mCornerDL   = fcn::Image::load("images/cornerdl.png");
        mCornerDR   = fcn::Image::load("images/cornerdr.png");
        mHorizontal = fcn::Image::load("images/horizontal.png");
        mVertical   = fcn::Image::load("images/vertical.png");
    }

    mInstances++;

    mRealWidth  = 0;
    mRealHeight = 0;
    mTime       = -1;
    mShow       = true;
    Container::setWidth(0);
    Container::setHeight(0);
    mSlideTarget  = 0;
    mCurrentSlide = 0;
    setBorderSize(0);
}

FFContainer::~FFContainer()
{
    mInstances--;

    if (mInstances == 0) {
        delete mCornerUL;
        delete mCornerUR;
        delete mCornerDL;
        delete mCornerDR;
        delete mHorizontal;
        delete mVertical;
    }
}

void FFContainer::draw(fcn::Graphics* graphics)
{
    int i;

    if (isOpaque()) {
        double height = (mRealHeight - 8) / 16.0;
        fcn::Color c(0x7070FF);

        for (i = 0; i < 16; ++i) {
            graphics->setColor(c * (1.0 - i / 18.0));
            graphics->fillRectangle(
                4, static_cast<int>(i * height + 4), getWidth() - 8, static_cast<int>((i * height) + height));
        }
    }

    graphics->pushClipArea(fcn::Rectangle(0, mCurrentSlide, getWidth(), getHeight()));
    for (auto* child : getChildren()) {
        child->_draw(graphics);
    }
    graphics->popClipArea();

    for (i = 5; i < getHeight() - 10; i += 5) {
        graphics->drawImage(mVertical, 0, i);
        graphics->drawImage(mVertical, getWidth() - 4, i);
    }
    graphics->drawImage(mVertical, 0, 0, 0, i, 4, getHeight() - 5 - i);
    graphics->drawImage(mVertical, 0, 0, getWidth() - 4, i, 4, getHeight() - 5 - i);

    for (i = 5; i < getWidth() - 10; i += 5) {
        graphics->drawImage(mHorizontal, i, 0);
        graphics->drawImage(mHorizontal, i, getHeight() - 4);
    }
    graphics->drawImage(mHorizontal, 0, 0, i, 0, getWidth() - 5 - i, 4);
    graphics->drawImage(mHorizontal, 0, 0, i, getHeight() - 4, getWidth() - 5 - i, 4);

    graphics->drawImage(mCornerUL, 0, 0);
    graphics->drawImage(mCornerUR, getWidth() - 5, 0);
    graphics->drawImage(mCornerDL, 0, getHeight() - 5);
    graphics->drawImage(mCornerDR, getWidth() - 5, getHeight() - 5);
}

void FFContainer::logic()
{
    if (mTime < 0) {
        mTime = SDL_GetTicks();
    }

    int deltaTime = SDL_GetTicks() - mTime;
    mTime         = SDL_GetTicks();

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
        if (mCurrentSlide > mSlideTarget) {
            mCurrentSlide = mSlideTarget;
        }
    }

    if (mCurrentSlide > mSlideTarget) {
        mCurrentSlide -= deltaTime;
        if (mCurrentSlide < mSlideTarget) {
            mCurrentSlide = mSlideTarget;
        }
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
    return fcn::Rectangle(0, 0, mRealWidth, mRealHeight);
}
