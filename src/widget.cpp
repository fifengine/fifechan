// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/widget.hpp"

#include <algorithm>
#include <iostream>
#include <list>
#include <ranges>
#include <string>

#include "fifechan/actionevent.hpp"
#include "fifechan/actionlistener.hpp"
#include "fifechan/deathlistener.hpp"
#include "fifechan/defaultfont.hpp"
#include "fifechan/event.hpp"
#include "fifechan/exception.hpp"
#include "fifechan/focushandler.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/keylistener.hpp"
#include "fifechan/mouselistener.hpp"
#include "fifechan/rectangle.hpp"
#include "fifechan/size.hpp"
#include "fifechan/visibilityeventhandler.hpp"
#include "fifechan/widgetlistener.hpp"

namespace fcn
{
    Font* Widget::mGlobalFont = nullptr;
    DefaultFont Widget::mDefaultFont;
    std::list<Widget*> Widget::mWidgetInstances;
    VisibilityEventHandler* Widget::mVisibilityEventHandler = nullptr;
    DeathListener* Widget::mGuiDeathListener                = nullptr;

    Widget::Widget()
    {
        mWidgetInstances.push_back(this);
    }

    Widget::~Widget()
    {
        // We're in a destructor, we can't throw.
        // We can only catch and print the error message.
        try {

            if (mParent != nullptr) {
                mParent->remove(this);
            }

            std::list<Widget*>::const_iterator childrenIter;
            for (childrenIter = mChildren.begin(); childrenIter != mChildren.end(); ++childrenIter) {
                (*childrenIter)->_setParent(nullptr);
            }

            std::list<DeathListener*>::const_iterator deathIter;
            for (deathIter = mDeathListeners.begin(); deathIter != mDeathListeners.end(); ++deathIter) {
                Event const event(this);
                (*deathIter)->death(event);
            }

            if (mGuiDeathListener != nullptr) {
                Event const event(this);
                mGuiDeathListener->death(event);
            }

            _setFocusHandler(nullptr);

            mWidgetInstances.remove(this);

        } catch (fcn::Exception const & e) {
            std::cerr << "Exception caught in Widget destructor: " << e.what() << '\n';
        } catch (std::exception const & e) {
            std::cerr << "Exception caught in Widget destructor: " << e.what() << '\n';
        } catch (...) {
            std::cerr << "Unknown exception caught in Widget destructor" << '\n';
        }
    }

    void Widget::drawOutline(Graphics* graphics)
    {
        Color const outlineColor = getOutlineColor();
        Color highlightColor;
        Color shadowColor;
        int const alpha  = getBaseColor().a;
        int const width  = getWidth() + (getOutlineSize() * 2) - 1;
        int const height = getHeight() + (getOutlineSize() * 2) - 1;
        highlightColor   = outlineColor + 0x303030;
        highlightColor.a = alpha;
        shadowColor      = outlineColor - 0x303030;
        shadowColor.a    = alpha;

        for (unsigned int i = 0; i < getOutlineSize(); ++i) {
            graphics->setColor(shadowColor);
            graphics->drawLine(i, i, width - i, i);
            graphics->drawLine(i, i + 1, i, height - i - 1);
            graphics->setColor(highlightColor);
            graphics->drawLine(width - i, i + 1, width - i, height - i);
            graphics->drawLine(i, height - i, width - i - 1, height - i);
        }
    }

    void Widget::drawBorder(Graphics* graphics)
    {
        Color const borderColor = getBorderColor();
        Color highlightColor;
        Color shadowColor;
        int const alpha  = getBaseColor().a;
        int const width  = getWidth() - 1;
        int const height = getHeight() - 1;

        highlightColor   = borderColor + 0x303030;
        highlightColor.a = alpha;
        shadowColor      = borderColor - 0x303030;
        shadowColor.a    = alpha;

        for (int i = 0; i < getBorderSize(); ++i) {
            graphics->setColor(shadowColor);
            graphics->drawLine(i, i, width - i, i);
            graphics->drawLine(i, i + 1, i, height - i - 1);
            graphics->setColor(highlightColor);
            graphics->drawLine(width - i, i + 1, width - i, height - i);
            graphics->drawLine(i, height - i, width - i - 1, height - i);
        }
    }

