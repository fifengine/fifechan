/***************************************************************************
 *   Copyright (C) 2012 by the fifechan team                               *
 *   http://fifechan.github.com/fifechan                                   *
 *   This file is part of fifechan.                                        *
 *                                                                         *
 *   fifechan is free software; you can redistribute it and/or             *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA          *
 ***************************************************************************/

/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naess�n and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naess�n a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * For comments regarding functions please see the header file.
 */

#include "fifechan/widget.hpp"

#include "fifechan/actionevent.hpp"
#include "fifechan/actionlistener.hpp"
#include "fifechan/deathlistener.hpp"
#include "fifechan/defaultfont.hpp"
#include "fifechan/event.hpp"
#include "fifechan/exception.hpp"
#include "fifechan/focushandler.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/keyinput.hpp"
#include "fifechan/keylistener.hpp"
#include "fifechan/mouseinput.hpp"
#include "fifechan/mouselistener.hpp"
#include "fifechan/sizeconstraint.hpp"
#include "fifechan/visibilityeventhandler.hpp"
#include "fifechan/widgetlistener.hpp"

#include <algorithm>

namespace fcn
{
    Font* Widget::mGlobalFont = NULL;
    DefaultFont Widget::mDefaultFont;
    std::list<Widget*> Widget::mWidgetInstances;
    VisibilityEventHandler* Widget::mVisibilityEventHandler = NULL;
    DeathListener* Widget::mGuiDeathListener = NULL;

    Widget::Widget()
            : mForegroundColor(0x000000),
              mBackgroundColor(0xffffff),
              mBaseColor(0x808090),
              mSelectionColor(0xc3d9ff),
              mFocusHandler(NULL),
              mInternalFocusHandler(NULL),
              mParent(NULL),
              mFrameSize(0),
              mSizeConstraint(NULL),
              mFocusable(false),
              mVisible(true),
              mTabIn(true),
              mTabOut(true),
              mEnabled(true),
              mCurrentFont(NULL)
    {
        mWidgetInstances.push_back(this);
    }

    Widget::~Widget()
    {
        delete mSizeConstraint;
        
        if (mParent != NULL)
            mParent->remove(this);

        std::list<Widget*>::const_iterator childrenIter;
        for (childrenIter = mChildren.begin(); childrenIter != mChildren.end(); childrenIter++)
            (*childrenIter)->_setParent(NULL);
        
        std::list<DeathListener*>::const_iterator deathIter;
        for (deathIter = mDeathListeners.begin(); deathIter != mDeathListeners.end(); ++deathIter)
        {
            Event event(this);
            (*deathIter)->death(event);
        }
        
        if (mGuiDeathListener) {
            Event event(this);
            mGuiDeathListener->death(event);
        }
        
        _setFocusHandler(NULL);

        mWidgetInstances.remove(this);
    }

