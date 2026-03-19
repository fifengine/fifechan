// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/widgets/dropdown.hpp"

#include <memory>

#include "fifechan/exception.hpp"
#include "fifechan/font.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/key.hpp"
#include "fifechan/listmodel.hpp"
#include "fifechan/mouseinput.hpp"
#include "fifechan/widgets/listbox.hpp"
#include "fifechan/widgets/scrollarea.hpp"

namespace fcn
{
    DropDown::DropDown(ListModel* listModel, ScrollArea* scrollArea, ListBox* listBox) :
        mInternalScrollArea(scrollArea == nullptr), mInternalListBox(listBox == nullptr)
    {
        setWidth(100);
        setFocusable(true);

        setInternalFocusHandler(&mInternalFocusHandler);

        if (mInternalScrollArea) {
            mOwnedScrollArea = std::make_unique<ScrollArea>();
            mScrollArea      = mOwnedScrollArea.get();
        } else {
            mScrollArea = scrollArea;
        }

        if (mInternalListBox) {
            mOwnedListBox = std::make_unique<ListBox>();
            mListBox      = mOwnedListBox.get();
        } else {
            mListBox = listBox;
        }

        mScrollArea->setContent(mListBox);
        add(mScrollArea);

        mListBox->addActionListener(this);
        mListBox->addSelectionListener(this);

        setListModel(listModel);

        addMouseListener(this);
        addKeyListener(this);
        addFocusListener(this);

        adjustHeight();
    }

    DropDown::~DropDown()
    {
        if (widgetExists(mListBox)) {
            mListBox->removeActionListener(this);
            mListBox->removeSelectionListener(this);
        }

        setInternalFocusHandler(nullptr);
    }

    void DropDown::draw(Graphics* graphics)
    {
        int h = 0;

        if (mDroppedDown) {
            h = mFoldedUpHeight;
        } else {
            h = getHeight();
        }

        Color const faceColor = getBaseColor();
        Color highlightColor;
        Color shadowColor;
        int const alpha  = getBaseColor().a;
        highlightColor   = faceColor + 0x303030;
        highlightColor.a = alpha;
        shadowColor      = faceColor - 0x303030;
        shadowColor.a    = alpha;

        // Draw a border.
        graphics->setColor(shadowColor);
        graphics->drawLine(0, 0, getWidth() - 1, 0);
        graphics->drawLine(0, 1, 0, h - 2);
        graphics->setColor(highlightColor);
        graphics->drawLine(getWidth() - 1, 1, getWidth() - 1, h - 1);
        graphics->drawLine(0, h - 1, getWidth() - 1, h - 1);

        // Push a clip area so the other drawings don't need to worry
        // about the border.
        graphics->pushClipArea(Rectangle(1, 1, getWidth() - 2, h - 2));
        ClipRectangle const & currentClipArea = graphics->getCurrentClipArea();

        graphics->setColor(getBackgroundColor());
        graphics->fillRectangle(0, 0, currentClipArea.width, currentClipArea.height);

        if (isFocused()) {
            graphics->setColor(getSelectionColor());
            graphics->fillRectangle(0, 0, currentClipArea.width - currentClipArea.height, currentClipArea.height);
            graphics->setColor(getForegroundColor());
        }

        if ((mListBox->getListModel() != nullptr) && mListBox->getSelected() >= 0) {
            graphics->setColor(getForegroundColor());
            graphics->setFont(getFont());

            graphics->drawText(mListBox->getListModel()->getElementAt(mListBox->getSelected()), 1, 0);
        } else if ((mListBox->getListModel() != nullptr) && mListBox->getSelected() < 0) {
            graphics->setColor(getForegroundColor());
            graphics->setFont(getFont());

            graphics->drawText("", 1, 0);
        }

        // Push a clip area before drawing the button.
        graphics->pushClipArea(Rectangle(
            currentClipArea.width - currentClipArea.height, 0, currentClipArea.height, currentClipArea.height));
        drawButton(graphics);
        graphics->popClipArea();
        graphics->popClipArea();

        if (mDroppedDown) {
            // Draw a border around the children.
            graphics->setColor(shadowColor);
            graphics->drawRectangle(0, mFoldedUpHeight, getWidth(), getHeight() - mFoldedUpHeight);
            // drawChildren(graphics);
        }
    }

