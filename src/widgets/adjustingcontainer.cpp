// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/widgets/adjustingcontainer.hpp"

#include <algorithm>
#include <utility>

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
        auto it = std::ranges::find_if(mContainedWidgets, [widget](fcn::Widget* w) {
            return w == widget;
        });
        if (it != mContainedWidgets.end()) {
            mContainedWidgets.erase(it);
        }
    }

    void AdjustingContainer::adjustSize()
    {
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

        mWidth = mPaddingLeft;

        for (i = 0; i < mColumnWidths.size(); i++) {
            mWidth += mColumnWidths[i] + mHorizontalSpacing;
        }

        mWidth -= mHorizontalSpacing;
        mWidth += mPaddingRight;

        mHeight = mPaddingTop;

        for (i = 0; i < mRowHeights.size(); i++) {
            mHeight += mRowHeights[i] + mVerticalSpacing;
        }

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
            unsigned basex = 0;
            if ((columnCount % mNumberOfColumns) != 0U) {
                basex          = mPaddingLeft;
                unsigned int j = 0;

                for (j = 0; j < columnCount; j++) {
                    basex += mColumnWidths[j] + mHorizontalSpacing;
                }
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
