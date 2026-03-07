// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_PIEGRAPH_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_PIEGRAPH_HPP_

#include <vector>

#include "fifechan/point.hpp"
#include "fifechan/widget.hpp"

namespace fcn
{
    class Graphics;

    /**
     * An implementation of a pie graph.
     *
     * A pie graph is a circular graph that is divided into segments.
     * Each segment represents a portion of the whole graph, and the size of
     * each segment is determined by the start and stop angles of the segment.
     */
    class FIFEGUI_API PieGraph : public Widget
    {
    public:
        PieGraph();

        explicit PieGraph(Point const & center);

        ~PieGraph() override = default;

        void setCenterX(int x);
        void setCenterY(int y);
        void setCenter(int x, int y);
        int getCenterX() const;
        int getCenterY() const;
        void setCenter(Point const & center);
        Point const & getCenter() const;

        void setRadius(int radius);
        int getRadius() const;

        void addSegment(int startAngle, int stopAngle, Color const & color);
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
        bool m_opaque;
        Point m_center;
        int m_radius;

        struct PieGraphSegment
        {
            int startAngle;
            int stopAngle;
            Color color;
        };

        std::vector<PieGraphSegment> m_segments;
    };
}; // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_PIEGRAPH_HPP_
