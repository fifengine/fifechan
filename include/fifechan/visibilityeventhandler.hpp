#ifndef FCN_VISIBILITYEVENTHANDLER_HPP
#define FCN_VISIBILITYEVENTHANDLER_HPP

#include "fifechan/widgetlistener.hpp"

namespace fcn
{
    class Event;
    class Gui;
    class Widget;
    
    /**
     * Class that handles Widget visiblity changes. Addresses input issues arised
     * when widgets are shown or hidden.
     */
    class FCN_CORE_DECLSPEC VisibilityEventHandler : public WidgetListener
    {
    public:
        
        /**
         * Constructor
         */
        VisibilityEventHandler(Gui* gui);
        
        /**
         * Destructor
         */
        virtual ~VisibilityEventHandler() {};
        
        /**
         * Informs gui that a widget was hidden.
         */
        virtual void widgetHidden(const Event& e);
        
        /**
         * Informs gui that a widget was shown.
         */
        virtual void widgetShown(const Event& e);
        
    protected:

        Gui *mGui;
    };
}

#endif //FCN_VISIBILITYEVENTHANDLER_HPP