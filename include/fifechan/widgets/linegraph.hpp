// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_LINEGRAPH_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_LINEGRAPH_HPP_

#include "fifechan/point.hpp"
#include "fifechan/widget.hpp"

namespace fcn
{
    class Graphics;

    /**
     * Displays data as a line graph connecting a vector of points.
     *
     * @ingroup graphs
     */
    class FIFEGUI_API LineGraph : public Widget
    {
    public:
        /** Default constructor. */
        LineGraph();

        /** Construct with initial data points. */
        explicit LineGraph(PointVector data);

        ~LineGraph() override = default;

        /** Set the raw point vector used to draw the graph. */
        void setPointVector(PointVector const & data);

        /** Get the current point vector. */
        PointVector const & getPointVector() const;

        /** Reset the stored data to an empty vector. */
        void resetPointVector();

        /** Set stroke thickness in pixels. */
        void setThickness(unsigned int thickness);

        /** Get stroke thickness in pixels. */
        unsigned int getThickness() const;

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
        void draw(Graphics* graphics) override;

    protected:
        /** True if the graph is drawn opaque. */
        bool m_opaque;

        /** Stroke thickness in pixels. */
        unsigned int m_thickness;

        /** The point data used to draw the graph. */
        PointVector m_data;
    };
}; // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_LINEGRAPH_HPP_
