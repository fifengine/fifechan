// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef __fpscheckbox_hpp__
#define __fpscheckbox_hpp__

#include <fifechan.hpp>

#include <string>

/*
 * Here we overload the CheckBox widget to make it draw itself a bit
 * differently. The ordinary CheckBox draws its box to the left and its
 * caption to the right. In this demo we wanted the caption to be drawn
 * to the left and the box to the right.
 * We could of course achieve the same result having no caption and but
 * a label next to the CheckBox. But we wanted to show how to overload
 * widgets to be able to change the apperance of a widget.
 */
class FPSCheckBox : public fcn::CheckBox
{
public:
    /**
     * Constructor.
     *
     * Well this constructor doesn't do much. But what it does is very
     * importent. The CheckBoxs constructor takes an argument, a caption.
     * To be able to inherit from CheckBox we must also have a constructor
     * which takes a caption as argument and then call the super
     * constructor (that is CheckBox constructor). Note that this cannot
     * be done in the constructor execution code but has to be done in
     * the constructors initialization code (becasue CheckBox constructor
     * must run before the execution code in the FPSCheckBox constructor).
     *
     */
    FPSCheckBox(std::string const & caption) : CheckBox(caption) { }

    /*
     * Inherited from Widget
     */
    void draw(fcn::Graphics* graphics);
};

#endif
