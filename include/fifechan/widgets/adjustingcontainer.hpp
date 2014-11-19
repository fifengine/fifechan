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

/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2007 - 2008 Josh Matthews and Olof Naessén
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef FCN_ADJUSTINGCONTAINER_HPP
#define FCN_ADJUSTINGCONTAINER_HPP

#include "fifechan/widgets/container.hpp"

#include <vector>

namespace fcn
{
    /**
     * Self-adjusting Container class. AdjustingContainers are an easy way to
     * have Fifechan position a group of widgets for you. It organizes elements
     * in a table layout, with fixed columns and variable rows.  The user specifies
     *
     * @verbitam
     * <ul>
     *   <li>the number of columns</li>
     *   <li>horizontal spacing between columns</li>
     *   <li>vertical spacing between rows</li>
     *   <li>padding around the sides of the container</li>
     *   <li>each column's alignment</li>
     * </ul>
     * @endverbitam
     *
     * These properties give the user a lot of flexibility to make the
     * widgets look just right.
     * @code
     * AdjustingContainer *adjust = new AdjustingContainer;
     * adjust->setPadding(5, 5, 5, 5); //left, right, top, bottom
     * adjust->setHorizontalSpacing(3);
     * adjust->setVerticalSpacing(3);
     * adjust->setColumns(3);
     * adjust->setColumnAlignment(0, AdjustingContainer::LEFT);
     * adjust->setColumnAlignment(1, AdjustingContainer::CENTER);
     * adjust->setColumnAlignment(2, AdjustingContainer::RIGHT);
     * top->add(adjust);
     *
     * for(int j = 0; j < 9; j++)
     * {
     *   fcn::Label *l;
     *   int r = rand() % 3;
     *   if(r == 0)
     *     l = new fcn::Label("Short");
     *   else if(r == 1)
     *     l = new fcn::Label("A longer phrase");
     *   else
     *     l = new fcn::Label("Extravagent and wordy text");
     *   adjust->add(l);
     * @endcode
     *
     * Output:
     * @verbitam
     * <pre>
     *+---------------------------------------------------------------------------+
     *|                                                                           |
     *| A longer phrase              Short             Extravagent and wordy text |
     *|                                                                           |
     *| Short             Extravagent and wordy text                        Short |
     *|                                                                           |
     *| Short                   A longer phrase                   A longer phrase |
     *|                                                                           |
     *+---------------------------------------------------------------------------+
     * </pre>
     * @endverbitam
     * As you can see, each column is only as big as its largest element.
     * The AdjustingContainer will resize itself and rearrange its contents
     * based on whatever widgets it contains, allowing dynamic addition and
     * removal while the program is running.  It also plays nicely with ScrollAreas,
     * allowing you to show a fixed, maximum size while not limiting the actual
     * container.
     *
     * For more help with using AdjustingContainers, try the Guichan forums
     * (http://guichan.sourceforge.net/forum/) or email mrlachatte@gmail.com.
     *
     * @author Josh Matthews
     */
    class FCN_CORE_DECLSPEC AdjustingContainer : public Container
    {
    public:
        /**
         * Constructor.
         */
        AdjustingContainer();
        
        /**
         * Destructor.
         */
        virtual ~AdjustingContainer();
        
        /**
         * Set the number of columns to divide the widgets into.
         * The number of rows is derived automatically from the number
         * of widgets based on the number of columns.  Default column
         * alignment is left.
         *
         * @param numberOfColumns the number of columns.
         */
        virtual void setNumberOfColumns(unsigned int numberOfColumns);
        
        /**
         * Get the number of columns the widget is divided.
         * The number of rows is derived automatically from the number
         * of widgets based on the number of columns. 
         *
         * @return The number of columns.
         */
        virtual unsigned int getNumberOfColumns() const;

        /**
         * Set a specific column's alignment.
         *
         * @param column the column number, starting from 0.
         * @param alignment the column's alignment. See enum with alignments.
         */
        virtual void setColumnAlignment(unsigned int column, unsigned int alignment);
        
        /**
         * Get a specific column's alignment.
         *
         * @param column the column number, starting from 0.
         * @return The column's alignment. See enum with alignments.
         */
        virtual unsigned int getColumnAlignment(unsigned int column) const;

        /**
         * Rearrange the widgets and resize the container.
         */
        virtual void adjustContent();


        // Inherited from Container

        virtual void resizeToContent(bool recursiv=true);

        virtual void adjustSize();

        virtual void expandContent(bool recursiv=true);

        virtual Rectangle getChildrenArea();

        //virtual void logic();
        
        virtual void add(Widget *widget);

        virtual void add(Widget *widget, int x, int y);

        virtual void remove(Widget *widget);

        virtual void clear();
               
        /**
         * Possible alignment values for each column.
         *
         * LEFT   - Align content to the left of the column.
         * MIDDLE - Align content to the middle of the column.
         * RIGHT  - Align content to the right of the column.
         */
        enum
        {
            LEFT = 0,
            CENTER,
            RIGHT
        };

    protected:
        std::vector<Widget*> mContainedWidgets;
        std::vector<unsigned int> mColumnWidths;
        std::vector<unsigned int> mColumnAlignment;
        std::vector<unsigned int> mRowHeights;
        unsigned int mWidth;
        unsigned int mHeight;
        unsigned int mNumberOfColumns;
        unsigned int mNumberOfRows;
    };
}

#endif