    void Widget::drawFrame(Graphics* graphics)
    {
        Color faceColor = getBaseColor();
        Color highlightColor, shadowColor;
        int alpha = getBaseColor().a;
        int width = getWidth() + getFrameSize() * 2 - 1;
        int height = getHeight() + getFrameSize() * 2 - 1;
        highlightColor = faceColor + 0x303030;
        highlightColor.a = alpha;
        shadowColor = faceColor - 0x303030;
        shadowColor.a = alpha;

        unsigned int i;
        for (i = 0; i < getFrameSize(); ++i)
        {
            graphics->setColor(shadowColor);
            graphics->drawLine(i,i, width - i, i);
            graphics->drawLine(i,i + 1, i, height - i - 1);
            graphics->setColor(highlightColor);
            graphics->drawLine(width - i,i + 1, width - i, height - i);
            graphics->drawLine(i,height - i, width - i - 1, height - i);
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
        newDimension.width = width;

        setDimension(newDimension);
    }

    int Widget::getWidth() const
    {
        return mDimension.width;
    }

    void Widget::setHeight(int height)
    {
        Rectangle newDimension = mDimension;
        newDimension.height = height;

        setDimension(newDimension);
    }

    int Widget::getHeight() const
    {
        return mDimension.height;
    }

    void Widget::setX(int x)
    {
        Rectangle newDimension = mDimension;
        newDimension.x = x;

        setDimension(newDimension);
    }

    int Widget::getX() const
    {
        return mDimension.x;
    }

    void Widget::setY(int y)
    {
        Rectangle newDimension = mDimension;
        newDimension.y = y;

        setDimension(newDimension);
    }

    int Widget::getY() const
    {
        return mDimension.y;
    }

    void Widget::setPosition(int x, int y)
    {
        Rectangle newDimension = mDimension;
        newDimension.x = x;
        newDimension.y = y;
        
        setDimension(newDimension);
    }

    void Widget::setDimension(const Rectangle& dimension)
    { 
        Rectangle oldDimension = mDimension;
        mDimension = dimension;
        
        if (mDimension.width != oldDimension.width
            || mDimension.height != oldDimension.height)
        {
            if(hasSizeConstraint())
            {
                enforceSizeConstraint();   
            }
            
            distributeResizedEvent();
        }

        if (mDimension.x != oldDimension.x
            || mDimension.y != oldDimension.y)
        {
            distributeMovedEvent();
            
            std::list<Widget*>::iterator currChild(mChildren.begin());
            std::list<Widget*>::iterator endChildren(mChildren.end());
            
            for(; currChild != endChildren; ++currChild)
            {
                (*currChild)->distributeAncestorMovedEvent(this);
            }
        }
    }

    void Widget::setFrameSize(unsigned int frameSize)
    {
        mFrameSize = frameSize;
    }

    unsigned int Widget::getFrameSize() const
    {
        return mFrameSize;
    }

    const Rectangle& Widget::getDimension() const
    {
        return mDimension;
    }

    const std::string& Widget::getActionEventId() const
    {
        return mActionEventId;
    }

    void Widget::setActionEventId(const std::string& actionEventId)
    {
        mActionEventId = actionEventId;
    }

    bool Widget::isFocused() const
    {
        if (!mFocusHandler)
        {
            return false;
        }

        return (mFocusHandler->isFocused(this));
    }

    void Widget::setFocusable(bool focusable)
    {
        if (!focusable && isFocused())
        {
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
        if (mFocusHandler == NULL)
            throw FCN_EXCEPTION("No focushandler set (did you add the widget to the gui?).");

        if (isFocusable())
            mFocusHandler->requestFocus(this);
    }

    void Widget::requestMoveToTop()
    {
        if (mParent != NULL)
            mParent->moveToTop(this);
    }

    void Widget::requestMoveToBottom()
    {
        if (mParent != NULL)
            mParent->moveToBottom(this);
    }

    void Widget::setVisible(bool visible)
    {   
        VisibilityEventHandler *visibilityEventHandler = _getVisibilityEventHandler();
        
        if (!visible && isFocused())
            mFocusHandler->focusNone();
        
        mVisible = visible;
        
        if (visible)
        {
            visibilityEventHandler->widgetShown(Event(this));
            distributeShownEvent();
            
            std::list<Widget*>::iterator currChild(mChildren.begin());
            std::list<Widget*>::iterator endChildren(mChildren.end());
            
            for(; currChild != endChildren; ++currChild)
            {
                (*currChild)->distributeAncestorShownEvent(this);
            }
        }
        else if(!visible)
        {
            visibilityEventHandler->widgetHidden(Event(this));
            distributeHiddenEvent();
            
            std::list<Widget*>::iterator currChild(mChildren.begin());
            std::list<Widget*>::iterator endChildren(mChildren.end());
            
            for(; currChild != endChildren; ++currChild)
            {
                (*currChild)->distributeAncestorHiddenEvent(this);
            }
        }
    }

    bool Widget::isVisible() const
    {
        if (getParent() == NULL)
            return mVisible;
        else
            return mVisible && getParent()->isVisible();
    }

    bool Widget::isSetVisible() const {
        return mVisible;
    }

    void Widget::setBaseColor(const Color& color)
    {
        mBaseColor = color;
    }

    const Color& Widget::getBaseColor() const
    {
        return mBaseColor;
    }

    void Widget::setForegroundColor(const Color& color)
    {
        mForegroundColor = color;
    }

    const Color& Widget::getForegroundColor() const
    {
        return mForegroundColor;
    }

    void Widget::setBackgroundColor(const Color& color)
    {
        mBackgroundColor = color;
    }

    const Color& Widget::getBackgroundColor() const
    {
        return mBackgroundColor;
    }

    void Widget::setSelectionColor(const Color& color)
    {
        mSelectionColor = color;
    }

    const Color& Widget::getSelectionColor() const
    {
        return mSelectionColor;
    }    
    
    void Widget::_setFocusHandler(FocusHandler* focusHandler)
    {
        if (mFocusHandler)
        {
            releaseModalFocus();
            if (mFocusHandler->getModalMouseInputFocused() == this) {
                releaseModalMouseInputFocus();
            }
            mFocusHandler->remove(this);
        }

        if (focusHandler)
            focusHandler->add(this);

        mFocusHandler = focusHandler;

        if (mInternalFocusHandler != NULL)
            return;

        std::list<Widget*>::const_iterator iter;
        for (iter = mChildren.begin(); iter != mChildren.end(); iter++)
        {
            if (widgetExists(*iter))
                (*iter)->_setFocusHandler(focusHandler);
        }
    }

    FocusHandler* Widget::_getFocusHandler()
    {
        return mFocusHandler;
    }
    
    void Widget::_setWidthWithoutNotifying(int width)
    {
        mDimension.width = width;
    }
    
    void Widget::_setHeightWithoutNotifying(int height)
    {
        mDimension.height = height;
    }
    
    void Widget::_setSizeWithoutNotifying(int width, int height)
    {
        mDimension.width = width;
        mDimension.height = height;
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
        if (getParent() == NULL)
        {
            x = mDimension.x;
            y = mDimension.y;
            return;
        }

        int parentX;
        int parentY;

        getParent()->getAbsolutePosition(parentX, parentY);

        x = parentX + mDimension.x + getParent()->getChildrenArea().x;
        y = parentY + mDimension.y + getParent()->getChildrenArea().y;
    }

    Font* Widget::getFont() const
    {
        if (mCurrentFont == NULL)
        {
            if (mGlobalFont == NULL)
                return &mDefaultFont;

            return mGlobalFont;
        }

        return mCurrentFont;
    }

    void Widget::setGlobalFont(Font* font)
    {
        mGlobalFont = font;

        std::list<Widget*>::iterator iter;
        for (iter = mWidgetInstances.begin(); iter != mWidgetInstances.end(); ++iter)
        {
            if ((*iter)->mCurrentFont == NULL)
                (*iter)->fontChanged();
        }
    }

    void Widget::setFont(Font* font)
    {
        mCurrentFont = font;
        fontChanged();
    }

    bool Widget::widgetExists(const Widget* widget)
    {
        std::list<Widget*>::const_iterator iter;
        for (iter = mWidgetInstances.begin(); iter != mWidgetInstances.end(); ++iter)
        {
            if (*iter == widget)
                return true;
        }

        return false;
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
        newDimension.width = width;
        newDimension.height = height;

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

    void Widget::requestModalFocus()
    {
        if (mFocusHandler == NULL)
            throw FCN_EXCEPTION("No focushandler set (did you add the widget to the gui?).");

        mFocusHandler->requestModalFocus(this);
    }

    void Widget::requestModalMouseInputFocus()
    {
        if (mFocusHandler == NULL)
            throw FCN_EXCEPTION("No focushandler set (did you add the widget to the gui?).");

        mFocusHandler->requestModalMouseInputFocus(this);
    }

    void Widget::releaseModalFocus()
    {
        if (mFocusHandler == NULL)
            return;

        mFocusHandler->releaseModalFocus(this);
    }

    void Widget::releaseModalMouseInputFocus()
    {
        if (mFocusHandler == NULL)
            return;

        mFocusHandler->releaseModalMouseInputFocus(this);
    }

    bool Widget::isModalFocused() const
    {
        if (mFocusHandler == NULL)
            throw FCN_EXCEPTION("No focushandler set (did you add the widget to the gui?).");

        if (getParent() != NULL)
        {
            return (mFocusHandler->getModalFocused() == this) 
                || getParent()->isModalFocused();
        }

        return mFocusHandler->getModalFocused() == this;
    }

    bool Widget::isModalMouseInputFocused() const
    {
        if (mFocusHandler == NULL)
            throw FCN_EXCEPTION("No focushandler set (did you add the widget to the gui?).");

        if (getParent() != NULL)
        {
            return (mFocusHandler->getModalMouseInputFocused() == this) 
                || getParent()->isModalMouseInputFocused();
        }

        return mFocusHandler->getModalMouseInputFocused() == this;
    }

    Widget *Widget::getWidgetAt(int x, int y, Widget* exclude)
    {
        Rectangle r = getChildrenArea();

        if (!r.isContaining(x, y))
            return NULL;

        x -= r.x;
        y -= r.y;

        std::list<Widget*>::reverse_iterator iter;
        for (iter = mChildren.rbegin(); iter != mChildren.rend(); iter++)
        {
            Widget* widget = (*iter);
            
            if (widget != exclude && widget->isVisible() && widget->getDimension().isContaining(x, y))
                return widget;
        }

        return NULL;
    }

    const std::list<MouseListener*>& Widget::_getMouseListeners()
    {
        return mMouseListeners;
    }

    const std::list<KeyListener*>& Widget::_getKeyListeners()
    {
        return mKeyListeners;
    }

    const std::list<FocusListener*>& Widget::_getFocusListeners()
    {
        return mFocusListeners;
    }

    Rectangle Widget::getChildrenArea()
    {
        return Rectangle(0, 0, 0, 0);
    }

    FocusHandler* Widget::_getInternalFocusHandler()
    {
        return mInternalFocusHandler;
    }

    void Widget::setInternalFocusHandler(FocusHandler* focusHandler)
    {
        mInternalFocusHandler = focusHandler;

        std::list<Widget*>::const_iterator iter;
        for (iter = mChildren.begin(); iter != mChildren.end(); iter++)
        {
            if (mInternalFocusHandler == NULL)
                (*iter)->_setFocusHandler(_getFocusHandler());
            else
                (*iter)->_setFocusHandler(mInternalFocusHandler);
        }
    }

    void Widget::setId(const std::string& id)
    {
        mId = id;
    }

    const std::string& Widget::getId() const
    {
        return mId;
    }

    void Widget::distributeResizedEvent()
    {
        std::list<WidgetListener*>::const_iterator iter;
        for (iter = mWidgetListeners.begin(); iter != mWidgetListeners.end(); ++iter)
        {
            Event event(this);
            (*iter)->widgetResized(event);
        }
    }

    void Widget::distributeMovedEvent()
    {
        std::list<WidgetListener*>::const_iterator iter;
        for (iter = mWidgetListeners.begin(); iter != mWidgetListeners.end(); ++iter)
        {
            Event event(this);
            (*iter)->widgetMoved(event);
        }
    }

    void Widget::distributeHiddenEvent()
    {
        std::list<WidgetListener*>::const_iterator iter;
        for (iter = mWidgetListeners.begin(); iter != mWidgetListeners.end(); ++iter)
        {
            Event event(this);
            (*iter)->widgetHidden(event);
        }
    }
    
    void Widget::distributeAncestorMovedEvent(Widget* ancestor)
    {
        std::list<WidgetListener*>::iterator currWidgetListener(mWidgetListeners.begin());
        std::list<WidgetListener*>::iterator endWidgetListeners(mWidgetListeners.end());
        Event event(ancestor);
        
        for(; currWidgetListener != endWidgetListeners; ++currWidgetListener)
        {
            (*currWidgetListener)->ancestorMoved(event);
        }
        
        std::list<Widget*>::iterator currChild(mChildren.begin());
        std::list<Widget*>::iterator endChildren(mChildren.end());
        
        for(; currChild != endChildren; ++currChild)
        {
            (*currChild)->distributeAncestorMovedEvent(ancestor);
        }
    }
    
    void Widget::distributeAncestorHiddenEvent(Widget* ancestor)
    {
        std::list<WidgetListener*>::iterator currWidgetListener(mWidgetListeners.begin());
        std::list<WidgetListener*>::iterator endWidgetListeners(mWidgetListeners.end());
        Event event(ancestor);
        
        for(; currWidgetListener != endWidgetListeners; ++currWidgetListener)
        {
            (*currWidgetListener)->ancestorHidden(event);
        }
        
        std::list<Widget*>::iterator currChild(mChildren.begin());
        std::list<Widget*>::iterator endChildren(mChildren.end());
        
        for(; currChild != endChildren; ++currChild)
        {
            (*currChild)->distributeAncestorHiddenEvent(ancestor);
        }
    }
    
    void Widget::distributeAncestorShownEvent(Widget* ancestor)
    {
        std::list<WidgetListener*>::iterator currWidgetListener(mWidgetListeners.begin());
        std::list<WidgetListener*>::iterator endWidgetListeners(mWidgetListeners.end());
        Event event(ancestor);
        
        for(; currWidgetListener != endWidgetListeners; ++currWidgetListener)
        {
            (*currWidgetListener)->ancestorShown(event);
        }
        
        std::list<Widget*>::iterator currChild(mChildren.begin());
        std::list<Widget*>::iterator endChildren(mChildren.end());
        
        for(; currChild != endChildren; ++currChild)
        {
            (*currChild)->distributeAncestorShownEvent(ancestor);
        }
    }

    void Widget::distributeActionEvent()
    {
        std::list<ActionListener*>::const_iterator iter;
        for (iter = mActionListeners.begin(); iter != mActionListeners.end(); ++iter)
        {
            ActionEvent actionEvent(this, mActionEventId);
            (*iter)->action(actionEvent);
        }
    }

    void Widget::distributeShownEvent()
    {
        std::list<WidgetListener*>::const_iterator iter;
        for (iter = mWidgetListeners.begin(); iter != mWidgetListeners.end(); ++iter)
        {
            Event event(this);
            (*iter)->widgetShown(event);
        }
    }
    
    void Widget::showPart(Rectangle rectangle)
    {
        if (mParent != NULL)
            mParent->showWidgetPart(this, rectangle);               
    }
    
    void Widget::setSizeConstraint(SizeConstraint* sizeConstraint)
    {
        delete mSizeConstraint;
        mSizeConstraint = sizeConstraint;
        enforceSizeConstraint();
    }
    
    SizeConstraint* Widget::getSizeConstraint() const
    {
        return mSizeConstraint;
    }
    
    bool Widget::hasSizeConstraint() const
    {
        return mSizeConstraint != NULL;
    }

    Widget* Widget::getTop() const
    {
        if (getParent() == NULL)
            return NULL;

        Widget* widget = getParent();
        Widget* parent = getParent()->getParent();
        
        while (parent != NULL)
        {
            widget = parent;
            parent = parent->getParent();
        }

        return widget;
    }

    std::list<Widget*> Widget::getWidgetsIn(const Rectangle& area, 
                                            Widget* ignore)
    {
        std::list<Widget*> result;
        
        std::list<Widget*>::const_iterator iter;
        for (iter = mChildren.begin(); iter != mChildren.end(); iter++)
        {
            Widget* widget = (*iter);
            if (ignore != widget && widget->getDimension().isIntersecting(area))
                result.push_back(widget);
        }

        return result;
    }

    void Widget::resizeToChildren()
    {
        int w = 0, h = 0;
        std::list<Widget*>::const_iterator iter;
        for (iter = mChildren.begin(); iter != mChildren.end(); iter++)
        {
            Widget* widget = (*iter);
            if(widget->isVisible())
            {
                if (widget->getX() + widget->getWidth() > w)
                    w = widget->getX() + widget->getWidth();

                if (widget->getY() + widget->getHeight() > h)
                    h = widget->getY() + widget->getHeight();
            }
        }
        
        setSize(w, h);
    }
    
    void Widget::enforceSizeConstraint()
    {
        if(mSizeConstraint == NULL)
            throw FCN_EXCEPTION("Enforcing a non-existent size constraint");
        
        int minWidth = mSizeConstraint->getMinWidth();
        int minHeight = mSizeConstraint->getMinHeight();
        int maxWidth = mSizeConstraint->getMaxWidth();
        int maxHeight = mSizeConstraint->getMaxHeight();
        int currWidth = mDimension.width;
        int currHeight = mDimension.height;
        
        mDimension.width = std::max(std::min(currWidth, maxWidth), minWidth);
        mDimension.height = std::max(std::min(currHeight, maxHeight), minHeight);
    }

    Widget* Widget::findWidgetById(const std::string& id)
    {
        std::list<Widget*>::const_iterator iter;
        for (iter = mChildren.begin(); iter != mChildren.end(); iter++)
        {
            Widget* widget = (*iter);
           
            if (widget->getId() == id)
                return widget;
            
            Widget *child = widget->findWidgetById(id);
            
            if (child != NULL)
                return child;
        }

        return NULL;
    }

    void Widget::showWidgetPart(Widget* widget, Rectangle area)
    {
        Rectangle widgetArea = getChildrenArea();

        area.x += widget->getX();
        area.y += widget->getY();
        
        if (area.x + area.width > widgetArea.width)
            widget->setX(widget->getX() - area.x - area.width + widgetArea.width);

        if (area.y + area.height > widgetArea.height)
            widget->setY(widget->getY() - area.y - area.height + widgetArea.height);

        if (area.x < 0)
            widget->setX(widget->getX() - area.x);

        if (area.y < 0)
            widget->setY(widget->getY() - area.y);
    }

    void Widget::clear()
    {
        std::list<Widget*>::const_iterator iter;
        for (iter = mChildren.begin(); iter != mChildren.end(); iter++)
        {
            Widget* widget = (*iter);
            widget->_setFocusHandler(NULL);
            widget->_setParent(NULL);
        }

        mChildren.clear();
    }

    void Widget::remove(Widget* widget)
    {
        std::list<Widget*>::iterator iter;
        for (iter = mChildren.begin(); iter != mChildren.end(); iter++)
        {
            if (*iter == widget)
            {
                mChildren.erase(iter);
                widget->_setFocusHandler(NULL);
                widget->_setParent(NULL);
                return;
            }
        }

        throw FCN_EXCEPTION("There is no such widget in this container.");
    }

    void Widget::add(Widget* widget)
    {
        mChildren.push_back(widget);

        if (mInternalFocusHandler == NULL)
            widget->_setFocusHandler(_getFocusHandler());
        else
            widget->_setFocusHandler(mInternalFocusHandler);

        widget->_setParent(this);
    }

    void Widget::moveToTop(Widget* widget)
    {
        std::list<Widget*>::iterator iter;
        iter = std::find(mChildren.begin(), mChildren.end(), widget);

        if (iter == mChildren.end())
            throw FCN_EXCEPTION("There is no such widget in this widget.");

        mChildren.remove(widget);
        mChildren.push_back(widget);
    }

    void Widget::moveToBottom(Widget* widget)
    {
        std::list<Widget*>::iterator iter;
        iter = find(mChildren.begin(), mChildren.end(), widget);

        if (iter == mChildren.end())
            throw FCN_EXCEPTION("There is no such widget in this widget.");

        mChildren.remove(widget);
        mChildren.push_front(widget);
    }

    void Widget::focusNext()
    {
        std::list<Widget*>::const_iterator iter;

        for (iter = mChildren.begin(); iter != mChildren.end(); iter++)
        {
            if ((*iter)->isFocused())
                break;
        }

        std::list<Widget*>::const_iterator end = iter;
        
        if (iter == mChildren.end())
            iter = mChildren.begin();

        iter++;

        for (; iter != end; iter++)
        {
            if (iter == mChildren.end())
                iter = mChildren.begin();

            if ((*iter)->isFocusable())
            {
                (*iter)->requestFocus();
                return;
            }
        }
    }

    void Widget::focusPrevious()
    {
        std::list<Widget*>::reverse_iterator iter;

        for (iter = mChildren.rbegin(); iter != mChildren.rend(); iter++)
        {
            if ((*iter)->isFocused())
                break;
        }

        std::list<Widget*>::reverse_iterator end = iter;
        iter++;

        if (iter == mChildren.rend())
            iter = mChildren.rbegin();

        for (; iter != end; iter++)
        {
            if (iter == mChildren.rend())
                iter = mChildren.rbegin();

            if ((*iter)->isFocusable())
            {
                (*iter)->requestFocus();
                return;
            }
        }
    }

    void Widget::_draw(Graphics* graphics)
    {
        if (mFrameSize > 0)
        {
            Rectangle rec = mDimension;
            rec.x -= mFrameSize;
            rec.y -= mFrameSize;
            rec.width += 2 * mFrameSize;
            rec.height += 2 * mFrameSize;
            graphics->pushClipArea(rec);
            drawFrame(graphics);
            graphics->popClipArea();
        }

        graphics->pushClipArea(mDimension);
        draw(graphics);

        const Rectangle& childrenArea = getChildrenArea();
        graphics->pushClipArea(childrenArea);

        std::list<Widget*>::const_iterator iter;
        for (iter = mChildren.begin(); iter != mChildren.end(); iter++)
        {
            Widget* widget = (*iter);
            // Only draw a widget if it's visible and if it visible
            // inside the children area.
            if (widget->isVisible() && childrenArea.isIntersecting(widget->getDimension()))
                widget->_draw(graphics);
        }

        graphics->popClipArea();
        graphics->popClipArea();
    }

    void Widget::_logic()
    {
        logic();

        std::list<Widget*>::const_iterator iter;
        for (iter = mChildren.begin(); iter != mChildren.end(); iter++)
            (*iter)->_logic();
    }

    const std::list<Widget*>& Widget::getChildren() const
    {
        return mChildren;
    }
}
