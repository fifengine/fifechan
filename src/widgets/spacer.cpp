/***************************************************************************
 *   Copyright (C) 2012-2019 by the fifechan team                          *
 *   http://fifechan.github.com/fifechan                                   *
 *   This file is part of fifechan.                                        *
 *                                                                         *
 *   fifechan is free software; you can redistribute it and/or             *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA          *
 ***************************************************************************/

#include "fifechan/widgets/spacer.hpp"
#include "fifechan/exception.hpp"

namespace fcn
{
    Spacer::Spacer() {
        setVerticalExpand(true);
        setHorizontalExpand(true);
    }
    
    Spacer::~Spacer() {
    }
    
    void Spacer::resizeToContent(bool recursiv) {
        setSize(getMinSize().getWidth(), getMinSize().getHeight());
    }

    void Spacer::draw(Graphics *graphics)
    {
    }
};
