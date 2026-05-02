// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Standard library includes
#include <memory>
#include <string>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include "fifechan/exception.hpp"
#include "fifechan/focushandler.hpp"
#include "fifechan/listeners/focuslistener.hpp"
#include "fifechan/widgets/container.hpp"
#include "fifechan/widgets/label.hpp"

namespace fcn
{
    // ========================================================================
    // Helper: concrete FocusListener that records events
    // ========================================================================
    class TestFocusListener : public FocusListener
    {
        public:
            void focusGained(Event const & event) override
            {
                mFocusGainedCount++;
                mLastGainedSource = event.getSource();
            }

            void focusLost(Event const & event) override
            {
                mFocusLostCount++;
                mLastLostSource = event.getSource();
            }

            int mFocusGainedCount     = 0;
            int mFocusLostCount       = 0;
            Widget* mLastGainedSource = nullptr;
            Widget* mLastLostSource   = nullptr;

            void reset()
            {
                mFocusGainedCount = 0;
                mFocusLostCount   = 0;
                mLastGainedSource = nullptr;
                mLastLostSource   = nullptr;
            }
    };

    // ========================================================================
    // Helper: focusable Label subclass for testing
    // ========================================================================
    class FocusableLabel : public Label
    {
        public:
            FocusableLabel() : Label()
            {
                setFocusable(true);
            }

            explicit FocusableLabel(std::string const & caption) : Label(caption)
            {
                setFocusable(true);
            }
    };

} // namespace fcn

using namespace fcn;

// ============================================================================
// FocusHandler constructor and initial state
// ============================================================================

TEST_CASE("FocusHandler constructor initializes to empty state", "[unit][focushandler]")
{
    FocusHandler fh;

    REQUIRE(fh.getFocused() == nullptr);
    REQUIRE(fh.getFocusOwner() == nullptr);
    REQUIRE(fh.getMouseCaptureOwner() == nullptr);
    REQUIRE(fh.getDraggedWidget() == nullptr);
    REQUIRE(fh.getLastWidgetWithMouse() == nullptr);
    REQUIRE(fh.getLastWidgetWithModalFocus() == nullptr);
    REQUIRE(fh.getLastWidgetWithModalMouseInputFocus() == nullptr);
    REQUIRE(fh.getLastWidgetPressed() == nullptr);
    REQUIRE_FALSE(fh.hasModalFocus());
}

// ============================================================================
// add / remove widget tracking
// ============================================================================

TEST_CASE("FocusHandler add tracks widgets", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("test");

    fh.add(&lbl);
    // After adding, widget is tracked but not focused yet
    REQUIRE(fh.getFocused() == nullptr);
}

TEST_CASE("FocusHandler remove untracks widgets", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("test");

    fh.add(&lbl);
    fh.remove(&lbl);

    // Removing a non-focused widget should leave focused as nullptr
    REQUIRE(fh.getFocused() == nullptr);
}

TEST_CASE("FocusHandler remove clears focus if focused widget removed", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("test");

    fh.add(&lbl);
    fh.requestFocus(&lbl);
    REQUIRE(fh.getFocused() == &lbl);

    fh.remove(&lbl);
    REQUIRE(fh.getFocused() == nullptr);
}

TEST_CASE("FocusHandler remove clears dragged widget reference", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("test");

    fh.add(&lbl);
    fh.setDraggedWidget(&lbl);
    REQUIRE(fh.getDraggedWidget() == &lbl);

    fh.remove(&lbl);
    REQUIRE(fh.getDraggedWidget() == nullptr);
}

TEST_CASE("FocusHandler remove clears last widget with mouse reference", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("test");

    fh.add(&lbl);
    fh.setLastWidgetWithMouse(&lbl);
    REQUIRE(fh.getLastWidgetWithMouse() == &lbl);

    fh.remove(&lbl);
    REQUIRE(fh.getLastWidgetWithMouse() == nullptr);
}

TEST_CASE("FocusHandler remove clears last widget with modal focus reference", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("test");

    fh.add(&lbl);
    fh.setLastWidgetWithModalFocus(&lbl);
    REQUIRE(fh.getLastWidgetWithModalFocus() == &lbl);

    fh.remove(&lbl);
    REQUIRE(fh.getLastWidgetWithModalFocus() == nullptr);
}

TEST_CASE("FocusHandler remove clears last widget with modal mouse input focus reference", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("test");

    fh.add(&lbl);
    fh.setLastWidgetWithModalMouseInputFocus(&lbl);
    REQUIRE(fh.getLastWidgetWithModalMouseInputFocus() == &lbl);

    fh.remove(&lbl);
    REQUIRE(fh.getLastWidgetWithModalMouseInputFocus() == nullptr);
}

