#ifndef FCN_VBOX_HPP
#define FCN_VBOX_HPP

#include "fifechan/sizeconstraint.hpp"
#include "fifechan/widgets/axislayoutcontainer.hpp"

namespace fcn
{
    class FCN_CORE_DECLSPEC VBox : public AxisLayoutContainer
    {
    public:
        
        VBox(SizeConstraint *sizeConstraint);
        
        virtual ~VBox();
        
    protected:
                 
        /**
         * Relayouts all children.
         */
        virtual void relayoutChildren();
        
        /**
         * Expands or shrinks the container based on content.
         */
        virtual void adjustSize();
        
        /**
         * Sets position of the added child in this container.
         * 
         * @param added The added child.
         */
        virtual void layoutAddedChild(Widget* added);
        
        /**
         * Sets all children to have the same width as this container.
         */
        virtual void fitChildrenToAxis();
        
        /**
         * @return Total height for all visible children.
         */
        int getVisibleChildrenHeight() const;
        
        /**
         * @return Width of widest child.
         */
        int getMaxVisibleChildWidth() const;
    };
};

#endif