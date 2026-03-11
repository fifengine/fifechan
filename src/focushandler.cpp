// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/focushandler.hpp"

#include <algorithm>
#include <list>
#include <ranges>
#include <utility>

#include "fifechan/event.hpp"
#include "fifechan/exception.hpp"
#include "fifechan/focuslistener.hpp"
#include "fifechan/widget.hpp"

namespace fcn
{
    FocusHandler::FocusHandler() :
        mFocusedWidget(nullptr),
        mModalFocusedWidget(nullptr),
        mModalMouseInputFocusedWidget(nullptr),
        mDraggedWidget(nullptr),
        mLastWidgetWithMouse(nullptr),
        mLastWidgetWithModalFocus(nullptr),
        mLastWidgetWithModalMouseInputFocus(nullptr),
        mLastWidgetPressed(nullptr)
    {
    }

    void FocusHandler::requestFocus(Widget* widget)
    {
        if (widget == nullptr || widget == mFocusedWidget) {
            return;
        }

        unsigned int i       = 0;
        int toBeFocusedIndex = -1;
        for (i = 0; i < mWidgets.size(); ++i) {
            if (mWidgets[i] == widget) {
                toBeFocusedIndex = i;
                break;
            }
        }

        if (toBeFocusedIndex < 0) {
            throwException("Trying to focus a none existing widget.");
        }

        Widget* oldFocused = mFocusedWidget;

        if (oldFocused != widget) {
            mFocusedWidget = mWidgets.at(toBeFocusedIndex);

            if (oldFocused != nullptr) {
                Event const focusEvent(oldFocused);
                distributeFocusLostEvent(focusEvent);
            }

            Event const focusEvent(mWidgets.at(toBeFocusedIndex));
            distributeFocusGainedEvent(focusEvent);
        }
    }

    void FocusHandler::requestModalFocus(Widget* widget)
    {
        if (mModalFocusedWidget != nullptr && mModalFocusedWidget != widget) {
            throwException("Another widget already has modal focus.");
        }

        mModalFocusedWidget = widget;

        if (mFocusedWidget != nullptr && !mFocusedWidget->isModalFocused()) {
            focusNone();
        }
    }

    void FocusHandler::requestModalMouseInputFocus(Widget* widget)
    {
        if (mModalMouseInputFocusedWidget != nullptr && mModalMouseInputFocusedWidget != widget) {
            throwException("Another widget already has modal input focus.");
        }

        mModalMouseInputFocusedWidget = widget;
    }

    void FocusHandler::releaseModalFocus(Widget* widget)
    {
        if (mModalFocusedWidget == widget) {
            mModalFocusedWidget = nullptr;
        }
    }

    void FocusHandler::releaseModalMouseInputFocus(Widget* widget)
    {
        if (mModalMouseInputFocusedWidget == widget) {
            mModalMouseInputFocusedWidget = nullptr;
        }
    }

    Widget* FocusHandler::getFocused() const
    {
        return mFocusedWidget;
    }

    Widget* FocusHandler::getModalFocused() const
    {
        return mModalFocusedWidget;
    }

    Widget* FocusHandler::getModalMouseInputFocused() const
    {
        return mModalMouseInputFocusedWidget;
    }

    void FocusHandler::focusNext()
    {
        int i             = 0;
        int focusedWidget = -1;

        for (i = 0; std::cmp_less(i, mWidgets.size()); ++i) {
            if (mWidgets[i] == mFocusedWidget) {
                focusedWidget = i;
            }
        }

        int const focused = focusedWidget;

        // i is a counter that ensures that the following loop
        // won't get stuck in an infinite loop
        int num_widgets = static_cast<int>(mWidgets.size());

        for (; num_widgets > 0; --num_widgets) {
            ++focusedWidget;

            if (std::cmp_greater_equal(focusedWidget, mWidgets.size())) {
                focusedWidget = 0;
            }

            if (focusedWidget == focused) {
                return;
            }

            if (mWidgets.at(focusedWidget)->isFocusable()) {
                break;
            }

            if (num_widgets == 1) {
                focusedWidget = -1;
                break;
            }
        }

        if (focusedWidget >= 0) {
            mFocusedWidget = mWidgets.at(focusedWidget);
            Event const focusEvent(mFocusedWidget);
            distributeFocusGainedEvent(focusEvent);
        }

        if (focused >= 0) {
            Event const focusEvent(mWidgets.at(focused));
            distributeFocusLostEvent(focusEvent);
        }
    }