    void DropDown::drawButton(Graphics* graphics)
    {
        Color faceColor;
        Color highlightColor;
        Color shadowColor;
        int offset      = 0;
        int const alpha = getBaseColor().a;

        if (mPushed) {
            faceColor        = getBaseColor() - 0x303030;
            faceColor.a      = alpha;
            highlightColor   = faceColor - 0x303030;
            highlightColor.a = alpha;
            shadowColor      = faceColor + 0x303030;
            shadowColor.a    = alpha;
            offset           = 1;
        } else {
            faceColor        = getBaseColor();
            faceColor.a      = alpha;
            highlightColor   = faceColor + 0x303030;
            highlightColor.a = alpha;
            shadowColor      = faceColor - 0x303030;
            shadowColor.a    = alpha;
            offset           = 0;
        }

        ClipRectangle const & currentClipArea = graphics->getCurrentClipArea();
        graphics->setColor(highlightColor);
        graphics->drawLine(0, 0, currentClipArea.width - 1, 0);
        graphics->drawLine(0, 1, 0, currentClipArea.height - 1);
        graphics->setColor(shadowColor);
        graphics->drawLine(currentClipArea.width - 1, 1, currentClipArea.width - 1, currentClipArea.height - 1);
        graphics->drawLine(1, currentClipArea.height - 1, currentClipArea.width - 2, currentClipArea.height - 1);

        graphics->setColor(faceColor);
        graphics->fillRectangle(1, 1, currentClipArea.width - 2, currentClipArea.height - 2);

        graphics->setColor(getForegroundColor());

        int i        = 0;
        int const n  = currentClipArea.height / 3;
        int const dx = currentClipArea.height / 2;
        int const dy = (currentClipArea.height * 2) / 3;
        for (i = 0; i < n; i++) {
            graphics->drawLine(dx - i + offset, dy - i + offset, dx + i + offset, dy - i + offset);
        }
    }

    int DropDown::getSelected() const
    {
        return mListBox->getSelected();
    }

    void DropDown::setSelected(int selected)
    {
        mListBox->setSelected(selected);
    }

    void DropDown::keyPressed(KeyEvent& keyEvent)
    {
        if (keyEvent.isConsumed()) {
            return;
        }

        Key const key = keyEvent.getKey();

        if ((key.getValue() == Key::Enter || key.getValue() == Key::Space) && !mDroppedDown) {
            dropDown();
            keyEvent.consume();
        } else if (key.getValue() == Key::Up) {
            setSelected(getSelected() - 1);
            keyEvent.consume();
        } else if (key.getValue() == Key::Down) {
            setSelected(getSelected() + 1);
            keyEvent.consume();
        }
    }

    void DropDown::mousePressed(MouseEvent& mouseEvent)
    {
        // If we have a mouse press on the widget.
        if (0 <= mouseEvent.getY() && mouseEvent.getY() < getHeight() && mouseEvent.getX() >= 0 &&
            mouseEvent.getX() < getWidth() && mouseEvent.getButton() == MouseEvent::Button::Left && !mDroppedDown &&
            mouseEvent.getSource() == this) {
            mPushed = true;
            dropDown();
            requestModalMouseInputFocus();
        } else if (
            // Fold up the listbox if the upper part is clicked after fold down
            0 <= mouseEvent.getY() && mouseEvent.getY() < mFoldedUpHeight && mouseEvent.getX() >= 0 &&
            mouseEvent.getX() < getWidth() && mouseEvent.getButton() == MouseEvent::Button::Left && mDroppedDown &&
            mouseEvent.getSource() == this) {
            mPushed = false;
            foldUp();
            releaseModalMouseInputFocus();
        } else if (
            // If we have a mouse press outside the widget
            0 > mouseEvent.getY() || mouseEvent.getY() >= getHeight() || mouseEvent.getX() < 0 ||
            mouseEvent.getX() >= getWidth()) {
            mPushed = false;
            foldUp();
        }
    }

    void DropDown::mouseReleased(MouseEvent& mouseEvent)
    {
        if (mIsDragged) {
            mPushed = false;
        }

        // Released outside of widget. Can happen when we have modal input focus.
        if ((0 > mouseEvent.getY() || mouseEvent.getY() >= getHeight() || mouseEvent.getX() < 0 ||
             mouseEvent.getX() >= getWidth()) &&
            mouseEvent.getButton() == MouseEvent::Button::Left && isModalMouseInputFocused()) {
            releaseModalMouseInputFocus();

            if (mIsDragged) {
                foldUp();
            }
        } else if (mouseEvent.getButton() == MouseEvent::Button::Left) {
            mPushed = false;
        }

        mIsDragged = false;
    }

    void DropDown::mouseDragged(MouseEvent& mouseEvent)
    {
        mIsDragged = true;

        mouseEvent.consume();
    }

    void DropDown::setListModel(ListModel* listModel)
    {
        mListBox->setListModel(listModel);

        adjustHeight();
    }

    ListModel* DropDown::getListModel() const
    {
        return mListBox->getListModel();
    }

