// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_SCROLLAREA_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_SCROLLAREA_HPP_

#include <string>

#include "fifechan/mouselistener.hpp"
#include "fifechan/platform.hpp"
#include "fifechan/widget.hpp"

namespace fcn
{
    /**
     * Implementation if a scrollable area used to view widgets larger than the scroll area.
     * A scroll area can be customized to always show scroll bars or to show them only when
     * necessary.
     */
    class FIFEGUI_API ScrollArea : public MouseListener, public Widget
    {
    public:
        /**
         * Scroll policies for the horizontal and vertical scrollbar.
         * The policies are:
         *
         * SHOW_ALWAYS - Always show the scrollbars no matter what.
         * SHOW_NEVER  - Never show the scrollbars no matter what.
         * SHOW_AUTO   - Show the scrollbars only when needed. That is if the
         *               content grows larger then the ScrollArea.
         */
        enum class ScrollPolicy : uint8_t
        {
            ShowAlways = 0,
            ShowNever,
            ShowAuto
        };

        ScrollArea();

        /**
         * Constructor.
         *
         * @param content The content of the scroll area.
         */
        explicit ScrollArea(Widget* content);

        /**
         * Constructor.
         *
         * @param content The content of the scroll area.
         * @param hPolicy The policy for the horizontal scrollbar. See enum with
         *                policies.
         * @param vPolicy The policy for the vertical scrollbar. See enum with
         *                policies.
         */
        ScrollArea(Widget* content, ScrollPolicy hPolicy, ScrollPolicy vPolicy);

        ~ScrollArea() override;

        /**
         * Sets the content.
         *
         * @param widget The content of the scroll area.
         */
        void setContent(Widget* widget);

        /**
         * Gets the content.
         *
         * @return The content of the scroll area.
         */
        Widget* getContent() const;

        /**
         * Sets the horizontal scrollbar policy. See enum with policies.
         *
         * @param hPolicy The policy for the horizontal scrollbar.
         * @see getHorizontalScrollPolicy
         */
        void setHorizontalScrollPolicy(ScrollPolicy hPolicy);

        /**
         * Gets the horizontal scrollbar policy. See enum with policies.
         *
         * @return The policy for the horizontal scrollbar policy.
         * @see setHorizontalScrollPolicy, setScrollPolicy
         */
        ScrollPolicy getHorizontalScrollPolicy() const;

        /**
         * Sets the vertical scrollbar policy. See enum with policies.
         *
         * @param vPolicy The policy for the vertical scrollbar.
         * @see getVerticalScrollPolicy
         */
        void setVerticalScrollPolicy(ScrollPolicy vPolicy);

        /**
         * Gets the vertical scrollbar policy. See enum with policies.
         *
         * @return The policy for the vertical scrollbar.
         * @see setVerticalScrollPolicy, setScrollPolicy
         */
        ScrollPolicy getVerticalScrollPolicy() const;

        /**
         * Sets the horizontal and vertical scrollbar policy.
         *
         * @param hPolicy The policy for the horizontal scrollbar.
         * @param vPolicy The policy for the vertical scrollbar.
         * @see getVerticalScrollPolicy, getHorizontalScrollPolicy
         */
        void setScrollPolicy(ScrollPolicy hPolicy, ScrollPolicy vPolicy);

        /**
         * Sets the amount to scroll vertically.
         *
         * @param vScroll The amount to scroll.
         * @see getVerticalScrollAmount
         */
        void setVerticalScrollAmount(int vScroll);

        /**
         * Gets the amount that is scrolled vertically.
         *
         * @return The scroll amount on vertical scroll.
         * @see setVerticalScrollAmount, setScrollAmount
         */
        int getVerticalScrollAmount() const;

        /**
         * Sets the amount to scroll horizontally.
         *
         * @param hScroll The amount to scroll.
         * @see getHorizontalScrollAmount
         */
        void setHorizontalScrollAmount(int hScroll);

        /**
         * Gets the amount that is scrolled horizontally.
         *
         * @return The scroll amount on horizontal scroll.
         * @see setHorizontalScrollAmount, setScrollAmount
         */
        int getHorizontalScrollAmount() const;

        /**
         * Sets the amount to scroll horizontally and vertically.
         *
         * @param hScroll The amount to scroll on horizontal scroll.
         * @param vScroll The amount to scroll on vertical scroll.
         * @see getHorizontalScrollAmount, getVerticalScrollAmount
         */
        void setScrollAmount(int hScroll, int vScroll);