    void Widget::drawSelectionFrame(Graphics* graphics)
    {
        int const width  = getWidth() - 1;
        int const height = getHeight() - 1;
        graphics->setColor(getSelectionColor());

        // currently border size is used here too, not sure an extra frame size is really needed.
        for (unsigned int i = 0; i < getBorderSize(); ++i) {
            // would be better but causes problems with OpenGL
            // graphics->drawRectangle(i, i, width - 2 * i, height - 2 * i);
            graphics->drawLine(i, i, width - i, i);
            graphics->drawLine(i, i + 1, i, height - i - 1);
            graphics->drawLine(width - i, i + 1, width - i, height - i);
            graphics->drawLine(i, height - i, width - i - 1, height - i);
        }
    }

    void Widget::_setParent(Widget* parent)
    {
        mParent = parent;
    }

    Widget* Widget::getParent() const
    {
        return mParent;
    }

    void Widget::setWidth(int width)
    {
        Rectangle newDimension = mDimension;
        newDimension.width     = width;

        setDimension(newDimension);
    }

    int Widget::getWidth() const
    {
        return mDimension.width;
    }

    void Widget::setHeight(int height)
    {
        Rectangle newDimension = mDimension;
        newDimension.height    = height;

        setDimension(newDimension);
    }

    int Widget::getHeight() const
    {
        return mDimension.height;
    }

    void Widget::setX(int x)
    {
        Rectangle newDimension = mDimension;
        newDimension.x         = x;

        setDimension(newDimension);
    }

    int Widget::getX() const
    {
        return mDimension.x;
    }

    void Widget::setY(int y)
    {
        Rectangle newDimension = mDimension;
        newDimension.y         = y;

        setDimension(newDimension);
    }

    int Widget::getY() const
    {
        return mDimension.y;
    }

    void Widget::setPosition(int x, int y)
    {
        Rectangle newDimension = mDimension;
        newDimension.x         = x;
        newDimension.y         = y;

        setDimension(newDimension);
    }

    void Widget::setDimension(Rectangle const & dimension)
    {
        Rectangle const oldDimension = mDimension;
        mDimension                   = dimension;

        if (mDimension.width != oldDimension.width || mDimension.height != oldDimension.height) {
            calculateSize();
            if (mDimension.width != oldDimension.width || mDimension.height != oldDimension.height) {
                distributeResizedEvent();
            }
        }

        if (mDimension.x != oldDimension.x || mDimension.y != oldDimension.y) {
            distributeMovedEvent();

            auto currChild(mChildren.begin());
            auto const endChildren(mChildren.end());

            for (; currChild != endChildren; ++currChild) {
                (*currChild)->distributeAncestorMovedEvent(this);
            }
        }
    }

    unsigned int Widget::getChildrenCount() const
    {
        unsigned int childs = 0;
        auto currChild(mChildren.begin());
        auto const endChildren(mChildren.end());
        for (; currChild != endChildren; ++currChild) {
            ++childs;
        }
        return childs;
    }

    unsigned int Widget::getVisibleChildrenCount() const
    {
        unsigned int childs = 0;
        auto currChild(mChildren.begin());
        auto const endChildren(mChildren.end());
        for (; currChild != endChildren; ++currChild) {
            if (isVisible()) {
                ++childs;
            }
        }
        return childs;
    }

    void Widget::setMinSize(Size const & size)
    {
        mMinSize = size;
        calculateSize();
    }

    Size const & Widget::getMinSize() const
    {
        return mMinSize;
    }

    void Widget::setMaxSize(Size const & size)
    {
        mMaxSize = size;
        calculateSize();
    }

    Size const & Widget::getMaxSize() const
    {
        return mMaxSize;
    }

    void Widget::setFixedSize(Size const & size)
    {
        mFixedSize = size;
        if (mFixedSize.getWidth() < 0 || mFixedSize.getHeight() < 0) {
            mFixedSizeUsed = false;
        } else {
            mFixedSizeUsed = true;
            calculateSize();
        }
    }

    Size const & Widget::getFixedSize() const
    {
        return mFixedSize;
    }

    bool Widget::isFixedSize() const
    {
        return mFixedSizeUsed;
    }

    void Widget::calculateSize()
    {
        if (mFixedSizeUsed) {
            mDimension.width  = mFixedSize.getWidth();
            mDimension.height = mFixedSize.getHeight();
            return;
        }
        int const minWidth   = mMinSize.getWidth();
        int const minHeight  = mMinSize.getHeight();
        int const maxWidth   = mMaxSize.getWidth();
        int const maxHeight  = mMaxSize.getHeight();
        int const currWidth  = mDimension.width;
        int const currHeight = mDimension.height;

        mDimension.width  = std::max(std::min(currWidth, maxWidth), minWidth);
        mDimension.height = std::max(std::min(currHeight, maxHeight), minHeight);
    }

