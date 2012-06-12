#ifndef FCN_AUTOLAYOUTCONTAINER_HPP
#define FCN_AUTOLAYOUTCONTAINER_HPP

#include "fifechan/containerlistener.hpp"
#include "fifechan/widgetlistener.hpp"
#include "fifechan/widgets/container.hpp"

namespace fcn
{
    /**
     * Base class for container widgets that layout their children automatically.
     */
    class FCN_CORE_DECLSPEC AutoLayoutContainer : public Container,
                                                   public ContainerListener,
                                                   public WidgetListener
    {
    public:
        
        AutoLayoutContainer();
        
        virtual ~AutoLayoutContainer();
        
        /**
         * Sets this container as a widget event listener for the added
         * widget.
         * 
         * @WARNING Must be called from child classes re-implementing this.
         *
         * @param containerEvent The container event.
         */
        virtual void widgetAdded(const ContainerEvent& containerEvent);
        
        /**
         * Removes this container from the removed widget's listeners.
         * 
         * @WARNING Must be called from  child classes re-implementing this.
         * 
         * @param containerEvent The container event.
         */
        virtual void widgetRemoved(const ContainerEvent& containerEvent);
        
        /**
         * Relayouts this container. Call this if you want to explicitly relayout
         * the container.
         */
        virtual void relayout() = 0;
    };
};

#endif