// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/widgets/scrollarea.hpp"

#include <algorithm>

#include "fifechan/exception.hpp"
#include "fifechan/graphics.hpp"

namespace fcn
{
    ScrollArea::ScrollArea()
    {
        addMouseListener(this);
    }

    ScrollArea::ScrollArea(Widget* content)
    {
        setContent(content);
        addMouseListener(this);
    }

    ScrollArea::ScrollArea(Widget* content, ScrollPolicy /*hPolicy*/, ScrollPolicy /*vPolicy*/)
    {
        setContent(content);
        addMouseListener(this);
    }

    ScrollArea::~ScrollArea()
    {
        try {
            setContent(nullptr);
        } catch (std::exception&) {
            // logError("Exception in ScrollArea destructor: " + std::string(e.what()));
        }
    }

    void ScrollArea::setContent(Widget* widget)
    {
        removeAllChildren();

        if (widget != nullptr) {
            add(widget);
            widget->setPosition(0, 0);
        }

        checkPolicies();
    }

    Widget* ScrollArea::getContent() const
    {
        if (!mChildren.empty()) {
            return *mChildren.begin();
        }

        return nullptr;
    }

    void ScrollArea::setHorizontalScrollPolicy(ScrollPolicy hPolicy)
    {
        mHPolicy = hPolicy;
        checkPolicies();
    }

    ScrollArea::ScrollPolicy ScrollArea::getHorizontalScrollPolicy() const
    {
        return mHPolicy;
    }

    void ScrollArea::setVerticalScrollPolicy(ScrollPolicy vPolicy)
    {
        mVPolicy = vPolicy;
        checkPolicies();
    }

    ScrollArea::ScrollPolicy ScrollArea::getVerticalScrollPolicy() const
    {
        return mVPolicy;
    }

    void ScrollArea::setScrollPolicy(ScrollPolicy hPolicy, ScrollPolicy vPolicy)
    {
        mHPolicy = hPolicy;
        mVPolicy = vPolicy;
        checkPolicies();
    }

    void ScrollArea::setVerticalScrollAmount(int vScroll)
    {
        int const max = getVerticalMaxScroll();

        mVScroll = vScroll;

        if (vScroll > max) {
            mVScroll = max;
        }

        if (vScroll < 0) {
            mVScroll = 0;
        }
    }

    int ScrollArea::getVerticalScrollAmount() const
    {
        return mVScroll;
    }

    void ScrollArea::setHorizontalScrollAmount(int hScroll)
    {
        int const max = getHorizontalMaxScroll();

        mHScroll = hScroll;

        if (hScroll > max) {
            mHScroll = max;
        } else if (hScroll < 0) {
            mHScroll = 0;
        }
    }

    int ScrollArea::getHorizontalScrollAmount() const
    {
        return mHScroll;
    }

    void ScrollArea::setScrollAmount(int hScroll, int vScroll)
    {
        setHorizontalScrollAmount(hScroll);
        setVerticalScrollAmount(vScroll);
    }

    int ScrollArea::getHorizontalMaxScroll()
    {
        checkPolicies();

        if (getContent() == nullptr) {
            return 0;
        }

        int const value = getContent()->getWidth() - getChildrenArea().width + (2 * getContent()->getBorderSize());

        if (value < 0) {
            return 0;
        }

        return value;
    }

    int ScrollArea::getVerticalMaxScroll()
    {
        checkPolicies();

        if (getContent() == nullptr) {
            return 0;
        }

        int value = 0;

        value = getContent()->getHeight() - getChildrenArea().height + 2 * getContent()->getBorderSize();

        if (value < 0) {
            return 0;
        }

        return value;
    }

    void ScrollArea::setScrollbarWidth(int width)
    {
        if (width <= 0) {
            throwException("Width should be greater then 0.");
        }

        mScrollbarWidth = width;
    }

    int ScrollArea::getScrollbarWidth() const
    {
        return mScrollbarWidth;
    }

