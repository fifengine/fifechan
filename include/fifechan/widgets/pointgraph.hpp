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

#ifndef FCN_POINTGRAPH_HPP
#define FCN_POINTGRAPH_HPP

#include "fifechan/point.hpp"
#include "fifechan/widget.hpp"


namespace fcn
{
    class Graphics;

    class FCN_CORE_DECLSPEC PointGraph : public Widget {
    public:

        /**
         * Default constructor.
         */
        PointGraph();
        PointGraph(const PointVector& data);
        
        /**
         * Destructor.
         */
        virtual ~PointGraph() { };

        void setPointVector(const PointVector& data);
        const PointVector& getPointVector() const;
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
        virtual void draw(Graphics* graphics);

    protected:
        bool m_opaque;
        unsigned int m_thickness;
        PointVector m_data;
    };
};

#endif //FCN_POINTGRAPH_HPP