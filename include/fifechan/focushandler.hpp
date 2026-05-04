// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_FOCUSHANDLER_HPP_
#define INCLUDE_FIFECHAN_FOCUSHANDLER_HPP_

// Standard library includes
#include <cstdio>
#include <vector>

// Project headers (subdirs before local)
#include "fifechan/events/event.hpp"

namespace fcn
{
    class Widget;

    /**
     * Manages focus navigation and assignment among widgets within a Gui instance.
     *
     * Each Gui has at least one focus handler.
     * You will probably not use the focus handler directly as Widget
     * has functions that automatically uses the active focus handler.
     *
     * @see Widget::isFocus, Widget::isModalFocused,
     *      Widget::isModalMouseInputFocused, Widget::requestFocus,
     *      Widget::isModalFocused, Widget::isModalMouseInputFocused,
     *      Widget::setFocusable, Widget::isFocusable, FocusListener
     *
     * @ingroup core
     */
    class FIFEGUI_API FocusHandler
    {
        public:
            virtual ~FocusHandler() = default;
            FocusHandler();

            FocusHandler(FocusHandler const &)            = delete;
            FocusHandler& operator=(FocusHandler const &) = delete;
            FocusHandler(FocusHandler&&)                  = delete;
            FocusHandler& operator=(FocusHandler&&)       = delete;

            /**
             * Requests focus for a widget.
             *
             * Focus will only be granted to a widget if it's focusable
             * and if no other widget has modal focus.
             * If a widget receives focus, a focus event will be sent to the
             * focus listeners of the widget.
             *
             * @param widget The widget to request focus for.
             * @see isFocused, Widget::requestFocus
             */
            virtual void requestFocus(Widget* widget);

            /**
             * Checks if a widget is focused.
             *
             * @param widget The widget to check.
             * @return True if the widget is focused, false otherwise.
             * @see Widget::isFocused
             */
            virtual bool isFocused(Widget const * widget) const;

            /**
             * Gets the widget with modal mouse input focus.
             *
             * @return The widget with modal mouse input focus. Nullptr if
             *         no widget has modal mouse input focus.
             */
            virtual Widget* getMouseCaptureOwner() const;

            /**
             * Checks if any modal state is active.
             *
             * @return True if modal focus or modal mouse input is active.
             */
            virtual bool hasModalFocus() const;

            /**
             * Pushes a new modal state onto the stack.
             *
             * @param focusOwner The widget that will have modal focus.
             * @param mouseOwner Optional widget for modal mouse input focus.
             * @see popModal, clearModal
             */
            virtual void pushModal(Widget* focusOwner, Widget* mouseOwner = nullptr);

            /**
             * Pops the current modal state from the stack.
             *
             * Restores focus to the previous modal level if any.
             * @see pushModal, clearModal
             */
            virtual void popModal() noexcept;

            /**
             * Clears all modal states from the stack.
             *
             * Resets all modal focus and mouse input.
             * @see pushModal, popModal
             */
            virtual void clearModal();

            /**
             * Modal scope management (RAII guard).
             *
             * Automatically pops the modal when destroyed, unless release() is called.
             */
            class ModalScope
            {
                public:
                    /**
                     * RAII helper that pushes a modal state on construction and
                     * pops it on destruction unless `release()` is called.
                     *
                     * @param handler The focus handler managing modal states.
                     * @param focusOwner Widget that will receive modal focus.
                     * @param mouseOwner Optional widget for modal mouse input focus.
                     */
                    ModalScope(FocusHandler* handler, Widget* focusOwner, Widget* mouseOwner = nullptr) :
                        mHandler(handler)
                    {
                        if (mHandler != nullptr) {
                            mHandler->pushModal(focusOwner, mouseOwner);
                        }
                    }

                    ~ModalScope() noexcept
                    {
                        if (mHandler != nullptr && !mReleased) {
                            mWasPopped = true;
                            // cppcheck-suppress throwInNoexceptFunction
                            mHandler->popModal();
                        }

                        if (!mWasPopped && !mReleased) {
                            // ModalScope was destroyed without calling release() or popModal()
                            // This indicates a bug where the modal was not properly released
                            std::fprintf(
                                stderr,
                                "Warning: ModalScope destroyed without calling release() or popModal(). Did you forget "
                                "to call release()?\n");
                        }
                    }

