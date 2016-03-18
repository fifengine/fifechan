#ifndef FCN_AXISLAYOUTINGCONTAINER_HPP
#define FCN_AXISLAYOUTINGCONTAINER_HPP

#include "fifechan/sizeconstraint.hpp"
#include "fifechan/widgets/autolayoutcontainer.hpp"

namespace fcn
{

    class FCN_CORE_DECLSPEC AxisLayoutContainer : public AutoLayoutContainer
   {
   public:
    
        /**
         * Constructor enforces a size constraint for these types of containers.
         */
        AxisLayoutContainer(SizeConstraint *sizeConstraint);
    
        ~AxisLayoutContainer();
    
        /**
         * Called when a widget is added to a container. If this object is the one
         * that fired the event, then the container should be relayouted.
         * 
         * @param containerEvent The container event.
         */
        virtual void widgetAdded(const ContainerEvent& containerEvent);
    
        /**
         * Called when a widget is removed from a container. If this object is the one
         * that fired the event, then the container should be relayouted.
         */
        virtual void widgetRemoved(const ContainerEvent& containerEvent);
    
        /**
         * Called when a widget is resized.
         * 
         * @param event The widget event.
         */
        virtual void widgetResized(const Event& event);
    
        /**
         * Called when a widget is shown.
         * 
         * @param event The widget event.
         */
        virtual void widgetShown(const Event& event);
    
        /**
         * Called when a widget is hidden.
         * 
         * @param event The widget event.
         */
        virtual void widgetHidden(const Event& event);
    
        /**
         * Relayout the container explicitly.
         */
        virtual void relayout();
        
        /**
         * Sets padding between children.
         * 
         * @param padding The desired padding.
         */
        void setPadding(int padding);
        
        /**
         * Sets starting position of children.
         * 
         * @param childrenOffset The desired children offset.
         */
        void setChildrenOffset(int childrenOffset);
    
    protected:
    
        /**
         * Layouts a child added to the container.
         * 
         * @param added The child added.
         */
        virtual void layoutAddedChild(Widget* added) = 0;
    
        /**
         * Relayouts all children in the container.
         */
        virtual void relayoutChildren() = 0;
    
        /**
         * Resizes the container to fit children.
         */
        virtual void adjustSize() = 0;
    
        /**
         * Sets all children to have the same size at the axis that the
         * container implements.
         */
        virtual void fitChildrenToAxis() = 0;
        
        /**
         * Checks if the size constraint passed is smaller than the
         * size of this containers size constraint in both width and
         * height.
         * 
         * @param checkAgainst Size constraint to do the check against.
         * @throw Exception If checkAgainst is bigger in width or height
         *                  than this container's size constraint.
         */
        void checkSizeConstraint(SizeConstraint* checkAgainst);
        
        /**
         * @return Count of visible children in this container.
         */
        int getVisibleChildrenCount() const;
        
        /**
         * @return Accumulated padding for all visible widgets.
         */
        int getTotalPadding() const;
        
        /**
         * Padding between children.
         */
        int mPadding;
        
        /**
         * Starting children position.
         */
        int mChildrenOffset;
        
        /**
         * A list of children in the order they were added.
         */
         std::list<Widget*> mAddedOrder;
};

}

#endif