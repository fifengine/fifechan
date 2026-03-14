// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_BARGRAPH_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_BARGRAPH_HPP_

#include "fifechan/point.hpp"
#include "fifechan/widget.hpp"

namespace fcn
{
    class Graphics;

    /**
     * Displays data as a rectangular bar graph.
     *
     * The graph can be set to be opaque or not with setOpaque.
     * If the graph is opaque, it will be drawn with a solid color,
     * otherwise it will be drawn with a transparent color.
     *
     * @ingroup graphs
     */
    class FIFEGUI_API BarGraph : public Widget
    {
    public:
        BarGraph();

        /**
         * Constructor.
         */
        BarGraph(int x, int y, int w, int h);

        ~BarGraph() override = default;

        /**
         * Sets the x position of the bar.
         */
        void setBarX(int x);

        /**
         * @return The x position of the bar.
         */
        int getBarX() const;

        /**
         * Sets the y position of the bar.
         */
        void setBarY(int y);

        /**
         * @return The y position of the bar.
         */
        int getBarY() const;

        /**
         * Sets the position of the bar.
         */
        void setBarPosition(int x, int y);

        /**
         * Sets the position of the bar.
         * @param pos The position of the bar.
         */
        void setBarPosition(Point const & pos);

        /**
         * Sets the width of the bar.
         */
        void setBarWidth(int w);

        /**
         * @return The width of the bar.
         */
        int getBarWidth() const;

        /**
         * Sets the height of the bar.
         */

        void setBarHeight(int h);

        /**
         * @return The height of the bar.
         */
        int getBarHeight() const;

        /**
         * Sets the size of the bar.
         */
        void setBarSize(int w, int h);

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
        /**
         * m_opaque is true if the graph is opaque, false otherwise.
         */
        bool m_opaque{false};

        /**
         * m_rec is the rectangle that represents the bar.
         */
        Rectangle m_rec{};
    };
}; // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_BARGRAPH_HPP_