    void DropDown::adjustHeight()
    {
        if (mScrollArea == nullptr) {
            throwException("Scroll area has been deleted.");
        }

        if (mListBox == nullptr) {
            throwException("List box has been deleted.");
        }

        int const listBoxHeight = mListBox->getHeight();

        // We add 2 for the border
        int const h2 = getFont()->getHeight() + 2;

        setHeight(h2);

        // The addition/subtraction of 2 compensates for the separation lines,
        // which are separating the selected element view and the scroll area.

        if (mDroppedDown && (getParent() != nullptr)) {
            int const h = getParent()->getChildrenArea().height - getY();

            if (listBoxHeight > h - h2 - 2) {
                mScrollArea->setHeight(h - h2 - 2);
                setHeight(h);
            } else {
                setHeight(listBoxHeight + h2 + 2);
                mScrollArea->setHeight(listBoxHeight);
            }
        }

        mScrollArea->setWidth(getWidth());
        // Resize the ListBox to exactly fit the ScrollArea.
        mListBox->setWidth(mScrollArea->getChildrenArea().width);
        mScrollArea->setPosition(0, 0);
    }

    void DropDown::resizeToContent(bool recursion)
    {
        (void)recursion; // unused parameter

        if (mScrollArea != nullptr) {
            mScrollArea->resizeToContent();
        }

        if (mListBox != nullptr) {
            mScrollArea->resizeToContent();
        }
        adjustHeight();
    }

    void DropDown::adjustSize()
    {
        adjustHeight();
    }

    void DropDown::dropDown()
    {
        if (!mDroppedDown) {
            mDroppedDown    = true;
            mFoldedUpHeight = getHeight();
            adjustHeight();

            if (getParent() != nullptr) {
                getParent()->moveToTop(this);
            }
        }

        mListBox->requestFocus();
    }

    void DropDown::foldUp()
    {
        if (mDroppedDown) {
            mDroppedDown = false;
            adjustHeight();
            mInternalFocusHandler.focusNone();
        }
    }

    void DropDown::focusLost([[maybe_unused]] Event const & event)
    {
        foldUp();
        mInternalFocusHandler.focusNone();
    }

    void DropDown::death(Event const & event)
    {
        if (event.getSource() == mScrollArea) {
            if (mOwnedScrollArea.get() == mScrollArea) {
                (void)mOwnedScrollArea.release();
            }
            mScrollArea = nullptr;
        }
    }

    void DropDown::action([[maybe_unused]] ActionEvent const & actionEvent)
    {
        foldUp();
        releaseModalMouseInputFocus();
        distributeActionEvent();
    }

    Rectangle DropDown::getChildrenArea()
    {
        if (mDroppedDown) {
            // Calculate the children area (with the one pixel border in mind)
            return {1, mFoldedUpHeight + 1, getWidth() - 2, getHeight() - mFoldedUpHeight - 2};
        }

        return {};
    }

    void DropDown::setBaseColor(Color const & color)
    {
        if (mInternalScrollArea) {
            mScrollArea->setBaseColor(color);
        }

        if (mInternalListBox) {
            mListBox->setBaseColor(color);
        }

        Widget::setBaseColor(color);
    }

    void DropDown::setBackgroundColor(Color const & color)
    {
        if (mInternalScrollArea) {
            mScrollArea->setBackgroundColor(color);
        }

        if (mInternalListBox) {
            mListBox->setBackgroundColor(color);
        }

        Widget::setBackgroundColor(color);
    }

    void DropDown::setForegroundColor(Color const & color)
    {
        if (mInternalScrollArea) {
            mScrollArea->setForegroundColor(color);
        }

        if (mInternalListBox) {
            mListBox->setForegroundColor(color);
        }

        Widget::setForegroundColor(color);
    }

    void DropDown::setFont(Font* font)
    {
        if (mInternalScrollArea) {
            mScrollArea->setFont(font);
        }

        if (mInternalListBox) {
            mListBox->setFont(font);
        }

        Widget::setFont(font);
    }

    void DropDown::mouseWheelMovedUp(MouseEvent& mouseEvent)
    {
        if (isFocused() && mouseEvent.getSource() == this) {
            mouseEvent.consume();

            if (mListBox->getSelected() > 0) {
                mListBox->setSelected(mListBox->getSelected() - 1);
            }
        }
    }

    void DropDown::mouseWheelMovedDown(MouseEvent& mouseEvent)
    {
        if (isFocused() && mouseEvent.getSource() == this) {
            mouseEvent.consume();

            mListBox->setSelected(mListBox->getSelected() + 1);
        }
    }

    void DropDown::setSelectionColor(Color const & color)
    {
        Widget::setSelectionColor(color);

        if (mInternalListBox) {
            mListBox->setSelectionColor(color);
        }
    }

    void DropDown::valueChanged([[maybe_unused]] SelectionEvent const & event)
    {
        distributeValueChangedEvent();
    }

    void DropDown::addSelectionListener(SelectionListener* selectionListener)
    {
        mSelectionListeners.push_back(selectionListener);
    }

    void DropDown::removeSelectionListener(SelectionListener* selectionListener)
    {
        mSelectionListeners.remove(selectionListener);
    }

    void DropDown::distributeValueChangedEvent()
    {
        SelectionListenerIterator iter;

        for (iter = mSelectionListeners.begin(); iter != mSelectionListeners.end(); ++iter) {
            SelectionEvent const event(this);
            (*iter)->valueChanged(event);
        }
    }
} // namespace fcn
