// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/gui.hpp"

#include "fifechan/deathlistener.hpp"
#include "fifechan/exception.hpp"
#include "fifechan/focushandler.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/input.hpp"
#include "fifechan/keyevent.hpp"
#include "fifechan/keyinput.hpp"
#include "fifechan/keylistener.hpp"
#include "fifechan/mouseevent.hpp"
#include "fifechan/mouseinput.hpp"
#include "fifechan/mouselistener.hpp"
#include "fifechan/rectangle.hpp"
#include "fifechan/visibilityeventhandler.hpp"
#include "fifechan/widget.hpp"

namespace fcn
{
    class GuiDeathListener : public DeathListener
    {
    public:
        explicit GuiDeathListener(Gui* gui) : mGui(gui) { }
        ~GuiDeathListener() override = default;

        void death(Event const & event) override
        {
            mGui->widgetDied(event.getSource());
        }

        // Delete copy constructor and copy assignment operator
        GuiDeathListener(GuiDeathListener const &)            = delete;
        GuiDeathListener& operator=(GuiDeathListener const &) = delete;

        // Delete move constructor and move assignment operator
        GuiDeathListener(GuiDeathListener&&)            = delete;
        GuiDeathListener& operator=(GuiDeathListener&&) = delete;

    private:
        Gui* mGui;
    };

    Gui::Gui() :
        mTop(nullptr),
        mGraphics(nullptr),
        mInput(nullptr),
        mTabbing(true),
        mFocusHandler(new FocusHandler()),
        mVisibilityEventHandler(new VisibilityEventHandler(this)),
        mShiftPressed(false),
        mMetaPressed(false),
        mControlPressed(false),
        mAltPressed(false),
        mLastMousePressButton(0),
        mLastMousePressTimeStamp(0),
        mLastMouseX(0),
        mLastMouseY(0),
        mClickCount(1),
        mLastMouseDragButton(0),
        mDeathListener(new GuiDeathListener(this))
    {
        Widget::_setVisibilityEventHandler(mVisibilityEventHandler);

        Widget::_setGuiDeathListener(mDeathListener);
    }

    Gui::~Gui()
    {
        if (Widget::widgetExists(mTop)) {
            setTop(nullptr);
        }
        Widget::_setVisibilityEventHandler(nullptr);
        Widget::_setGuiDeathListener(nullptr);

        delete mFocusHandler;
        delete mVisibilityEventHandler;
        delete mDeathListener;
    }

    void Gui::setTop(Widget* top)
    {
        if (mTop != nullptr) {
            mTop->_setFocusHandler(nullptr);
        }
        if (top != nullptr) {
            top->_setFocusHandler(mFocusHandler);
        }

        mTop = top;
    }

    Widget* Gui::getTop() const
    {
        return mTop;
    }

    void Gui::setGraphics(Graphics* graphics)
    {
        mGraphics = graphics;
    }

    Graphics* Gui::getGraphics() const
    {
        return mGraphics;
    }

    void Gui::setInput(Input* input)
    {
        mInput = input;
    }

    Input* Gui::getInput() const
    {
        return mInput;
    }

