// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include <fifechan/exception.hpp>
#include <fifechan/graphics.hpp>
#include <fifechan/widgets/bargraph.hpp>

namespace fcn
{

    BarGraph::BarGraph() : m_opaque(false), m_rec() { }

    BarGraph::BarGraph(int x, int y, int w, int h) : m_opaque(false), m_rec(Rectangle(x, y, w, h)) { }

    void BarGraph::setBarX(int x)
    {
        m_rec.x = x;
    }

    int BarGraph::getBarX() const
    {
        return m_rec.x;
    }

    void BarGraph::setBarY(int y)
    {
        m_rec.y = y;
    }

    int BarGraph::getBarY() const
    {
        return m_rec.y;
    }

    void BarGraph::setBarPosition(int x, int y)
    {
        m_rec.x = x;
        m_rec.y = y;
    }

    void BarGraph::setBarPosition(Point const & pos)
    {
        m_rec.x = pos.x;
        m_rec.y = pos.y;
    }

    void BarGraph::setBarWidth(int w)
    {
        m_rec.width = w;
    }

    int BarGraph::getBarWidth() const
    {
        return m_rec.width;
    }

    void BarGraph::setBarHeight(int h)
    {
        m_rec.height = h;
    }

    int BarGraph::getBarHeight() const
    {
        return m_rec.height;
    }

    void BarGraph::setBarSize(int w, int h)
    {
        m_rec.width  = w;
        m_rec.height = h;
    }

    void BarGraph::setOpaque(bool opaque)
    {
        m_opaque = opaque;
    }

    bool BarGraph::isOpaque() const
    {
        return m_opaque;
    }

    void BarGraph::draw(Graphics* graphics)
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

        // draw bar
        graphics->setColor(getBaseColor());
        // top left, bottom left, bottom right, top right
        graphics->fillRectangle(m_rec);
    }

}; // namespace fcn