TEST_CASE("FocusHandler remove clears last widget pressed reference", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("test");

    fh.add(&lbl);
    fh.setLastWidgetPressed(&lbl);
    REQUIRE(fh.getLastWidgetPressed() == &lbl);

    fh.remove(&lbl);
    REQUIRE(fh.getLastWidgetPressed() == nullptr);
}

TEST_CASE("FocusHandler remove non-existent widget is safe", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("test");

    // Removing a widget that was never added should not throw
    REQUIRE_NOTHROW(fh.remove(&lbl));
}

// ============================================================================
// requestFocus / getFocused / isFocused
// ============================================================================

TEST_CASE("FocusHandler requestFocus sets focused widget", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("test");

    fh.add(&lbl);
    fh.requestFocus(&lbl);

    REQUIRE(fh.getFocused() == &lbl);
    REQUIRE(fh.isFocused(&lbl));
    REQUIRE(lbl.isFocused());
}

TEST_CASE("FocusHandler requestFocus with nullptr is no-op", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("test");

    fh.add(&lbl);
    fh.requestFocus(nullptr);

    REQUIRE(fh.getFocused() == nullptr);
}

TEST_CASE("FocusHandler requestFocus with already focused widget is no-op", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("test");

    fh.add(&lbl);
    fh.requestFocus(&lbl);
    REQUIRE(fh.getFocused() == &lbl);

    // Requesting focus again on same widget should not change anything
    fh.requestFocus(&lbl);
    REQUIRE(fh.getFocused() == &lbl);
}

TEST_CASE("FocusHandler requestFocus throws for untracked widget", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("test");

    // Widget not added to focus handler
    REQUIRE_THROWS_AS(fh.requestFocus(&lbl), Exception);
}

TEST_CASE("FocusHandler requestFocus switches focus between widgets", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");

    fh.add(&lbl1);
    fh.add(&lbl2);

    fh.requestFocus(&lbl1);
    REQUIRE(fh.getFocused() == &lbl1);

    fh.requestFocus(&lbl2);
    REQUIRE(fh.getFocused() == &lbl2);
    REQUIRE_FALSE(lbl1.isFocused());
    REQUIRE(lbl2.isFocused());
}

TEST_CASE("FocusHandler requestFocus distributes focus events", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");
    TestFocusListener listener1;
    TestFocusListener listener2;

    lbl1.addFocusListener(&listener1);
    lbl2.addFocusListener(&listener2);

    fh.add(&lbl1);
    fh.add(&lbl2);

    fh.requestFocus(&lbl1);
    REQUIRE(listener1.mFocusGainedCount == 1);
    REQUIRE(listener1.mLastGainedSource == &lbl1);

    fh.requestFocus(&lbl2);
    REQUIRE(listener1.mFocusLostCount == 1);
    REQUIRE(listener1.mLastLostSource == &lbl1);
    REQUIRE(listener2.mFocusGainedCount == 1);
    REQUIRE(listener2.mLastGainedSource == &lbl2);
}

// ============================================================================
// setFocusedWidget / releaseFocus
// ============================================================================

TEST_CASE("FocusHandler setFocusedWidget sets focus directly", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("test");

    fh.setFocusedWidget(&lbl);

    REQUIRE(fh.getFocused() == &lbl);
    REQUIRE(lbl.isFocused());
}

TEST_CASE("FocusHandler setFocusedWidget with nullptr clears focus", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("test");

    fh.setFocusedWidget(&lbl);
    fh.setFocusedWidget(nullptr);

    REQUIRE(fh.getFocused() == nullptr);
    REQUIRE_FALSE(lbl.isFocused());
}

TEST_CASE("FocusHandler setFocusedWidget switches focus", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");

    fh.setFocusedWidget(&lbl1);
    REQUIRE(fh.getFocused() == &lbl1);

    fh.setFocusedWidget(&lbl2);
    REQUIRE(fh.getFocused() == &lbl2);
    REQUIRE_FALSE(lbl1.isFocused());
    REQUIRE(lbl2.isFocused());
}

TEST_CASE("FocusHandler setFocusedWidget same widget is no-op", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("test");

    fh.setFocusedWidget(&lbl);
    fh.setFocusedWidget(&lbl);

    REQUIRE(fh.getFocused() == &lbl);
}

TEST_CASE("FocusHandler releaseFocus clears focus for matching widget", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("test");
    TestFocusListener listener;

    lbl.addFocusListener(&listener);

    fh.add(&lbl);
    fh.requestFocus(&lbl);
    REQUIRE(fh.getFocused() == &lbl);

    fh.releaseFocus(&lbl);
    REQUIRE(fh.getFocused() == nullptr);
    REQUIRE(listener.mFocusLostCount == 1);
}

