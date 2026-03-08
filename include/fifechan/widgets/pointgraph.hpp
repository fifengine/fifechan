// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_POINTGRAPH_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_POINTGRAPH_HPP_

#include "fifechan/point.hpp"
#include "fifechan/widget.hpp"

namespace fcn
{
    class Graphics;

    /**
     * A widget that draws a graph of points.
     *
     * The graph is drawn by connecting the points with lines.
     * The thickness of the lines can be set with setThickness.
     * The graph can be set to be opaque or not with setOpaque.
     * If the graph is opaque, it will be drawn with a solid color,
     * otherwise it will be drawn with a transparent color.
     */
    class FIFEGUI_API PointGraph : public Widget
    {
    public:
        /**
         * Default constructor.
         */
        PointGraph();
        explicit PointGraph(PointVector data);

        ~PointGraph() override = default;

        /**
         * Sets the data to draw.
         *
         * @param data The data to draw.
         */
        void setPointVector(PointVector const & data);

        /**
         * @return The data to draw.
         */
        PointVector const & getPointVector() const;

        /**
         * Resets the data to draw.
         */
        void resetPointVector();

        /**
         * Set the thickness of the lines drawn between points.
         *
         * @param thickness Line thickness in pixels.
         */
        void setThickness(unsigned int thickness);

        /**
         * Get the thickness of the lines drawn between points.
         *
         * @return Line thickness in pixels.
         */
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

        /** Line thickness in pixels. */
        unsigned int m_thickness;

        /** The point data used to draw the graph. */
        PointVector m_data;
    };
}; // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_POINTGRAPH_HPP_