    void FocusHandler::focusPrevious()
    {
        if (mWidgets.empty()) {
            mFocusedWidget = nullptr;
            return;
        }

        int i             = 0;
        int focusedWidget = -1;
        for (i = 0; std::cmp_less(i, mWidgets.size()); ++i) {
            if (mWidgets[i] == mFocusedWidget) {
                focusedWidget = i;
            }
        }
        int const focused = focusedWidget;

        // i is a counter that ensures that the following loop
        // won't get stuck in an infinite loop
        i = static_cast<int>(mWidgets.size());
        while (i > 0) {
            --focusedWidget;

            if (i == 1) {
                focusedWidget = -1;
                break;
            }

            --i;

            if (focusedWidget < 0) {
                focusedWidget = mWidgets.size() - 1;
            }

            if (focusedWidget == focused) {
                return;
            }

            if (mWidgets.at(focusedWidget)->isFocusable()) {
                break;
            }
        }

        if (focusedWidget >= 0) {
            mFocusedWidget = mWidgets.at(focusedWidget);
            Event const focusEvent(mFocusedWidget);
            distributeFocusGainedEvent(focusEvent);
        }

        if (focused >= 0) {
            Event const focusEvent(mWidgets.at(focused));
            distributeFocusLostEvent(focusEvent);
        }
    }

    bool FocusHandler::isFocused(Widget const * widget) const
    {
        return mFocusedWidget == widget;
    }

    void FocusHandler::add(Widget* widget)
    {
        mWidgets.push_back(widget);
    }

    void FocusHandler::remove(Widget* widget)
    {
        if (isFocused(widget)) {
            mFocusedWidget = nullptr;
        }

        auto iter = std::ranges::find(mWidgets, widget);
        if (iter != std::ranges::end(mWidgets)) {
            mWidgets.erase(iter);
        }

        if (mDraggedWidget == widget) {
            mDraggedWidget = nullptr;
            return;
        }

        if (mLastWidgetWithMouse == widget) {
            mLastWidgetWithMouse = nullptr;
            return;
        }

        if (mLastWidgetWithModalFocus == widget) {
            mLastWidgetWithModalFocus = nullptr;
            return;
        }

        if (mLastWidgetWithModalMouseInputFocus == widget) {
            mLastWidgetWithModalMouseInputFocus = nullptr;
            return;
        }

        if (mLastWidgetPressed == widget) {
            mLastWidgetPressed = nullptr;
            return;
        }
    }

    void FocusHandler::focusNone()
    {
        if (mFocusedWidget != nullptr) {
            Widget* focused = mFocusedWidget;
            mFocusedWidget  = nullptr;

            Event const focusEvent(focused);
            distributeFocusLostEvent(focusEvent);
        }
    }

    void FocusHandler::tabNext()
    {
        if (mFocusedWidget != nullptr) {
            if (!mFocusedWidget->isTabOutEnabled()) {
                return;
            }
        }

        if (mWidgets.empty()) {
            mFocusedWidget = nullptr;
            return;
        }

        int i             = 0;
        int focusedWidget = -1;
        for (i = 0; std::cmp_less(i, mWidgets.size()); ++i) {
            if (mWidgets[i] == mFocusedWidget) {
                focusedWidget = i;
            }
        }
        int const focused = focusedWidget;

        i = static_cast<int>(mWidgets.size());

        while (i > 0) {
            ++focusedWidget;

            if (i == 1) {
                focusedWidget = -1;
                break;
            }

            --i;

            if (std::cmp_greater_equal(focusedWidget, mWidgets.size())) {
                focusedWidget = 0;
            }

            if (focusedWidget == focused) {
                return;
            }

            if (mWidgets.at(focusedWidget)->isFocusable() && mWidgets.at(focusedWidget)->isTabInEnabled() &&
                (mModalFocusedWidget == nullptr || mWidgets.at(focusedWidget)->isModalFocused())) {
                break;
            }
        }

        if (focusedWidget >= 0) {
            mFocusedWidget = mWidgets.at(focusedWidget);
            Event const focusEvent(mFocusedWidget);
            distributeFocusGainedEvent(focusEvent);
        }

        if (focused >= 0) {
            Event const focusEvent(mWidgets.at(focused));
            distributeFocusLostEvent(focusEvent);
        }
    }