TEST_CASE("FocusHandler releaseFocus with nullptr is no-op", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("test");

    fh.add(&lbl);
    fh.requestFocus(&lbl);

    fh.releaseFocus(nullptr);
    REQUIRE(fh.getFocused() == &lbl);
}

TEST_CASE("FocusHandler releaseFocus with non-focused widget is no-op", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");

    fh.add(&lbl1);
    fh.add(&lbl2);
    fh.requestFocus(&lbl1);

    fh.releaseFocus(&lbl2);
    REQUIRE(fh.getFocused() == &lbl1);
}

// ============================================================================
// focusNone
// ============================================================================

TEST_CASE("FocusHandler focusNone clears focus", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("test");
    TestFocusListener listener;

    lbl.addFocusListener(&listener);

    fh.add(&lbl);
    fh.requestFocus(&lbl);
    REQUIRE(fh.getFocused() == &lbl);

    fh.focusNone();
    REQUIRE(fh.getFocused() == nullptr);
    REQUIRE(listener.mFocusLostCount == 1);
}

TEST_CASE("FocusHandler focusNone when no focus is no-op", "[unit][focushandler]")
{
    FocusHandler fh;

    REQUIRE_NOTHROW(fh.focusNone());
    REQUIRE(fh.getFocused() == nullptr);
}

// ============================================================================
// focusNext / focusPrevious
// ============================================================================

TEST_CASE("FocusHandler focusNext focuses first widget when none focused", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");

    fh.add(&lbl1);
    fh.add(&lbl2);

    fh.focusNext();
    REQUIRE(fh.getFocused() == &lbl1);
}

TEST_CASE("FocusHandler focusNext cycles to next widget", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");
    FocusableLabel lbl3("third");

    fh.add(&lbl1);
    fh.add(&lbl2);
    fh.add(&lbl3);

    fh.focusNext();
    REQUIRE(fh.getFocused() == &lbl1);

    fh.focusNext();
    REQUIRE(fh.getFocused() == &lbl2);

    fh.focusNext();
    REQUIRE(fh.getFocused() == &lbl3);
}

TEST_CASE("FocusHandler focusNext wraps around", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");

    fh.add(&lbl1);
    fh.add(&lbl2);

    fh.requestFocus(&lbl2);
    fh.focusNext();
    REQUIRE(fh.getFocused() == &lbl1);
}

TEST_CASE("FocusHandler focusNext skips non-focusable widgets", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    Label lbl2("non-focusable"); // Label is not focusable by default
    FocusableLabel lbl3("third");

    fh.add(&lbl1);
    fh.add(&lbl2);
    fh.add(&lbl3);

    fh.requestFocus(&lbl1);
    fh.focusNext();
    REQUIRE(fh.getFocused() == &lbl3);
}

TEST_CASE("FocusHandler focusNext with empty widget list", "[unit][focushandler]")
{
    FocusHandler fh;

    fh.focusNext();
    REQUIRE(fh.getFocused() == nullptr);
}

TEST_CASE("FocusHandler focusNext with all non-focusable widgets", "[unit][focushandler]")
{
    FocusHandler fh;
    Label lbl1("non-focusable-1");
    Label lbl2("non-focusable-2");

    fh.add(&lbl1);
    fh.add(&lbl2);

    fh.focusNext();
    // No focusable widget exists, focus should remain nullptr
    REQUIRE(fh.getFocused() == nullptr);
}

TEST_CASE("FocusHandler focusPrevious focuses last widget when none focused", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");

    fh.add(&lbl1);
    fh.add(&lbl2);

    fh.focusPrevious();
    REQUIRE(fh.getFocused() == &lbl2);
}

TEST_CASE("FocusHandler focusPrevious cycles backwards", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");
    FocusableLabel lbl3("third");

    fh.add(&lbl1);
    fh.add(&lbl2);
    fh.add(&lbl3);

    fh.requestFocus(&lbl3);
    fh.focusPrevious();
    REQUIRE(fh.getFocused() == &lbl2);

    fh.focusPrevious();
    REQUIRE(fh.getFocused() == &lbl1);
}

TEST_CASE("FocusHandler focusPrevious wraps around", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");

    fh.add(&lbl1);
    fh.add(&lbl2);

    fh.requestFocus(&lbl1);
    fh.focusPrevious();
    REQUIRE(fh.getFocused() == &lbl2);
}

