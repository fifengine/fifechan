// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include <fifechan/widgets/curvegraph.hpp>

// Standard library includes
#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>

// Project headers (subdirs before local)
#include "fifechan/exception.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/math.hpp"

namespace fcn
{

    CurveGraph::CurveGraph() = default;

    CurveGraph::CurveGraph(PointVector data) : m_data(std::move(data))
    {
    }

    void CurveGraph::setPointVector(PointVector const & data)
    {
        m_needUpdate = true;
        m_data       = data;
    }

    PointVector const & CurveGraph::getPointVector() const
    {
        return m_data;
    }

    void CurveGraph::resetPointVector()
    {
        m_needUpdate = true;
        m_data.clear();
    }

    void CurveGraph::setThickness(unsigned int thickness)
    {
        m_needUpdate = true;
        m_thickness  = thickness;
    }

    unsigned int CurveGraph::getThickness() const
    {
        return m_thickness;
    }

    void CurveGraph::setAutomaticControlPoints(bool acp)
    {
        m_needUpdate = true;
        m_acp        = acp;
    }

    bool CurveGraph::isAutomaticControlPoints() const
    {
        return m_acp;
    }

    void CurveGraph::setOpaque(bool opaque)
    {
        m_opaque = opaque;
    }

    bool CurveGraph::isOpaque() const
    {
        return m_opaque;
    }

    void CurveGraph::draw(Graphics* graphics)
    {
        assert("graphics must not be null" && graphics != nullptr);
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

        if (m_needUpdate) {
            update();
        }
        if (m_curveData.empty()) {
            return;
        }
        // draw bezier curve
        graphics->setColor(getBaseColor());

        if (m_thickness <= 1 || m_curveData.size() < 2) {
            graphics->drawPolyLine(m_curveData, m_thickness);
            return;
        }

        for (size_t i = 0; i < m_curveData.size() - 1; ++i) {
            Point const & start = m_curveData.at(i);
            Point const & end   = m_curveData.at(i + 1);
            graphics->drawRoundStroke(start.x, start.y, end.x, end.y, m_thickness);
        }
    }

    void CurveGraph::update()
    {
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
        int const elements = static_cast<int>(newPoints.size());

        auto it   = newPoints.begin();
        Point old = *it;
        ++it;
        for (; it != newPoints.end(); ++it) {
            Point const & next = *it;
            auto const rx      = static_cast<float>(old.x - next.x);
            auto const ry      = static_cast<float>(old.y - next.y);
            old                = next;
            distance += Mathf::Sqrt((rx * rx) + (ry * ry));
        }

        int lines =
            static_cast<int>(std::ceil((distance / static_cast<float>(elements)) / static_cast<float>(m_thickness)));

        lines = std::max(lines, 2);

        float const step = 1.0F / static_cast<float>(lines - 1);
        float t          = 0.0F;
        m_curveData.push_back(getBezierPoint(newPoints, elements, t));
        for (int i = 0; i <= (elements * lines); ++i) {
            t += step;
            m_curveData.push_back(getBezierPoint(newPoints, elements, t));
        }
        m_needUpdate = false;
    }

