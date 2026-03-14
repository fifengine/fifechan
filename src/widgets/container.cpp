// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/widgets/container.hpp"

#include <algorithm>
#include <list>
#include <set>
#include <string>

#include "fifechan/exception.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/math.hpp"

namespace fcn
{
    Container::Container() = default;

    Container::~Container() = default;

    void Container::draw(Graphics* graphics)
    {
        bool const active = isFocused();
        if (isOpaque()) {
            if (active &&
                ((getSelectionMode() & Widget::SelectionMode::Background) == Widget::SelectionMode::Background)) {
                graphics->setColor(getSelectionColor());
            } else {
                graphics->setColor(getBaseColor());
            }
            graphics->fillRectangle(
                getBorderSize(),
                getBorderSize(),
                getWidth() - (2 * getBorderSize()),
                getHeight() - (2 * getBorderSize()));
        }
        if (mBackgroundWidget != nullptr) {
            Rectangle const rec(
                getBorderSize(),
                getBorderSize(),
                getWidth() - (2 * getBorderSize()),
                getHeight() - (2 * getBorderSize()));
            mBackgroundWidget->setDimension(rec);
            mBackgroundWidget->_draw(graphics);
        }
        if (getBorderSize() > 0) {
            if (active && (getSelectionMode() & Widget::SelectionMode::Border) == Widget::SelectionMode::Border) {
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

    void Container::addWidget(std::unique_ptr<Widget> widget)
    {
        if (widget == nullptr) {
            return;
        }

        Widget* rawWidget = widget.release();
        add(rawWidget);
    }

    void Container::add(Widget* widget, int x, int y)
    {
        widget->setPosition(x, y);
        Widget::add(widget);
        distributeWidgetAddedEvent(widget);
    }

    void Container::addWidget(std::unique_ptr<Widget> widget, int x, int y)
    {
        if (widget == nullptr) {
            return;
        }

        Widget* rawWidget = widget.release();
        add(rawWidget, x, y);
    }

    void Container::remove(Widget* widget)
    {
        Widget::remove(widget);
        distributeWidgetRemovedEvent(widget);
    }

    void Container::removeAllChildren()
    {
        Widget::removeAllChildren();
    }

    Widget* Container::findWidgetById(std::string const & id)
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

        for (iter = mContainerListeners.begin(); iter != mContainerListeners.end(); ++iter) {
            ContainerEvent const event(source, this);
            (*iter)->widgetAdded(event);
        }
    }

    void Container::distributeWidgetRemovedEvent(Widget* source)
    {
        ContainerListenerIterator iter;

        for (iter = mContainerListeners.begin(); iter != mContainerListeners.end(); ++iter) {
            ContainerEvent const event(source, this);
            (*iter)->widgetRemoved(event);
        }
    }

    std::list<Widget*> const & Container::getChildren() const
    {
        return Widget::getChildren();
    }

    Widget* Container::getChild(unsigned int index) const
    {
        if (index >= getChildrenCount()) {
            return nullptr;
        }

        auto const & children = getChildren();
        auto iter             = children.begin();
        for (unsigned int i = 0; i < index; ++i) {
            ++iter;
        }

        return *iter;
    }

    void Container::resizeToContent(bool recursion)
    {
        if (mLayout == LayoutPolicy::Absolute) {
            if (recursion) {
                std::list<Widget*>::const_iterator currChild(mChildren.begin());
                std::list<Widget*>::const_iterator const endChildren(mChildren.end());
                for (; currChild != endChildren; ++currChild) {
                    if (!(*currChild)->isVisible()) {
                        continue;
                    }
                    (*currChild)->resizeToContent(recursion);
                }
            }
            return;
        }

        int childMaxW     = 0;
        int childMaxH     = 0;
        int layoutMaxW    = 0;
        int layoutMaxH    = 0;
        int totalW        = 0;
        int totalH        = 0;
        int visibleChilds = 0;

        std::list<Widget*>::const_iterator currChild(mChildren.begin());
        std::list<Widget*>::const_iterator endChildren(mChildren.end());
        for (; currChild != endChildren; ++currChild) {
            if (!(*currChild)->isVisible()) {
                continue;
            }
            if (recursion) {
                (*currChild)->resizeToContent(recursion);
            }
            Rectangle const & rec = (*currChild)->getDimension();
            childMaxW             = std::max(childMaxW, rec.width);
            childMaxH             = std::max(childMaxH, rec.height);
            layoutMaxW =
                std::max(layoutMaxW, rec.width + (*currChild)->getMarginLeft() + (*currChild)->getMarginRight());
            layoutMaxH =
                std::max(layoutMaxH, rec.height + (*currChild)->getMarginTop() + (*currChild)->getMarginBottom());
            ++visibleChilds;
        }

        // diff means border, padding, ...
        int const diffW = getDimension().width - getChildrenArea().width;
        int const diffH = getDimension().height - getChildrenArea().height;
        Rectangle dimensions(0, 0, childMaxW, childMaxH);

        if (mLayout == LayoutPolicy::AutoSize && visibleChilds > 0) {
            currChild   = mChildren.begin();
            endChildren = mChildren.end();
            for (; currChild != endChildren; ++currChild) {
                if (!(*currChild)->isVisible()) {
                    continue;
                }
                Rectangle const & rec = (*currChild)->getDimension();
                int const childW = rec.x + rec.width + (*currChild)->getMarginLeft() + (*currChild)->getMarginRight();
                int const childH = rec.y + rec.height + (*currChild)->getMarginTop() + (*currChild)->getMarginBottom();
                totalW           = std::max(totalW, childW);
                totalH           = std::max(totalH, childH);
            }
            totalW += diffW;
            totalH += diffH;
        } else if (mLayout == LayoutPolicy::Vertical && visibleChilds > 0) {
            currChild   = mChildren.begin();
            endChildren = mChildren.end();
            for (; currChild != endChildren; ++currChild) {
                if (!(*currChild)->isVisible()) {
                    continue;
                }
                dimensions.x = (*currChild)->getMarginLeft();
                dimensions.y += (*currChild)->getMarginTop();
                int const layoutW = (*currChild)->getWidth() + (*currChild)->getMarginLeft() +
                                    ((*currChild)->getMarginRight() > 0 ? (*currChild)->getMarginRight() : 0);
                dimensions.width  = (*currChild)->getWidth() + (layoutMaxW - layoutW);
                dimensions.height = (*currChild)->getHeight();
                (*currChild)->setDimension(dimensions);
                dimensions.y += (*currChild)->getHeight() + (*currChild)->getMarginBottom() + getVerticalSpacing();
            }
            // remove last spacing
            dimensions.y -= getVerticalSpacing();
            totalW = std::max(layoutMaxW, childMaxW) + diffW;
            totalH = dimensions.y + diffH;
        } else if (mLayout == LayoutPolicy::Horizontal && visibleChilds > 0) {
            currChild   = mChildren.begin();
            endChildren = mChildren.end();
            for (; currChild != endChildren; ++currChild) {
                if (!(*currChild)->isVisible()) {
                    continue;
                }
                dimensions.x += (*currChild)->getMarginLeft();
                dimensions.y      = (*currChild)->getMarginTop();
                dimensions.width  = (*currChild)->getWidth();
                int const layoutH = (*currChild)->getHeight() + (*currChild)->getMarginTop() +
                                    ((*currChild)->getMarginBottom() > 0 ? (*currChild)->getMarginBottom() : 0);
                dimensions.height = (*currChild)->getHeight() + (layoutMaxH - layoutH);
                (*currChild)->setDimension(dimensions);
                dimensions.x += (*currChild)->getWidth() + (*currChild)->getMarginRight() + getHorizontalSpacing();
            }
            // remove last spacing
            dimensions.x -= getHorizontalSpacing();
            totalW = dimensions.x + diffW;
            totalH = std::max(layoutMaxH, childMaxH) + diffH;
        } else if (mLayout == LayoutPolicy::Circular && visibleChilds > 0) {
            float const angle  = 360.0F / visibleChilds;
            auto const xRadius = static_cast<float>((childMaxW * 2) + getHorizontalSpacing());
            auto const yRadius = static_cast<float>((childMaxH * 2) + getVerticalSpacing());
            currChild          = mChildren.begin();
            endChildren        = mChildren.end();
            int w              = 0;
            int h              = 0;
            int i              = 0;
            int minW           = 50000;
            int minH           = 50000;
            int maxW           = -50000;
            int maxH           = -50000;
            for (; currChild != endChildren; ++currChild) {
                if (!(*currChild)->isVisible()) {
                    continue;
                }
                float const tmpAngle =
                    static_cast<float>(static_cast<int>((angle * i) + 270) % 360) / (180.0F / Mathf::pi());
                int const x = static_cast<int>(
                    (xRadius * std::cos(tmpAngle)) - (static_cast<float>((*currChild)->getWidth()) / 2.0f));
                int const y = static_cast<int>(
                    (yRadius * std::sin(tmpAngle)) - (static_cast<float>((*currChild)->getHeight()) / 2.0f));
                minW = std::min(minW, x);
                maxW = std::max(maxW, x + (*currChild)->getWidth());
                minH = std::min(minH, y);
                maxH = std::max(maxH, y + (*currChild)->getHeight());

                (*currChild)->setPosition(x, y);
                ++i;
            }

            w = std::abs(minW) + std::abs(maxW);
            h = std::abs(minH) + std::abs(maxH);

            int const centerX = w / 2;
            int const centerY = h / 2;

            currChild   = mChildren.begin();
            endChildren = mChildren.end();
            for (; currChild != endChildren; ++currChild) {
                if (!(*currChild)->isVisible()) {
                    continue;
                }
                int const x = (*currChild)->getX() + centerX;
                int const y = (*currChild)->getY() + centerY;

                (*currChild)->setPosition(x, y);
            }

            totalW = w + diffW;
            totalH = h + diffH;
        }

        setSize(totalW, totalH);
    }

    void Container::adjustSize()
    {
        resizeToChildren();
        int const w = getWidth() + (2 * getBorderSize()) + getPaddingLeft() + getPaddingRight();
        int const h = getHeight() + (2 * getBorderSize()) + getPaddingTop() + getPaddingBottom();
        setSize(w, h);
    }

    // TODO(jakoch): This is a very complex method. It should be refactored.
    void Container::expandContent(bool recursion)
    {
        if (mLayout == LayoutPolicy::Absolute) {
            if (recursion) {
                std::list<Widget*>::const_iterator currChild(mChildren.begin());
                std::list<Widget*>::const_iterator const endChildren(mChildren.end());
                for (; currChild != endChildren; ++currChild) {
                    if (!(*currChild)->isVisible()) {
                        continue;
                    }
                    (*currChild)->expandContent(recursion);
                }
            }
            return;
        }

        if (mLayout == LayoutPolicy::AutoSize) {
            return;
        }

        Rectangle const childrenArea = getChildrenArea();
        int const spaceW             = childrenArea.width;
        int const spaceH             = childrenArea.height;
        int neededSpaceW             = 0;
        int neededSpaceH             = 0;
        int maxMinW                  = 0;
        int maxMinH                  = 0;
        int minMaxW                  = 50000;
        int minMaxH                  = 50000;
        int maxHExpander             = 0;
        int maxVExpander             = 0;
        int expanderNeededSpaceW     = 0;
        int expanderNeededSpaceH     = 0;
        unsigned int visibleChilds   = 0;
        std::list<Widget*> hExpander;
        std::list<Widget*> vExpander;

        std::list<Widget*>::const_iterator currChild(mChildren.begin());
        std::list<Widget*>::const_iterator endChildren(mChildren.end());
        for (; currChild != endChildren; ++currChild) {
            if (!(*currChild)->isVisible()) {
                continue;
            }
            ++visibleChilds;
            // get needed space
            neededSpaceW += (*currChild)->getWidth() + (*currChild)->getMarginLeft() + (*currChild)->getMarginRight() +
                            getHorizontalSpacing();
            neededSpaceH += (*currChild)->getHeight() + (*currChild)->getMarginTop() + (*currChild)->getMarginBottom() +
                            getVerticalSpacing();
            // get expander and expander max/min size
            if ((*currChild)->isVerticalExpand()) {
                maxVExpander = std::max(maxVExpander, (*currChild)->getHeight());
                maxMinH      = std::max(maxMinH, (*currChild)->getMinSize().getHeight());
                minMaxH      = std::min(minMaxH, (*currChild)->getMaxSize().getHeight());
                expanderNeededSpaceH += (*currChild)->getHeight() + getVerticalSpacing();
                vExpander.push_back((*currChild));
            }
            if ((*currChild)->isHorizontalExpand()) {
                maxHExpander = std::max(maxHExpander, (*currChild)->getWidth());
                maxMinW      = std::max(maxMinW, (*currChild)->getMinSize().getWidth());
                minMaxW      = std::min(minMaxW, (*currChild)->getMaxSize().getWidth());
                expanderNeededSpaceW += (*currChild)->getWidth() + getHorizontalSpacing();
                hExpander.push_back((*currChild));
            }
        }

        if (mLayout == LayoutPolicy::Vertical && visibleChilds > 0) {
            bool const hexpand = isHorizontalExpand() && (getParent() == nullptr);
            neededSpaceH -= getVerticalSpacing();
            int freeSpace = spaceH - neededSpaceH;
            if (freeSpace > 0) {
                if (!vExpander.empty()) {
                    expanderNeededSpaceH -= getVerticalSpacing();
                }
                if (mUniform) {
                    // check against the smallest maximal height
                    maxVExpander = std::min(minMaxH, maxVExpander);
                    // check against the largest minimal height
                    maxVExpander = std::max(maxMinH, maxVExpander);
                    int h        = 0;
                    // calculate maximal height if all expanders get this max height
                    int const maxNeeded =
                        ((maxVExpander + getVerticalSpacing()) * vExpander.size()) - getVerticalSpacing();
                    int const tmpSpace = (freeSpace + expanderNeededSpaceH) - maxNeeded;
                    if (tmpSpace > 0) {
                        h         = maxVExpander;
                        freeSpace = tmpSpace;
                    }
                    // distribute space
                    if (freeSpace > 0 || h > 0) {

                        auto it = vExpander.begin();

                        int expanders = vExpander.size();

                        for (; it != vExpander.end(); ++it) {
                            int const layoutH = (*it)->getHeight() + (*it)->getMarginTop() +
                                                ((*it)->getMarginBottom() > 0 ? (*it)->getMarginBottom() : 0);
                            // divide the space so that all expanders get the same size
                            int const diff = h > 0 ? 0 : (*it)->getHeight() + (maxVExpander - layoutH);
                            int delta      = ((freeSpace - diff) / expanders) + diff;
                            if (delta == 0) {
                                delta = 1;
                            }
                            delta          = std::min(delta, freeSpace);
                            int const oldH = h > 0 ? h : (*it)->getHeight();
                            int tmpH       = oldH + delta;
                            (*it)->setHeight(tmpH);
                            tmpH  = (*it)->getHeight();
                            delta = tmpH - oldH;
                            freeSpace -= delta;
                            --expanders;
                        }
                    }

                } else {
                    if (!vExpander.empty()) {
                        // simply add one to each expander until free space is empty
                        // or all expanders reached the max height
                        std::set<Widget*> maxExpanders;
                        while ((freeSpace != 0) && maxExpanders.size() < vExpander.size()) {
                            auto it = vExpander.begin();
                            for (; it != vExpander.end(); ++it) {
                                int const h = (*it)->getHeight();
                                (*it)->setHeight(h + 1);
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
                currChild   = mChildren.begin();
                endChildren = mChildren.end();
                for (; currChild != endChildren; ++currChild) {
                    if (!(*currChild)->isVisible()) {
                        continue;
                    }
                    if (hexpand || (*currChild)->isHorizontalExpand()) {
                        int const layoutW = (*currChild)->getMarginLeft() +
                                            ((*currChild)->getMarginRight() > 0 ? (*currChild)->getMarginRight() : 0);
                        rec.width = spaceW - layoutW;
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
        } else if (mLayout == LayoutPolicy::Horizontal && visibleChilds > 0) {
            bool const vexpand = isVerticalExpand() || (getParent() == nullptr);
            neededSpaceW -= getHorizontalSpacing();
            int freeSpace = spaceW - neededSpaceW;
            if (freeSpace > 0) {
                if (mUniform) {
                    if (!hExpander.empty()) {
                        expanderNeededSpaceW -= getHorizontalSpacing();
                    }
                    // check against the smallest maximal width
                    maxHExpander = std::min(minMaxW, maxHExpander);
                    // check against the largest minimal width
                    maxHExpander = std::max(maxMinW, maxHExpander);
                    int w        = 0;
                    // calculate maximal width if all expanders get this max width
                    int const maxNeeded =
                        ((maxHExpander + getHorizontalSpacing()) * hExpander.size()) - getHorizontalSpacing();
                    int const tmpSpace = (freeSpace + expanderNeededSpaceW) - maxNeeded;
                    if (tmpSpace > 0) {
                        w         = maxHExpander;
                        freeSpace = tmpSpace;
                    }
                    // distribute space
                    if (freeSpace > 0 || w > 0) {
                        auto it       = hExpander.begin();
                        int expanders = hExpander.size();
                        for (; it != hExpander.end(); ++it) {
                            // divide the space so that all expanders get the same size
                            int const layoutW = (*it)->getWidth() + (*it)->getMarginLeft() +
                                                ((*it)->getMarginRight() > 0 ? (*it)->getMarginRight() : 0);
                            int const diff = w > 0 ? 0 : (*it)->getWidth() + (maxHExpander - layoutW);
                            int delta      = ((freeSpace - diff) / expanders) + diff;
                            if (delta == 0) {
                                delta = 1;
                            }
                            delta          = std::min(delta, freeSpace);
                            int const oldW = w > 0 ? w : (*it)->getWidth();
                            int tmpW       = oldW + delta;
                            (*it)->setWidth(tmpW);
                            tmpW  = (*it)->getWidth();
                            delta = tmpW - oldW;
                            freeSpace -= delta;
                            --expanders;
                        }
                    }
                } else {
                    if (!hExpander.empty()) {
                        // simply add one to each expander until free space is empty
                        // or all expanders reached the max width
                        std::set<Widget*> maxExpanders;
                        while ((freeSpace != 0) && maxExpanders.size() < hExpander.size()) {
                            auto it = hExpander.begin();
                            for (; it != hExpander.end(); ++it) {
                                int const w = (*it)->getWidth();
                                (*it)->setWidth(w + 1);
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
                currChild   = mChildren.begin();
                endChildren = mChildren.end();
                for (; currChild != endChildren; ++currChild) {
                    if (!(*currChild)->isVisible()) {
                        continue;
                    }
                    if (vexpand || (*currChild)->isVerticalExpand()) {
                        int const layoutH = (*currChild)->getMarginTop() +
                                            ((*currChild)->getMarginBottom() > 0 ? (*currChild)->getMarginBottom() : 0);
                        rec.height = spaceH - layoutH;
                    } else {
                        rec.height = (*currChild)->getHeight();
                    }
                    rec.x += (*currChild)->getMarginLeft();
                    rec.y     = (*currChild)->getMarginTop();
                    rec.width = (*currChild)->getWidth();
                    (*currChild)->setDimension(rec);
                    rec.x += rec.width + (*currChild)->getMarginRight() + getHorizontalSpacing();
                }
            }
        } else if (mLayout == LayoutPolicy::Circular && visibleChilds > 0) {
            float const angle = 360.0F / visibleChilds;
            int childMaxW     = 0;
            int childMaxH     = 0;
            currChild         = mChildren.begin();
            endChildren       = mChildren.end();
            for (; currChild != endChildren; ++currChild) {
                if (!(*currChild)->isVisible()) {
                    continue;
                }
                childMaxW = std::max(childMaxW, (*currChild)->getWidth());
                childMaxH = std::max(childMaxH, (*currChild)->getHeight());
            }
            // childMaxW += getHorizontalSpacing();
            // childMaxH += getVerticalSpacing();
            float xRadius       = (spaceW - childMaxW) / 2.0F;
            float yRadius       = (spaceH - childMaxH) / 2.0F;
            float const centerX = spaceW / 2.0F;
            float const centerY = spaceH / 2.0F;
            if (xRadius < 1) {
                xRadius = static_cast<float>(childMaxW);
            }
            if (yRadius < 1) {
                yRadius = static_cast<float>(childMaxH);
            }
            // this forces a uniform circle
            // xRadius = std::max(xRadius, yRadius);
            // yRadius = xRadius;
            int i       = 0;
            currChild   = mChildren.begin();
            endChildren = mChildren.end();
            for (; currChild != endChildren; ++currChild) {
                if (!(*currChild)->isVisible()) {
                    continue;
                }
                float const tmpAngle =
                    static_cast<float>(static_cast<int>((angle * i) + 270) % 360) / (180.0F / Mathf::pi());
                int x = static_cast<int>(centerX + (xRadius * std::cos(tmpAngle)));
                int y = static_cast<int>(centerY + (yRadius * std::sin(tmpAngle)));
                x -= (*currChild)->getWidth() / 2;
                y -= (*currChild)->getHeight() / 2;

                (*currChild)->setPosition(x, y);
                ++i;
            }
        }

        if (recursion) {
            currChild   = mChildren.begin();
            endChildren = mChildren.end();
            for (; currChild != endChildren; ++currChild) {
                if (!(*currChild)->isVisible()) {
                    continue;
                }
                (*currChild)->expandContent(recursion);
            }
        }
    }

    void Container::setLayout(LayoutPolicy policy)
    {
        mLayout = policy;
    }

    Container::LayoutPolicy Container::getLayout() const
    {
        return mLayout;
    }

    void Container::setUniformSize(bool uniform)
    {
        mUniform = uniform;
    }

    bool Container::isUniformSize() const
    {
        return mUniform;
    }

    Rectangle Container::getChildrenArea()
    {
        Rectangle rec;
        rec.x      = getBorderSize() + getPaddingLeft();
        rec.y      = getBorderSize() + getPaddingTop();
        rec.width  = getWidth() - 2 * getBorderSize() - getPaddingLeft() - getPaddingRight();
        rec.height = getHeight() - 2 * getBorderSize() - getPaddingTop() - getPaddingBottom();
        return rec;
    }

    void Container::setVerticalSpacing(unsigned int spacing)
    {
        mVerticalSpacing = spacing;
    }

    unsigned int Container::getVerticalSpacing() const
    {
        return mVerticalSpacing;
    }

    void Container::setHorizontalSpacing(unsigned int spacing)
    {
        mHorizontalSpacing = spacing;
    }

    unsigned int Container::getHorizontalSpacing() const
    {
        return mHorizontalSpacing;
    }

    void Container::setBackgroundWidget(Widget* widget)
    {
        if (mBackgroundWidget == widget) {
            return;
        }

        mBackgroundWidget = widget;
    }

    Widget* Container::getBackgroundWidget()
    {
        return mBackgroundWidget;
    }
} // namespace fcn
