#ifndef FCN_VBOX_HPP
#define FCN_VBOX_HPP

#include "fifechan/widgets/autolayoutcontainer.hpp"

namespace fcn
{
    class FCN_CORE_DECLSPEC VBox : public AutoLayoutContainer
    {
    public:
        
        VBox(SizeConstraint *sizeConstraint);
        
        virtual ~VBox();
        
        /**
         * Called when a widget is added to a container.
         * 
         * @param containerEvent The container event.
         */
        virtual void widgetAdded(const ContainerEvent& containerEvent);
        
        /**
         * Called when a widget is removed from a container.
         * 
         * @param containerEvent The container event.
         */
        virtual void widgetRemoved(const ContainerEvent& containerEvent);
        
        /**
         * Called when a widget's size is changed.
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
         * Relayouts this VBox.
         */
        virtual void relayout();
        
        /**
         * Sets vertical axis padding between aligned widgets.
         * 
         * @param padding The desired padding.
         */
        void setPadding(int padding);
        
        /**
         * Sets starting offset for children in this container.
         * 
         * @param childrenOffset The desired children offset.
         */
        void setChildrenOffset(int childrenOffset);
    private:
          
        /**
         * Relayouts all children.
         */
        void relayoutChildren();
        
        /**
         * Expands or shrinks the container based on content.
         */
        void adjustSize();
        
        /**
         * Sets position of the added child in this container.
         * 
         * @param added The added child.
         */
        void layoutAddedChild(Widget* added);
        
        /**
         * Sets all children to have the same width as this container.
         */
        void fitChildrenToWidth();
        
        /**
         * @return Visible child with biggest width.
         * @retval NULL If container has no children.
         */
        Widget* getWidestVisibleChild() const;
        
        /**
         * Vertical axis padding between children.
         */
        int mPadding;
        
        /**
         * Starting offset for children.
         */
        int mChildrenOffset;
    };
};

#endif