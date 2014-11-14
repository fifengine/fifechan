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

#ifndef FCN_BARGRAPH_HPP
#define FCN_BARGRAPH_HPP

#include "fifechan/point.hpp"
#include "fifechan/widget.hpp"


namespace fcn
{
    class Graphics;

    class FCN_CORE_DECLSPEC BarGraph : public Widget {
    public:

        /**
         * Default constructor.
         */
        BarGraph();
        BarGraph(int x, int y, int w, int h);
        
        /**
         * Destructor.
         */
        virtual ~BarGraph() { };

        void setBarX(int x);
        int getBarX() const;
        void setBarY(int y);
        int getBarY() const;
        void setBarPosition(int x, int y);
        void setBarPosition(const Point& pos);
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
};

#endif //FCN_BARGRAPH_HPP