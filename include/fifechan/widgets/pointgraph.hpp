// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_POINTGRAPH_HPP
#define FCN_POINTGRAPH_HPP

#include "fifechan/point.hpp"
#include "fifechan/widget.hpp"

namespace fcn
{
    class Graphics;

    class FIFEGUI_API PointGraph : public Widget
    {
    public:
        /**
         * Default constructor.
         */
        PointGraph();
        PointGraph(PointVector const & data);

        /**
         * Destructor.
         */
        virtual ~PointGraph(){};

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
        virtual void draw(Graphics* graphics);

    protected:
        bool m_opaque;
        unsigned int m_thickness;
        PointVector m_data;
    };
}; // namespace fcn

#endif // FCN_POINTGRAPH_HPP
