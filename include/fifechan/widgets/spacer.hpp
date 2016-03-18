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