    void Widget::setVerticalExpand(bool expand)
    {
        mVExpand = expand;
    }

    bool Widget::isVerticalExpand() const
    {
        return mVExpand;
    }

    void Widget::setHorizontalExpand(bool expand)
    {
        mHExpand = expand;
    }

    bool Widget::isHorizontalExpand() const
    {
        return mHExpand;
    }

    void Widget::adaptLayout(bool top)
    {
        Widget* widget = this;
        while ((widget->getParent() != nullptr) && top) {
            Widget* parent = widget->getParent();
            if (!parent->isLayouted()) {
                break;
            }
            widget = parent;
        }
        widget->resizeToContent();
        widget->expandContent();
    }

    void Widget::setOutlineSize(unsigned int size)
    {
        mOutlineSize = size;
    }

    unsigned int Widget::getOutlineSize() const
    {
        return mOutlineSize;
    }

    void Widget::setBorderSize(unsigned int size)
    {
        mBorderSize = size;
    }

    unsigned int Widget::getBorderSize() const
    {
        return mBorderSize;
    }

    void Widget::setMargin(int margin)
    {
        mMarginTop    = margin;
        mMarginRight  = margin;
        mMarginBottom = margin;
        mMarginLeft   = margin;
    }

    void Widget::setMarginTop(int margin)
    {
        mMarginTop = margin;
    }

    int Widget::getMarginTop() const
    {
        return mMarginTop;
    }

    void Widget::setMarginRight(int margin)
    {
        mMarginRight = margin;
    }

    int Widget::getMarginRight() const
    {
        return mMarginRight;
    }

    void Widget::setMarginBottom(int margin)
    {
        mMarginBottom = margin;
    }

    int Widget::getMarginBottom() const
    {
        return mMarginBottom;
    }

    void Widget::setMarginLeft(int margin)
    {
        mMarginLeft = margin;
    }

    int Widget::getMarginLeft() const
    {
        return mMarginLeft;
    }

    void Widget::setPadding(unsigned int padding)
    {
        mPaddingTop    = padding;
        mPaddingRight  = padding;
        mPaddingBottom = padding;
        mPaddingLeft   = padding;
    }

    void Widget::setPaddingTop(unsigned int padding)
    {
        mPaddingTop = padding;
    }

    unsigned int Widget::getPaddingTop() const
    {
        return mPaddingTop;
    }

    void Widget::setPaddingRight(unsigned int padding)
    {
        mPaddingRight = padding;
    }

    unsigned int Widget::getPaddingRight() const
    {
        return mPaddingRight;
    }

    void Widget::setPaddingBottom(unsigned int padding)
    {
        mPaddingBottom = padding;
    }

    unsigned int Widget::getPaddingBottom() const
    {
        return mPaddingBottom;
    }

    void Widget::setPaddingLeft(unsigned int padding)
    {
        mPaddingLeft = padding;
    }

    unsigned int Widget::getPaddingLeft() const
    {
        return mPaddingLeft;
    }

    Rectangle const & Widget::getDimension() const
    {
        return mDimension;
    }

    std::string const & Widget::getActionEventId() const
    {
        return mActionEventId;
    }

    void Widget::setActionEventId(std::string const & actionEventId)
    {
        mActionEventId = actionEventId;
    }

    bool Widget::isFocused() const
    {
        if (mFocusHandler == nullptr) {
            return false;
        }

        return (mFocusHandler->isFocused(this));
    }

    void Widget::setFocusable(bool focusable)
    {
        if (!focusable && isFocused()) {
            mFocusHandler->focusNone();
        }

        mFocusable = focusable;
    }

    bool Widget::isFocusable() const
    {
        return mFocusable && isVisible() && isEnabled();
    }

    void Widget::requestFocus()
    {
        if (mFocusHandler == nullptr) {
            throwException("No focus handler is set (did you add the widget to the GUI?)");
        }

        if (isFocusable()) {
            mFocusHandler->requestFocus(this);
        }
    }

    void Widget::requestMoveToTop()
    {
        if (mParent != nullptr) {
            mParent->moveToTop(this);
        }
    }

    void Widget::requestMoveToBottom()
    {
        if (mParent != nullptr) {
            mParent->moveToBottom(this);
        }
    }