    void FocusHandler::tabPrevious()
    {
        if (mFocusedWidget != nullptr) {
            if (!mFocusedWidget->isTabOutEnabled()) {
                return;
            }
        }

        if (mWidgets.empty()) {
            mFocusedWidget = nullptr;
            return;
        }

        int i             = 0;
        int focusedWidget = -1;
        for (i = 0; std::cmp_less(i, mWidgets.size()); ++i) {
            if (mWidgets[i] == mFocusedWidget) {
                focusedWidget = i;
            }
        }
        int const focused = focusedWidget;

        i = static_cast<int>(mWidgets.size());

        while (i > 0) {
            --focusedWidget;

            if (i == 1) {
                focusedWidget = -1;
                break;
            }

            --i;

            if (focusedWidget <= 0) {
                focusedWidget = mWidgets.size() - 1;
            }

            if (focusedWidget == focused) {
                return;
            }

            if (mWidgets.at(focusedWidget)->isFocusable() && mWidgets.at(focusedWidget)->isTabInEnabled() &&
                (mModalFocusedWidget == nullptr || mWidgets.at(focusedWidget)->isModalFocused())) {
                break;
            }
        }

        if (focusedWidget >= 0) {
            mFocusedWidget = mWidgets.at(focusedWidget);
            Event const focusEvent(mFocusedWidget);
            distributeFocusGainedEvent(focusEvent);
        }

        if (focused >= 0) {
            Event const focusEvent(mWidgets.at(focused));
            distributeFocusLostEvent(focusEvent);
        }
    }

    void FocusHandler::distributeFocusLostEvent(Event const & focusEvent)
    {
        Widget* sourceWidget = focusEvent.getSource();

        std::list<FocusListener*> focusListeners = sourceWidget->_getFocusListeners();

        // Send the event to all focus listeners of the widget.
        for (auto& focusListener : focusListeners) {
            focusListener->focusLost(focusEvent);
        }
    }

    void FocusHandler::distributeFocusGainedEvent(Event const & focusEvent)
    {
        Widget* sourceWidget = focusEvent.getSource();

        std::list<FocusListener*> focusListeners = sourceWidget->_getFocusListeners();

        // Send the event to all focus listeners of the widget.
        for (auto& focusListener : focusListeners) {
            focusListener->focusGained(focusEvent);
        }
    }

    Widget* FocusHandler::getDraggedWidget()
    {
        return mDraggedWidget;
    }

    void FocusHandler::setDraggedWidget(Widget* draggedWidget)
    {
        mDraggedWidget = draggedWidget;
    }

    Widget* FocusHandler::getLastWidgetWithMouse()
    {
        return mLastWidgetWithMouse;
    }

    void FocusHandler::setLastWidgetWithMouse(Widget* lastWidgetWithMouse)
    {
        mLastWidgetWithMouse = lastWidgetWithMouse;
    }

    Widget* FocusHandler::getLastWidgetWithModalFocus()
    {
        return mLastWidgetWithModalFocus;
    }

    void FocusHandler::setLastWidgetWithModalFocus(Widget* lastWidgetWithModalFocus)
    {
        mLastWidgetWithModalFocus = lastWidgetWithModalFocus;
    }

    Widget* FocusHandler::getLastWidgetWithModalMouseInputFocus()
    {
        return mLastWidgetWithModalMouseInputFocus;
    }

    void FocusHandler::setLastWidgetWithModalMouseInputFocus(Widget* lastWidgetWithModalMouseInputFocus)
    {
        mLastWidgetWithModalMouseInputFocus = lastWidgetWithModalMouseInputFocus;
    }

    Widget* FocusHandler::getLastWidgetPressed()
    {
        return mLastWidgetPressed;
    }

    void FocusHandler::setLastWidgetPressed(Widget* lastWidgetPressed)
    {
        mLastWidgetPressed = lastWidgetPressed;
    }

    void FocusHandler::widgetHidden(Widget* widget) { }
} // namespace fcn