        /**
         * Gets the maximum amount of horizontal scroll.
         *
         * @return The horizontal max scroll.
         */
        int getHorizontalMaxScroll();

        /**
         * Gets the maximum amount of vertical scroll.
         *
         * @return The vertical max scroll.
         */
        int getVerticalMaxScroll();

        /**
         * Sets the width of the scroll bars.
         *
         * @param width The width of the scroll bars.
         * @see getScrollbarWidth
         */
        void setScrollbarWidth(int width);

        /**
         * Gets the width of the scroll bars.
         *
         * @return the width of the ScrollBar.
         * @see setScrollbarWidth
         */
        int getScrollbarWidth() const;

        /**
         * Sets the amount to scroll in pixels when the left scroll button is
         * pushed.
         *
         * @param amount The amount to scroll in pixels.
         * @see getLeftButtonScrollAmount
         */
        void setLeftButtonScrollAmount(int amount);

        /**
         * Sets the amount to scroll in pixels when the right scroll button is
         * pushed.
         *
         * @param amount The amount to scroll in pixels.
         * @see getRightButtonScrollAmount
         */
        void setRightButtonScrollAmount(int amount);

        /**
         * Sets the amount to scroll in pixels when the up scroll button is
         * pushed.
         *
         * @param amount The amount to scroll in pixels.
         * @see getUpButtonScrollAmount
         */
        void setUpButtonScrollAmount(int amount);

        /**
         * Sets the amount to scroll in pixels when the down scroll button is
         * pushed.
         *
         * @param amount The amount to scroll in pixels.
         * @see getDownButtonScrollAmount
         */
        void setDownButtonScrollAmount(int amount);

        /**
         * Gets the amount to scroll in pixels when the left scroll button is
         * pushed.
         *
         * @return The amount to scroll in pixels.
         * @see setLeftButtonScrollAmount
         */
        int getLeftButtonScrollAmount() const;

        /**
         * Gets the amount to scroll in pixels when the right scroll button is
         * pushed.
         *
         * @return The amount to scroll in pixels.
         * @see setRightButtonScrollAmount
         */
        int getRightButtonScrollAmount() const;

        /**
         * Gets the amount to scroll in pixels when the up scroll button is
         * pushed.
         *
         * @return The amount to scroll in pixels.
         * @see setUpButtonScrollAmount
         */
        int getUpButtonScrollAmount() const;

        /**
         * Gets the amount to scroll in pixels when the down scroll button is
         * pushed.
         *
         * @return The amount to scroll in pixels.
         * @see setDownButtonScrollAmount
         */
        int getDownButtonScrollAmount() const;

        /**
         * Sets the scroll area to be opaque, that is sets the scroll area
         * to display its background.
         *
         * @param opaque True if the scroll area should be opaque, false otherwise.
         */
        void setOpaque(bool opaque);

        /**
         * Checks if the scroll area is opaque, that is if the scroll area
         * displays its background.
         *
         * @return True if the scroll area is opaque, false otherwise.
         */
        bool isOpaque() const;

        // Inherited from BasicContainer

        virtual void showWidgetPart(Widget* widget, Rectangle area);

        virtual Rectangle getChildrenArea();

        virtual Widget* getWidgetAt(int x, int y);

        // Inherited from Widget

        virtual void draw(Graphics* graphics);

        virtual void logic();

        void setWidth(int width);

        void setHeight(int height);

        void setDimension(Rectangle const & dimension);

        virtual void resizeToContent(bool recursion = true);

        virtual void adjustSize();

        virtual void expandContent(bool recursion = true);

        // Inherited from MouseListener

        virtual void mousePressed(MouseEvent& mouseEvent);

        virtual void mouseReleased(MouseEvent& mouseEvent);

        virtual void mouseDragged(MouseEvent& mouseEvent);

        virtual void mouseWheelMovedUp(MouseEvent& mouseEvent);

        virtual void mouseWheelMovedDown(MouseEvent& mouseEvent);

        virtual void mouseWheelMovedRight(MouseEvent& mouseEvent);

        virtual void mouseWheelMovedLeft(MouseEvent& mouseEvent);

    protected:
        /**
         * Draws the background of the scroll area, that is
         * the area behind the content.
         *
         * @param graphics a Graphics object to draw with.
         */
        virtual void drawBackground(Graphics* graphics);

        /**
         * Draws the up button.
         *
         * @param graphics a Graphics object to draw with.
         */
        virtual void drawUpButton(Graphics* graphics);

        /**
         * Draws the down button.
         *
         * @param graphics a Graphics object to draw with.
         */
        virtual void drawDownButton(Graphics* graphics);