    void Widget::setVisible(bool visible)
    {
        VisibilityEventHandler* visibilityEventHandler = _getVisibilityEventHandler();

        if (!visible && isFocused()) {
            mFocusHandler->focusNone();
        }

        if (visible) {
            visibilityEventHandler->widgetShown(Event(this));
            distributeShownEvent();

            auto currChild(mChildren.begin());
            auto const endChildren(mChildren.end());

            for (; currChild != endChildren; ++currChild) {
                (*currChild)->distributeAncestorShownEvent(this);
            }
        } else {
            visibilityEventHandler->widgetHidden(Event(this));
            distributeHiddenEvent();

            auto currChild(mChildren.begin());
            auto const endChildren(mChildren.end());

            for (; currChild != endChildren; ++currChild) {
                (*currChild)->distributeAncestorHiddenEvent(this);
            }
        }

        mVisible = visible;
    }

    bool Widget::isVisible() const
    {
        if (getParent() == nullptr) {
            return mVisible;
        }
        return mVisible && getParent()->isVisible();
    }

    bool Widget::isSetVisible() const
    {
        return mVisible;
    }

    void Widget::setBaseColor(Color const & color)
    {
        mBaseColor = color;
    }

    Color const & Widget::getBaseColor() const
    {
        return mBaseColor;
    }

    void Widget::setForegroundColor(Color const & color)
    {
        mForegroundColor = color;
    }

    Color const & Widget::getForegroundColor() const
    {
        return mForegroundColor;
    }

    void Widget::setBackgroundColor(Color const & color)
    {
        mBackgroundColor = color;
    }

    Color const & Widget::getBackgroundColor() const
    {
        return mBackgroundColor;
    }

    void Widget::setSelectionColor(Color const & color)
    {
        mSelectionColor = color;
    }

    Color const & Widget::getSelectionColor() const
    {
        return mSelectionColor;
    }

    void Widget::setOutlineColor(Color const & color)
    {
        mOutlineColor = color;
    }

    Color const & Widget::getOutlineColor() const
    {
        return mOutlineColor;
    }

    void Widget::setBorderColor(Color const & color)
    {
        mBorderColor = color;
    }

    Color const & Widget::getBorderColor() const
    {
        return mBorderColor;
    }

    void Widget::setSelectionMode(SelectionMode mode)
    {
        mSelectionMode = mode;
    }

    Widget::SelectionMode Widget::getSelectionMode() const
    {
        return mSelectionMode;
    }

    void Widget::_setFocusHandler(FocusHandler* focusHandler)
    {
        if (mFocusHandler != nullptr) {
            releaseModalFocus();
            if (mFocusHandler->getModalMouseInputFocused() == this) {
                releaseModalMouseInputFocus();
            }
            mFocusHandler->remove(this);
        }

        if (focusHandler != nullptr) {
            focusHandler->add(this);
        }

        mFocusHandler = focusHandler;

        if (mInternalFocusHandler != nullptr) {
            return;
        }

        std::list<Widget*>::const_iterator iter;
        for (iter = mChildren.begin(); iter != mChildren.end(); ++iter) {
            if (widgetExists(*iter)) {
                (*iter)->_setFocusHandler(focusHandler);
            }
        }
    }

    FocusHandler* Widget::_getFocusHandler()
    {
        return mFocusHandler;
    }

    void Widget::_setVisibilityEventHandler(VisibilityEventHandler* visibilityEventHandler)
    {
        mVisibilityEventHandler = visibilityEventHandler;
    }

    VisibilityEventHandler* Widget::_getVisibilityEventHandler()
    {
        return mVisibilityEventHandler;
    }

    void Widget::_setGuiDeathListener(DeathListener* deathListener)
    {
        mGuiDeathListener = deathListener;
    }

    DeathListener* Widget::_getGuiDeathListener()
    {
        return mGuiDeathListener;
    }

    void Widget::addActionListener(ActionListener* actionListener)
    {
        mActionListeners.push_back(actionListener);
    }

    void Widget::removeActionListener(ActionListener* actionListener)
    {
        mActionListeners.remove(actionListener);
    }

    void Widget::addDeathListener(DeathListener* deathListener)
    {
        mDeathListeners.push_back(deathListener);
    }

    void Widget::removeDeathListener(DeathListener* deathListener)
    {
        mDeathListeners.remove(deathListener);
    }

    void Widget::addKeyListener(KeyListener* keyListener)
    {
        mKeyListeners.push_back(keyListener);
    }

    void Widget::removeKeyListener(KeyListener* keyListener)
    {
        mKeyListeners.remove(keyListener);
    }

    void Widget::addFocusListener(FocusListener* focusListener)
    {
        mFocusListeners.push_back(focusListener);
    }

    void Widget::removeFocusListener(FocusListener* focusListener)
    {
        mFocusListeners.remove(focusListener);
    }

    void Widget::addMouseListener(MouseListener* mouseListener)
    {
        mMouseListeners.push_back(mouseListener);
    }

