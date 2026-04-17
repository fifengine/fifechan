// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/widgets/adjustingcontainer.hpp"

// Standard library includes
#include <algorithm>
#include <numeric>
#include <utility>

// Project headers (subdirs before local)
#include "fifechan/exception.hpp"

namespace fcn
{
    AdjustingContainer::AdjustingContainer()
    {
        setPadding(0);
        setVerticalSpacing(0);
        setHorizontalSpacing(0);
        mColumnWidths.push_back(0);
        mRowHeights.push_back(0);
    }

    AdjustingContainer::~AdjustingContainer() = default;

    void AdjustingContainer::setNumberOfColumns(unsigned int numberOfColumns)
    {
        mNumberOfColumns = numberOfColumns;

        if (mColumnAlignment.size() < numberOfColumns) {
            while (mColumnAlignment.size() < numberOfColumns) {
                mColumnAlignment.push_back(Alignment::Left);
            }
        } else {
            while (mColumnAlignment.size() > numberOfColumns) {
                mColumnAlignment.pop_back();
            }
        }
    }

    unsigned int AdjustingContainer::getNumberOfColumns() const
    {
        return mNumberOfColumns;
    }

    void AdjustingContainer::setColumnAlignment(unsigned int column, Alignment alignment)
    {
        if (column < mColumnAlignment.size()) {
            mColumnAlignment[column] = alignment;
        }
    }

    AdjustingContainer::Alignment AdjustingContainer::getColumnAlignment(unsigned int column) const
    {
        if (column < mColumnAlignment.size()) {
            return mColumnAlignment[column];
        }
        return Alignment::Left;
    }

    void AdjustingContainer::resizeToContent(bool recursion)
    {
        Container::resizeToContent(recursion);
        adjustContent();
    }

    void AdjustingContainer::expandContent(bool recursion) { }

    Rectangle AdjustingContainer::getChildrenArea()
    {
        Rectangle rec;
        rec.x      = getBorderSize();
        rec.y      = getBorderSize();
        rec.width  = getWidth() - 2 * getBorderSize();
        rec.height = getHeight() - 2 * getBorderSize();
        return rec;
    }

    // void AdjustingContainer::logic()
    //{
    //     Container::logic();
    //     adjustContent();
    // }

    void AdjustingContainer::add(Widget* widget)
    {
        Container::add(widget);
        mContainedWidgets.push_back(widget);
    }

    void AdjustingContainer::add(Widget* widget, int /*x*/, int /*y*/)
    {
        add(widget);
    }

    void AdjustingContainer::removeAllChildren()
    {
        Container::removeAllChildren();
        mContainedWidgets.clear();
    }

    void AdjustingContainer::remove(Widget* widget)
    {
        Container::remove(widget);
        auto it = std::ranges::find_if(mContainedWidgets, [widget](fcn::Widget const * w) {
            return w == widget;
        });
        if (it != mContainedWidgets.end()) {
            mContainedWidgets.erase(it);
        }
    }

    void AdjustingContainer::adjustSize()
    {
        // TODO(jakoch): is this calc correct?
        mNumberOfRows = mContainedWidgets.size() / mNumberOfColumns + mContainedWidgets.size() % mNumberOfColumns;

        mColumnWidths.clear();

        unsigned int i = 0;

        for (i = 0; i < mNumberOfColumns; i++) {
            mColumnWidths.push_back(0);
        }

        mRowHeights.clear();

        for (i = 0; i < mNumberOfRows; i++) {
            mRowHeights.push_back(0);
        }

        for (i = 0; i < mNumberOfColumns; i++) {
            unsigned int j = 0;
            for (j = 0; j < mNumberOfRows && (mNumberOfColumns * j) + i < mContainedWidgets.size(); j++) {
                if (std::cmp_greater(mContainedWidgets[(mNumberOfColumns * j) + i]->getWidth(), mColumnWidths[i])) {
                    mColumnWidths[i] = mContainedWidgets[(mNumberOfColumns * j) + i]->getWidth();
                }
                if (std::cmp_greater(mContainedWidgets[(mNumberOfColumns * j) + i]->getHeight(), mRowHeights[j])) {
                    mRowHeights[j] = mContainedWidgets[(mNumberOfColumns * j) + i]->getHeight();
                }
            }
        }

        // width calculation
        mWidth = mPaddingLeft;
        mWidth += std::accumulate(mColumnWidths.begin(), mColumnWidths.end(), 0u);
        mWidth += mColumnWidths.size() * mHorizontalSpacing;
        mWidth -= mHorizontalSpacing;
        mWidth += mPaddingRight;

        // height calculation
        mHeight = mPaddingTop;
        mHeight += std::accumulate(mRowHeights.begin(), mRowHeights.end(), 0u);
        mHeight += mRowHeights.size() * mVerticalSpacing;
        mHeight -= mVerticalSpacing;
        mHeight += mPaddingBottom;

        setHeight(mHeight + (2 * getBorderSize()));
        setWidth(mWidth + (2 * getBorderSize()));
    }

    void AdjustingContainer::adjustContent()
    {
        adjustSize();

        unsigned int columnCount = 0;
        unsigned int rowCount    = 0;
        unsigned int y           = mPaddingTop;

        for (auto& mContainedWidget : mContainedWidgets) {

            // calculate the x position of the widget
            unsigned basex = 0;
            if ((columnCount % mNumberOfColumns) != 0U) {
                basex = mPaddingLeft;
                basex += std::accumulate(mColumnWidths.begin(), mColumnWidths.begin() + columnCount, 0u);
                basex += columnCount * mHorizontalSpacing;
            } else {
                basex = mPaddingLeft;
            }

            switch (mColumnAlignment[columnCount]) {
            case Alignment::Left:
                mContainedWidget->setX(basex);
                break;
            case Alignment::Center:
                mContainedWidget->setX(basex + ((mColumnWidths[columnCount] - mContainedWidget->getWidth()) / 2));
                break;
            case Alignment::Right:
                mContainedWidget->setX(basex + mColumnWidths[columnCount] - mContainedWidget->getWidth());
                break;
            default:
                throwException("Unknown alignment.");
            }

            mContainedWidget->setY(y);
            columnCount++;

            if (columnCount == mNumberOfColumns) {
                columnCount = 0;
                y += mRowHeights[rowCount] + mVerticalSpacing;
                rowCount++;
            }
        }
    }
} // namespace fcn