    void ScrollArea::mousePressed(MouseEvent& mouseEvent)
    {
        int const x = mouseEvent.getX();
        int const y = mouseEvent.getY();

        if (getUpButtonDimension().isContaining(x, y)) {
            setVerticalScrollAmount(getVerticalScrollAmount() - mUpButtonScrollAmount);
            mUpButtonPressed = true;
        } else if (getDownButtonDimension().isContaining(x, y)) {
            setVerticalScrollAmount(getVerticalScrollAmount() + mDownButtonScrollAmount);
            mDownButtonPressed = true;
        } else if (getLeftButtonDimension().isContaining(x, y)) {
            setHorizontalScrollAmount(getHorizontalScrollAmount() - mLeftButtonScrollAmount);
            mLeftButtonPressed = true;
        } else if (getRightButtonDimension().isContaining(x, y)) {
            setHorizontalScrollAmount(getHorizontalScrollAmount() + mRightButtonScrollAmount);
            mRightButtonPressed = true;
        } else if (getVerticalMarkerDimension().isContaining(x, y)) {
            mIsHorizontalMarkerDragged = false;
            mIsVerticalMarkerDragged   = true;

            mVerticalMarkerDragOffset = y - getVerticalMarkerDimension().y;
        } else if (getVerticalBarDimension().isContaining(x, y)) {
            if (y < getVerticalMarkerDimension().y) {
                setVerticalScrollAmount(
                    getVerticalScrollAmount() - static_cast<int>((getChildrenArea().height * 0.95)));
            } else {
                setVerticalScrollAmount(
                    getVerticalScrollAmount() + static_cast<int>((getChildrenArea().height * 0.95)));
            }
        } else if (getHorizontalMarkerDimension().isContaining(x, y)) {
            mIsHorizontalMarkerDragged = true;
            mIsVerticalMarkerDragged   = false;

            mHorizontalMarkerDragOffset = x - getHorizontalMarkerDimension().x;
        } else if (getHorizontalBarDimension().isContaining(x, y)) {
            if (x < getHorizontalMarkerDimension().x) {
                setHorizontalScrollAmount(
                    getHorizontalScrollAmount() - static_cast<int>((getChildrenArea().width * 0.95)));
            } else {
                setHorizontalScrollAmount(
                    getHorizontalScrollAmount() + static_cast<int>((getChildrenArea().width * 0.95)));
            }
        }
    }

    void ScrollArea::mouseReleased(MouseEvent& mouseEvent)
    {
        mUpButtonPressed           = false;
        mDownButtonPressed         = false;
        mLeftButtonPressed         = false;
        mRightButtonPressed        = false;
        mIsHorizontalMarkerDragged = false;
        mIsVerticalMarkerDragged   = false;

        mouseEvent.consume();
    }

    void ScrollArea::mouseDragged(MouseEvent& mouseEvent)
    {
        if (mIsVerticalMarkerDragged) {
            int const pos    = mouseEvent.getY() - getVerticalBarDimension().y - mVerticalMarkerDragOffset;
            int const length = getVerticalMarkerDimension().height;

            Rectangle const barDim = getVerticalBarDimension();

            if ((barDim.height - length) > 0) {
                setVerticalScrollAmount((getVerticalMaxScroll() * pos) / (barDim.height - length));
            } else {
                setVerticalScrollAmount(0);
            }
        }

        if (mIsHorizontalMarkerDragged) {
            int const pos    = mouseEvent.getX() - getHorizontalBarDimension().x - mHorizontalMarkerDragOffset;
            int const length = getHorizontalMarkerDimension().width;

            Rectangle const barDim = getHorizontalBarDimension();

            if ((barDim.width - length) > 0) {
                setHorizontalScrollAmount((getHorizontalMaxScroll() * pos) / (barDim.width - length));
            } else {
                setHorizontalScrollAmount(0);
            }
        }

        mouseEvent.consume();
    }