    void Widget::removeMouseListener(MouseListener* mouseListener)
    {
        mMouseListeners.remove(mouseListener);
    }

    void Widget::addWidgetListener(WidgetListener* widgetListener)
    {
        mWidgetListeners.push_back(widgetListener);
    }

    void Widget::removeWidgetListener(WidgetListener* widgetListener)
    {
        mWidgetListeners.remove(widgetListener);
    }

    void Widget::getAbsolutePosition(int& x, int& y) const
    {
        if (getParent() == nullptr) {
            if (isLastPositionSet()) {
                x = mLastX;
                y = mLastY;
            } else {
                x = mDimension.x;
                y = mDimension.y;
            }
            return;
        }

        int parentX = 0;
        int parentY = 0;

        getParent()->getAbsolutePosition(parentX, parentY);

        x = parentX + mDimension.x + getParent()->getChildrenArea().x;
        y = parentY + mDimension.y + getParent()->getChildrenArea().y;
    }

    Font* Widget::getFont() const
    {
        if (mCurrentFont == nullptr) {
            if (mGlobalFont == nullptr) {
                return &mDefaultFont;
            }

            return mGlobalFont;
        }

        return mCurrentFont;
    }

    void Widget::setGlobalFont(Font* font)
    {
        mGlobalFont = font;

        std::list<Widget*>::iterator iter;
        for (iter = mWidgetInstances.begin(); iter != mWidgetInstances.end(); ++iter) {
            if ((*iter)->mCurrentFont == nullptr) {
                (*iter)->fontChanged();
            }
        }
    }

    void Widget::setFont(Font* font)
    {
        mCurrentFont = font;
        fontChanged();
    }

    bool Widget::widgetExists(Widget const * widget)
    {
        auto iter = std::ranges::find_if(mWidgetInstances, [widget](Widget* w) {
            return w == widget;
        });
        return iter != mWidgetInstances.end();
    }

    bool Widget::isTabInEnabled() const
    {
        return mTabIn;
    }

    void Widget::setTabInEnabled(bool enabled)
    {
        mTabIn = enabled;
    }

    bool Widget::isTabOutEnabled() const
    {
        return mTabOut;
    }

    void Widget::setTabOutEnabled(bool enabled)
    {
        mTabOut = enabled;
    }

    void Widget::setSize(int width, int height)
    {
        Rectangle newDimension = mDimension;
        newDimension.width     = width;
        newDimension.height    = height;

        setDimension(newDimension);
    }

    void Widget::setEnabled(bool enabled)
    {
        mEnabled = enabled;
    }

    bool Widget::isEnabled() const
    {
        return mEnabled && isVisible();
    }

    bool Widget::isModalFocusable() const
    {
        if (mFocusHandler == nullptr) {
            throwException("No focus handler is set (did you add the widget to the GUI?)");
            return false;
        }
        return mFocusHandler->getModalFocused() == nullptr;
    }

    bool Widget::isModalMouseInputFocusable() const
    {
        if (mFocusHandler == nullptr) {
            throwException("No focus handler is set (did you add the widget to the GUI?)");
            return false;
        }
        return mFocusHandler->getModalMouseInputFocused() == nullptr;
    }

    void Widget::requestModalFocus()
    {
        if (mFocusHandler == nullptr) {
            throwException("No focus handler is set (did you add the widget to the GUI?)");
        }

        mFocusHandler->requestModalFocus(this);
    }

    void Widget::requestModalMouseInputFocus()
    {
        if (mFocusHandler == nullptr) {
            throwException("No focus handler is set (did you add the widget to the GUI?)");
        }

        mFocusHandler->requestModalMouseInputFocus(this);
    }

    void Widget::releaseModalFocus()
    {
        if (mFocusHandler == nullptr) {
            return;
        }

        mFocusHandler->releaseModalFocus(this);
    }

    void Widget::releaseModalMouseInputFocus()
    {
        if (mFocusHandler == nullptr) {
            return;
        }

        mFocusHandler->releaseModalMouseInputFocus(this);
    }

    bool Widget::isModalFocused() const
    {
        if (mFocusHandler == nullptr) {
            throwException("No focus handler is set (did you add the widget to the GUI?)");
        }

        if (getParent() != nullptr) {
            return (mFocusHandler->getModalFocused() == this) || getParent()->isModalFocused();
        }

        return mFocusHandler->getModalFocused() == this;
    }

