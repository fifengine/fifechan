// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_PIEGRAPH_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_PIEGRAPH_HPP_

#include <vector>

#include "fifechan/point.hpp"
#include "fifechan/widget.hpp"

namespace fcn
{
    class Graphics;

    /**
     * Displays data as a circular pie chart.
     *
     * A pie graph is a circular graph that is divided into segments.
     * Each segment represents a portion of the whole graph, and the size of
     * each segment is determined by the start and stop angles of the segment.
     *
     * @ingroup graphs
     */
    class FIFEGUI_API PieGraph : public Widget
    {
    public:
        /**
         * Default constructor.
         */
        PieGraph();

        /**
         * Construct a PieGraph with an explicit center point.
         *
         * @param center The center point of the pie graph.
         */
        explicit PieGraph(Point center);

        ~PieGraph() override = default;

        PieGraph(PieGraph const &)            = delete;
        PieGraph& operator=(PieGraph const &) = delete;
        PieGraph(PieGraph&&)                  = delete;
        PieGraph& operator=(PieGraph&&)       = delete;

        /**
         * Set the center x-coordinate.
         *
         * @param x X coordinate for the center.
         */
        void setCenterX(int x);

        /**
         * Set the center y-coordinate.
         *
         * @param y Y coordinate for the center.
         */
        void setCenterY(int y);

        /**
         * Set the center coordinates.
         *
         * @param x X coordinate for the center.
         * @param y Y coordinate for the center.
         */
        void setCenter(int x, int y);

        /**
         * Get the center x-coordinate.
         *
         * @return The center X coordinate.
         */
        int getCenterX() const;

        /**
         * Get the center y-coordinate.
         *
         * @return The center Y coordinate.
         */
        int getCenterY() const;

        /**
         * Set the center using a Point.
         *
         * @param center The new center.
         */
        void setCenter(Point const & center);

        /**
         * Get the center point.
         *
         * @return The center point of the pie graph.
         */
        Point const & getCenter() const;

        /**
         * Set the radius of the pie graph.
         *
         * @param radius Radius in pixels.
         */
        void setRadius(int radius);

        /**
         * Get the radius of the pie graph.
         *
         * @return Radius in pixels.
         */
        int getRadius() const;

        /**
         * Add a segment to the pie graph.
         *
         * @param startAngle Start angle in degrees.
         * @param stopAngle Stop angle in degrees.
         * @param color Color to draw the segment with.
         */
        void addSegment(int startAngle, int stopAngle, Color const & color);

        /**
         * Remove all segments from the pie graph.
         */
        void clearSegments();

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
        /** True if the pie graph is drawn opaque. */
        bool m_opaque;

        /** The center point of the pie graph. */
        Point m_center;

        /** The radius of the pie graph in pixels. */
        int m_radius;

        /**
         * @ingroup graphs
         * Represents a single segment of a pie graph (angles and color).
         */
        struct PieGraphSegment
        {
            /** Start angle in degrees. */
            int startAngle{};

            /** Stop angle in degrees. */
            int stopAngle{};

            /** Color of the segment. */
            Color color;
        };

        /** The list of segments that make up the pie graph. */
        std::vector<PieGraphSegment> m_segments;
    };
}; // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_PIEGRAPH_HPP_