    void ScrollArea::draw(Graphics* graphics)
    {
        drawBackground(graphics);

        if (mVBarVisible) {
            drawUpButton(graphics);
            drawDownButton(graphics);
            drawVBar(graphics);
            drawVMarker(graphics);
        }

        if (mHBarVisible) {
            drawLeftButton(graphics);
            drawRightButton(graphics);
            drawHBar(graphics);
            drawHMarker(graphics);
        }

        if (mHBarVisible && mVBarVisible) {
            graphics->setColor(getBaseColor());
            graphics->fillRectangle(
                getWidth() - mScrollbarWidth, getHeight() - mScrollbarWidth, mScrollbarWidth, mScrollbarWidth);
        }
    }

    void ScrollArea::drawHBar(Graphics* graphics)
    {
        Rectangle const dim = getHorizontalBarDimension();

        graphics->pushClipArea(dim);

        int const alpha   = getBaseColor().a;
        Color trackColor  = getBaseColor() - 0x101010;
        trackColor.a      = alpha;
        Color shadowColor = getBaseColor() - 0x303030;
        shadowColor.a     = alpha;

        graphics->setColor(trackColor);
        graphics->fillRectangle(0, 0, dim.width, dim.height);

        graphics->setColor(shadowColor);
        graphics->drawLine(0, 0, dim.width, 0);

        graphics->popClipArea();
    }

    void ScrollArea::drawVBar(Graphics* graphics)
    {
        Rectangle const dim = getVerticalBarDimension();

        graphics->pushClipArea(dim);

        int const alpha   = getBaseColor().a;
        Color trackColor  = getBaseColor() - 0x101010;
        trackColor.a      = alpha;
        Color shadowColor = getBaseColor() - 0x303030;
        shadowColor.a     = alpha;

        graphics->setColor(trackColor);
        graphics->fillRectangle(0, 0, dim.width, dim.height);

        graphics->setColor(shadowColor);
        graphics->drawLine(0, 0, 0, dim.height);

        graphics->popClipArea();
    }

    void ScrollArea::drawBackground(Graphics* graphics)
    {
        if (isOpaque()) {
            graphics->setColor(getBackgroundColor());
            graphics->fillRectangle(getChildrenArea());
        }
    }

    void ScrollArea::drawUpButton(Graphics* graphics)
    {
        Rectangle const dim = getUpButtonDimension();

        graphics->pushClipArea(dim);

        Color highlightColor;
        Color shadowColor;
        Color faceColor;
        int offset      = 0;
        int const alpha = getBaseColor().a;

        if (mUpButtonPressed) {
            faceColor        = getBaseColor() - 0x303030;
            faceColor.a      = alpha;
            highlightColor   = faceColor - 0x303030;
            highlightColor.a = alpha;
            shadowColor      = getBaseColor();
            shadowColor.a    = alpha;

            offset = 1;
        } else {
            faceColor        = getBaseColor();
            faceColor.a      = alpha;
            highlightColor   = faceColor + 0x303030;
            highlightColor.a = alpha;
            shadowColor      = faceColor - 0x303030;
            shadowColor.a    = alpha;

            offset = 0;
        }

        graphics->setColor(faceColor);
        graphics->fillRectangle(0, 0, dim.width, dim.height);

        graphics->setColor(highlightColor);
        graphics->drawLine(0, 0, dim.width - 1, 0);
        graphics->drawLine(0, 1, 0, dim.height - 1);

        graphics->setColor(shadowColor);
        graphics->drawLine(dim.width - 1, 0, dim.width - 1, dim.height - 1);
        graphics->drawLine(1, dim.height - 1, dim.width - 1, dim.height - 1);

        graphics->setColor(getForegroundColor());

        int i       = 0;
        int const w = dim.height / 2;
        int const h = (w / 2) + 2;
        for (i = 0; i < w / 2; ++i) {
            graphics->drawLine(w - i + offset, i + h + offset, w + i + offset, i + h + offset);
        }

        graphics->popClipArea();
    }

