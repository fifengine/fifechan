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

#ifndef FCN_CURVEGRAPH_HPP
#define FCN_CURVEGRAPH_HPP

#include <fifechan/point.hpp>
#include <fifechan/widget.hpp>


namespace fcn
{
    class Graphics;

    class FCN_CORE_DECLSPEC CurveGraph : public Widget {
    public:

        /**
         * Default constructor.
         */
        CurveGraph();
        CurveGraph(const PointVector& data);
        CurveGraph(const PointVector& data, const Color& color);
        
        /**
         * Destructor.
         */
        virtual ~CurveGraph() { };

        void setPointVector(const PointVector& data);
        const PointVector& getPointVector() const;
        void resetPointVector();

        void setColor(const Color& color);
        const Color& getColor() const;

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
        Point getBezierPoint(const PointVector& points, int elements, float t);

        /** Helper that adds the control points for bezier curves.
         */
        void addControlPoints(const PointVector& points, PointVector& newPoints);

        bool m_opaque;
        bool m_acp;
        bool m_needUpdate;
        unsigned int m_thickness;
        Color m_color;
        PointVector m_data;
        PointVector m_curveData;
    };
};

#endif //FCN_CURVEGRAPH_HPP