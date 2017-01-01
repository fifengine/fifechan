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

#ifndef FCN_FIXEDSIZECONSTRAINT_HPP
#define FCN_FIXEDSIZECONSTRAINT_HPP

#include "fifechan/size.hpp"
#include "fifechan/sizeconstraint.hpp"

namespace fcn
{
    class FCN_CORE_DECLSPEC FixedSizeConstraint : public SizeConstraint
    {
    public:
        
        /**
         * Constructor sets the constraint's minimum and maximum sizes. 
         * 
         * @param minimum Minimum size of the constraint.
         * @param maximum Maximum size of the constraint.
         * @throw Exception If a dimension of minimum size is bigger than the correspondive
         *                     dimension of the maximum.
         */
        FixedSizeConstraint(const Size& minimum, const Size& maximum);
        
        /**
         * Constructor sets the constraint's size. Minimum and maximum size
         * are both set to the same value.
         * 
         * @param size Size of the constraint.
         */
        FixedSizeConstraint(const Size& size);
        
        virtual ~FixedSizeConstraint();
        
        /**
         * @return Minimum width of this size constraint.
         */
        virtual int getMinWidth() const;
        
        /**
         * @return Minimum height of this size constraint.
         */
        virtual int getMinHeight() const;
        
        /**
         * @return Maximum width of this size constraint.
         */
        virtual int getMaxWidth() const;
        
        /**
         * @return Maximum height of this size constraint.
         */
        virtual int getMaxHeight() const;
        
        /**
         * @return A clone of this constraint.
         */
        virtual FixedSizeConstraint* clone() const;
        
    private:
        
        Size mMinimum;
        
        Size mMaximum;
    };
};

#endif