    void ScrollArea::drawDownButton(Graphics* graphics)
    {
        Rectangle const dim = getDownButtonDimension();

        graphics->pushClipArea(dim);

        Color highlightColor;
        Color shadowColor;
        Color faceColor;
        int offset      = 0;
        int const alpha = getBaseColor().a;

        if (mDownButtonPressed) {
            faceColor        = getBaseColor() - 0x303030;
            faceColor.a      = alpha;
            highlightColor   = faceColor - 0x303030;
            highlightColor.a = alpha;
            shadowColor      = getBaseColor();
            shadowColor.a    = alpha;

            offset = 1;
        } else {
            faceColor        = getBaseColor();
            faceColor.a      = alpha;
            highlightColor   = faceColor + 0x303030;
            highlightColor.a = alpha;
            shadowColor      = faceColor - 0x303030;
            shadowColor.a    = alpha;

            offset = 0;
        }

        graphics->setColor(faceColor);
        graphics->fillRectangle(0, 0, dim.width, dim.height);

        graphics->setColor(highlightColor);
        graphics->drawLine(0, 0, dim.width - 1, 0);
        graphics->drawLine(0, 1, 0, dim.height - 1);

        graphics->setColor(shadowColor);
        graphics->drawLine(dim.width - 1, 0, dim.width - 1, dim.height - 1);
        graphics->drawLine(1, dim.height - 1, dim.width - 1, dim.height - 1);

        graphics->setColor(getForegroundColor());

        int i       = 0;
        int const w = dim.height / 2;
        int const h = w + 1;
        for (i = 0; i < w / 2; ++i) {
            graphics->drawLine(w - i + offset, -i + h + offset, w + i + offset, -i + h + offset);
        }

        graphics->popClipArea();
    }

    void ScrollArea::drawLeftButton(Graphics* graphics)
    {
        Rectangle const dim = getLeftButtonDimension();
        graphics->pushClipArea(dim);

        Color highlightColor;
        Color shadowColor;
        Color faceColor;
        int offset      = 0;
        int const alpha = getBaseColor().a;

        if (mLeftButtonPressed) {
            faceColor        = getBaseColor() - 0x303030;
            faceColor.a      = alpha;
            highlightColor   = faceColor - 0x303030;
            highlightColor.a = alpha;
            shadowColor      = getBaseColor();
            shadowColor.a    = alpha;

            offset = 1;
        } else {
            faceColor        = getBaseColor();
            faceColor.a      = alpha;
            highlightColor   = faceColor + 0x303030;
            highlightColor.a = alpha;
            shadowColor      = faceColor - 0x303030;
            shadowColor.a    = alpha;

            offset = 0;
        }

        graphics->setColor(faceColor);
        graphics->fillRectangle(0, 0, dim.width, dim.height);

        graphics->setColor(highlightColor);
        graphics->drawLine(0, 0, dim.width - 1, 0);
        graphics->drawLine(0, 1, 0, dim.height - 1);

        graphics->setColor(shadowColor);
        graphics->drawLine(dim.width - 1, 0, dim.width - 1, dim.height - 1);
        graphics->drawLine(1, dim.height - 1, dim.width - 1, dim.height - 1);

        graphics->setColor(getForegroundColor());

        int i       = 0;
        int const w = dim.width / 2;
        int const h = w - 2;
        for (i = 0; i < w / 2; ++i) {
            graphics->drawLine(i + h + offset, w - i + offset, i + h + offset, w + i + offset);
        }

        graphics->popClipArea();
    }

