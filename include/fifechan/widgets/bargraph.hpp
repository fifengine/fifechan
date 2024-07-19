// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_BARGRAPH_HPP
#define FCN_BARGRAPH_HPP

#include "fifechan/point.hpp"
#include "fifechan/widget.hpp"

namespace fcn
{
    class Graphics;

    class FIFEGUI_API BarGraph : public Widget
    {
    public:
        /**
         * Default constructor.
         */
        BarGraph();
        BarGraph(int x, int y, int w, int h);

        /**
         * Destructor.
         */
        virtual ~BarGraph(){};

        void setBarX(int x);
        int getBarX() const;
        void setBarY(int y);
        int getBarY() const;
        void setBarPosition(int x, int y);
        void setBarPosition(Point const & pos);
        void setBarWidth(int w);
        int getBarWidth() const;
        void setBarHeight(int h);
        int getBarHeight() const;
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
        virtual void draw(Graphics* graphics);

    protected:
        bool m_opaque;
        Rectangle m_rec;
    };
}; // namespace fcn

#endif // FCN_BARGRAPH_HPP