    void Gui::logic()
    {
        if (mTop == nullptr) {
            fcn::throwException("No top widget set", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        handleModalFocus();
        handleModalMouseInputFocus();

        if (mInput != nullptr) {
            mInput->_pollInput();

            handleKeyInput();
            handleMouseInput();
        }

        mTop->_logic();

        handleHiddenWidgets();
        handleShownWidgets();
    }

    void Gui::draw()
    {
        if (mTop == nullptr) {
            fcn::throwException("No top widget set", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        if (mGraphics == nullptr) {
            fcn::throwException("No graphics set", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        if (!mTop->isVisible()) {
            return;
        }

        mGraphics->_beginDraw();
        mTop->_draw(mGraphics);
        mGraphics->_endDraw();
    }

    void Gui::focusNone()
    {
        mFocusHandler->focusNone();
    }

    void Gui::setTabbingEnabled(bool tabbing)
    {
        mTabbing = tabbing;
    }

    bool Gui::isTabbingEnabled()
    {
        return mTabbing;
    }

    void Gui::addGlobalKeyListener(KeyListener* keyListener)
    {
        mKeyListeners.push_back(keyListener);
    }

    void Gui::removeGlobalKeyListener(KeyListener* keyListener)
    {
        mKeyListeners.remove(keyListener);
    }

    void Gui::enqueueHiddenWidget(Widget* widget)
    {
        mHiddenWidgets.push(widget);
    }

    void Gui::enqueueShownWidget(Widget* widget)
    {
        mShownWidgets.push(widget);
    }

    void Gui::widgetDied(Widget* widget)
    {
        std::queue<Widget*> tmp;
        while (!mShownWidgets.empty()) {
            Widget* shownWidget = mShownWidgets.front();
            if (shownWidget != widget) {
                tmp.push(shownWidget);
            }
            mShownWidgets.pop();
        }
        mShownWidgets = tmp;

        tmp = std::queue<Widget*>();
        while (!mHiddenWidgets.empty()) {
            Widget* hiddenWidget = mHiddenWidgets.front();
            if (hiddenWidget != widget) {
                tmp.push(hiddenWidget);
            }
            mHiddenWidgets.pop();
        }
        mHiddenWidgets = tmp;
    }

    void Gui::handleMouseInput()
    {
        while (!mInput->isMouseQueueEmpty()) {
            MouseInput const mouseInput = mInput->dequeueMouseInput();

            switch (mouseInput.getType()) {
            case MouseInput::Pressed:
                handleMousePressed(mouseInput);
                break;
            case MouseInput::Released:
                handleMouseReleased(mouseInput);
                break;
            case MouseInput::Moved:
                handleMouseMoved(mouseInput);
                break;
            case MouseInput::WheelMovedDown:
                handleMouseWheelMovedDown(mouseInput);
                break;
            case MouseInput::WheelMovedUp:
                handleMouseWheelMovedUp(mouseInput);
                break;
            case MouseInput::WheelMovedRight:
                handleMouseWheelMovedRight(mouseInput);
                break;
            case MouseInput::WheelMovedLeft:
                handleMouseWheelMovedLeft(mouseInput);
                break;
            default:
                fcn::throwException(
                    "Unknown mouse input type.", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
                break;
            }

            // Save the current mouse state. It's needed to send
            // mouse exited events and mouse entered events when
            // the mouse exits a widget and when a widget releases
            // modal mouse input focus.
            mLastMouseX = mouseInput.getX();
            mLastMouseY = mouseInput.getY();
        }
    }

    void Gui::handleKeyInput()
    {
        while (!mInput->isKeyQueueEmpty()) {
            KeyInput const keyInput = mInput->dequeueKeyInput();

            // Save modifiers state
            mShiftPressed   = keyInput.isShiftPressed();
            mMetaPressed    = keyInput.isMetaPressed();
            mControlPressed = keyInput.isControlPressed();
            mAltPressed     = keyInput.isAltPressed();

            KeyEvent keyEventToGlobalKeyListeners(
                nullptr,
                nullptr,
                mShiftPressed,
                mControlPressed,
                mAltPressed,
                mMetaPressed,
                keyInput.getType(),
                keyInput.isNumericPad(),
                keyInput.getKey());

            distributeKeyEventToGlobalKeyListeners(keyEventToGlobalKeyListeners);

            // If a global key listener consumes the event it will not be
            // sent further to the source of the event.
            if (keyEventToGlobalKeyListeners.isConsumed()) {
                continue;
            }

            bool keyEventConsumed = false;

            // Send key inputs to the focused widgets
            if (mFocusHandler->getFocused() != nullptr) {
                Widget* source = getKeyEventSource();
                KeyEvent keyEvent(
                    source,
                    source,
                    mShiftPressed,
                    mControlPressed,
                    mAltPressed,
                    mMetaPressed,
                    keyInput.getType(),
                    keyInput.isNumericPad(),
                    keyInput.getKey());

                if (!mFocusHandler->getFocused()->isFocusable()) {
                    mFocusHandler->focusNone();
                } else {
                    distributeKeyEvent(keyEvent);
                }

                keyEventConsumed = keyEvent.isConsumed();
            }

            // If the key event hasn't been consumed and
            // tabbing is enable check for tab press and
            // change focus.
            if (!keyEventConsumed && mTabbing && keyInput.getKey().getValue() == Key::Tab &&
                keyInput.getType() == KeyInput::Pressed) {
                if (keyInput.isShiftPressed()) {
                    mFocusHandler->tabPrevious();
                } else {
                    mFocusHandler->tabNext();
                }
            }
        }
    }

    void Gui::handleMouseMoved(MouseInput const & mouseInput)
    {
        // Get tha last widgets with the mouse using the
        // last known mouse position.
        std::set<Widget*> const mLastWidgetsWithMouse = getWidgetsAt(mLastMouseX, mLastMouseY);

        // Check if the mouse has left the application window.
        if (mouseInput.getX() < 0 || mouseInput.getY() < 0 ||
            !mTop->getDimension().isContaining(mouseInput.getX(), mouseInput.getY())) {
            std::set<Widget*>::const_iterator iter;
            for (iter = mLastWidgetsWithMouse.begin(); iter != mLastWidgetsWithMouse.end(); iter++) {
                distributeMouseEvent(
                    (*iter),
                    MouseEvent::Type::Exited,
                    static_cast<MouseEvent::Button>(mouseInput.getButton()),
                    mouseInput.getX(),
                    mouseInput.getY(),
                    true,
                    true);
            }
        } else {
            // The mouse is in the application window.

            // Calculate which widgets should receive a mouse exited event
            // and which should receive a mouse entered event by using the
            // last known mouse position and the latest mouse position.
            std::set<Widget*> const mWidgetsWithMouse = getWidgetsAt(mouseInput.getX(), mouseInput.getY());
            std::set<Widget*> mWidgetsWithMouseExited;
            std::set<Widget*> mWidgetsWithMouseEntered;
            std::set_difference(
                mLastWidgetsWithMouse.begin(),
                mLastWidgetsWithMouse.end(),
                mWidgetsWithMouse.begin(),
                mWidgetsWithMouse.end(),
                std::inserter(mWidgetsWithMouseExited, mWidgetsWithMouseExited.begin()));
            std::set_difference(
                mWidgetsWithMouse.begin(),
                mWidgetsWithMouse.end(),
                mLastWidgetsWithMouse.begin(),
                mLastWidgetsWithMouse.end(),
                std::inserter(mWidgetsWithMouseEntered, mWidgetsWithMouseEntered.begin()));

            std::set<Widget*>::const_iterator iter;
            for (iter = mWidgetsWithMouseExited.begin(); iter != mWidgetsWithMouseExited.end(); iter++) {
                distributeMouseEvent(
                    (*iter),
                    MouseEvent::Type::Exited,
                    static_cast<MouseEvent::Button>(mouseInput.getButton()),
                    mouseInput.getX(),
                    mouseInput.getY(),
                    true,
                    true);
                // As the mouse has exited a widget we need
                // to reset the click count and the last mouse
                // press time stamp.
                mClickCount              = 1;
                mLastMousePressTimeStamp = 0;
            }

            for (iter = mWidgetsWithMouseEntered.begin(); iter != mWidgetsWithMouseEntered.end(); iter++) {
                Widget* widget = (*iter);
                // If a widget has modal mouse input focus we
                // only want to send entered events to that widget
                // and the widget's parents.
                if ((mFocusHandler->getModalMouseInputFocused() != nullptr && widget->isModalMouseInputFocused()) ||
                    mFocusHandler->getModalMouseInputFocused() == nullptr) {
                    distributeMouseEvent(
                        widget,
                        MouseEvent::Type::Entered,
                        static_cast<MouseEvent::Button>(mouseInput.getButton()),
                        mouseInput.getX(),
                        mouseInput.getY(),
                        true,
                        true);
                }
            }
        }

        if (mFocusHandler->getDraggedWidget() != nullptr) {
            distributeMouseEvent(
                mFocusHandler->getDraggedWidget(),
                MouseEvent::Type::Dragged,
                static_cast<MouseEvent::Button>(mLastMouseDragButton),
                mouseInput.getX(),
                mouseInput.getY());
        } else {
            Widget* sourceWidget = getMouseEventSource(mouseInput.getX(), mouseInput.getY());
            distributeMouseEvent(
                sourceWidget,
                MouseEvent::Type::Moved,
                static_cast<MouseEvent::Button>(mouseInput.getButton()),
                mouseInput.getX(),
                mouseInput.getY());
        }
    }

    void Gui::handleMousePressed(MouseInput const & mouseInput)
    {
        Widget* sourceWidget = getMouseEventSource(mouseInput.getX(), mouseInput.getY());

        if (mFocusHandler->getDraggedWidget() != nullptr) {
            sourceWidget = mFocusHandler->getDraggedWidget();
        }

        int sourceWidgetX = 0;
        int sourceWidgetY = 0;
        sourceWidget->getAbsolutePosition(sourceWidgetX, sourceWidgetY);

        if ((mFocusHandler->getModalFocused() != nullptr && sourceWidget->isModalFocused()) ||
            mFocusHandler->getModalFocused() == nullptr) {
            sourceWidget->requestFocus();
        }

        if (mouseInput.getTimeStamp() - mLastMousePressTimeStamp < 250 &&
            mLastMousePressButton == mouseInput.getButton()) {
            mClickCount++;
        } else {
            mClickCount = 1;
        }

        distributeMouseEvent(
            sourceWidget,
            MouseEvent::Type::Pressed,
            static_cast<MouseEvent::Button>(mouseInput.getButton()),
            mouseInput.getX(),
            mouseInput.getY());

        mFocusHandler->setLastWidgetPressed(sourceWidget);

        mFocusHandler->setDraggedWidget(sourceWidget);
        mLastMouseDragButton = static_cast<int>(mouseInput.getButton());

        mLastMousePressButton    = mouseInput.getButton();
        mLastMousePressTimeStamp = mouseInput.getTimeStamp();
    }

    void Gui::handleMouseWheelMovedDown(MouseInput const & mouseInput)
    {
        Widget* sourceWidget = getMouseEventSource(mouseInput.getX(), mouseInput.getY());

        if (mFocusHandler->getDraggedWidget() != nullptr) {
            sourceWidget = mFocusHandler->getDraggedWidget();
        }

        int sourceWidgetX = 0;
        int sourceWidgetY = 0;
        sourceWidget->getAbsolutePosition(sourceWidgetX, sourceWidgetY);

        distributeMouseEvent(
            sourceWidget,
            MouseEvent::Type::WheelMovedDown,
            static_cast<MouseEvent::Button>(mouseInput.getButton()),
            mouseInput.getX(),
            mouseInput.getY());
    }

    void Gui::handleMouseWheelMovedUp(MouseInput const & mouseInput)
    {
        Widget* sourceWidget = getMouseEventSource(mouseInput.getX(), mouseInput.getY());

        if (mFocusHandler->getDraggedWidget() != nullptr) {
            sourceWidget = mFocusHandler->getDraggedWidget();
        }

        int sourceWidgetX = 0;
        int sourceWidgetY = 0;
        sourceWidget->getAbsolutePosition(sourceWidgetX, sourceWidgetY);

        distributeMouseEvent(
            sourceWidget,
            MouseEvent::Type::WheelMovedUp,
            static_cast<MouseEvent::Button>(mouseInput.getButton()),
            mouseInput.getX(),
            mouseInput.getY());
    }

    void Gui::handleMouseWheelMovedRight(MouseInput const & mouseInput)
    {
        Widget* sourceWidget = getMouseEventSource(mouseInput.getX(), mouseInput.getY());

        if (mFocusHandler->getDraggedWidget() != nullptr) {
            sourceWidget = mFocusHandler->getDraggedWidget();
        }

        int sourceWidgetX = 0;
        int sourceWidgetY = 0;
        sourceWidget->getAbsolutePosition(sourceWidgetX, sourceWidgetY);

        distributeMouseEvent(
            sourceWidget,
            MouseEvent::Type::WheelMovedRight,
            static_cast<MouseEvent::Button>(mouseInput.getButton()),
            mouseInput.getX(),
            mouseInput.getY());
    }

    void Gui::handleMouseWheelMovedLeft(MouseInput const & mouseInput)
    {
        Widget* sourceWidget = getMouseEventSource(mouseInput.getX(), mouseInput.getY());

        if (mFocusHandler->getDraggedWidget() != nullptr) {
            sourceWidget = mFocusHandler->getDraggedWidget();
        }

        int sourceWidgetX = 0;
        int sourceWidgetY = 0;
        sourceWidget->getAbsolutePosition(sourceWidgetX, sourceWidgetY);

        distributeMouseEvent(
            sourceWidget,
            MouseEvent::Type::WheelMovedLeft,
            static_cast<MouseEvent::Button>(mouseInput.getButton()),
            mouseInput.getX(),
            mouseInput.getY());
    }

    void Gui::handleMouseReleased(MouseInput const & mouseInput)
    {
        Widget* sourceWidget = getMouseEventSource(mouseInput.getX(), mouseInput.getY());

        if (mFocusHandler->getDraggedWidget() != nullptr) {
            if (sourceWidget != mFocusHandler->getLastWidgetPressed()) {
                mFocusHandler->setLastWidgetPressed(nullptr);
            }

            sourceWidget = mFocusHandler->getDraggedWidget();
        }

        int sourceWidgetX = 0;
        int sourceWidgetY = 0;
        sourceWidget->getAbsolutePosition(sourceWidgetX, sourceWidgetY);

        distributeMouseEvent(
            sourceWidget,
            MouseEvent::Type::Released,
            static_cast<MouseEvent::Button>(mouseInput.getButton()),
            mouseInput.getX(),
            mouseInput.getY());

        if (mouseInput.getButton() == mLastMousePressButton && mFocusHandler->getLastWidgetPressed() == sourceWidget) {
            distributeMouseEvent(
                sourceWidget,
                MouseEvent::Type::Clicked,
                static_cast<MouseEvent::Button>(mouseInput.getButton()),
                mouseInput.getX(),
                mouseInput.getY());

            mFocusHandler->setLastWidgetPressed(nullptr);
        } else {
            mLastMousePressButton = 0;
            mClickCount           = 0;
        }

        if (mFocusHandler->getDraggedWidget() != nullptr) {
            mFocusHandler->setDraggedWidget(nullptr);
        }
    }

    Widget* Gui::getWidgetAt(int x, int y, Widget* exclude)
    {
        // If the widget's parent has no child then we have found the widget..
        Widget* parent = mTop;
        Widget* child  = mTop;

        while (child != nullptr) {
            Widget* swap = child;
            int parentX  = 0;
            int parentY  = 0;
            parent->getAbsolutePosition(parentX, parentY);
            child  = parent->getWidgetAt(x - parentX, y - parentY, exclude);
            parent = swap;
        }

        return parent;
    }

    std::set<Widget*> Gui::getWidgetsAt(int x, int y)
    {
        std::set<Widget*> result;

        Widget* widget = mTop;

        while (widget != nullptr) {
            result.insert(widget);
            int absoluteX = 0;
            int absoluteY = 0;
            widget->getAbsolutePosition(absoluteX, absoluteY);
            widget = widget->getWidgetAt(x - absoluteX, y - absoluteY);
        }

        return result;
    }

    Widget* Gui::getMouseEventSource(int x, int y)
    {
        Widget* widget = getWidgetAt(x, y);

        if (mFocusHandler->getModalMouseInputFocused() != nullptr && !widget->isModalMouseInputFocused()) {
            return mFocusHandler->getModalMouseInputFocused();
        }

        return widget;
    }

    Widget* Gui::getKeyEventSource()
    {
        Widget* widget = mFocusHandler->getFocused();

        while (widget->_getInternalFocusHandler() != nullptr &&
               widget->_getInternalFocusHandler()->getFocused() != nullptr) {
            widget = widget->_getInternalFocusHandler()->getFocused();
        }

        return widget;
    }

    void Gui::distributeMouseEvent(
        Widget* source, MouseEvent::Type type, MouseEvent::Button button, int x, int y, bool force, bool toSourceOnly)
    {
        Widget* parent = source;
        Widget* widget = source;

        if (mFocusHandler->getModalFocused() != nullptr && !widget->isModalFocused() && !force) {
            return;
        }

        if (mFocusHandler->getModalMouseInputFocused() != nullptr && !widget->isModalMouseInputFocused() && !force) {
            return;
        }

        MouseEvent mouseEvent(
            source, source, mShiftPressed, mControlPressed, mAltPressed, mMetaPressed, type, button, x, y, mClickCount);

        while (parent != nullptr) {
            // If the widget has been removed due to input
            // cancel the distribution.
            if (!Widget::widgetExists(widget)) {
                break;
            }

            parent = widget->getParent();

            if (widget->isEnabled() || force) {
                int widgetX = 0;
                int widgetY = 0;
                widget->getAbsolutePosition(widgetX, widgetY);

                mouseEvent.mX                            = x - widgetX;
                mouseEvent.mY                            = y - widgetY;
                mouseEvent.mDistributor                  = widget;
                std::list<MouseListener*> mouseListeners = widget->_getMouseListeners();

                // Send the event to all mouse listeners of the widget.
                for (auto& mouseListener : mouseListeners) {
                    switch (mouseEvent.getType()) {
                    case MouseEvent::Type::Entered:
                        mouseListener->mouseEntered(mouseEvent);
                        break;
                    case MouseEvent::Type::Exited:
                        mouseListener->mouseExited(mouseEvent);
                        break;
                    case MouseEvent::Type::Moved:
                        mouseListener->mouseMoved(mouseEvent);
                        break;
                    case MouseEvent::Type::Pressed:
                        mouseListener->mousePressed(mouseEvent);
                        break;
                    case MouseEvent::Type::Released:
                        mouseListener->mouseReleased(mouseEvent);
                        break;
                    case MouseEvent::Type::WheelMovedUp:
                        mouseListener->mouseWheelMovedUp(mouseEvent);
                        break;
                    case MouseEvent::Type::WheelMovedDown:
                        mouseListener->mouseWheelMovedDown(mouseEvent);
                        break;
                    case MouseEvent::Type::WheelMovedRight:
                        mouseListener->mouseWheelMovedRight(mouseEvent);
                        break;
                    case MouseEvent::Type::WheelMovedLeft:
                        mouseListener->mouseWheelMovedLeft(mouseEvent);
                        break;
                    case MouseEvent::Type::Dragged:
                        mouseListener->mouseDragged(mouseEvent);
                        break;
                    case MouseEvent::Type::Clicked:
                        mouseListener->mouseClicked(mouseEvent);
                        break;
                    default:
                        fcn::throwException(
                            "Unknown mouse event type.", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
                    }
                }

                if (toSourceOnly) {
                    break;
                }
            }

            Widget* swap = widget;
            widget       = parent;
            parent       = swap->getParent();

            // If a non modal focused widget has been reach
            // and we have modal focus cancel the distribution.
            if (mFocusHandler->getModalFocused() != nullptr && widget != nullptr && !widget->isModalFocused()) {
                break;
            }

            // If a non modal mouse input focused widget has been reach
            // and we have modal mouse input focus cancel the distribution.
            if (mFocusHandler->getModalMouseInputFocused() != nullptr && widget != nullptr &&
                !widget->isModalMouseInputFocused()) {
                break;
            }
        }
    }

    void Gui::distributeKeyEvent(KeyEvent& keyEvent)
    {
        Widget* parent = keyEvent.getSource();
        Widget* widget = keyEvent.getSource();

        if (mFocusHandler->getModalFocused() != nullptr && !widget->isModalFocused()) {
            return;
        }

        while (parent != nullptr) {
            // If the widget has been removed due to input
            // cancel the distribution.
            if (!Widget::widgetExists(widget)) {
                break;
            }

            parent = widget->getParent();

            if (widget->isEnabled()) {
                keyEvent.mDistributor                = widget;
                std::list<KeyListener*> keyListeners = widget->_getKeyListeners();

                // Send the event to all key listeners of the source widget.
                for (auto& keyListener : keyListeners) {
                    switch (keyEvent.getType()) {
                    case static_cast<unsigned int>(KeyEvent::Type::Pressed):
                        keyListener->keyPressed(keyEvent);
                        break;
                    case static_cast<unsigned int>(KeyEvent::Type::Released):
                        keyListener->keyReleased(keyEvent);
                        break;
                    default:
                        fcn::throwException(
                            "Unknown key event type.", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
                    }
                }
            }

            Widget* swap = widget;
            widget       = parent;
            parent       = swap->getParent();

            // If a non modal focused widget has been reach
            // and we have modal focus cancel the distribution.
            if (mFocusHandler->getModalFocused() != nullptr && !widget->isModalFocused()) {
                break;
            }
        }
    }

    void Gui::distributeKeyEventToGlobalKeyListeners(KeyEvent& keyEvent)
    {
        KeyListenerListIterator it;

        for (it = mKeyListeners.begin(); it != mKeyListeners.end(); it++) {
            switch (keyEvent.getType()) {
            case static_cast<unsigned int>(KeyEvent::Type::Pressed):
                (*it)->keyPressed(keyEvent);
                break;
            case static_cast<unsigned int>(KeyEvent::Type::Released):
                (*it)->keyReleased(keyEvent);
                break;
            default:
                fcn::throwException(
                    "Unknown key event type.", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
            }

            if (keyEvent.isConsumed()) {
                break;
            }
        }
    }

    void Gui::handleModalMouseInputFocus()
    {
        // Check if modal mouse input focus has been gained by a widget.
        if ((mFocusHandler->getLastWidgetWithModalMouseInputFocus() != mFocusHandler->getModalMouseInputFocused()) &&
            (mFocusHandler->getLastWidgetWithModalMouseInputFocus() == nullptr)) {
            handleModalMouseInputFocusGained();
        } else if (
            // Check if modal mouse input focus has been released.
            (mFocusHandler->getLastWidgetWithModalMouseInputFocus() != mFocusHandler->getModalMouseInputFocused()) &&
            (mFocusHandler->getLastWidgetWithModalMouseInputFocus() != nullptr)) {
            handleModalMouseInputFocusReleased();
        }
    }

    void Gui::handleModalFocus()
    {
        // Check if modal focus has been gained by a widget.
        if ((mFocusHandler->getLastWidgetWithModalFocus() != mFocusHandler->getModalFocused()) &&
            (mFocusHandler->getLastWidgetWithModalFocus() == nullptr)) {
            handleModalFocusGained();
        } else if (
            // Check if modal focus has been released.
            (mFocusHandler->getLastWidgetWithModalFocus() != mFocusHandler->getModalFocused()) &&
            (mFocusHandler->getLastWidgetWithModalFocus() != nullptr)) {
            handleModalFocusReleased();
        }
    }

    void Gui::handleModalFocusGained()
    {
        // Get all widgets at the last known mouse position
        // and send them a mouse exited event.
        std::set<Widget*> const mWidgetsWithMouse = getWidgetsAt(mLastMouseX, mLastMouseY);

        std::set<Widget*>::const_iterator iter;
        for (iter = mWidgetsWithMouse.begin(); iter != mWidgetsWithMouse.end(); iter++) {
            if ((*iter)->isModalFocused() || (*iter)->isModalMouseInputFocused()) {
                continue;
            }
            distributeMouseEvent(
                (*iter),
                MouseEvent::Type::Exited,
                static_cast<MouseEvent::Button>(mLastMousePressButton),
                mLastMouseX,
                mLastMouseY,
                true,
                true);
        }
        mFocusHandler->setLastWidgetWithModalFocus(mFocusHandler->getModalFocused());
    }

    void Gui::handleModalFocusReleased()
    {
        // Get all widgets at the last known mouse position
        // and send them a mouse entered event.
        std::set<Widget*> const mWidgetsWithMouse = getWidgetsAt(mLastMouseX, mLastMouseY);

        std::set<Widget*>::const_iterator iter;
        for (iter = mWidgetsWithMouse.begin(); iter != mWidgetsWithMouse.end(); iter++) {
            distributeMouseEvent(
                (*iter),
                MouseEvent::Type::Entered,
                static_cast<MouseEvent::Button>(mLastMousePressButton),
                mLastMouseX,
                mLastMouseY,
                false,
                true);
        }
        mFocusHandler->setLastWidgetWithModalFocus(nullptr);
    }

    void Gui::handleModalMouseInputFocusGained()
    {
        // Get all widgets at the last known mouse position
        // and send them a mouse exited event.
        std::set<Widget*> const mWidgetsWithMouse = getWidgetsAt(mLastMouseX, mLastMouseY);

        std::set<Widget*>::const_iterator iter;
        for (iter = mWidgetsWithMouse.begin(); iter != mWidgetsWithMouse.end(); iter++) {
            if ((*iter)->isModalMouseInputFocused()) {
                continue;
            }
            distributeMouseEvent(
                (*iter),
                MouseEvent::Type::Exited,
                static_cast<MouseEvent::Button>(mLastMousePressButton),
                mLastMouseX,
                mLastMouseY,
                true,
                true);
        }
        mFocusHandler->setLastWidgetWithModalMouseInputFocus(mFocusHandler->getModalMouseInputFocused());
    }

    void Gui::handleModalMouseInputFocusReleased()
    {
        // Get all widgets at the last known mouse position
        // and send them a mouse entered event.
        std::set<Widget*> const mWidgetsWithMouse = getWidgetsAt(mLastMouseX, mLastMouseY);

        std::set<Widget*>::const_iterator iter;
        for (iter = mWidgetsWithMouse.begin(); iter != mWidgetsWithMouse.end(); iter++) {
            distributeMouseEvent(
                (*iter),
                MouseEvent::Type::Entered,
                static_cast<MouseEvent::Button>(mLastMousePressButton),
                mLastMouseX,
                mLastMouseY,
                false,
                true);
        }
        mFocusHandler->setLastWidgetWithModalMouseInputFocus(nullptr);
    }

    void Gui::handleHiddenWidgets()
    {
        // process each hidden widget in queue
        while (!mHiddenWidgets.empty()) {
            // if the hidden widget had the mouse cursor inside
            Widget* hiddenWidget = mHiddenWidgets.front();

            // make sure that the widget wasn't freed after hiding
            if (Widget::widgetExists(hiddenWidget) && hiddenWidget->isEnabled()) {
                int hiddenWidgetX = 0;
                int hiddenWidgetY = 0;
                hiddenWidget->getAbsolutePosition(hiddenWidgetX, hiddenWidgetY);

                Rectangle const r(hiddenWidgetX, hiddenWidgetY, hiddenWidget->getWidth(), hiddenWidget->getHeight());

                if (r.isContaining(mLastMouseX, mLastMouseY)) {
                    // get the widget that has the cursor now and distribute that the mouse entered it
                    Widget* underMouseCursor = getWidgetAt(mLastMouseX, mLastMouseY);

                    distributeMouseEvent(
                        underMouseCursor,
                        MouseEvent::Type::Entered,
                        MouseEvent::Button::Empty,
                        mLastMouseX,
                        mLastMouseY,
                        true,
                        true);
                }
            }

            mHiddenWidgets.pop();
        }
    }

    void Gui::handleShownWidgets()
    {
        // process each shown widget in queue
        while (!mShownWidgets.empty()) {
            Widget* shownWidget = mShownWidgets.front();

            // if the shown widget has the mouse cursor inside it
            int shownWidgetX = 0;
            int shownWidgetY = 0;
            shownWidget->getAbsolutePosition(shownWidgetX, shownWidgetY);

            Rectangle const r(shownWidgetX, shownWidgetY, shownWidget->getWidth(), shownWidget->getHeight());

            if (r.isContaining(mLastMouseX, mLastMouseY) && shownWidget->isEnabled()) {
                // find which widget had the mouse before and distribute that the mouse exited it
                Widget* underMouseCursorBefore = getWidgetAt(mLastMouseX, mLastMouseY, shownWidget);

                distributeMouseEvent(
                    underMouseCursorBefore,
                    MouseEvent::Type::Exited,
                    MouseEvent::Button::Empty,
                    mLastMouseX,
                    mLastMouseY,
                    true,
                    true);

                // find which specific widget in the shown widget had the mouse before
                // and distribute that the mouse exited it
                Widget* underMouseCursorNow = getWidgetAt(mLastMouseX, mLastMouseY);

                distributeMouseEvent(
                    underMouseCursorNow,
                    MouseEvent::Type::Entered,
                    MouseEvent::Button::Empty,
                    mLastMouseX,
                    mLastMouseY,
                    true,
                    true);
            }

            mShownWidgets.pop();
        }
    }
} // namespace fcn
