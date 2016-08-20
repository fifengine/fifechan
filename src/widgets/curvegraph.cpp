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

#include <algorithm>

#include <fifechan/exception.hpp>
#include <fifechan/graphics.hpp>
#include <fifechan/util/fcn_math.hpp>
#include <fifechan/widgets/curvegraph.hpp>


namespace fcn {

    CurveGraph::CurveGraph():
        m_opaque(false),
        m_acp(true),
        m_needUpdate(false),
        m_thickness(1),
        m_data() {
    }

    CurveGraph::CurveGraph(const PointVector& data):
        m_opaque(false),
        m_acp(true),
        m_needUpdate(true),
        m_thickness(1),
        m_data(data) {
    }

    void CurveGraph::setPointVector(const PointVector& data) {
        m_needUpdate = true;
        m_data = data;
    }

    const PointVector& CurveGraph::getPointVector() const {
        return m_data;
    }

    void CurveGraph::resetPointVector() {
        m_needUpdate = true;
        m_data.clear();
    }

    void CurveGraph::setThickness(unsigned int thickness) {
        m_needUpdate = true;
        m_thickness = thickness;
    }

    unsigned int CurveGraph::getThickness() const {
        return m_thickness;
    }

    void CurveGraph::setAutomaticControllPoints(bool acp) {
        m_needUpdate = true;
        m_acp = acp;
    }

    bool CurveGraph::isAutomaticControllPoints() const {
        return m_acp;
    }

    void CurveGraph::setOpaque(bool opaque) {
        m_opaque = opaque;
    }

    bool CurveGraph::isOpaque() const {
        return m_opaque;
    }

    void CurveGraph::draw(Graphics* graphics) {
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

        if (m_needUpdate) {
            update();
        }
        if (m_curveData.empty()) {
            return;
        }
        // draw bezier curve
        graphics->setColor(getBaseColor());
        graphics->drawPolyLine(m_curveData, m_thickness);
    }

    void CurveGraph::update() {
        // calc steps and bezier points
        m_curveData.clear();
        if (m_data.size() < 2) {
            return;
        }
        float distance = 0;
        PointVector newPoints;
        if (m_acp) {
            addControlPoints(m_data, newPoints);
        } else {
            newPoints = m_data;
        }
        int elements = newPoints.size();

        std::vector<Point>::const_iterator it = newPoints.begin();
        Point old = *it;
        ++it;
        for (; it != newPoints.end(); ++it) {
            const Point& next = *it;
            float rx = old.x - next.x;
            float ry = old.y - next.y;
            old = next;
            distance += Mathf::Sqrt(rx*rx + ry*ry);
        }

        int32_t lines = ceil((distance / elements) / m_thickness);
        if (lines < 2) {
            lines = 2;
        }

        float step = 1.0 / static_cast<float>(lines-1);
        float t = 0.0;
        m_curveData.push_back(getBezierPoint(newPoints, newPoints.size()+1, t));
        for (int32_t i = 0; i <= (elements*lines); ++i) {
            t += step;
            m_curveData.push_back(getBezierPoint(newPoints, newPoints.size(), t));
        }
        m_needUpdate = false;
    }

    Point CurveGraph::getBezierPoint(const PointVector& points, int elements, float t) {
        if (t < 0.0) {
            return points[0];
        } else if (t >= static_cast<double>(elements)) {
            return points.back();
        }

        // Interpolate
        double px = 0.0;
        double py = 0.0;
        int n = elements - 1;
        double muk = 1.0;
        double mu = static_cast<double>(t) / static_cast<double>(elements);
        double munk = Mathd::Pow(1.0 - mu, static_cast<double>(n));
        for (int i = 0; i <= n; ++i) {
            int tmpn = n;
            int tmpi = i;
            int diffn = n - i;
            double blend = muk * munk;
            muk *= mu;
            munk /= 1.0 - mu;
            while (tmpn) {
                blend *= static_cast<double>(tmpn);
                tmpn--;
                if (tmpi > 1) {
                    blend /= static_cast<double>(tmpi);
                    tmpi--;
                }
                if (diffn > 1) {
                    blend /= static_cast<double>(diffn);
                    diffn--;
                }
            }
            px += static_cast<double>(points[i].x) * blend;
            py += static_cast<double>(points[i].y) * blend;
        }

        return Point(static_cast<int>(px), static_cast<int>(py));
    }

    void CurveGraph::addControlPoints(const PointVector& points, PointVector& newPoints) {
        if (points.empty()) {
            return;
        }

        int n = points.size() - 1;
        // min 2 points
        if (n < 1) {
            return;
        }

        Point p;
        // straight line
        if (n == 1) {
            newPoints.push_back(points[0]);
            p.x = (2 * points[0].x + points[1].x) / 3;
            p.y = (2 * points[0].y + points[1].y) / 3;
            newPoints.push_back(p);
            p.x = 2 * p.x - points[0].x;
            p.y = 2 * p.y - points[0].y;
            newPoints.push_back(p);
            newPoints.push_back(points[1]);
            return;
        }

        // calculate x and y values
        float xrhs[n];
        float yrhs[n];
        // first
        xrhs[0] = points[0].x + 2 * points[1].x;
        yrhs[0] = points[0].y + 2 * points[1].y;
        // last
        xrhs[n - 1] = (8 * points[n - 1].x + points[n].x) / 2.0;
        yrhs[n - 1] = (8 * points[n - 1].y + points[n].y) / 2.0;
        // rest
        for (int i = 1; i < n - 1; ++i) {
            xrhs[i] = 4 * points[i].x + 2 * points[i + 1].x;
            yrhs[i] = 4 * points[i].y + 2 * points[i + 1].y;
        }

        float x[n];
        float y[n];
        float xtmp[n];
        float ytmp[n];
        float xb = 2.0;
        float yb = 2.0;
        x[0] = xrhs[0] / xb;
        y[0] = yrhs[0] / yb;
        // Decomposition and forward substitution.
        for (int i = 1; i < n; i++) {
            xtmp[i] = 1 / xb;
            ytmp[i] = 1 / yb;
            xb = (i < n - 1 ? 4.0 : 3.5) - xtmp[i];
            yb = (i < n - 1 ? 4.0 : 3.5) - ytmp[i];
            x[i] = (xrhs[i] - x[i - 1]) / xb;
            y[i] = (yrhs[i] - y[i - 1]) / yb;
        }
        // Backward substitution
        for (int i = 1; i < n; i++) {
            x[n - i - 1] -= xtmp[n - i] * x[n - i];
            y[n - i - 1] -= ytmp[n - i] * y[n - i];
        }

        // start point
        newPoints.push_back(points[0]);
        for (int i = 0; i < n - 1; ++i) {
            p.x = x[i];
            p.y = y[i];
            newPoints.push_back(p);
            p.x = 2 * points[i + 1].x - x[i + 1];
            p.y = 2 * points[i + 1].y - y[i + 1];
            newPoints.push_back(p);

            newPoints.push_back(points[i+1]);
        }
        p.x = x[n - 1];
        p.y = y[n - 1];
        newPoints.push_back(p);
        p.x = (points[n].x + x[n - 1]) / 2;
        p.y = (points[n].y + y[n - 1]) / 2;
        newPoints.push_back(p);
        // end point
        newPoints.push_back(points[n]);
    }
};