                    ModalScope(ModalScope const &)            = delete;
                    ModalScope& operator=(ModalScope const &) = delete;
                    ModalScope(ModalScope&&)                  = delete;
                    ModalScope& operator=(ModalScope&&)       = delete;

                    /**
                     * Release ownership so the modal is not popped during destruction.
                     * After calling `release()` the ModalScope destructor will not
                     * call `popModal()`.
                     */
                    void release()
                    {
                        mReleased  = true;
                        mWasPopped = true;
                    }

                private:
                    FocusHandler* mHandler{nullptr};
                    bool mReleased{false};
                    bool mWasPopped{false};
            };

            /**
             * Alias for ModalScope to clarify its purpose for input handling.
             *
             * @deprecated Use InputModalScope instead.
             */
            using InputModalScope [[deprecated("Use InputModalScope instead")]] = ModalScope;

            /**
             * Gets the active input root widget for focus routing.
             *
             * Returns the top of the modal stack for focus, or nullptr if no modal.
             *
             * @return The widget that currently owns modal focus, or nullptr.
             */
            virtual Widget* getFocusOwner() const;

            /**
             * Releases focus for the specified widget if it is currently focused.
             *
             * @param widget The widget to release focus for.
             * @see requestFocus, setFocusedWidget
             */
            virtual void releaseFocus(Widget* widget);

            /**
             * Sets the focused widget directly.
             *
             * @param widget The widget to focus, or nullptr to clear focus.
             * @see requestFocus, releaseFocus
             */
            virtual void setFocusedWidget(Widget* widget);

            /**
             * Gets the widget with focus.
             *
             * @return The widget with focus. Nullptr if no widget has focus.
             */
            virtual Widget* getFocused() const;

            /**
             * Focuses the next widget added to a container.
             *
             * If no widget has focus the first widget gets focus. The order
             * in which the widgets are focused is determined by the order
             * they were added to a container.
             *
             * @see focusPrevious
             */
            virtual void focusNext();

            /**
             * Focuses the previous widget added to a container.
             *
             * If no widget has focus the first widget gets focus. The order
             * in which the widgets are focused is determined by the order
             * they were added to a container.
             *
             * @see focusNext
             */
            virtual void focusPrevious();

            /**
             * Adds a widget to by handles by the focus handler.
             *
             * @param widget The widget to add.
             * @see remove
             */
            virtual void add(Widget* widget);

            /**
             * Removes a widget from the focus handler.
             *
             * @param widget The widget to remove.
             * @see add
             */
            virtual void remove(Widget* widget);

            /**
             * Focuses nothing.
             *
             * A focus event will also be sent to the focused widget's
             * focus listeners if a widget has focus.
             */
            virtual void focusNone();

            /**
             * Focuses the next widget which allows tabbing in
             * unless the current focused Widget disallows tabbing out.
             *
             * @see tabPrevious
             */
            virtual void tabNext();

            /**
             * Focuses the previous widget which allows tabbing in
             * unless current focused Widget disallows tabbing out.
             *
             * @see tabNext
             */
            virtual void tabPrevious();

            /**
             * Gets the widget being dragged.
             *
             * Used by the GUI class to keep track of the dragged widget.
             *
             * @return the widget being dragged.
             * @see setDraggedWidget
             */
            virtual Widget* getDraggedWidget();

            /**
             * Sets the widget being dragged.
             *
             * Used by the GUI class to keep track of the dragged widget.
             *
             * @param draggedWidget The widget being dragged.
             * @see getDraggedWidget
             */
            virtual void setDraggedWidget(Widget* draggedWidget);

            /**
             * Gets the last widget with the mouse.
             *
             * Used by the GUI class to keep track the last widget with the mouse.
             *
             * @return The last widget with the mouse.
             * @see setLastWidgetWithMouse
             */
            virtual Widget* getLastWidgetWithMouse();

