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
     * A widget that displays a line graph based on a vector of points.
     */
    class FIFEGUI_API LineGraph : public Widget
    {
    public:
        LineGraph();

        explicit LineGraph(PointVector data);

        ~LineGraph() override = default;

        void setPointVector(PointVector const & data);
        PointVector const & getPointVector() const;
        void resetPointVector();

        void setThickness(unsigned int thickness);
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
        bool m_opaque;
        unsigned int m_thickness;
        PointVector m_data;
    };
}; // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_LINEGRAPH_HPP_
