// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include <fifechan/widgets/piegraph.hpp>

#include <fifechan/exception.hpp>
#include <fifechan/graphics.hpp>

#include <utility>
#include <vector>

namespace fcn
{

    PieGraph::PieGraph() : m_opaque(false), m_radius(10) { }

    PieGraph::PieGraph(Point center) : m_opaque(false), m_center(std::move(center)), m_radius(10) { }

    void PieGraph::setCenterX(int x)
    {
        m_center.x = x;
    }

    void PieGraph::setCenterY(int y)
    {
        m_center.y = y;
    }

    void PieGraph::setCenter(int x, int y)
    {
        m_center.x = x;
        m_center.y = y;
    }

    int PieGraph::getCenterX() const
    {
        return m_center.x;
    }

    int PieGraph::getCenterY() const
    {
        return m_center.y;
    }

    void PieGraph::setCenter(Point const & center)
    {
        m_center = center;
    }

    Point const & PieGraph::getCenter() const
    {
        return m_center;
    }

    void PieGraph::setRadius(int radius)
    {
        m_radius = radius;
    }

    int PieGraph::getRadius() const
    {
        return m_radius;
    }

    void PieGraph::addSegment(int startAngle, int stopAngle, Color const & color)
    {
        PieGraphSegment segment;
        segment.startAngle = startAngle;
        segment.stopAngle  = stopAngle;
        segment.color      = color;
        m_segments.push_back(segment);
    }

    void PieGraph::clearSegments()
    {
        m_segments.clear();
    }

    void PieGraph::setOpaque(bool opaque)
    {
        m_opaque = opaque;
    }

    bool PieGraph::isOpaque() const
    {
        return m_opaque;
    }

    void PieGraph::draw(Graphics* graphics)
    {
        bool const active = isFocused();

        if (isOpaque()) {
            // Fill the background around the content
            if (active &&
                ((getSelectionMode() & Widget::SelectionMode::Background) == Widget::SelectionMode::Background)) {
                graphics->setColor(getSelectionColor());
            } else {
                graphics->setColor(getBackgroundColor());
            }
            graphics->fillRectangle(
                getBorderSize(),
                getBorderSize(),
                getWidth() - (2 * getBorderSize()),
                getHeight() - (2 * getBorderSize()));
        }
        // draw border or frame
        if (getBorderSize() > 0) {
            if (active && (getSelectionMode() & Widget::SelectionMode::Border) == Widget::SelectionMode::Border) {
                drawSelectionFrame(graphics);
            } else {
                drawBorder(graphics);
            }
        }

        if (m_segments.empty() || m_radius < 1) {
            return;
        }
        // draw circle segments
        auto it = m_segments.begin();
        for (; it != m_segments.end(); ++it) {
            graphics->setColor((*it).color);
            graphics->drawFillCircleSegment(m_center, m_radius, (*it).startAngle, (*it).stopAngle);
        }
    }

}; // namespace fcn