    Point CurveGraph::getBezierPoint(PointVector const & points, int elements, float t)
    {
        if (t < 0.0F) {
            return points.at(0);
        }

        if (t >= static_cast<float>(elements)) {
            return points.back();
        }

        // Interpolate
        double px       = 0.0;
        double py       = 0.0;
        int const n     = elements - 1;
        double muk      = 1.0;
        double const mu = static_cast<double>(t) / static_cast<double>(elements);
        double munk     = Mathd::Pow(1.0 - mu, static_cast<double>(n));

        for (int i = 0; i <= n; ++i) {
            int tmpn     = n;
            int tmpi     = i;
            int diffn    = n - i;
            double blend = muk * munk;
            muk *= mu;
            munk /= 1.0 - mu;
            while (tmpn != 0) {
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
            px += static_cast<double>(points.at(static_cast<size_t>(i)).x) * blend;
            py += static_cast<double>(points.at(static_cast<size_t>(i)).y) * blend;
        }

        return Point(static_cast<int>(px), static_cast<int>(py));
    }

    void CurveGraph::addControlPoints(PointVector const & points, PointVector& newPoints)
    {
        if (points.empty()) {
            return;
        }

        int const n = static_cast<int>(points.size()) - 1;

        // min 2 points
        if (n < 1) {
            return;
        }

        Point p;
        // straight line
        if (n == 1) {
            newPoints.push_back(points.at(0));
            p.x = ((2 * points.at(0).x) + points.at(1).x) / 3;
            p.y = ((2 * points.at(0).y) + points.at(1).y) / 3;
            newPoints.push_back(p);
            p.x = (2 * p.x) - points.at(0).x;
            p.y = (2 * p.y) - points.at(0).y;
            newPoints.push_back(p);
            newPoints.push_back(points.at(1));
            return;
        }

        // calculate x and y values
        std::vector<float> xrhs(static_cast<size_t>(n));
        std::vector<float> yrhs(static_cast<size_t>(n));
        // first
        xrhs.at(0) = static_cast<float>(points.at(0).x) + (2.0F * static_cast<float>(points.at(1).x));
        yrhs.at(0) = static_cast<float>(points.at(0).y) + (2.0F * static_cast<float>(points.at(1).y));
        // last
        xrhs.at(static_cast<size_t>(n) - 1) = ((8.0F * static_cast<float>(points.at(static_cast<size_t>(n) - 1).x)) +
                                               static_cast<float>(points.at(static_cast<size_t>(n)).x)) /
                                              2.0F;
        yrhs.at(static_cast<size_t>(n) - 1) = ((8.0F * static_cast<float>(points.at(static_cast<size_t>(n) - 1).y)) +
                                               static_cast<float>(points.at(static_cast<size_t>(n)).y)) /
                                              2.0F;
        // rest
        for (int i = 1; i < n - 1; ++i) {
            xrhs.at(static_cast<size_t>(i)) = (4.0F * static_cast<float>(points.at(static_cast<size_t>(i)).x)) +
                                              (2.0F * static_cast<float>(points.at(static_cast<size_t>(i) + 1).x));
            yrhs.at(static_cast<size_t>(i)) = (4.0F * static_cast<float>(points.at(static_cast<size_t>(i)).y)) +
                                              (2.0F * static_cast<float>(points.at(static_cast<size_t>(i) + 1).y));
        }

        std::vector<float> x(static_cast<size_t>(n));
        std::vector<float> y(static_cast<size_t>(n));
        std::vector<float> xtmp(static_cast<size_t>(n));
        std::vector<float> ytmp(static_cast<size_t>(n));
        float xb = 2.0;
        float yb = 2.0;
        x.at(0)  = xrhs.at(0) / xb;
        y.at(0)  = yrhs.at(0) / yb;
        // Decomposition and forward substitution.
        for (int i = 1; i < n; i++) {
            size_t const idx = static_cast<size_t>(i);
            xtmp.at(idx)     = 1 / xb;
            ytmp.at(idx)     = 1 / yb;
            xb               = (i < n - 1 ? 4.0F : 3.5F) - xtmp.at(idx);
            yb               = (i < n - 1 ? 4.0F : 3.5F) - ytmp.at(idx);
            x.at(idx)        = (xrhs.at(idx) - x.at(idx - 1)) / xb;
            y.at(idx)        = (yrhs.at(idx) - y.at(idx - 1)) / yb;
        }
        // Backward substitution
        for (int i = 1; i < n; i++) {
            size_t const idx          = static_cast<size_t>(i);
            size_t const revIdx       = static_cast<size_t>(n) - idx;
            size_t const revIdxMinus1 = revIdx - 1;
            x.at(revIdxMinus1) -= xtmp.at(revIdx) * x.at(revIdx);
            y.at(revIdxMinus1) -= ytmp.at(revIdx) * y.at(revIdx);
        }

        // start point
        newPoints.push_back(points.at(0));
        for (int i = 0; i < n - 1; ++i) {
            size_t const idx = static_cast<size_t>(i);
            p.x              = static_cast<int>(x.at(idx));
            p.y              = static_cast<int>(y.at(idx));
            newPoints.push_back(p);
            p.x = static_cast<int>(static_cast<float>(2 * points.at(idx + 1).x) - x.at(idx + 1));
            p.y = static_cast<int>(static_cast<float>(2 * points.at(idx + 1).y) - y.at(idx + 1));
            newPoints.push_back(p);

            newPoints.push_back(points.at(idx + 1));
        }
        p.x = static_cast<int>(x.at(static_cast<size_t>(n) - 1));
        p.y = static_cast<int>(y.at(static_cast<size_t>(n) - 1));
        newPoints.push_back(p);
        p.x = static_cast<int>(
            (static_cast<float>(points.at(static_cast<size_t>(n)).x) + x.at(static_cast<size_t>(n) - 1)) / 2);
        p.y = static_cast<int>(
            (static_cast<float>(points.at(static_cast<size_t>(n)).y) + y.at(static_cast<size_t>(n) - 1)) / 2);
        newPoints.push_back(p);
        // end point
        newPoints.push_back(points.at(static_cast<size_t>(n)));
    }
}; // namespace fcn
