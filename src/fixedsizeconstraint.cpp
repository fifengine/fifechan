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