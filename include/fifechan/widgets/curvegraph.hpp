// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_CURVEGRAPH_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_CURVEGRAPH_HPP_

// Project headers (subdirs before local)
#include "fifechan/point.hpp"
#include "fifechan/widget.hpp"

namespace fcn
{
    class Graphics;

    /**
     * Displays data as a smooth curve based on a vector of points.
     *
     * The graph can be set to be opaque or not with setOpaque.
     * If the graph is opaque, it will be drawn with a solid color,
     * otherwise it will be drawn with a transparent color.
     *
     * @ingroup graphs
     */
    class FIFEGUI_API CurveGraph : public Widget
    {
    public:
        /** Default constructor. */
        CurveGraph();

        /**
         * Construct with initial data points.
         *
         * @param data The initial vector of points to be used for drawing the curve.
         */
        explicit CurveGraph(PointVector data);

        ~CurveGraph() override = default;

        CurveGraph(CurveGraph const &)            = delete;
        CurveGraph& operator=(CurveGraph const &) = delete;
        CurveGraph(CurveGraph&&)                  = delete;
        CurveGraph& operator=(CurveGraph&&)       = delete;

        /**
         * Set the raw point vector used to draw the curve.
         *
         * @param data The vector of points to be used for drawing the curve.
         */
        void setPointVector(PointVector const & data);

        /** Get the current point vector. */
        PointVector const & getPointVector() const;

        /** Reset the stored data to an empty vector. */
        void resetPointVector();

        /**
         *  Set stroke thickness in pixels.
         *
         * @param thickness The thickness of the stroke in pixels.
         */
        void setThickness(unsigned int thickness);

        /** Get stroke thickness in pixels. */
        unsigned int getThickness() const;

        /**
         * Enable/disable automatic computation of bezier control points.
         *
         * @param acp True to enable automatic control points, false to disable.
         */
        void setAutomaticControlPoints(bool acp);

        /** Return whether automatic control points are enabled. */
        bool isAutomaticControlPoints() const;

        /**
         * Sets the opacity of the graph.
         *
         * @param opaque True if opaque, false otherwise.
         */
        void setOpaque(bool opaque);

        /**
         * @return Whether this graph is opaque or not.
         */
        bool isOpaque() const;

        /**
         * Draws this widget.
         *
         * @param graphics A graphics object to draw with.
         */
        void draw(Graphics* graphics) override;

    protected:
        /**
         * Precalculate bezier curve.
         */
        void update();

        /**
         * Helper that returns an interpolated Point
         *
         * @param points The control points for the bezier curve.
         * @param elements The number of control points.
         * @param t The interpolation parameter, between 0 and 1.
         */
        static Point getBezierPoint(PointVector const & points, int elements, float t);

        /**
         * Helper that adds the control points for bezier curves.
         *
         * @param points The control points for the bezier curve.
         * @param newPoints The vector to which the new control points will be added.
         */
        static void addControlPoints(PointVector const & points, PointVector& newPoints);

        /** True if the curve is drawn opaque. */
        bool m_opaque{false};

        /** Whether automatic control points are enabled. */
        bool m_acp{true};

        /** Internal flag marking that the precalculated curve data needs update. */
        bool m_needUpdate{false};

        /** Stroke thickness in pixels. */
        unsigned int m_thickness{1};

        /** Raw input point data. */
        PointVector m_data;

        /** Precalculated curve points (bezier/interpolated). */
        PointVector m_curveData;
    };
}; // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_CURVEGRAPH_HPP_
