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

#ifndef FCN_SPACER_HPP
#define FCN_SPACER_HPP

#include "fifechan/widget.hpp"

namespace fcn
{
    class Graphics;
    class SizeConstraint;
    
    class FCN_CORE_DECLSPEC Spacer : public Widget
    {
    public:
        /**
         * Initializes the spacer which should always have a size constraint.
         * 
         * @param sizeConstraint The desired size constraint.
         */
        Spacer(SizeConstraint* sizeConstraint);

        ~Spacer();
        
        /**
         * Overrides this so that the spacer doesn't draw nothing.
         * 
         * @param graphics A graphics object to draw with.
         */
        virtual void draw(Graphics *graphics);
    };
};

#endif