#ifndef FCN_SIZECONSTRAINT_HPP
#define FCN_SIZECONSTRAINT_HPP

#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * Interface class for size constraints.
     */
    class FCN_CORE_DECLSPEC SizeConstraint
    {
    public:
        
        SizeConstraint() {};
        
        virtual ~SizeConstraint() {};
        
        /**
         * @return Constraint's minimum width.
         */
        virtual int getMinWidth() const = 0;
        
        /**
         * @return Constraint's minimum height.
         */
        virtual int getMinHeight() const = 0;
        
        /**
         * @return Constraint's maximum width.
         */
        virtual int getMaxWidth() const = 0;
        
        /**
         * @return Constraint's maximum height.
         */
        virtual int getMaxHeight() const = 0;
        
        /**
         * Clones this size constraint and returns
         * the clone.
         * 
         * @return A clone of the concrete implementation.
         */
        virtual SizeConstraint* clone() const = 0;
    };
};

#endif