            /**
             * Sets the last widget with the mouse.
             *
             * Used by the GUI class to keep track the last widget with the mouse.
             *
             * @param lastWidgetWithMouse The last widget with the mouse.
             * @see getLastWidgetWithMouse
             */
            virtual void setLastWidgetWithMouse(Widget* lastWidgetWithMouse);

            /**
             * Gets the last widget with modal focus.
             *
             * @return The last widget with modal focus.
             * @see setLastWidgetWithModalFocus
             */
            virtual Widget* getLastWidgetWithModalFocus();

            /**
             * Sets the last widget with modal focus.
             *
             * @param lastWidgetWithModalFocus The last widget with modal focus.
             * @see getLastWidgetWithModalFocus
             */
            virtual void setLastWidgetWithModalFocus(Widget* lastWidgetWithModalFocus);

            /**
             * Gets the last widget with modal mouse input focus.
             *
             * @return The last widget with modal mouse input focus.
             * @see setLastWidgetWithModalMouseInputFocus
             */
            virtual Widget* getLastWidgetWithModalMouseInputFocus();

            /**
             * Sets the last widget with modal mouse input focus.
             *
             * @param lastWidgetWithModalMouseInputFocus The last widget with modal mouse input focus.
             * @see getLastWidgetWithModalMouseInputFocus
             */
            virtual void setLastWidgetWithModalMouseInputFocus(Widget* lastWidgetWithModalMouseInputFocus);

            /**
             * Gets the last widget pressed.
             *
             * Used by the GUI class to keep track of pressed widgets.
             *
             * @return The last widget pressed.
             * @see setLastWidgetPressed
             */
            virtual Widget* getLastWidgetPressed();

            /**
             * Sets the last widget pressed.
             *
             * Used by the GUI class to keep track of pressed widgets.
             *
             * @param lastWidgetPressed The last widget pressed.
             * @see getLastWidgetPressed
             */
            virtual void setLastWidgetPressed(Widget* lastWidgetPressed);

            /**
             * Informs the focus handler that a widget was hidden.
             *
             * This is needed because the focus handler must determine
             * which widget has the mouse now.
             *
             * @param widget Widget that was hidden
             */
            virtual void widgetHidden(Widget* widget);

        protected:
            /**
             * Distributes a focus lost event.
             *
             * @param focusEvent the event to distribute.
             */
            virtual void distributeFocusLostEvent(Event const & focusEvent);

            /**
             * Distributes a focus gained event.
             *
             * @param focusEvent the event to distribute.
             */
            virtual void distributeFocusGainedEvent(Event const & focusEvent);

            /** Vector of Widget pointers. */
            using WidgetVector = std::vector<Widget*>;

            /** Iterator for WidgetVector. */
            using WidgetIterator = WidgetVector::iterator;

            /**
             * Represents a single modal state level with focus and mouse owners.
             */
            struct ModalState
            {
                    /**
                     * Widget with modal focus at this level.
                     */
                    Widget* focusOwner;

                    /**
                     * Widget with modal mouse input at this level.
                     */
                    Widget* mouseOwner;
            };

            /**
             * Holds the modal stack for nested modal dialogs.
             */
            std::vector<ModalState> mModalStack;

            /**
             * Holds the widgets currently being handled by the focus handler.
             */
            WidgetVector mWidgets;

            /**
             * Holds the focused widget. Nullptr if no widget has focus.
             */
            Widget* mFocusedWidget;

            /**
             * Holds the dragged widget.
             *
             * Nullptr if no widget is being dragged.
             */
            Widget* mDraggedWidget;

            /**
             * Holds the last widget with the mouse.
             */
            Widget* mLastWidgetWithMouse;

            /**
             * Holds the last widget with modal focus.
             */
            Widget* mLastWidgetWithModalFocus;

            /**
             * Holds the last widget with modal mouse input focus.
             */
            Widget* mLastWidgetWithModalMouseInputFocus;

            /**
             * Holds the last widget pressed.
             */
            Widget* mLastWidgetPressed;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_FOCUSHANDLER_HPP_