TEST_CASE("FocusHandler focusPrevious skips non-focusable widgets", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    Label lbl2("non-focusable");
    FocusableLabel lbl3("third");

    fh.add(&lbl1);
    fh.add(&lbl2);
    fh.add(&lbl3);

    fh.requestFocus(&lbl3);
    fh.focusPrevious();
    REQUIRE(fh.getFocused() == &lbl1);
}

TEST_CASE("FocusHandler focusPrevious with empty widget list", "[unit][focushandler]")
{
    FocusHandler fh;

    fh.focusPrevious();
    REQUIRE(fh.getFocused() == nullptr);
}

// ============================================================================
// tabNext / tabPrevious
// ============================================================================

TEST_CASE("FocusHandler tabNext focuses first tab-in enabled widget", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");

    lbl1._setFocusHandler(&fh);
    lbl2._setFocusHandler(&fh);

    fh.add(&lbl1);
    fh.add(&lbl2);

    fh.tabNext();
    REQUIRE(fh.getFocused() == &lbl1);
}

TEST_CASE("FocusHandler tabNext cycles forward", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");
    FocusableLabel lbl3("third");

    lbl1._setFocusHandler(&fh);
    lbl2._setFocusHandler(&fh);
    lbl3._setFocusHandler(&fh);

    fh.add(&lbl1);
    fh.add(&lbl2);
    fh.add(&lbl3);

    fh.tabNext();
    REQUIRE(fh.getFocused() == &lbl1);

    fh.tabNext();
    REQUIRE(fh.getFocused() == &lbl2);
}

TEST_CASE("FocusHandler tabNext wraps around", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");

    lbl1._setFocusHandler(&fh);
    lbl2._setFocusHandler(&fh);

    fh.add(&lbl1);
    fh.add(&lbl2);

    fh.requestFocus(&lbl2);
    fh.tabNext();
    REQUIRE(fh.getFocused() == &lbl1);
}

TEST_CASE("FocusHandler tabNext skips widgets with tab-in disabled", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");
    FocusableLabel lbl3("third");

    lbl1._setFocusHandler(&fh);
    lbl2._setFocusHandler(&fh);
    lbl3._setFocusHandler(&fh);

    lbl2.setTabInEnabled(false);

    fh.add(&lbl1);
    fh.add(&lbl2);
    fh.add(&lbl3);

    fh.requestFocus(&lbl1);
    fh.tabNext();
    REQUIRE(fh.getFocused() == &lbl3);
}

TEST_CASE("FocusHandler tabNext respects tab-out disabled on current widget", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");

    lbl1._setFocusHandler(&fh);
    lbl2._setFocusHandler(&fh);

    fh.add(&lbl1);
    fh.add(&lbl2);

    fh.requestFocus(&lbl1);
    lbl1.setTabOutEnabled(false);

    fh.tabNext();
    // Focus should stay on lbl1 because tab-out is disabled
    REQUIRE(fh.getFocused() == &lbl1);
}

TEST_CASE("FocusHandler tabNext with empty widget list", "[unit][focushandler]")
{
    FocusHandler fh;

    fh.tabNext();
    REQUIRE(fh.getFocused() == nullptr);
}

TEST_CASE("FocusHandler tabPrevious focuses last tab-in enabled widget", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");

    lbl1._setFocusHandler(&fh);
    lbl2._setFocusHandler(&fh);

    fh.add(&lbl1);
    fh.add(&lbl2);

    fh.requestFocus(&lbl2);
    fh.tabPrevious();
    REQUIRE(fh.getFocused() == &lbl1);
}

TEST_CASE("FocusHandler tabPrevious cycles backward", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");
    FocusableLabel lbl3("third");

    lbl1._setFocusHandler(&fh);
    lbl2._setFocusHandler(&fh);
    lbl3._setFocusHandler(&fh);

    fh.add(&lbl1);
    fh.add(&lbl2);
    fh.add(&lbl3);

    fh.requestFocus(&lbl3);
    fh.tabPrevious();
    REQUIRE(fh.getFocused() == &lbl2);

    fh.tabPrevious();
    REQUIRE(fh.getFocused() == &lbl1);
}

TEST_CASE("FocusHandler tabPrevious wraps around", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");

    lbl1._setFocusHandler(&fh);
    lbl2._setFocusHandler(&fh);

    fh.add(&lbl1);
    fh.add(&lbl2);

    fh.requestFocus(&lbl1);
    fh.tabPrevious();
    REQUIRE(fh.getFocused() == &lbl2);
}

TEST_CASE("FocusHandler tabPrevious skips widgets with tab-in disabled", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");
    FocusableLabel lbl3("third");

    lbl1._setFocusHandler(&fh);
    lbl2._setFocusHandler(&fh);
    lbl3._setFocusHandler(&fh);

    lbl2.setTabInEnabled(false);

    fh.add(&lbl1);
    fh.add(&lbl2);
    fh.add(&lbl3);

    fh.requestFocus(&lbl3);
    fh.tabPrevious();
    REQUIRE(fh.getFocused() == &lbl1);
}

