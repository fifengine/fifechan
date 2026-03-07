// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_ADJUSTINGCONTAINER_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_ADJUSTINGCONTAINER_HPP_

#include <vector>

#include "fifechan/widgets/container.hpp"

namespace fcn
{
    /**
     * Self-adjusting Container class.
     * AdjustingContainers are an easy way to have the GUI position a group
     * of widgets for you. It organizes elements in a table layout
     * with fixed columns and variable rows.
     *
     * The user specifies
     *
     * @verbatim
     * <ul>
     *   <li>the number of columns</li>
     *   <li>horizontal spacing between columns</li>
     *   <li>vertical spacing between rows</li>
     *   <li>padding around the sides of the container</li>
     *   <li>each column's alignment</li>
     * </ul>
     * @endverbatim
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
     *     l = new fcn::Label("Super long and wordy text");
     *   adjust->add(l);
     * @endcode
     *
     * Output:
     * @verbatim
     * <pre>
     *+---------------------------------------------------------------------------+
     *|                                                                           |
     *| A longer phrase              Short             Super long and wordy text |
     *|                                                                           |
     *| Short             Super long and wordy text                        Short |
     *|                                                                           |
     *| Short                   A longer phrase                   A longer phrase |
     *|                                                                           |
     *+---------------------------------------------------------------------------+
     * </pre>
     * @endverbatim
     *
     * As you can see, each column is only as big as its largest element.
     * The AdjustingContainer will resize itself and rearrange its contents
     * based on whatever widgets it contains, allowing dynamic addition and
     * removal while the program is running.  It also plays nicely with ScrollAreas,
     * allowing you to show a fixed, maximum size while not limiting the actual
     * container.
     *

     *
     */
    class FIFEGUI_API AdjustingContainer : public Container
    {
    public:
        /**
         * Possible alignment values for each column.
         *
         * LEFT   - Align content to the left of the column.
         * MIDDLE - Align content to the middle of the column.
         * RIGHT  - Align content to the right of the column.
         */
        enum class Alignment : uint8_t
        {
            Left = 0,
            Center,
            Right
        };

        AdjustingContainer();

        ~AdjustingContainer() override;

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
        virtual void setColumnAlignment(unsigned int column, Alignment alignment);

        /**
         * Get a specific column's alignment.
         *
         * @param column the column number, starting from 0.
         * @return The column's alignment. See enum with alignments.
         */
        Alignment getColumnAlignment(unsigned int column) const;

        /**
         * Rearrange the widgets and resize the container.
         */
        virtual void adjustContent();

        // Inherited from Container

        using Container::resizeToContent;

        void resizeToContent(bool recursion) override;

        void adjustSize() override;

        using Container::expandContent;

        void expandContent(bool recursion) override;

        Rectangle getChildrenArea() override;

        // virtual void logic();

        void add(Widget* widget) override;

        void add(Widget* widget, int x, int y) override;

        void remove(Widget* widget) override;

        void clear() override;

    protected:
        std::vector<Widget*> mContainedWidgets;
        std::vector<unsigned int> mColumnWidths;
        /**
         * The alignment of each column.
         */
        std::vector<Alignment> mColumnAlignment;
        std::vector<unsigned int> mRowHeights;
        unsigned int mWidth;
        unsigned int mHeight;
        unsigned int mNumberOfColumns;
        unsigned int mNumberOfRows;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_ADJUSTINGCONTAINER_HPP_