    void ScrollArea::drawRightButton(Graphics* graphics)
    {
        Rectangle const dim = getRightButtonDimension();

        graphics->pushClipArea(dim);

        Color highlightColor;
        Color shadowColor;
        Color faceColor;
        int offset      = 0;
        int const alpha = getBaseColor().a;

        if (mRightButtonPressed) {
            faceColor        = getBaseColor() - 0x303030;
            faceColor.a      = alpha;
            highlightColor   = faceColor - 0x303030;
            highlightColor.a = alpha;
            shadowColor      = getBaseColor();
            shadowColor.a    = alpha;

            offset = 1;
        } else {
            faceColor        = getBaseColor();
            faceColor.a      = alpha;
            highlightColor   = faceColor + 0x303030;
            highlightColor.a = alpha;
            shadowColor      = faceColor - 0x303030;
            shadowColor.a    = alpha;

            offset = 0;
        }

        graphics->setColor(faceColor);
        graphics->fillRectangle(0, 0, dim.width, dim.height);

        graphics->setColor(highlightColor);
        graphics->drawLine(0, 0, dim.width - 1, 0);
        graphics->drawLine(0, 1, 0, dim.height - 1);

        graphics->setColor(shadowColor);
        graphics->drawLine(dim.width - 1, 0, dim.width - 1, dim.height - 1);
        graphics->drawLine(1, dim.height - 1, dim.width - 1, dim.height - 1);

        graphics->setColor(getForegroundColor());

        int i       = 0;
        int const w = dim.width / 2;
        int const h = w + 1;
        for (i = 0; i < w / 2; ++i) {
            graphics->drawLine(-i + h + offset, w - i + offset, -i + h + offset, w + i + offset);
        }

        graphics->popClipArea();
    }

    void ScrollArea::drawVMarker(Graphics* graphics)
    {
        Rectangle const dim = getVerticalMarkerDimension();
        graphics->pushClipArea(dim);

        int const alpha      = getBaseColor().a;
        Color faceColor      = getBaseColor();
        faceColor.a          = alpha;
        Color highlightColor = faceColor + 0x303030;
        highlightColor.a     = alpha;
        Color shadowColor    = faceColor - 0x303030;
        shadowColor.a        = alpha;

        graphics->setColor(faceColor);
        graphics->fillRectangle(1, 1, dim.width - 1, dim.height - 1);

        graphics->setColor(highlightColor);
        graphics->drawLine(0, 0, dim.width - 1, 0);
        graphics->drawLine(0, 1, 0, dim.height - 1);

        graphics->setColor(shadowColor);
        graphics->drawLine(1, dim.height - 1, dim.width - 1, dim.height - 1);
        graphics->drawLine(dim.width - 1, 0, dim.width - 1, dim.height - 1);

        graphics->popClipArea();
    }

    void ScrollArea::drawHMarker(Graphics* graphics)
    {
        Rectangle const dim = getHorizontalMarkerDimension();
        graphics->pushClipArea(dim);

        int const alpha      = getBaseColor().a;
        Color faceColor      = getBaseColor();
        faceColor.a          = alpha;
        Color highlightColor = faceColor + 0x303030;
        highlightColor.a     = alpha;
        Color shadowColor    = faceColor - 0x303030;
        shadowColor.a        = alpha;

        graphics->setColor(faceColor);
        graphics->fillRectangle(1, 1, dim.width - 1, dim.height - 1);

        graphics->setColor(highlightColor);
        graphics->drawLine(0, 0, dim.width - 1, 0);
        graphics->drawLine(0, 1, 0, dim.height - 1);

        graphics->setColor(shadowColor);
        graphics->drawLine(1, dim.height - 1, dim.width - 1, dim.height - 1);
        graphics->drawLine(dim.width - 1, 0, dim.width - 1, dim.height - 1);

        graphics->popClipArea();
    }

    void ScrollArea::logic()
    {
        checkPolicies();

        setVerticalScrollAmount(getVerticalScrollAmount());
        setHorizontalScrollAmount(getHorizontalScrollAmount());

        if (getContent() != nullptr) {
            getContent()->setPosition(
                -mHScroll + getContent()->getBorderSize(), -mVScroll + getContent()->getBorderSize());
            getContent()->logic();
        }
    }

