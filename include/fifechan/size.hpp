/***************************************************************************
 *   Copyright (C) 2012-2014 by the fifechan team                          *
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

#ifndef FCN_SIZE_HPP
#define FCN_SIZE_HPP

#include "fifechan/platform.hpp"

namespace fcn
{
    class FCN_CORE_DECLSPEC Size
    {
    public:
        
        /**
         * Constructor initializes size values.
         * 
         * @param width Width of size.
         * @param height Height of size.
         */
        Size(int width = 0, int height = 0);
        
        ~Size();
        
        /**
         * @return Width of size.
         */
        int getWidth() const;
        
        /**
         * @return Height of size.
         */
        int getHeight() const;
        
        /**
         * Sets the width of size.
         * 
         * @param width Desired width.
         */
        void setWidth(int width);
        
        /**
         * Sets the height of size.
         * 
         * @param height Desired height.
         */
        void setHeight(int height);
        
    private:
        // width of the size
        int mWidth;
        // height of the size
        int mHeight;
    };
};

#endif
