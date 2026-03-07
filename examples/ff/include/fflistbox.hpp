// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef __FFLISTBOX_HPP
#define __FFLISTBOX_HPP

#include <memory>

#include <fifechan.hpp>

class FFListBox : public fcn::ListBox
{
public:
    FFListBox();
    ~FFListBox();
    void draw(fcn::Graphics* graphics);
    void setSelected(int i);

private:
    static std::unique_ptr<fcn::Image> mHand;
    static int mInstances;
};

#endif