        /**
         * Draws the left button.
         *
         * @param graphics a Graphics object to draw with.
         */
        virtual void drawLeftButton(Graphics* graphics);

        /**
         * Draws the right button.
         *
         * @param graphics a Graphics object to draw with.
         */
        virtual void drawRightButton(Graphics* graphics);

        /**
         * Draws the vertical scroll bar.
         *
         * @param graphics a Graphics object to draw with.
         */
        virtual void drawVBar(Graphics* graphics);

        /**
         * Draws the horizontal scroll bar.
         *
         * @param graphics a Graphics object to draw with.
         */
        virtual void drawHBar(Graphics* graphics);

        /**
         * Draws the vertical marker.
         *
         * @param graphics a Graphics object to draw with.
         */
        virtual void drawVMarker(Graphics* graphics);

        /**
         * Draws the horizontal marker.
         *
         * @param graphics a Graphics object to draw with.
         */
        virtual void drawHMarker(Graphics* graphics);

        /**
         * Checks the policies for the scroll bars.
         */
        virtual void checkPolicies();

        /**
         * Gets the up button dimension.
         *
         * @return the dimension of the up button.
         */
        Rectangle getUpButtonDimension();

        /**
         * Gets the down button dimension.
         *
         * @return the dimension of the down button.
         */
        Rectangle getDownButtonDimension();

        /**
         * Gets the left button dimension.
         *
         * @return the dimension of the left button.
         */
        Rectangle getLeftButtonDimension();

        /**
         * Gets the right button dimension.
         *
         * @return the dimension of the right button.
         */
        Rectangle getRightButtonDimension();

        /**
         * Gets the vertical scrollbar dimension.
         *
         * @return the dimension of the vertical scrollbar.
         */
        Rectangle getVerticalBarDimension();

        /**
         * Gets the horizontal scrollbar dimension.
         *
         * @return the dimension of the horizontal scrollbar.
         */
        Rectangle getHorizontalBarDimension();

        /**
         * Gets the vertical marker dimension.
         *
         * @return the dimension of the vertical marker.
         */
        Rectangle getVerticalMarkerDimension();

        /**
         * Gets the horizontal marker dimension.
         *
         * @return the dimension of the horizontal marker.
         */
        Rectangle getHorizontalMarkerDimension();

        /**
         * Holds the vertical scroll amount.
         */
        int mVScroll{0};

        /**
         * Holds the horizontal scroll amount.
         */
        int mHScroll{0};

        /**
         * Holds the width of the scroll bars.
         */
        int mScrollbarWidth{12};

        /**
         * Holds the horizontal scroll bar policy.
         */
        ScrollPolicy mHPolicy{ScrollPolicy::ShowAuto};

        /**
         * Holds the vertical scroll bar policy.
         */
        ScrollPolicy mVPolicy{ScrollPolicy::ShowAuto};

        /**
         * True if the vertical scroll bar is visible, false otherwise.
         */
        bool mVBarVisible{false};

        /**
         * True if the horizontal scroll bar is visible, false otherwise.
         */
        bool mHBarVisible{false};

        /**
         * True if the up button is pressed, false otherwise.
         */
        bool mUpButtonPressed{false};

        /**
         * True if the down button is pressed, false otherwise.
         */
        bool mDownButtonPressed{false};

        /**
         * True if the left button is pressed, false otherwise.
         */
        bool mLeftButtonPressed{false};

        /**
         * True if the right button is pressed, false otherwise.
         */
        bool mRightButtonPressed{false};

        /**
         * Holds the up button scroll amount.
         */
        int mUpButtonScrollAmount{10};

        /**
         * Holds the down button scroll amount.
         */
        int mDownButtonScrollAmount{10};

        /**
         * Holds the left button scroll amount.
         */
        int mLeftButtonScrollAmount{10};

        /**
         * Holds the right button scroll amount.
         */
        int mRightButtonScrollAmount{10};

        /**
         * True if the vertical marked is dragged.
         */
        bool mIsVerticalMarkerDragged{false};

        /**
         * True if the horizontal marked is dragged.
         */
        bool mIsHorizontalMarkerDragged{false};

        /**
         * Holds the horizontal markers drag offset.
         */
        int mHorizontalMarkerDragOffset{0};

        /**
         * Holds the vertical markers drag offset.
         */
        int mVerticalMarkerDragOffset{0};

        /**
         * True if the scroll area should be opaque (that is
         * display its background), false otherwise.
         */
        bool mOpaque{true};
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_SCROLLAREA_HPP_