    void ScrollArea::checkPolicies()
    {
        int const w = getWidth();
        int const h = getHeight();

        mHBarVisible = false;
        mVBarVisible = false;

        if (getContent() == nullptr) {
            mHBarVisible = (mHPolicy == ScrollPolicy::ShowAlways);
            mVBarVisible = (mVPolicy == ScrollPolicy::ShowAlways);
            return;
        }

        if (mHPolicy == ScrollPolicy::ShowAuto && mVPolicy == ScrollPolicy::ShowAuto) {
            if (getContent()->getWidth() <= w && getContent()->getHeight() <= h) {
                mHBarVisible = false;
                mVBarVisible = false;
            }

            if (getContent()->getWidth() > w) {
                mHBarVisible = true;
            }

            if ((getContent()->getHeight() > h) || (mHBarVisible && getContent()->getHeight() > h - mScrollbarWidth)) {
                mVBarVisible = true;
            }

            if (mVBarVisible && getContent()->getWidth() > w - mScrollbarWidth) {
                mHBarVisible = true;
            }

            return;
        }

        switch (mHPolicy) {
        case ScrollPolicy::ShowNever:
            mHBarVisible = false;
            break;

        case ScrollPolicy::ShowAlways:
            mHBarVisible = true;
            break;

        case ScrollPolicy::ShowAuto:
            if (mVPolicy == ScrollPolicy::ShowNever) {
                mHBarVisible = getContent()->getWidth() > w;
            } else {
                // (mVPolicy == ScrollPolicy::ShowAlways)
                mHBarVisible = getContent()->getWidth() > w - mScrollbarWidth;
            }
            break;

        default:
            throwException("Horizontal scroll policy invalid.");
        }

        switch (mVPolicy) {
        case ScrollPolicy::ShowNever:
            mVBarVisible = false;
            break;

        case ScrollPolicy::ShowAlways:
            mVBarVisible = true;
            break;

        case ScrollPolicy::ShowAuto:
            if (mHPolicy == ScrollPolicy::ShowNever) {
                mVBarVisible = getContent()->getHeight() > h;
            } else {
                // (mHPolicy == ScrollPolicy::ShowAlways)
                mVBarVisible = getContent()->getHeight() > h - mScrollbarWidth;
            }
            break;
        default:
            throwException("Vertical scroll policy invalid.");
        }
    }

    Rectangle ScrollArea::getUpButtonDimension()
    {
        if (!mVBarVisible) {
            return {0, 0, 0, 0};
        }

        return {getWidth() - mScrollbarWidth, 0, mScrollbarWidth, mScrollbarWidth};
    }

    Rectangle ScrollArea::getDownButtonDimension()
    {
        if (!mVBarVisible) {
            return {0, 0, 0, 0};
        }

        if (mVBarVisible && mHBarVisible) {
            return {
                getWidth() - mScrollbarWidth, getHeight() - (mScrollbarWidth * 2), mScrollbarWidth, mScrollbarWidth};
        }

        return {getWidth() - mScrollbarWidth, getHeight() - mScrollbarWidth, mScrollbarWidth, mScrollbarWidth};
    }

    Rectangle ScrollArea::getLeftButtonDimension()
    {
        if (!mHBarVisible) {
            return {0, 0, 0, 0};
        }

        return {0, getHeight() - mScrollbarWidth, mScrollbarWidth, mScrollbarWidth};
    }

    Rectangle ScrollArea::getRightButtonDimension()
    {
        if (!mHBarVisible) {
            return {0, 0, 0, 0};
        }

        if (mVBarVisible && mHBarVisible) {
            return {
                getWidth() - (mScrollbarWidth * 2), getHeight() - mScrollbarWidth, mScrollbarWidth, mScrollbarWidth};
        }

        return {getWidth() - mScrollbarWidth, getHeight() - mScrollbarWidth, mScrollbarWidth, mScrollbarWidth};
    }

    Rectangle ScrollArea::getChildrenArea()
    {
        Rectangle area = Rectangle(
            0,
            0,
            mVBarVisible ? getWidth() - mScrollbarWidth : getWidth(),
            mHBarVisible ? getHeight() - mScrollbarWidth : getHeight());

        if (area.isEmpty()) {
            return {};
        }

        return area;
    }

