#ifndef FCN_HBOX_HPP
#define FCN_HBOX_HPP

#include "fifechan/sizeconstraint.hpp"
#include "fifechan/widgets/axislayoutcontainer.hpp"

namespace fcn
{
    class FCN_CORE_DECLSPEC HBox : public AxisLayoutContainer
    {
    public:
        HBox(SizeConstraint *sizeConstraint);
        
        virtual ~HBox();
    
    private:
        
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
         * Sets all children to have the same height as this container.
         */
        virtual void fitChildrenToAxis();
        
        /**
         * @return Total width of all visible children.
         */
        int getVisibleChildrenWidth() const;
        
        /**
         * @return Height of highest child.
         */
        int getMaxVisibleChildHeight() const;
    };
};

#endif