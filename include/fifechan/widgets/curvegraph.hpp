// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_CURVEGRAPH_HPP
#define FCN_CURVEGRAPH_HPP

#include "fifechan/point.hpp"
#include "fifechan/widget.hpp"

namespace fcn
{
    class Graphics;

    class FCN_CORE_DECLSPEC CurveGraph : public Widget
    {
    public:
        /**
         * Default constructor.
         */
        CurveGraph();
        CurveGraph(PointVector const & data);

        /**
         * Destructor.
         */
        virtual ~CurveGraph(){};

        void setPointVector(PointVector const & data);
        PointVector const & getPointVector() const;
        void resetPointVector();

        void setThickness(unsigned int thickness);
        unsigned int getThickness() const;

        void setAutomaticControllPoints(bool acp);
        bool isAutomaticControllPoints() const;

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
         */
        virtual void draw(Graphics* graphics);

    protected:
        /** Precalculate bezier curve.
         */
        void update();

        /** Helper that returns an interpolated Point
         */
        Point getBezierPoint(PointVector const & points, int elements, float t);

        /** Helper that adds the control points for bezier curves.
         */
        void addControlPoints(PointVector const & points, PointVector& newPoints);

        bool m_opaque;
        bool m_acp;
        bool m_needUpdate;
        unsigned int m_thickness;
        PointVector m_data;
        PointVector m_curveData;
    };
}; // namespace fcn

#endif // FCN_CURVEGRAPH_HPP
