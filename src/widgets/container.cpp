/***************************************************************************
 *   Copyright (c) 2017 by the fifechan team                               *
 *   https://github.com/fifengine/fifechan                                 *
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
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
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

#include <set>

#include "fifechan/widgets/container.hpp"

#include "fifechan/exception.hpp"
#include "fifechan/util/fcn_math.hpp"
#include "fifechan/graphics.hpp"

#include <algorithm>

namespace fcn
{
    Container::Container()
    {
        mOpaque = true;
        mLayout = Absolute;
        mUniform = false;
        mVerticalSpacing = 2;
        mHorizontalSpacing = 2;
        mBackgroundWidget = NULL;
    }

    Container::~Container()
    {
    }

    void Container::draw(Graphics* graphics)
    {
        bool active = isFocused();
        if (isOpaque()) {
            if (active && ((getSelectionMode() & Widget::Selection_Background) == Widget::Selection_Background)) {
                graphics->setColor(getSelectionColor());
            } else {
                graphics->setColor(getBaseColor());
            }
            graphics->fillRectangle(getBorderSize(), getBorderSize(),
                getWidth() - 2 * getBorderSize(), getHeight() - 2 * getBorderSize());
        }
        if (mBackgroundWidget) {
            Rectangle rec(getBorderSize(), getBorderSize(), getWidth() - 2 * getBorderSize(), getHeight() - 2 * getBorderSize());
            mBackgroundWidget->setDimension(rec);
            mBackgroundWidget->_draw(graphics);
        }
        if (getBorderSize() > 0) {
            if (active && (getSelectionMode() & Widget::Selection_Border) == Widget::Selection_Border) {
                drawSelectionFrame(graphics);
            } else {
                drawBorder(graphics);
            }
        }
    }

    void Container::setOpaque(bool opaque)
    {
        mOpaque = opaque;
    }

    bool Container::isOpaque() const
    {
        return mOpaque;
    }

    void Container::add(Widget* widget)
    {
        Widget::add(widget);
        distributeWidgetAddedEvent(widget);
    }

    void Container::add(Widget* widget, int x, int y)
    {
        widget->setPosition(x, y);
        Widget::add(widget);
        distributeWidgetAddedEvent(widget);
    }

    void Container::remove(Widget* widget)
    {
        Widget::remove(widget);
        distributeWidgetRemovedEvent(widget);
    }

    void Container::clear()
    {
        Widget::clear();
    }

    Widget* Container::findWidgetById(const std::string &id)
    {
        return Widget::findWidgetById(id);
    }

    void Container::addContainerListener(ContainerListener* containerListener)
    {
        mContainerListeners.push_back(containerListener);
    }
   
    void Container::removeContainerListener(ContainerListener* containerListener)
    {
        mContainerListeners.remove(containerListener);
    }

    void Container::distributeWidgetAddedEvent(Widget* source)
    {
        ContainerListenerIterator iter;

        for (iter = mContainerListeners.begin(); iter != mContainerListeners.end(); ++iter)
        {
            ContainerEvent event(source, this);
            (*iter)->widgetAdded(event);
        }
    }
    
    void Container::distributeWidgetRemovedEvent(Widget* source)
    {
        ContainerListenerIterator iter;

        for (iter = mContainerListeners.begin(); iter != mContainerListeners.end(); ++iter)
        {
            ContainerEvent event(source, this);
            (*iter)->widgetRemoved(event);
        }
    }

    const std::list<Widget*>& Container::getChildren() const
    {
        return Widget::getChildren();
    }

    void Container::resizeToContent(bool recursiv) {
        if (mLayout == Absolute) {
            if (recursiv) {
                std::list<Widget*>::const_iterator currChild(mChildren.begin());
                std::list<Widget*>::const_iterator endChildren(mChildren.end());
                for(; currChild != endChildren; ++currChild) {
                    if (!(*currChild)->isVisible()) {
                        continue;
                    }
                    (*currChild)->resizeToContent(recursiv);
                }
            }
            return;
        }

        int childMaxW = 0;
        int childMaxH = 0;
        int layoutMaxW = 0;
        int layoutMaxH = 0;
        int totalW = 0;
        int totalH = 0;
        int visibleChilds = 0;

        std::list<Widget*>::const_iterator currChild(mChildren.begin());
        std::list<Widget*>::const_iterator endChildren(mChildren.end());
        for(; currChild != endChildren; ++currChild) {
            if (!(*currChild)->isVisible()) {
                continue;
            }
            if (recursiv) {
                (*currChild)->resizeToContent(recursiv);
            }
            const Rectangle& rec = (*currChild)->getDimension();
            childMaxW = std::max(childMaxW, rec.width);
            childMaxH = std::max(childMaxH, rec.height);
            layoutMaxW = std::max(layoutMaxW, rec.width + (*currChild)->getMarginLeft() + (*currChild)->getMarginRight());
            layoutMaxH = std::max(layoutMaxH, rec.height + (*currChild)->getMarginTop() + (*currChild)->getMarginBottom());
            ++visibleChilds;
        }

        // diff means border, padding, ...
        int diffW = getDimension().width - getChildrenArea().width;
        int diffH = getDimension().height - getChildrenArea().height;
        Rectangle dimensions(0, 0, childMaxW, childMaxH);

        if (mLayout == Vertical && visibleChilds > 0) {
            currChild = mChildren.begin();
            endChildren = mChildren.end();
            for(; currChild != endChildren; ++currChild) {
                if (!(*currChild)->isVisible()) {
                    continue;
                }
                dimensions.x = (*currChild)->getMarginLeft();
                dimensions.y += (*currChild)->getMarginTop();
                int layoutW = (*currChild)->getWidth() + (*currChild)->getMarginLeft() + ((*currChild)->getMarginRight() > 0 ? (*currChild)->getMarginRight() : 0);
                dimensions.width = (*currChild)->getWidth() + (layoutMaxW  - layoutW);
                dimensions.height = (*currChild)->getHeight();
                (*currChild)->setDimension(dimensions);
                dimensions.y += (*currChild)->getHeight() + (*currChild)->getMarginBottom() + getVerticalSpacing();
            }
            // remove last spacing
            dimensions.y -= getVerticalSpacing();
            totalW = std::max(layoutMaxW, childMaxW) + diffW;
            totalH = dimensions.y + diffH;
        } else if (mLayout == Horizontal && visibleChilds > 0) {
            currChild = mChildren.begin();
            endChildren = mChildren.end();
            for(; currChild != endChildren; ++currChild) {
                if (!(*currChild)->isVisible()) {
                    continue;
                }
                dimensions.x += (*currChild)->getMarginLeft();
                dimensions.y = (*currChild)->getMarginTop();
                dimensions.width = (*currChild)->getWidth();
                int layoutH = (*currChild)->getHeight() + (*currChild)->getMarginTop() + ((*currChild)->getMarginBottom() > 0 ? (*currChild)->getMarginBottom() : 0);
                dimensions.height = (*currChild)->getHeight() + (layoutMaxH  - layoutH);
                (*currChild)->setDimension(dimensions);
                dimensions.x += (*currChild)->getWidth() + (*currChild)->getMarginRight() + getHorizontalSpacing();
            }
            // remove last spacing
            dimensions.x -= getHorizontalSpacing();
            totalW = dimensions.x + diffW;
            totalH = std::max(layoutMaxH, childMaxH) + diffH;
        } else if (mLayout == Circular && visibleChilds > 0) {
            const float angle = 360.0 / visibleChilds;
            float xRadius = childMaxW * 2 + getHorizontalSpacing();
            float yRadius = childMaxH * 2 + getVerticalSpacing();
            currChild = mChildren.begin();
            endChildren = mChildren.end();
            int w = 0;
            int h = 0;
            int i = 0;
            int minW = 50000;
            int minH = 50000;
            int maxW = -50000;
            int maxH = -50000;
            for (; currChild != endChildren; ++currChild) {
                if (!(*currChild)->isVisible()) {
                    continue;
                }
                const float tmpAngle = (int(angle * i + 270) % 360) / (180.0 / Mathf::pi());
                int x = xRadius * cos(tmpAngle) - (*currChild)->getWidth() / 2;
                int y = yRadius * sin(tmpAngle) - (*currChild)->getHeight() / 2;
                minW = std::min(minW, x);
                maxW = std::max(maxW, x + (*currChild)->getWidth());
                minH = std::min(minH, y);
                maxH = std::max(maxH, y + (*currChild)->getHeight());
                
                (*currChild)->setPosition(x, y);
                ++i;
            }

            w = ABS(minW)+ABS(maxW);
            h = ABS(minH)+ABS(maxH);

            int centerX = w / 2;
            int centerY = h / 2;

            currChild = mChildren.begin();
            endChildren = mChildren.end();
            for(; currChild != endChildren; ++currChild) {
                if (!(*currChild)->isVisible()) {
                    continue;
                }
                int x = (*currChild)->getX() + centerX;
                int y = (*currChild)->getY() + centerY;

                (*currChild)->setPosition(x, y);
            }

            totalW = w + diffW;
            totalH = h + diffH;
        }

        setSize(totalW, totalH);
    }

    void Container::adjustSize() {
        resizeToChildren();
        int w = getWidth() + 2 * getBorderSize() + getPaddingLeft() + getPaddingRight();
        int h = getHeight() + 2 * getBorderSize() + getPaddingTop() + getPaddingBottom();
        setSize(w, h);
    }

    void Container::expandContent(bool recursiv) {
        if (mLayout == Absolute) {
            if (recursiv) {
                std::list<Widget*>::const_iterator currChild(mChildren.begin());
                std::list<Widget*>::const_iterator endChildren(mChildren.end());
                for(; currChild != endChildren; ++currChild) {
                    if (!(*currChild)->isVisible()) {
                        continue;
                    }
                    (*currChild)->expandContent(recursiv);
                }
            }
            return;
        }

        Rectangle childrenArea = getChildrenArea();
        int spaceW = childrenArea.width;
        int spaceH = childrenArea.height;
        int neededSpaceW = 0;
        int neededSpaceH = 0;
        int maxMinW = 0;
        int maxMinH = 0;
        int minMaxW = 50000;
        int minMaxH = 50000;
        int maxHExpander = 0;
        int maxVExpander = 0;
        int expanderNeededSpaceW = 0;
        int expanderNeededSpaceH = 0;
        unsigned int visibleChilds = 0;
        std::list<Widget*> hExpander;
        std::list<Widget*> vExpander;

        std::list<Widget*>::const_iterator currChild(mChildren.begin());
        std::list<Widget*>::const_iterator endChildren(mChildren.end());
        for(; currChild != endChildren; ++currChild) {
            if (!(*currChild)->isVisible()) {
                continue;
            }
            ++visibleChilds;
            // get needed space
            neededSpaceW += (*currChild)->getWidth() + (*currChild)->getMarginLeft() + (*currChild)->getMarginRight() + getHorizontalSpacing();
            neededSpaceH += (*currChild)->getHeight() + (*currChild)->getMarginTop() + (*currChild)->getMarginBottom() + getVerticalSpacing();
            // get expander and expander max/min size
            if ((*currChild)->isVerticalExpand()) {
                maxVExpander = std::max(maxVExpander, (*currChild)->getHeight());
                maxMinH = std::max(maxMinH, (*currChild)->getMinSize().getHeight());
                minMaxH = std::min(minMaxH, (*currChild)->getMaxSize().getHeight());
                expanderNeededSpaceH += (*currChild)->getHeight() + getVerticalSpacing();
                vExpander.push_back((*currChild));
            }
            if ((*currChild)->isHorizontalExpand()) {
                maxHExpander = std::max(maxHExpander, (*currChild)->getWidth());
                maxMinW = std::max(maxMinW, (*currChild)->getMinSize().getWidth());
                minMaxW = std::min(minMaxW, (*currChild)->getMaxSize().getWidth());
                expanderNeededSpaceW += (*currChild)->getWidth() + getHorizontalSpacing();
                hExpander.push_back((*currChild));
            }
        }

        if (mLayout == Vertical && visibleChilds > 0) {
            bool hexpand = !(!isHorizontalExpand() && getParent());
            neededSpaceH -= getVerticalSpacing();
            int freeSpace = spaceH - neededSpaceH;
            if (freeSpace > 0) {
                if (vExpander.size() > 0) {
                    expanderNeededSpaceH -= getVerticalSpacing();
                }
                if (mUniform) {
                    // check against the smallest maximal height
                    if (minMaxH < maxVExpander) {
                        maxVExpander = minMaxH;
                    }
                    // check against the largest minimal height
                    if (maxMinH > maxVExpander) {
                        maxVExpander = maxMinH;
                    }
                    int h = 0;
                    // calculate maximal height if all expanders get this max height
                    int maxNeeded = ((maxVExpander + getVerticalSpacing()) * vExpander.size()) - getVerticalSpacing();
                    int tmpSpace = (freeSpace + expanderNeededSpaceH) - maxNeeded;
                    if (tmpSpace > 0) {
                        h = maxVExpander;
                        freeSpace = tmpSpace;
                    }
                    // distribute space
                    if (freeSpace > 0 || h > 0) {
                        std::list<Widget*>::iterator it = vExpander.begin();
                        int expanders = vExpander.size();
                        for (; it != vExpander.end(); ++it) {
                            int layoutH = (*it)->getHeight() + (*it)->getMarginTop() + ((*it)->getMarginBottom() > 0 ? (*it)->getMarginBottom() : 0);
                            // divide the space so that all expanders get the same size
                            int diff = h > 0 ? 0 : (*it)->getHeight() + (maxVExpander  - layoutH);
                            int delta = ((freeSpace-diff) / expanders) + diff;
                            if (delta == 0) {
                                delta = 1;
                            }
                            if (delta > freeSpace) {
                                delta = freeSpace;
                            }
                            int oldH = h > 0 ? h : (*it)->getHeight();
                            int tmpH = oldH + delta;
                            (*it)->setHeight(tmpH);
                            tmpH = (*it)->getHeight();
                            delta = tmpH - oldH;
                            freeSpace -= delta;
                            --expanders;
                        }
                    }

                } else {
                    if (vExpander.size() > 0) {
                        // simply add one to each expander until free space is empty
                        // or all expanders reached the max height
                        std::set<Widget*> maxExpanders;
                        while (freeSpace && maxExpanders.size() < vExpander.size()) {
                            std::list<Widget*>::iterator it = vExpander.begin();
                            for (; it != vExpander.end(); ++it) {
                                int h = (*it)->getHeight();
                                (*it)->setHeight(h+1);
                                if (h != (*it)->getHeight()) {
                                    --freeSpace;
                                    if (freeSpace == 0) {
                                        break;
                                    }
                                } else {
                                    maxExpanders.insert(*it);
                                }
                            }
                        }
                    }
                }
            }
            // adapt position
            if (!hExpander.empty() || !vExpander.empty() || hexpand) {
                Rectangle rec(0, 0, spaceW, 0);
                currChild = mChildren.begin();
                endChildren = mChildren.end();
                for(; currChild != endChildren; ++currChild) {
                    if (!(*currChild)->isVisible()) {
                        continue;
                    }
                    if (hexpand || (*currChild)->isHorizontalExpand()) {
                        int layoutW = (*currChild)->getMarginLeft() + ((*currChild)->getMarginRight() > 0 ? (*currChild)->getMarginRight() : 0);
                        rec.width = spaceW-layoutW;
                    } else {
                        rec.width = (*currChild)->getWidth();
                    }
                    rec.x = (*currChild)->getMarginLeft();
                    rec.y += (*currChild)->getMarginTop();
                    rec.height = (*currChild)->getHeight();
                    (*currChild)->setDimension(rec);
                    rec.y += rec.height + (*currChild)->getMarginBottom() + getVerticalSpacing();
                }
            }
        } else if (mLayout == Horizontal && visibleChilds > 0) {
            bool vexpand = !(!isVerticalExpand() && getParent());
            neededSpaceW -= getHorizontalSpacing();
            int freeSpace = spaceW - neededSpaceW;
            if (freeSpace > 0) {
                if (mUniform) {
                    if (hExpander.size() > 0) {
                        expanderNeededSpaceW -= getHorizontalSpacing();
                    }
                    // check against the smallest maximal width
                    if (minMaxW < maxHExpander) {
                        maxHExpander = minMaxW;
                    }
                    // check against the largest minimal width
                    if (maxMinW > maxHExpander) {
                        maxHExpander = maxMinW;
                    }
                    int w = 0;
                    // calculate maximal width if all expanders get this max width
                    int maxNeeded = ((maxHExpander + getHorizontalSpacing()) * hExpander.size()) - getHorizontalSpacing();
                    int tmpSpace = (freeSpace + expanderNeededSpaceW) - maxNeeded;
                    if (tmpSpace > 0) {
                        w = maxHExpander;
                        freeSpace = tmpSpace;
                    }
                    // distribute space
                    if (freeSpace > 0 || w > 0) {
                        std::list<Widget*>::iterator it = hExpander.begin();
                        int expanders = hExpander.size();
                        for (; it != hExpander.end(); ++it) {
                            // divide the space so that all expanders get the same size
                            int layoutW = (*it)->getWidth() + (*it)->getMarginLeft() + ((*it)->getMarginRight() > 0 ? (*it)->getMarginRight() : 0);
                            int diff = w > 0 ? 0 : (*it)->getWidth() + (maxHExpander  - layoutW);
                            int delta = ((freeSpace-diff) / expanders) + diff;
                            if (delta == 0) {
                                delta = 1;
                            }
                            if (delta > freeSpace) {
                                delta = freeSpace;
                            }
                            int oldW = w > 0 ? w : (*it)->getWidth();
                            int tmpW = oldW + delta;
                            (*it)->setWidth(tmpW);
                            tmpW = (*it)->getWidth();
                            delta = tmpW - oldW;
                            freeSpace -= delta;
                            --expanders;
                        }
                    }
                } else {
                    if (hExpander.size() > 0) {
                        // simply add one to each expander until free space is empty
                        // or all expanders reached the max width
                        std::set<Widget*> maxExpanders;
                        while (freeSpace && maxExpanders.size() < hExpander.size()) {
                            std::list<Widget*>::iterator it = hExpander.begin();
                            for (; it != hExpander.end(); ++it) {
                                int w = (*it)->getWidth();
                                (*it)->setWidth(w+1);
                                if (w != (*it)->getWidth()) {
                                    --freeSpace;
                                    if (freeSpace == 0) {
                                        break;
                                    }
                                } else {
                                    maxExpanders.insert(*it);
                                }
                            }
                        }
                    }
                }
            }
            // adapt position
            if (!hExpander.empty() || !vExpander.empty() || vexpand) {
                Rectangle rec(0, 0, 0, spaceH);
                currChild = mChildren.begin();
                endChildren = mChildren.end();
                for(; currChild != endChildren; ++currChild) {
                    if (!(*currChild)->isVisible()) {
                        continue;
                    }
                    if (vexpand || (*currChild)->isVerticalExpand()) {
                        int layoutH = (*currChild)->getMarginTop() + ((*currChild)->getMarginBottom() > 0 ? (*currChild)->getMarginBottom() : 0);
                        rec.height= spaceH-layoutH;
                    } else {
                        rec.height = (*currChild)->getHeight();
                    }
                    rec.x += (*currChild)->getMarginLeft();
                    rec.y = (*currChild)->getMarginTop();
                    rec.width = (*currChild)->getWidth();
                    (*currChild)->setDimension(rec);
                    rec.x += rec.width + (*currChild)->getMarginRight() + getHorizontalSpacing();
                }
            }
        }else if (mLayout == Circular && visibleChilds > 0) {
            const float angle = 360.0 / visibleChilds;
            int childMaxW = 0;
            int childMaxH = 0;
            currChild = mChildren.begin();
            endChildren = mChildren.end();
            for(; currChild != endChildren; ++currChild) {
                if (!(*currChild)->isVisible()) {
                    continue;
                }
                childMaxW = std::max(childMaxW, (*currChild)->getWidth());
                childMaxH = std::max(childMaxH, (*currChild)->getHeight());
            }
            //childMaxW += getHorizontalSpacing();
            //childMaxH += getVerticalSpacing();
            float xRadius = (spaceW-childMaxW)/2.0;
            float yRadius = (spaceH-childMaxH)/2.0;
            float centerX = spaceW/2.0;
            float centerY = spaceH/2.0;
            if (xRadius < 1) {
                xRadius = childMaxW;
            }
            if (yRadius < 1) {
                yRadius = childMaxH;
            }
            // this forces a uniform circle
            //xRadius = std::max(xRadius, yRadius);
            //yRadius = xRadius;
            int i = 0;
            currChild = mChildren.begin();
            endChildren = mChildren.end();
            for(; currChild != endChildren; ++currChild) {
                if (!(*currChild)->isVisible()) {
                    continue;
                }
                float tmpAngle = (int(angle * i + 270) % 360) / (180.0 / Mathf::pi());
                int x = centerX + xRadius * cos(tmpAngle);
                int y = centerY + yRadius * sin(tmpAngle);
                x -= (*currChild)->getWidth() / 2;
                y -= (*currChild)->getHeight() / 2;

                (*currChild)->setPosition(x, y);
                ++i;
            }
        }

        if (recursiv) {
            currChild = mChildren.begin();
            endChildren = mChildren.end();
            for(; currChild != endChildren; ++currChild) {
                if (!(*currChild)->isVisible()) {
                    continue;
                }
                (*currChild)->expandContent(recursiv);
            }
        }
    }

    void Container::setLayout(LayoutPolicy policy) {
        mLayout = policy;
    }

    Container::LayoutPolicy Container::getLayout() const {
        return mLayout;
    }

    void Container::setUniformSize(bool uniform) {
        mUniform = uniform;
    }

    bool Container::isUniformSize() const {
        return mUniform;
    }

    Rectangle Container::getChildrenArea() {
        Rectangle rec;
        rec.x = getBorderSize() + getPaddingLeft();
        rec.y = getBorderSize() + getPaddingTop();
        rec.width = getWidth() - 2 * getBorderSize() - getPaddingLeft() - getPaddingRight();
        rec.height = getHeight() - 2 * getBorderSize() - getPaddingTop() - getPaddingBottom();
        return rec;
    }

    void Container::setVerticalSpacing(unsigned int spacing) {
        mVerticalSpacing = spacing;
    }
    
    unsigned int Container::getVerticalSpacing() const {
        return mVerticalSpacing;
    }

    void Container::setHorizontalSpacing(unsigned int spacing) {
        mHorizontalSpacing = spacing;
    }

    unsigned int Container::getHorizontalSpacing() const {
        return mHorizontalSpacing;
    }

    void Container::setBackgroundWidget(Widget* widget) {
        if (mBackgroundWidget != widget) {
            mBackgroundWidget = widget;
        }
    }

    Widget* Container::getBackgroundWidget() {
        return mBackgroundWidget;
    }
}