TEST_CASE("FocusHandler tabPrevious respects tab-out disabled on current widget", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");

    lbl1._setFocusHandler(&fh);
    lbl2._setFocusHandler(&fh);

    fh.add(&lbl1);
    fh.add(&lbl2);

    fh.requestFocus(&lbl2);
    lbl2.setTabOutEnabled(false);

    fh.tabPrevious();
    // Focus should stay on lbl2 because tab-out is disabled
    REQUIRE(fh.getFocused() == &lbl2);
}

TEST_CASE("FocusHandler tabPrevious with empty widget list", "[unit][focushandler]")
{
    FocusHandler fh;

    fh.tabPrevious();
    REQUIRE(fh.getFocused() == nullptr);
}

// ============================================================================
// Modal focus: pushModal / popModal / clearModal / hasModalFocus
// ============================================================================

TEST_CASE("FocusHandler pushModal sets modal focus", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel modal("modal");

    fh.pushModal(&modal);

    REQUIRE(fh.hasModalFocus());
    REQUIRE(fh.getFocusOwner() == &modal);
}

TEST_CASE("FocusHandler pushModal with both focus and mouse owner", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel focusOwner("focus");
    FocusableLabel mouseOwner("mouse");

    fh.pushModal(&focusOwner, &mouseOwner);

    REQUIRE(fh.hasModalFocus());
    REQUIRE(fh.getFocusOwner() == &focusOwner);
    REQUIRE(fh.getMouseCaptureOwner() == &mouseOwner);
}

TEST_CASE("FocusHandler pushModal with nullptr for both is no-op", "[unit][focushandler]")
{
    FocusHandler fh;

    fh.pushModal(nullptr, nullptr);

    REQUIRE_FALSE(fh.hasModalFocus());
    REQUIRE(fh.getFocusOwner() == nullptr);
}

TEST_CASE("FocusHandler pushModal clears existing focus when modal is pushed", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel regular("regular");
    FocusableLabel modal("modal");

    regular._setFocusHandler(&fh);
    modal._setFocusHandler(&fh);

    fh.add(&regular);
    fh.requestFocus(&regular);
    REQUIRE(fh.getFocused() == &regular);

    fh.pushModal(&modal);
    // The regular widget should lose focus when modal is pushed
    REQUIRE_FALSE(fh.isFocused(&regular));
}

TEST_CASE("FocusHandler popModal restores previous state", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel modal1("modal1");
    FocusableLabel modal2("modal2");

    fh.add(&modal1);
    fh.add(&modal2);

    fh.pushModal(&modal1);
    fh.pushModal(&modal2);

    REQUIRE(fh.getFocusOwner() == &modal2);

    fh.popModal();
    REQUIRE(fh.getFocusOwner() == &modal1);
}

TEST_CASE("FocusHandler popModal on empty stack is no-op", "[unit][focushandler]")
{
    FocusHandler fh;

    REQUIRE_NOTHROW(fh.popModal());
    REQUIRE_FALSE(fh.hasModalFocus());
}

TEST_CASE("FocusHandler popModal clears focus owner when stack becomes empty", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel modal("modal");

    fh.pushModal(&modal);
    REQUIRE(fh.getFocusOwner() == &modal);

    fh.popModal();
    REQUIRE(fh.getFocusOwner() == nullptr);
    REQUIRE_FALSE(fh.hasModalFocus());
}

TEST_CASE("FocusHandler clearModal removes all modal states", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel modal1("modal1");
    FocusableLabel modal2("modal2");

    fh.pushModal(&modal1);
    fh.pushModal(&modal2);

    fh.clearModal();

    REQUIRE_FALSE(fh.hasModalFocus());
    REQUIRE(fh.getFocusOwner() == nullptr);
    REQUIRE(fh.getMouseCaptureOwner() == nullptr);
}

TEST_CASE("FocusHandler nested modal stack works correctly", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel m1("modal1");
    FocusableLabel m2("modal2");
    FocusableLabel m3("modal3");

    fh.add(&m1);
    fh.add(&m2);
    fh.add(&m3);

    fh.pushModal(&m1);
    fh.pushModal(&m2);
    fh.pushModal(&m3);

    REQUIRE(fh.getFocusOwner() == &m3);

    fh.popModal();
    REQUIRE(fh.getFocusOwner() == &m2);

    fh.popModal();
    REQUIRE(fh.getFocusOwner() == &m1);

    fh.popModal();
    REQUIRE(fh.getFocusOwner() == nullptr);
}