    bool Widget::isModalMouseInputFocused() const
    {
        if (mFocusHandler == nullptr) {
            throwException("No focus handler is set (did you add the widget to the GUI?)");
        }

        if (getParent() != nullptr) {
            return (mFocusHandler->getModalMouseInputFocused() == this) || getParent()->isModalMouseInputFocused();
        }

        return mFocusHandler->getModalMouseInputFocused() == this;
    }

    Widget* Widget::getWidgetAt(int x, int y, Widget* exclude)
    {
        Rectangle const r = getChildrenArea();

        if (!r.isContaining(x, y)) {
            return nullptr;
        }

        x -= r.x;
        y -= r.y;

        std::list<Widget*>::reverse_iterator iter;
        for (iter = mChildren.rbegin(); iter != mChildren.rend(); ++iter) {
            Widget* widget = (*iter);

            if (widget != exclude && widget->isVisible() && widget->getDimension().isContaining(x, y)) {
                return widget;
            }
        }

        return nullptr;
    }

    std::list<MouseListener*> const & Widget::_getMouseListeners()
    {
        return mMouseListeners;
    }

    std::list<KeyListener*> const & Widget::_getKeyListeners()
    {
        return mKeyListeners;
    }

    std::list<FocusListener*> const & Widget::_getFocusListeners()
    {
        return mFocusListeners;
    }

    Rectangle Widget::getChildrenArea()
    {
        return {0, 0, 0, 0};
    }

    FocusHandler* Widget::_getInternalFocusHandler()
    {
        return mInternalFocusHandler;
    }

    void Widget::setInternalFocusHandler(FocusHandler* focusHandler)
    {
        mInternalFocusHandler = focusHandler;

        std::list<Widget*>::const_iterator iter;
        for (iter = mChildren.begin(); iter != mChildren.end(); ++iter) {
            if (mInternalFocusHandler == nullptr) {
                (*iter)->_setFocusHandler(_getFocusHandler());
            } else {
                (*iter)->_setFocusHandler(mInternalFocusHandler);
            }
        }
    }

    void Widget::setId(std::string const & id)
    {
        mId = id;
    }

    std::string const & Widget::getId() const
    {
        return mId;
    }

    void Widget::distributeResizedEvent()
    {
        std::list<WidgetListener*>::const_iterator iter;
        for (iter = mWidgetListeners.begin(); iter != mWidgetListeners.end(); ++iter) {
            Event const event(this);
            (*iter)->widgetResized(event);
        }
    }

    void Widget::distributeMovedEvent()
    {
        std::list<WidgetListener*>::const_iterator iter;
        for (iter = mWidgetListeners.begin(); iter != mWidgetListeners.end(); ++iter) {
            Event const event(this);
            (*iter)->widgetMoved(event);
        }
    }

    void Widget::distributeHiddenEvent()
    {
        std::list<WidgetListener*>::const_iterator iter;
        for (iter = mWidgetListeners.begin(); iter != mWidgetListeners.end(); ++iter) {
            Event const event(this);
            (*iter)->widgetHidden(event);
        }
    }

    void Widget::distributeAncestorMovedEvent(Widget* ancestor)
    {
        auto currWidgetListener(mWidgetListeners.begin());
        auto const endWidgetListeners(mWidgetListeners.end());
        Event const event(ancestor);

        for (; currWidgetListener != endWidgetListeners; ++currWidgetListener) {
            (*currWidgetListener)->ancestorMoved(event);
        }

        auto currChild(mChildren.begin());
        auto const endChildren(mChildren.end());

        for (; currChild != endChildren; ++currChild) {
            (*currChild)->distributeAncestorMovedEvent(ancestor);
        }
    }

    void Widget::distributeAncestorHiddenEvent(Widget* ancestor)
    {
        // additional call VisibilityEventHandler, needed to get new focus / MouseEvent::Entered or Exited
        _getVisibilityEventHandler()->widgetHidden(Event(this));

        auto currWidgetListener(mWidgetListeners.begin());
        auto const endWidgetListeners(mWidgetListeners.end());
        Event const event(ancestor);

        for (; currWidgetListener != endWidgetListeners; ++currWidgetListener) {
            (*currWidgetListener)->ancestorHidden(event);
        }

        auto currChild(mChildren.begin());
        auto const endChildren(mChildren.end());

        for (; currChild != endChildren; ++currChild) {
            (*currChild)->distributeAncestorHiddenEvent(ancestor);
        }
    }