    Rectangle ScrollArea::getVerticalBarDimension()
    {
        if (!mVBarVisible) {
            return {0, 0, 0, 0};
        }

        if (mHBarVisible) {
            return {
                getWidth() - mScrollbarWidth,
                getUpButtonDimension().height,
                mScrollbarWidth,
                getHeight() - getUpButtonDimension().height - getDownButtonDimension().height - mScrollbarWidth};
        }

        return {
            getWidth() - mScrollbarWidth,
            getUpButtonDimension().height,
            mScrollbarWidth,
            getHeight() - getUpButtonDimension().height - getDownButtonDimension().height};
    }

    Rectangle ScrollArea::getHorizontalBarDimension()
    {
        if (!mHBarVisible) {
            return {0, 0, 0, 0};
        }

        if (mVBarVisible) {
            return {
                getLeftButtonDimension().width,
                getHeight() - mScrollbarWidth,
                getWidth() - getLeftButtonDimension().width - getRightButtonDimension().width - mScrollbarWidth,
                mScrollbarWidth};
        }

        return {
            getLeftButtonDimension().width,
            getHeight() - mScrollbarWidth,
            getWidth() - getLeftButtonDimension().width - getRightButtonDimension().width,
            mScrollbarWidth};
    }

    Rectangle ScrollArea::getVerticalMarkerDimension()
    {
        if (!mVBarVisible) {
            return {0, 0, 0, 0};
        }

        int length = 0;
        int pos    = 0;

        Rectangle const barDim = getVerticalBarDimension();

        if ((getContent() != nullptr) && getContent()->getHeight() != 0) {
            length = (barDim.height * getChildrenArea().height) / getContent()->getHeight();
        } else {
            length = barDim.height;
        }

        length = std::max(length, mScrollbarWidth);

        length = std::min(length, barDim.height);

        if (getVerticalMaxScroll() != 0) {
            pos = ((barDim.height - length) * getVerticalScrollAmount()) / getVerticalMaxScroll();
        } else {
            pos = 0;
        }

        return {barDim.x, barDim.y + pos, mScrollbarWidth, length};
    }

    Rectangle ScrollArea::getHorizontalMarkerDimension()
    {
        if (!mHBarVisible) {
            return {0, 0, 0, 0};
        }

        int length = 0;
        int pos    = 0;

        Rectangle const barDim = getHorizontalBarDimension();

        if ((getContent() != nullptr) && getContent()->getWidth() != 0) {
            length = (barDim.width * getChildrenArea().width) / getContent()->getWidth();
        } else {
            length = barDim.width;
        }

        length = std::max(length, mScrollbarWidth);

        length = std::min(length, barDim.width);

        if (getHorizontalMaxScroll() != 0) {
            pos = ((barDim.width - length) * getHorizontalScrollAmount()) / getHorizontalMaxScroll();
        } else {
            pos = 0;
        }

        return {barDim.x + pos, barDim.y, length, mScrollbarWidth};
    }

    void ScrollArea::showWidgetPart(Widget* widget, Rectangle area)
    {
        if (widget != getContent()) {
            throwException("Widget not content widget");
        }

        Widget::showWidgetPart(widget, area);

        setHorizontalScrollAmount(getContent()->getBorderSize() - getContent()->getX());
        setVerticalScrollAmount(getContent()->getBorderSize() - getContent()->getY());
    }

    Widget* ScrollArea::getWidgetAt(int x, int y)
    {
        if (getChildrenArea().isContaining(x, y)) {
            return getContent();
        }

        return nullptr;
    }

    void ScrollArea::mouseWheelMovedUp(MouseEvent& mouseEvent)
    {
        if (mouseEvent.isConsumed()) {
            return;
        }

        if (!mVBarVisible) {
            mouseEvent.consume();
            return;
        }

        setVerticalScrollAmount(getVerticalScrollAmount() - (getChildrenArea().height / 8));

        mouseEvent.consume();
    }

