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
#include <fifechan/widgets/pointgraph.hpp>


namespace fcn {

    PointGraph::PointGraph():
        m_opaque(false),
        m_thickness(1),
        m_color(0x000000),
        m_data() {
    }

    PointGraph::PointGraph(const PointVector& data):
        m_opaque(false),
        m_thickness(1),
        m_color(0x000000),
        m_data(data) {
    }

    PointGraph::PointGraph(const PointVector& data, const Color& color):
        m_opaque(false),
        m_thickness(1),
        m_color(color),
        m_data(data) {
    }

    void PointGraph::setPointVector(const PointVector& data) {
        m_data = data;
    }

    const PointVector& PointGraph::getPointVector() const {
        return m_data;
    }

    void PointGraph::resetPointVector() {
        m_data.clear();
    }

    void PointGraph::setColor(const Color& color) {
        m_color = color;
    }

    const Color& PointGraph::getColor() const {
        return m_color;
    }

    void PointGraph::setThickness(unsigned int thickness) {
        m_thickness = thickness;
    }

    unsigned int PointGraph::getThickness() const {
        return m_thickness;
    }

    void PointGraph::setOpaque(bool opaque) {
        m_opaque = opaque;
    }

    bool PointGraph::isOpaque() const {
        return m_opaque;
    }

    void PointGraph::draw(Graphics* graphics) {
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
        // draw points
        graphics->setColor(m_color);
        bool thick = m_thickness > 1;
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

};