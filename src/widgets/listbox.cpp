// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/widgets/listbox.hpp"

#include <algorithm>

#include "fifechan/font.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/key.hpp"
#include "fifechan/listmodel.hpp"
#include "fifechan/mouseinput.hpp"
#include "fifechan/selectionlistener.hpp"

namespace fcn
{
    ListBox::ListBox() : mSelected(-1), mListModel(NULL), mWrappingEnabled(false)
    {
        setWidth(100);
        setFocusable(true);

        addMouseListener(this);
        addKeyListener(this);
    }

    ListBox::ListBox(ListModel* listModel) : mSelected(-1), mWrappingEnabled(false)
    {
        setWidth(100);
        setListModel(listModel);
        setFocusable(true);

        addMouseListener(this);
        addKeyListener(this);
    }

    void ListBox::draw(Graphics* graphics)
    {
        graphics->setColor(getBackgroundColor());
        graphics->fillRectangle(0, 0, getWidth(), getHeight());

        if (mListModel == NULL) {
            return;
        }

        graphics->setColor(getForegroundColor());
        graphics->setFont(getFont());

        // Check the current clip area so we don't draw unnecessary items
        // that are not visible.
        ClipRectangle const currentClipArea = graphics->getCurrentClipArea();
        int rowHeight                       = getRowHeight();

        // Calculate the number of rows to draw by checking the clip area.
        // The addition of two makes covers a partial visible row at the top
        // and a partial visible row at the bottom.
        int numberOfRows = currentClipArea.height / rowHeight + 2;

        if (numberOfRows > mListModel->getNumberOfElements()) {
            numberOfRows = mListModel->getNumberOfElements();
        }

        // Calculate which row to start drawing. If the list box
        // has a negative y coordinate value we should check if
        // we should drop rows in the begining of the list as
        // they might not be visible. A negative y value is very
        // common if the list box for instance resides in a scroll
        // area and the user has scrolled the list box downwards.
        int startRow;
        if (getY() < 0) {
            startRow = -1 * (getY() / rowHeight);
        } else {
            startRow = 0;
        }

        int i;
        // The y coordinate where we start to draw the text is
        // simply the y coordinate multiplied with the font height.
        int y = rowHeight * startRow;
        for (i = startRow; i < startRow + numberOfRows; ++i) {
            if (i == mSelected) {
                graphics->setColor(getSelectionColor());
                graphics->fillRectangle(0, y, getWidth(), rowHeight);
                graphics->setColor(getForegroundColor());
            }

            // If the row height is greater than the font height we
            // draw the text with a center vertical alignment.
            if (rowHeight > getFont()->getHeight()) {
                graphics->drawText(mListModel->getElementAt(i), 1, y + rowHeight / 2 - getFont()->getHeight() / 2);
            } else {
                graphics->drawText(mListModel->getElementAt(i), 1, y);
            }

            y += rowHeight;
        }
    }

    void ListBox::logic()
    {
        adjustSize();
    }

    int ListBox::getSelected() const
    {
        return mSelected;
    }

    void ListBox::setSelected(int selected)
    {
        if (mListModel == NULL) {
            mSelected = -1;
        } else {
            if (selected < 0) {
                mSelected = -1;
            } else if (selected >= mListModel->getNumberOfElements()) {
                mSelected = mListModel->getNumberOfElements() - 1;
            } else {
                mSelected = selected;
            }
        }

        Rectangle scroll;

        if (mSelected < 0) {
            scroll.y = 0;
        } else {
            scroll.y = getRowHeight() * mSelected;
        }

        scroll.height = getRowHeight();
        showPart(scroll);

        distributeValueChangedEvent();
    }

    void ListBox::keyPressed(KeyEvent& keyEvent)
    {
        Key key = keyEvent.getKey();

        if (key.getValue() == Key::Enter || key.getValue() == Key::Space) {
            distributeActionEvent();
            keyEvent.consume();
        } else if (key.getValue() == Key::Up) {
            setSelected(mSelected - 1);

            if (mSelected == -1) {
                if (mWrappingEnabled) {
                    setSelected(getListModel()->getNumberOfElements() - 1);
                } else {
                    setSelected(0);
                }
            }

            keyEvent.consume();
        } else if (key.getValue() == Key::Down) {
            if (mWrappingEnabled && getSelected() == getListModel()->getNumberOfElements() - 1) {
                setSelected(0);
            } else {
                setSelected(getSelected() + 1);
            }

            keyEvent.consume();
        } else if (key.getValue() == Key::Home) {
            setSelected(0);
            keyEvent.consume();
        } else if (key.getValue() == Key::End) {
            setSelected(getListModel()->getNumberOfElements() - 1);
            keyEvent.consume();
        }
    }

    void ListBox::mousePressed(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getButton() == MouseEvent::Left) {
            setSelected(mouseEvent.getY() / getRowHeight());
            distributeActionEvent();
        }
    }

    void ListBox::mouseWheelMovedUp(MouseEvent& mouseEvent)
    {
        if (isFocused()) {
            if (getSelected() > 0) {
                setSelected(getSelected() - 1);
            }

            mouseEvent.consume();
        }
    }

    void ListBox::mouseWheelMovedDown(MouseEvent& mouseEvent)
    {
        if (isFocused()) {
            setSelected(getSelected() + 1);

            mouseEvent.consume();
        }
    }

    void ListBox::mouseDragged(MouseEvent& mouseEvent)
    {
        mouseEvent.consume();
    }

    void ListBox::setListModel(ListModel* listModel)
    {
        mSelected  = -1;
        mListModel = listModel;
        adjustSize();
    }

    ListModel* ListBox::getListModel() const
    {
        return mListModel;
    }

    void ListBox::resizeToContent(bool recursiv)
    {
        adjustSize();
    }

    void ListBox::adjustSize()
    {
        if (mListModel != NULL) {
            // min width in case the lit contains no element
            int w        = getRowHeight();
            int elements = mListModel->getNumberOfElements();
            for (int i = 0; i < elements; ++i) {
                // std::string element = mListModel->getElementAt(i);
                w = std::max(w, getFont()->getWidth(mListModel->getElementAt(i)));
            }
            setWidth(w);
            setHeight(getRowHeight() * mListModel->getNumberOfElements());
        }
    }

    bool ListBox::isWrappingEnabled() const
    {
        return mWrappingEnabled;
    }

    void ListBox::setWrappingEnabled(bool wrappingEnabled)
    {
        mWrappingEnabled = wrappingEnabled;
    }

    void ListBox::addSelectionListener(SelectionListener* selectionListener)
    {
        mSelectionListeners.push_back(selectionListener);
    }

    void ListBox::removeSelectionListener(SelectionListener* selectionListener)
    {
        mSelectionListeners.remove(selectionListener);
    }

    void ListBox::distributeValueChangedEvent()
    {
        SelectionListenerIterator iter;

        for (iter = mSelectionListeners.begin(); iter != mSelectionListeners.end(); ++iter) {
            SelectionEvent event(this);
            (*iter)->valueChanged(event);
        }
    }

    unsigned int ListBox::getRowHeight() const
    {
        return getFont()->getHeight();
    }
} // namespace fcn
