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

/*
 * For comments regarding functions please see the header file.
 */

#include <algorithm>

#include "fifechan/widgets/flowcontainer.hpp"

#include "fifechan/exception.hpp"

namespace fcn {

    FlowContainer::FlowContainer():
        mAlignment(Center) {
        setOpaque(true);
    }

    FlowContainer::~FlowContainer() {
    }

    void FlowContainer::setAlignment(FlowContainer::Alignment alignment) {
        mAlignment = alignment;
    }

    FlowContainer::Alignment FlowContainer::getAlignment() const {
        return mAlignment;
    }

    void FlowContainer::adjustContent() {
        // diff means border, padding, ...
        int diffW = ABS(getDimension().width - getChildrenArea().width);
        int diffH = ABS(getDimension().height - getChildrenArea().height);

        int containerW = getChildrenArea().width;
        int containerH = getChildrenArea().height;

        // calculates max layout size per column or row
        std::vector<int> layoutMax;
        int tmpSize = 0;
        int x = 0;
        int y = 0;
        int visibleChilds = 0;
        std::list<Widget*>::const_iterator currChild(mChildren.begin());
        std::list<Widget*>::const_iterator endChildren(mChildren.end());
        for(; currChild != endChildren; ++currChild) {
            Widget* child = *currChild;
            if (!child->isVisible()) {
                continue;
            }
            ++visibleChilds;
            const Rectangle& rec = child->getDimension();
            if (mLayout == Vertical) {
                // new column
                // negative bottom margin have no effect here
                if (y + child->getMarginTop() + rec.height + (child->getMarginBottom() > 0 ? child->getMarginBottom() : 0) > containerH) {
                    y = 0;
                    layoutMax.push_back(tmpSize);
                    tmpSize = 0;
                }
                y += rec.height + child->getMarginTop() + child->getMarginBottom() + getVerticalSpacing();
                tmpSize = std::max(tmpSize, rec.width + child->getMarginLeft() + (child->getMarginRight() > 0 ? child->getMarginRight() : 0));
            } else {
                // new row
                // negative right margin have no effect here
                if (x + rec.width + child->getMarginLeft() + (child->getMarginRight() > 0 ? child->getMarginRight() : 0) > containerW) {
                    x = 0;
                    layoutMax.push_back(tmpSize);
                    tmpSize = 0;
                }
                x += rec.width + child->getMarginLeft() + child->getMarginRight() + getHorizontalSpacing();
                tmpSize = std::max(tmpSize, rec.height + child->getMarginTop() + (child->getMarginBottom() > 0 ? child->getMarginBottom() : 0));
            }
        }
        if (tmpSize != 0) {
            layoutMax.push_back(tmpSize);
        }

        // places all widgets
        x = 0;
        y = 0;
        int totalW = 0;
        int totalH = 0;
        unsigned int layout = 0;
        if (mLayout == Vertical && visibleChilds > 0) {
            currChild = mChildren.begin();
            endChildren = mChildren.end();
            for(; currChild != endChildren; ++currChild) {
                if (!(*currChild)->isVisible()) {
                    continue;
                }
                int columnW = layoutMax[layout];
                int layoutW = (*currChild)->getWidth() + (*currChild)->getMarginLeft() + ((*currChild)->getMarginRight() > 0 ? (*currChild)->getMarginRight() : 0);
                Rectangle dim((*currChild)->getMarginLeft(), (*currChild)->getMarginTop(), layoutW, (*currChild)->getHeight());

                // new column
                // negative bottom margin have no effect here
                if (y + (*currChild)->getMarginTop() + dim.height + ((*currChild)->getMarginBottom() > 0 ? (*currChild)->getMarginBottom() : 0) > containerH) {
                    x += columnW + getHorizontalSpacing();
                    y = 0;
                    ++layout;
                    columnW = layoutMax[layout];
                }
                dim.y += y;

                switch (getAlignment()) {
                  case Left:
                      dim.x += x;
                      break;
                  case Center:
                      dim.x += x + (columnW - layoutW) / 2;
                      break;
                  case Right:
                      dim.x += x + (columnW - layoutW);
                      break;
                  default:
                      throw FCN_EXCEPTION("Unknown alignment.");
                }

                (*currChild)->setDimension(dim);
                y += (*currChild)->getHeight() + (*currChild)->getMarginTop() + (*currChild)->getMarginBottom() + getVerticalSpacing();
                totalW = std::max(totalW, dim.x + dim.width);
                totalH = std::max(totalH, y);
            }
            // remove last spacing
            totalH -= getVerticalSpacing();
            // always expand height but width only if horizontal expand is enabled
            totalH = std::max(totalH, containerH);
            if (isHorizontalExpand()) {
                totalW = std::max(totalW, containerW);
            }
        } else if (mLayout == Horizontal && visibleChilds > 0) {
            currChild = mChildren.begin();
            endChildren = mChildren.end();
            for(; currChild != endChildren; ++currChild) {
                if (!(*currChild)->isVisible()) {
                    continue;
                }
                int rowH = layoutMax[layout];
                int layoutH = (*currChild)->getHeight() + (*currChild)->getMarginTop() + ((*currChild)->getMarginBottom() > 0 ? (*currChild)->getMarginBottom() : 0);
                Rectangle dim((*currChild)->getMarginLeft(), (*currChild)->getMarginTop(), (*currChild)->getWidth(), layoutH);

                // new row
                // negative right margin have no effect here
                if (x + (*currChild)->getMarginLeft() + dim.width + ((*currChild)->getMarginRight() > 0 ? (*currChild)->getMarginRight() : 0) > containerW) {
                    x = 0;
                    y += rowH + getVerticalSpacing();
                    ++layout;
                    rowH = layoutMax[layout];
                }
                dim.x += x;

                switch (getAlignment()) {
                  case Top:
                      dim.y += y;
                      break;
                  case Center:
                      dim.y += y + (rowH - layoutH) / 2;
                      break;
                  case Bottom:
                      dim.y += y + (rowH - layoutH);
                      break;
                  default:
                      throw FCN_EXCEPTION("Unknown alignment.");
                }

                (*currChild)->setDimension(dim);
                x += (*currChild)->getWidth() + (*currChild)->getMarginLeft() + (*currChild)->getMarginRight() + getHorizontalSpacing();
                totalW = std::max(totalW, x);
                totalH = std::max(totalH, dim.y + dim.height);

            }
            // remove last spacing
            totalW -= getHorizontalSpacing();
            // always expand width but height only if vertical expand is enabled
            totalW = std::max(totalW, containerW);
            if (isVerticalExpand()) {
                totalH = std::max(totalH, containerH);
            }
        }

        // resize container
        totalW += diffW;
        totalH += diffH;
        setSize(totalW, totalH);
    }

    void FlowContainer::setLayout(Container::LayoutPolicy policy) {
        if (policy == Circular) {
            throw FCN_EXCEPTION("Circular layout is not implemented for the FlowContainer.");
        } else {
            Container::setLayout(policy);
        }
    }

    void FlowContainer::resizeToContent(bool recursiv) {
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

        if (mLayout != Absolute) {
            if (getParent()) {
                setSize(getMinSize().getWidth(), getMinSize().getHeight());		
            }
        }
    }

    void FlowContainer::expandContent(bool recursiv) {
        if (mLayout != Absolute) {
            adjustContent();
        }
        // not really needed
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
    }

}
