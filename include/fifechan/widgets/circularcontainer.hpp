#ifndef CIRCULAR_CONTAINER
#define CIRCULAR_CONTAINER

#include <fifechan/widgets/autolayoutcontainer.hpp>

#include<queue>
#include<utility>
#include<vector>

namespace fcn
{
    /**
     * Auto Layouting container that arranges its children in a circular fashion.
     * This type of container has a limited amount of slots so widgets added that do
     * not have a slot available are queued in order to be displayed if a slot gets
     * available later (by removing a widget that owned a slot).
     */
    class FCN_CORE_DECLSPEC CircularContainer : public AutoLayoutContainer
    {
    public:
        
        static const float PI;
        
        /**
         * Constructor.
         * 
         * @param radius Radius of the circular container, effectively making it's
         *                size radius * radius.
         * 
         * @param spacing Spacing between children in rads.
         */
        CircularContainer(int radius, float spacing);
        
        /**
         * Destructor.
         */
        virtual ~CircularContainer();
        
        /**
         * Sets the container's radius, this means that it's children will be relayouted.
         * 
         * @param radius New Radius
         */
        void setRadius(int radius);
        
        // Inherited from AutoLayoutContainer
        
        virtual void widgetAdded(const ContainerEvent& containerEvent);
        
        virtual void widgetRemoved(const ContainerEvent& containerEvent);
        
        virtual void widgetHidden(const Event& event);
        
        virtual void widgetShown(const Event& event);
        
        virtual void relayout();
        
        // Inherited from Container
        
        virtual void add(Widget* widget);
        
        // Inherited from Widget
        
        virtual void setWidth(int width);
        
        virtual void setHeight(int height);
        
    protected:
        
        /**
         * Calculates the available slots on the circular container's circumference.
         */
        void calculateAvailableSlots(int radius);
        
        /**
         * Sets position of the added child in this container.
         * 
         * @param added The added child.
         */
        void layoutAddedChild(Widget* added);
        
        /**
         * Adjusts the position of a child widget in order to keep
         * it in this container's bounds.
         * 
         * @param widget Child widget.
         */
        void keepChildInBounds(Widget* widget);
        
        /**
         * Circular container's radius.
         */
        int mRadius;
        
        /**
         * Spacing between widgets in rads.
         */
        float mSpacing;
        
        /**
         * Vector holding all available slot positions.
         */
        typedef std::pair<int, int> Position;
        
        std::vector<Position> mAvailableSlots;
        
        /**
         * Holds how many slots have already been used.
         */
        int mUsedSlots;
        
        /**
         * Widgets that do no 
         */
        std::queue<Widget*> mQueuedWidgets;
    };
};

#endif