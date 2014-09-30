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
#include <fifechan/widgets/bargraph.hpp>


namespace fcn {

    BarGraph::BarGraph():
        m_opaque(false),
        m_color(0x000000),
        m_rec() {
    }

    BarGraph::BarGraph(int x, int y, int w, int h):
        m_opaque(false),
        m_color(0x000000),
        m_rec(Rectangle(x, y, w, h)) {
    }

    BarGraph::BarGraph(int x, int y, int w, int h, const Color& color):
        m_opaque(false),
        m_color(color),
        m_rec(Rectangle(x, y, w, h)) {
    }

    void BarGraph::setBarX(int x) {
        m_rec.x = x;
    }

    int BarGraph::getBarX() const {
        return m_rec.x;
    }

    void BarGraph::setBarY(int y) {
        m_rec.y = y;
    }

    int BarGraph::getBarY() const {
        return m_rec.y;
    }

    void BarGraph::setBarPosition(int x, int y) {
        m_rec.x = x;
        m_rec.y = y;
    }

    void BarGraph::setBarPosition(const Point& pos) {
        m_rec.x = pos.x;
        m_rec.y = pos.y;
    }

    void BarGraph::setBarWidth(int w) {
        m_rec.width = w;
    }

    int BarGraph::getBarWidth() const {
        return m_rec.width;
    }

    void BarGraph::setBarHeight(int h) {
        m_rec.height = h;
    }

    int BarGraph::getBarHeight() const {
        return m_rec.height;
    }

    void BarGraph::setBarSize(int w, int h) {
        m_rec.width = w;
        m_rec.height = h;
    }

    void BarGraph::setColor(const Color& color) {
        m_color = color;
    }

    const Color& BarGraph::getColor() const {
        return m_color;
    }

    void BarGraph::setOpaque(bool opaque) {
        m_opaque = opaque;
    }

    bool BarGraph::isOpaque() const {
        return m_opaque;
    }

    void BarGraph::draw(Graphics* graphics) {
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

        // draw bar
        graphics->setColor(m_color);
        // top left, bottom left, bottom right, top right
        graphics->fillRectangle(m_rec);
    }

};