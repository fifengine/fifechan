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