// ============================================================================
// ModalScope RAII
// ============================================================================

TEST_CASE("FocusHandler ModalScope pushes modal on construction", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel modal("modal");

    REQUIRE_FALSE(fh.hasModalFocus());

    {
        FocusHandler::ModalScope scope(&fh, &modal);
        REQUIRE(fh.hasModalFocus());
        REQUIRE(fh.getFocusOwner() == &modal);
    }

    // After scope destruction, modal should be popped
    REQUIRE_FALSE(fh.hasModalFocus());
}

TEST_CASE("FocusHandler ModalScope with nullptr handler is safe", "[unit][focushandler]")
{
    FocusableLabel modal("modal");

    // Should not crash even with nullptr handler
    FocusHandler::ModalScope scope(nullptr, &modal);
}

TEST_CASE("FocusHandler ModalScope release prevents pop on destruction", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel modal("modal");

    {
        FocusHandler::ModalScope scope(&fh, &modal);
        REQUIRE(fh.hasModalFocus());
        scope.release();
    }

    // Modal should still be present since release() was called
    REQUIRE(fh.hasModalFocus());
    REQUIRE(fh.getFocusOwner() == &modal);

    // Clean up
    fh.popModal();
}

TEST_CASE("FocusHandler ModalScope nested scopes work correctly", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel m1("modal1");
    FocusableLabel m2("modal2");

    fh.add(&m1);
    fh.add(&m2);

    {
        FocusHandler::ModalScope scope1(&fh, &m1);
        REQUIRE(fh.getFocusOwner() == &m1);

        {
            FocusHandler::ModalScope scope2(&fh, &m2);
            REQUIRE(fh.getFocusOwner() == &m2);
        }

        REQUIRE(fh.getFocusOwner() == &m1);
    }

    REQUIRE_FALSE(fh.hasModalFocus());
}

// ============================================================================
// Getter/Setter pairs for internal state tracking
// ============================================================================

TEST_CASE("FocusHandler dragged widget getter/setter", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("test");

    REQUIRE(fh.getDraggedWidget() == nullptr);

    fh.setDraggedWidget(&lbl);
    REQUIRE(fh.getDraggedWidget() == &lbl);

    fh.setDraggedWidget(nullptr);
    REQUIRE(fh.getDraggedWidget() == nullptr);
}

TEST_CASE("FocusHandler last widget with mouse getter/setter", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("test");

    REQUIRE(fh.getLastWidgetWithMouse() == nullptr);

    fh.setLastWidgetWithMouse(&lbl);
    REQUIRE(fh.getLastWidgetWithMouse() == &lbl);

    fh.setLastWidgetWithMouse(nullptr);
    REQUIRE(fh.getLastWidgetWithMouse() == nullptr);
}

TEST_CASE("FocusHandler last widget with modal focus getter/setter", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("test");

    REQUIRE(fh.getLastWidgetWithModalFocus() == nullptr);

    fh.setLastWidgetWithModalFocus(&lbl);
    REQUIRE(fh.getLastWidgetWithModalFocus() == &lbl);

    fh.setLastWidgetWithModalFocus(nullptr);
    REQUIRE(fh.getLastWidgetWithModalFocus() == nullptr);
}

TEST_CASE("FocusHandler last widget with modal mouse input focus getter/setter", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("test");

    REQUIRE(fh.getLastWidgetWithModalMouseInputFocus() == nullptr);

    fh.setLastWidgetWithModalMouseInputFocus(&lbl);
    REQUIRE(fh.getLastWidgetWithModalMouseInputFocus() == &lbl);

    fh.setLastWidgetWithModalMouseInputFocus(nullptr);
    REQUIRE(fh.getLastWidgetWithModalMouseInputFocus() == nullptr);
}

TEST_CASE("FocusHandler last widget pressed getter/setter", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("test");

    REQUIRE(fh.getLastWidgetPressed() == nullptr);

    fh.setLastWidgetPressed(&lbl);
    REQUIRE(fh.getLastWidgetPressed() == &lbl);

    fh.setLastWidgetPressed(nullptr);
    REQUIRE(fh.getLastWidgetPressed() == nullptr);
}

// ============================================================================
// widgetHidden
// ============================================================================

TEST_CASE("FocusHandler widgetHidden is callable", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("test");

    // widgetHidden is currently a no-op but should be callable
    REQUIRE_NOTHROW(fh.widgetHidden(&lbl));
}

TEST_CASE("FocusHandler widgetHidden with nullptr is safe", "[unit][focushandler]")
{
    FocusHandler fh;

    REQUIRE_NOTHROW(fh.widgetHidden(nullptr));
}

