// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef __FFCONTAINER_HPP
#define __FFCONTAINER_HPP

#include <fifechan.hpp>

class FFContainer : public fcn::Container
{
public:
    FFContainer();
    ~FFContainer();
    void logic();
    void draw(fcn::Graphics* graphics);
    void setVisible(bool visible);
    void setWidth(int width);
    void setHeight(int width);
    void setDimension(fcn::Rectangle const & dimension);
    void slideContentTo(int y);
    fcn::Rectangle getChildrenArea();

private:
    int mRealWidth;
    int mRealHeight;
    int mSlideTarget;
    int mCurrentSlide;
    int mTime;
    bool mShow;

    static int mInstances;
    static fcn::Image* mCornerUL;
    static fcn::Image* mCornerUR;
    static fcn::Image* mCornerDL;
    static fcn::Image* mCornerDR;
    static fcn::Image* mHorizontal;
    static fcn::Image* mVertical;
};

#endif
