// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include <fifechan/exception.hpp>
#include <fifechan/graphics.hpp>
#include <fifechan/widgets/pointgraph.hpp>

namespace fcn
{

    PointGraph::PointGraph() : m_opaque(false), m_thickness(1), m_data() { }

    PointGraph::PointGraph(PointVector const & data) : m_opaque(false), m_thickness(1), m_data(data) { }

    void PointGraph::setPointVector(PointVector const & data)
    {
        m_data = data;
    }

    PointVector const & PointGraph::getPointVector() const
    {
        return m_data;
    }

    void PointGraph::resetPointVector()
    {
        m_data.clear();
    }

    void PointGraph::setThickness(unsigned int thickness)
    {
        m_thickness = thickness;
    }

    unsigned int PointGraph::getThickness() const
    {
        return m_thickness;
    }

    void PointGraph::setOpaque(bool opaque)
    {
        m_opaque = opaque;
    }

    bool PointGraph::isOpaque() const
    {
        return m_opaque;
    }

    void PointGraph::draw(Graphics* graphics)
    {
        bool active = isFocused();

        if (isOpaque()) {
            // Fill the background around the content
            if (active && ((getSelectionMode() & Widget::Selection_Background) == Widget::Selection_Background)) {
                graphics->setColor(getSelectionColor());
            } else {
                graphics->setColor(getBackgroundColor());
            }
            graphics->fillRectangle(
                getBorderSize(), getBorderSize(), getWidth() - 2 * getBorderSize(), getHeight() - 2 * getBorderSize());
        }
        // draw border or frame
        if (getBorderSize() > 0) {
            if (active && (getSelectionMode() & Widget::Selection_Border) == Widget::Selection_Border) {
                drawSelectionFrame(graphics);
            } else {
                drawBorder(graphics);
            }
        }

        if (m_data.empty()) {
            return;
        }
        // draw points
        graphics->setColor(getBaseColor());
        bool thick                = m_thickness > 1;
        PointVector::iterator pit = m_data.begin();
        if (thick) {
            for (; pit != m_data.end(); ++pit) {
                graphics->drawFillCircle(*pit, m_thickness);
            }
        } else {
            for (; pit != m_data.end(); ++pit) {
                graphics->drawPoint((*pit).x, (*pit).y);
            }
        }
    }

}; // namespace fcn
