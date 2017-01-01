/***************************************************************************
 *   Copyright (c) 2017 by the fifechan team                               *
 *   https://github.com/fifengine/fifechan                                 *
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
 
/*
 * For comments regarding functions please see the header file.
 */

#include "fifechan/exception.hpp"
#include "fifechan/fixedsizeconstraint.hpp"

namespace fcn
{
    FixedSizeConstraint::FixedSizeConstraint(const Size& minimum, const Size& maximum)
    :
    mMinimum(minimum),
    mMaximum(maximum)
    {
        if(minimum.getWidth() > maximum.getWidth() || minimum.getHeight() > maximum.getHeight())
        {
            throw FCN_EXCEPTION("Specified a minimum size that has a bigger dimension than the maximum");
        }
    }
    
    FixedSizeConstraint::FixedSizeConstraint(const Size& size)
    :
    mMinimum(size),
    mMaximum(size)
    {
    }
    
    FixedSizeConstraint::~FixedSizeConstraint()
    {
    }
    
    int FixedSizeConstraint::getMinWidth() const
    {
        return mMinimum.getWidth();
    }
    
    int FixedSizeConstraint::getMinHeight() const
    {
        return mMinimum.getHeight();
    }
    
    int FixedSizeConstraint::getMaxWidth() const
    {
        return mMaximum.getWidth();
    }
    
    int FixedSizeConstraint::getMaxHeight() const 
    {
        return mMaximum.getHeight();
    }
    
    FixedSizeConstraint* FixedSizeConstraint::clone() const
    {
        return new FixedSizeConstraint(mMinimum, mMaximum);
    }
};