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

#include <fifechan/exception.hpp>
#include <fifechan/graphics.hpp>
#include <fifechan/widgets/piegraph.hpp>


namespace fcn {

    PieGraph::PieGraph():
        m_opaque(false),
        m_center(),
        m_radius(10) {
    }

    PieGraph::PieGraph(const Point& center):
        m_opaque(false),
        m_center(center),
        m_radius(10) {
    }

    void PieGraph::setCenterX(int x) {
        m_center.x = x;
    }

    void PieGraph::setCenterY(int y) {
        m_center.y = y;
    }

    void PieGraph::setCenter(int x, int y) {
        m_center.x = x;
        m_center.y = y;
    }

    int PieGraph::getCenterX() const {
        return m_center.x;
    }

    int PieGraph::getCenterY() const {
        return m_center.y;
    }

    void PieGraph::setCenter(const Point& center) {
        m_center = center;
    }

    const Point& PieGraph::getCenter() const {
        return m_center;
    }

    void PieGraph::setRadius(int radius) {
        m_radius = radius;
    }

    int PieGraph::getRadius() const {
        return m_radius;
    }

    void PieGraph::addSegment(int startAngle, int stopAngle, const Color& color) {
        PieGraphSegment segment;
        segment.startAngle = startAngle;
        segment.stopAngle = stopAngle;
        segment.color = color;
        m_segments.push_back(segment);
    }

    void PieGraph::clearSegments() {
        m_segments.clear();
    }

    void PieGraph::setOpaque(bool opaque) {
        m_opaque = opaque;
    }

    bool PieGraph::isOpaque() const {
        return m_opaque;
    }

    void PieGraph::draw(Graphics* graphics) {
        bool active = isFocused();

        if (isOpaque()) {
            // Fill the background around the content
            if (active && ((getSelectionMode() & Widget::Selection_Background) == Widget::Selection_Background)) {
                graphics->setColor(getSelectionColor());
            } else {
                graphics->setColor(getBackgroundColor());
            }
            graphics->fillRectangle(getBorderSize(), getBorderSize(),
                getWidth() - 2 * getBorderSize(), getHeight() - 2 * getBorderSize());
        }
        // draw border or frame
        if (getBorderSize() > 0) {
            if (active && (getSelectionMode() & Widget::Selection_Border) == Widget::Selection_Border) {
                drawSelectionFrame(graphics);
            } else {
                drawBorder(graphics);
            }
        }

        if (m_segments.empty() || m_radius < 1) {
            return;
        }
        // draw circle segments
        std::vector<PieGraphSegment>::iterator it = m_segments.begin();
        for (; it != m_segments.end(); ++it) {
            graphics->setColor((*it).color);
            graphics->drawFillCircleSegment(m_center, m_radius, (*it).startAngle, (*it).stopAngle);
        }
    }

};