    void ScrollArea::mouseWheelMovedDown(MouseEvent& mouseEvent)
    {
        if (mouseEvent.isConsumed()) {
            return;
        }

        if (!mVBarVisible) {
            mouseEvent.consume();
            return;
        }

        setVerticalScrollAmount(getVerticalScrollAmount() + (getChildrenArea().height / 8));

        mouseEvent.consume();
    }

    void ScrollArea::mouseWheelMovedRight(MouseEvent& mouseEvent)
    {
        if (mouseEvent.isConsumed()) {
            return;
        }

        if (!mHBarVisible) {
            mouseEvent.consume();
            return;
        }

        setHorizontalScrollAmount(getHorizontalScrollAmount() + (getChildrenArea().width / 8));

        mouseEvent.consume();
    }

    void ScrollArea::mouseWheelMovedLeft(MouseEvent& mouseEvent)
    {
        if (mouseEvent.isConsumed()) {
            return;
        }

        if (!mHBarVisible) {
            mouseEvent.consume();
            return;
        }

        setHorizontalScrollAmount(getHorizontalScrollAmount() - (getChildrenArea().width / 8));

        mouseEvent.consume();
    }

    void ScrollArea::setWidth(int width)
    {
        Widget::setWidth(width);
        Widget* content = getContent();
        if (content != nullptr) {
            int const contW = std::max(getWidth(), content->getWidth());
            content->setWidth(contW);
        }
        checkPolicies();
    }

    void ScrollArea::setHeight(int height)
    {
        Widget::setHeight(height);
        Widget* content = getContent();
        if (content != nullptr) {
            int const contH = std::max(getHeight(), content->getHeight());
            content->setHeight(contH);
        }
        checkPolicies();
    }

    void ScrollArea::setDimension(Rectangle const & dimension)
    {
        Widget::setDimension(dimension);
        Widget* content = getContent();
        if (content != nullptr) {
            int const contW = std::max(getWidth(), content->getWidth());
            content->setWidth(contW);
            int const contH = std::max(getHeight(), content->getHeight());
            content->setHeight(contH);
        }
        checkPolicies();
    }

    void ScrollArea::resizeToContent(bool recursion)
    {
        static_cast<void>(recursion);
        Widget* content = getContent();
        if (content != nullptr) {
            content->resizeToContent();
        }
        Size const & min = getMinSize();
        setWidth(min.getWidth());
        setHeight(min.getHeight());
    }

    void ScrollArea::adjustSize()
    {
        Widget* content = getContent();
        if (content != nullptr) {
            content->adjustSize();
        }
        Size const & min = getMinSize();
        setWidth(min.getWidth());
        setHeight(min.getHeight());
    }

    void ScrollArea::expandContent(bool recursion)
    {
        static_cast<void>(recursion);
        // remove that hack
        setWidth(getWidth());
        setHeight(getHeight());

        Widget* content = getContent();
        if (content != nullptr) {
            content->expandContent();
        }
        checkPolicies();
    }

    void ScrollArea::setLeftButtonScrollAmount(int amount)
    {
        mLeftButtonScrollAmount = amount;
    }

    void ScrollArea::setRightButtonScrollAmount(int amount)
    {
        mRightButtonScrollAmount = amount;
    }

    void ScrollArea::setUpButtonScrollAmount(int amount)
    {
        mUpButtonScrollAmount = amount;
    }

    void ScrollArea::setDownButtonScrollAmount(int amount)
    {
        mDownButtonScrollAmount = amount;
    }

    int ScrollArea::getLeftButtonScrollAmount() const
    {
        return mLeftButtonScrollAmount;
    }

    int ScrollArea::getRightButtonScrollAmount() const
    {
        return mRightButtonScrollAmount;
    }

    int ScrollArea::getUpButtonScrollAmount() const
    {
        return mUpButtonScrollAmount;
    }

    int ScrollArea::getDownButtonScrollAmount() const
    {
        return mDownButtonScrollAmount;
    }

    void ScrollArea::setOpaque(bool opaque)
    {
        mOpaque = opaque;
    }

    bool ScrollArea::isOpaque() const
    {
        return mOpaque;
    }
} // namespace fcn