// ============================================================================
// Focus event distribution with listeners
// ============================================================================

TEST_CASE("FocusHandler focus events distributed to multiple listeners", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("test");
    TestFocusListener listener1;
    TestFocusListener listener2;

    lbl.addFocusListener(&listener1);
    lbl.addFocusListener(&listener2);

    fh.add(&lbl);
    fh.requestFocus(&lbl);

    REQUIRE(listener1.mFocusGainedCount == 1);
    REQUIRE(listener2.mFocusGainedCount == 1);
}

TEST_CASE("FocusHandler focus lost events distributed on focus switch", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");
    TestFocusListener listener1;
    TestFocusListener listener2;

    lbl1.addFocusListener(&listener1);
    lbl2.addFocusListener(&listener2);

    fh.add(&lbl1);
    fh.add(&lbl2);

    fh.requestFocus(&lbl1);
    fh.requestFocus(&lbl2);

    REQUIRE(listener1.mFocusLostCount == 1);
    REQUIRE(listener1.mFocusGainedCount == 1);
    REQUIRE(listener2.mFocusGainedCount == 1);
    REQUIRE(listener2.mFocusLostCount == 0);
}

// ============================================================================
// Edge cases and integration scenarios
// ============================================================================

TEST_CASE("FocusHandler focusNext distributes focus events", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");
    TestFocusListener listener1;
    TestFocusListener listener2;

    lbl1.addFocusListener(&listener1);
    lbl2.addFocusListener(&listener2);

    fh.add(&lbl1);
    fh.add(&lbl2);

    fh.focusNext();
    REQUIRE(listener1.mFocusGainedCount == 1);

    fh.focusNext();
    REQUIRE(listener1.mFocusLostCount == 1);
    REQUIRE(listener2.mFocusGainedCount == 1);
}

TEST_CASE("FocusHandler focusPrevious distributes focus events", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");
    TestFocusListener listener1;
    TestFocusListener listener2;

    lbl1.addFocusListener(&listener1);
    lbl2.addFocusListener(&listener2);

    fh.add(&lbl1);
    fh.add(&lbl2);

    fh.requestFocus(&lbl2);
    listener1.reset();
    listener2.reset();

    fh.focusPrevious();
    REQUIRE(listener2.mFocusLostCount == 1);
    REQUIRE(listener1.mFocusGainedCount == 1);
}

TEST_CASE("FocusHandler tabNext distributes focus events", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");
    TestFocusListener listener1;
    TestFocusListener listener2;

    lbl1._setFocusHandler(&fh);
    lbl2._setFocusHandler(&fh);

    lbl1.addFocusListener(&listener1);
    lbl2.addFocusListener(&listener2);

    fh.add(&lbl1);
    fh.add(&lbl2);

    fh.tabNext();
    REQUIRE(listener1.mFocusGainedCount == 1);

    fh.tabNext();
    REQUIRE(listener1.mFocusLostCount == 1);
    REQUIRE(listener2.mFocusGainedCount == 1);
}

TEST_CASE("FocusHandler tabPrevious distributes focus events", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");
    TestFocusListener listener1;
    TestFocusListener listener2;

    lbl1._setFocusHandler(&fh);
    lbl2._setFocusHandler(&fh);

    lbl1.addFocusListener(&listener1);
    lbl2.addFocusListener(&listener2);

    fh.add(&lbl1);
    fh.add(&lbl2);

    fh.requestFocus(&lbl2);
    listener1.reset();
    listener2.reset();

    fh.tabPrevious();
    REQUIRE(listener2.mFocusLostCount == 1);
    REQUIRE(listener1.mFocusGainedCount == 1);
}

TEST_CASE("FocusHandler with Container and child widgets", "[unit][focushandler]")
{
    FocusHandler fh;
    Container container;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");

    container.add(&lbl1);
    container.add(&lbl2);

    // Add children to focus handler
    fh.add(&lbl1);
    fh.add(&lbl2);

    fh.requestFocus(&lbl1);
    REQUIRE(fh.getFocused() == &lbl1);

    fh.focusNext();
    REQUIRE(fh.getFocused() == &lbl2);
}

TEST_CASE("FocusHandler isFocused returns false for non-focused widget", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");

    fh.add(&lbl1);
    fh.add(&lbl2);

    fh.requestFocus(&lbl1);

    REQUIRE(fh.isFocused(&lbl1));
    REQUIRE_FALSE(fh.isFocused(&lbl2));
}

TEST_CASE("FocusHandler isFocused with nullptr returns false", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("test");

    fh.add(&lbl);
    fh.requestFocus(&lbl);

    REQUIRE_FALSE(fh.isFocused(nullptr));
}

