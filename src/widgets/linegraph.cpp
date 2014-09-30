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
#include <fifechan/widgets/linegraph.hpp>


namespace fcn {

    LineGraph::LineGraph():
        m_opaque(false),
        m_thickness(1),
        m_color(0x000000),
        m_data() {
    }

    LineGraph::LineGraph(const PointVector& data):
        m_opaque(false),
        m_thickness(1),
        m_color(0x000000),
        m_data(data) {
    }

    LineGraph::LineGraph(const PointVector& data, const Color& color):
        m_opaque(false),
        m_thickness(1),
        m_color(color),
        m_data(data) {
    }

    void LineGraph::setPointVector(const PointVector& data) {
        m_data = data;
    }

    const PointVector& LineGraph::getPointVector() const {
        return m_data;
    }

    void LineGraph::resetPointVector() {
        m_data.clear();
    }

    void LineGraph::setColor(const Color& color) {
        m_color = color;
    }

    const Color& LineGraph::getColor() const {
        return m_color;
    }

    void LineGraph::setThickness(unsigned int thickness) {
        m_thickness = thickness;
    }

    unsigned int LineGraph::getThickness() const {
        return m_thickness;
    }

    void LineGraph::setOpaque(bool opaque) {
        m_opaque = opaque;
    }

    bool LineGraph::isOpaque() const {
        return m_opaque;
    }

    void LineGraph::draw(Graphics* graphics) {
        const Color &faceColor = getBaseColor();

        if (isOpaque()) {
            // Fill the background around the content
            graphics->setColor(faceColor);
            graphics->fillRectangle(getBorderSize(), getBorderSize(),
                getWidth() - 2 * getBorderSize(), getHeight() - 2 * getBorderSize());
        }
        // draw border
        if (getBorderSize() > 0) {
            drawBorder(graphics);
        }

        if (m_data.empty()) {
            return;
        }
        // draw lines
        graphics->setColor(m_color);
        bool thick = m_thickness > 1;
        PointVector::iterator pit = m_data.begin();
        int x1 = (*pit).x;
        int y1 = (*pit).y;
        ++pit;
        if (thick) {
            for (; pit != m_data.end(); ++pit) {
                int x2 = (*pit).x;
                int y2 = (*pit).y;
                graphics->drawLine(x1, y1, x2, y2, m_thickness);
                x1 = x2;
                y1 = y2;
            }
        } else {
            for (; pit != m_data.end(); ++pit) {
                int x2 = (*pit).x;
                int y2 = (*pit).y;
                graphics->drawLine(x1, y1, x2, y2);
                x1 = x2;
                y1 = y2;
            }
        }
    }

};