    void Widget::distributeAncestorShownEvent(Widget* ancestor)
    {
        // additional call VisibilityEventHandler, needed to get new focus / MouseEvent::Entered or Exited
        _getVisibilityEventHandler()->widgetShown(Event(this));

        auto currWidgetListener(mWidgetListeners.begin());
        auto const endWidgetListeners(mWidgetListeners.end());
        Event const event(ancestor);

        for (; currWidgetListener != endWidgetListeners; ++currWidgetListener) {
            (*currWidgetListener)->ancestorShown(event);
        }

        auto currChild(mChildren.begin());
        auto const endChildren(mChildren.end());

        for (; currChild != endChildren; ++currChild) {
            (*currChild)->distributeAncestorShownEvent(ancestor);
        }
    }

    void Widget::distributeActionEvent()
    {
        std::list<ActionListener*>::const_iterator iter;
        for (iter = mActionListeners.begin(); iter != mActionListeners.end(); ++iter) {
            ActionEvent const actionEvent(this, mActionEventId);
            (*iter)->action(actionEvent);
        }
    }

    void Widget::distributeShownEvent()
    {
        std::list<WidgetListener*>::const_iterator iter;
        for (iter = mWidgetListeners.begin(); iter != mWidgetListeners.end(); ++iter) {
            Event const event(this);
            (*iter)->widgetShown(event);
        }
    }

    void Widget::showPart(Rectangle rectangle)
    {
        if (mParent != nullptr) {
            mParent->showWidgetPart(this, rectangle);
        }
    }

    Widget* Widget::getTop() const
    {
        if (getParent() == nullptr) {
            return nullptr;
        }

        Widget* widget = getParent();
        Widget* parent = getParent()->getParent();

        while (parent != nullptr) {
            widget = parent;
            parent = parent->getParent();
        }

        return widget;
    }

    std::list<Widget*> Widget::getWidgetsIn(Rectangle const & area, Widget* ignore)
    {
        std::list<Widget*> result;

        std::list<Widget*>::const_iterator iter;
        for (iter = mChildren.begin(); iter != mChildren.end(); ++iter) {
            Widget* widget = (*iter);
            if (ignore != widget && widget->getDimension().isIntersecting(area)) {
                result.push_back(widget);
            }
        }

        return result;
    }

    void Widget::resizeToChildren()
    {
        int w = 0;
        int h = 0;
        std::list<Widget*>::const_iterator iter;
        for (iter = mChildren.begin(); iter != mChildren.end(); ++iter) {
            Widget* widget = (*iter);
            w              = std::max(widget->getX() + widget->getWidth(), w);
            h              = std::max(widget->getY() + widget->getHeight(), h);
        }

        setSize(w, h);
    }

    Widget* Widget::findWidgetById(std::string const & id)
    {
        std::list<Widget*>::const_iterator iter;
        for (iter = mChildren.begin(); iter != mChildren.end(); ++iter) {
            Widget* widget = (*iter);

            if (widget->getId() == id) {
                return widget;
            }

            Widget* child = widget->findWidgetById(id);

            if (child != nullptr) {
                return child;
            }
        }

        return nullptr;
    }

    void Widget::showWidgetPart(Widget* widget, Rectangle area)
    {
        Rectangle const widgetArea = getChildrenArea();

        area.x += widget->getX();
        area.y += widget->getY();

        if (area.x + area.width > widgetArea.width) {
            widget->setX(widget->getX() - area.x - area.width + widgetArea.width);
        }

        if (area.y + area.height > widgetArea.height) {
            widget->setY(widget->getY() - area.y - area.height + widgetArea.height);
        }

        if (area.x < 0) {
            widget->setX(widget->getX() - area.x);
        }

        if (area.y < 0) {
            widget->setY(widget->getY() - area.y);
        }
    }

    void Widget::removeAllChildren()
    {
        std::list<Widget*>::const_iterator iter;
        for (iter = mChildren.begin(); iter != mChildren.end(); ++iter) {
            Widget* widget = (*iter);
            int x          = 0;
            int y          = 0;
            widget->getAbsolutePosition(x, y);
            widget->setLastPosition(x, y);
            widget->_setFocusHandler(nullptr);
            widget->_setParent(nullptr);
            // thats more a hack but needed
            if (_getVisibilityEventHandler() != nullptr) {
                _getVisibilityEventHandler()->widgetHidden(Event(widget));
            }
        }

        mChildren.clear();
    }

    void Widget::remove(Widget* widget)
    {
        auto it = std::ranges::find(mChildren, widget);

        if (it == std::ranges::end(mChildren)) {
            throwException("There is no such widget in this container.");
        } else {
            int x = 0;
            int y = 0;
            widget->getAbsolutePosition(x, y);
            widget->setLastPosition(x, y);

            mChildren.erase(it);

            widget->_setFocusHandler(nullptr);
            widget->_setParent(nullptr);

            // TODO thats more a hack but needed
            if (_getVisibilityEventHandler() != nullptr) {
                _getVisibilityEventHandler()->widgetHidden(Event(widget));
            }
        }
    }