TEST_CASE("FocusHandler focusNext with single focusable widget", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("only");

    fh.add(&lbl);

    fh.focusNext();
    REQUIRE(fh.getFocused() == &lbl);

    // Calling again should stay on the same widget (wraps to itself)
    fh.focusNext();
    REQUIRE(fh.getFocused() == &lbl);
}

TEST_CASE("FocusHandler focusPrevious with single focusable widget", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("only");

    fh.add(&lbl);

    // First call: no widget focused, loop sets focusedWidget to -1 (single widget edge case)
    fh.focusPrevious();
    // With single widget and no initial focus, focusPrevious doesn't set focus
    REQUIRE(fh.getFocused() == nullptr);

    // Manually set focus and try again
    fh.requestFocus(&lbl);
    REQUIRE(fh.getFocused() == &lbl);

    // Second call: widget is focused, loop finds focusedWidget == focused and returns early
    fh.focusPrevious();
    REQUIRE(fh.getFocused() == &lbl);
}

TEST_CASE("FocusHandler tabNext with single focusable widget", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("only");

    lbl._setFocusHandler(&fh);

    fh.add(&lbl);

    fh.tabNext();
    REQUIRE(fh.getFocused() == &lbl);
}

TEST_CASE("FocusHandler tabPrevious with single focusable widget", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl("only");

    lbl._setFocusHandler(&fh);

    fh.add(&lbl);

    fh.requestFocus(&lbl);
    fh.tabPrevious();
    // With single widget, tabPrevious will loop and find no other widget,
    // so focus stays on the same widget
    REQUIRE(fh.getFocused() == &lbl);
}

TEST_CASE("FocusHandler focusNext with all widgets having tab-in disabled", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");

    lbl1.setTabInEnabled(false);
    lbl2.setTabInEnabled(false);

    fh.add(&lbl1);
    fh.add(&lbl2);

    // focusNext does not check tab-in, only focusable
    fh.focusNext();
    REQUIRE(fh.getFocused() == &lbl1);
}

TEST_CASE("FocusHandler tabNext with all widgets having tab-in disabled", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");

    lbl1._setFocusHandler(&fh);
    lbl2._setFocusHandler(&fh);

    lbl1.setTabInEnabled(false);
    lbl2.setTabInEnabled(false);

    fh.add(&lbl1);
    fh.add(&lbl2);

    fh.tabNext();
    // No widget allows tab-in, so focus should remain nullptr
    REQUIRE(fh.getFocused() == nullptr);
}

TEST_CASE("FocusHandler pushModal with only mouse owner", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel mouseOwner("mouse");

    fh.pushModal(nullptr, &mouseOwner);

    REQUIRE(fh.hasModalFocus());
    REQUIRE(fh.getFocusOwner() == nullptr);
    REQUIRE(fh.getMouseCaptureOwner() == &mouseOwner);
}

TEST_CASE("FocusHandler pushModal with only focus owner", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel focusOwner("focus");

    fh.pushModal(&focusOwner, nullptr);

    REQUIRE(fh.hasModalFocus());
    REQUIRE(fh.getFocusOwner() == &focusOwner);
    REQUIRE(fh.getMouseCaptureOwner() == nullptr);
}

TEST_CASE("FocusHandler multiple widgets focus cycling", "[unit][focushandler]")
{
    FocusHandler fh;
    FocusableLabel lbl1("first");
    FocusableLabel lbl2("second");
    FocusableLabel lbl3("third");
    FocusableLabel lbl4("fourth");

    fh.add(&lbl1);
    fh.add(&lbl2);
    fh.add(&lbl3);
    fh.add(&lbl4);

    // Test full cycle forward
    fh.focusNext();
    REQUIRE(fh.getFocused() == &lbl1);
    fh.focusNext();
    REQUIRE(fh.getFocused() == &lbl2);
    fh.focusNext();
    REQUIRE(fh.getFocused() == &lbl3);
    fh.focusNext();
    REQUIRE(fh.getFocused() == &lbl4);
    fh.focusNext();
    REQUIRE(fh.getFocused() == &lbl1); // wrapped

    // Test full cycle backward
    fh.focusPrevious();
    REQUIRE(fh.getFocused() == &lbl4);
    fh.focusPrevious();
    REQUIRE(fh.getFocused() == &lbl3);
    fh.focusPrevious();
    REQUIRE(fh.getFocused() == &lbl2);
    fh.focusPrevious();
    REQUIRE(fh.getFocused() == &lbl1);
    fh.focusPrevious();
    REQUIRE(fh.getFocused() == &lbl4); // wrapped
}