    void Widget::add(Widget* widget)
    {
        mChildren.push_back(widget);

        if (mInternalFocusHandler == nullptr) {
            widget->_setFocusHandler(_getFocusHandler());
        } else {
            widget->_setFocusHandler(mInternalFocusHandler);
        }

        widget->_setParent(this);
        setLastPosition(0, 0);
        // thats more a hack but needed
        if (_getVisibilityEventHandler() != nullptr) {
            _getVisibilityEventHandler()->widgetShown(Event(widget));
        }
    }

    void Widget::moveToTop(Widget* widget)
    {
        std::list<Widget*>::iterator iter;
        iter = std::ranges::find(mChildren, widget);

        if (iter == mChildren.end()) {
            throwException("There is no such widget in this widget.");
        }

        mChildren.remove(widget);
        mChildren.push_back(widget);
    }

    void Widget::moveToBottom(Widget* widget)
    {
        std::list<Widget*>::iterator iter;
        iter = std::ranges::find(mChildren, widget);

        if (iter == mChildren.end()) {
            throwException("There is no such widget in this widget.");
        }

        mChildren.remove(widget);
        mChildren.push_front(widget);
    }

    void Widget::focusNext()
    {
        if (mChildren.empty()) {
            return;
        }

        auto focused = std::ranges::find_if(mChildren, [](auto* w) {
            return w->isFocused();
        });

        auto next = (focused == std::ranges::end(mChildren)) ? std::ranges::begin(mChildren) : std::next(focused);

        auto it = std::ranges::find_if(next, std::ranges::end(mChildren), [](auto* w) {
            return w->isFocusable();
        });

        if (it == std::ranges::end(mChildren)) {
            it = std::ranges::find_if(std::ranges::begin(mChildren), next, [](auto* w) {
                return w->isFocusable();
            });
        }

        if (it != next) {
            (*it)->requestFocus();
        }
    }

    void Widget::focusPrevious()
    {
        if (mChildren.empty()) {
            return;
        }

        auto rev = std::ranges::reverse_view(mChildren);

        auto focused = std::ranges::find_if(rev, [](auto* w) {
            return w->isFocused();
        });

        auto next = (focused == std::ranges::end(rev)) ? std::ranges::begin(rev) : std::next(focused);

        auto rit = std::ranges::find_if(next, std::ranges::end(rev), [](auto* w) {
            return w->isFocusable();
        });

        if (rit == std::ranges::end(rev)) {
            rit = std::ranges::find_if(std::ranges::begin(rev), next, [](auto* w) {
                return w->isFocusable();
            });
        }

        if (rit != next) {
            (*rit)->requestFocus();
        }
    }

    void Widget::_draw(Graphics* graphics)
    {
        if (mOutlineSize > 0) {
            Rectangle rec = mDimension;
            rec.x -= mOutlineSize;
            rec.y -= mOutlineSize;
            rec.width += 2 * mOutlineSize;
            rec.height += 2 * mOutlineSize;
            graphics->pushClipArea(rec);
            drawOutline(graphics);
            graphics->popClipArea();
        }

        graphics->pushClipArea(mDimension);
        draw(graphics);

        if (!mChildren.empty()) {
            Rectangle const & childrenArea = getChildrenArea();
            graphics->pushClipArea(childrenArea);

            for (auto* widget : mChildren) {
                // Only draw a widget:
                // if it's visible
                // and inside the children area.
                if (widget->isVisible() && childrenArea.isIntersecting(widget->getDimension())) {
                    widget->_draw(graphics);
                }
            }
            graphics->popClipArea();
        }
        graphics->popClipArea();
    }

    void Widget::_logic()
    {
        logic();

        std::list<Widget*>::const_iterator iter;
        for (iter = mChildren.begin(); iter != mChildren.end(); ++iter) {
            (*iter)->_logic();
        }
    }

    std::list<Widget*> const & Widget::getChildren() const
    {
        return mChildren;
    }

    void Widget::getLastPosition(int& x, int& y) const
    {
        x = mLastX;
        y = mLastY;
    }

    void Widget::setLastPosition(int x, int y)
    {
        mLastX = x;
        mLastY = y;
    }

    bool Widget::isLastPositionSet() const
    {
        return mLastX != 0 || mLastY != 0;
    }
} // namespace fcn
