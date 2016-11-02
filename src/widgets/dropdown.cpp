/***************************************************************************
 *   Copyright (c) 2016 by the fifechan team                               *
 *   https://github.com/fifengine/fifechan                                 *
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
 * Copyright (c) 2004 - 2008 Olof Naess�n and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naess�n a.k.a jansem/yakslem
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

#include "fifechan/widgets/dropdown.hpp"

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
    DropDown::DropDown(ListModel *listModel,
                       ScrollArea *scrollArea,
                       ListBox *listBox)
    {
        setWidth(100);
        setFocusable(true);
        mDroppedDown = false;
        mPushed = false;
        mIsDragged = false;

        setInternalFocusHandler(&mInternalFocusHandler);

        mInternalScrollArea = (scrollArea == NULL);
        mInternalListBox = (listBox == NULL);

        if (mInternalScrollArea)
        {
            mScrollArea = new ScrollArea();
        }
        else
        {
            mScrollArea = scrollArea;
        }

        if (mInternalListBox)
        {
            mListBox = new ListBox();
        }
        else
        {
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
        if (widgetExists(mListBox))
        {
            mListBox->removeActionListener(this);
            mListBox->removeSelectionListener(this);
        }

        if (mInternalScrollArea)
        {
            delete mScrollArea;
        }

        if (mInternalListBox)
        {
            delete mListBox;
        }

        setInternalFocusHandler(NULL);
    }

    void DropDown::draw(Graphics* graphics)
    {
        int h;

        if (mDroppedDown)
        {
            h = mFoldedUpHeight;
        }
        else
        {
            h = getHeight();
        }

        Color faceColor = getBaseColor();
        Color highlightColor, shadowColor;
        int alpha = getBaseColor().a;
        highlightColor = faceColor + 0x303030;
        highlightColor.a = alpha;
        shadowColor = faceColor - 0x303030;
        shadowColor.a = alpha;

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
        const Rectangle currentClipArea = graphics->getCurrentClipArea();

        graphics->setColor(getBackgroundColor());
        graphics->fillRectangle(0, 0, currentClipArea.width, currentClipArea.height);
       
        if (isFocused())
        {
            graphics->setColor(getSelectionColor());
            graphics->fillRectangle(0,
                                    0,
                                    currentClipArea.width - currentClipArea.height,
                                    currentClipArea.height);
            graphics->setColor(getForegroundColor());
        }

        if (mListBox->getListModel() 
            && mListBox->getSelected() >= 0)
        {
            graphics->setColor(getForegroundColor());
            graphics->setFont(getFont());

            graphics->drawText(mListBox->getListModel()->getElementAt(mListBox->getSelected()), 1, 0);
        }
        else if (mListBox->getListModel() 
            && mListBox->getSelected() < 0)
        {
            graphics->setColor(getForegroundColor());
            graphics->setFont(getFont());

            graphics->drawText("", 1, 0);
        }
        
        // Push a clip area before drawing the button.
        graphics->pushClipArea(Rectangle(currentClipArea.width - currentClipArea.height,
                                         0,
                                         currentClipArea.height,
                                         currentClipArea.height));
        drawButton(graphics);
        graphics->popClipArea();
        graphics->popClipArea();

         if (mDroppedDown)
         {
             // Draw a border around the children.
             graphics->setColor(shadowColor);
             graphics->drawRectangle(0,
                                     mFoldedUpHeight,
                                     getWidth(),
                                     getHeight() - mFoldedUpHeight);
             //drawChildren(graphics);
         }
    }

    void DropDown::drawButton(Graphics *graphics)
    {
        Color faceColor, highlightColor, shadowColor;
        int offset;
        int alpha = getBaseColor().a;

        if (mPushed)
        {
            faceColor = getBaseColor() - 0x303030;
            faceColor.a = alpha;
            highlightColor = faceColor - 0x303030;
            highlightColor.a = alpha;
            shadowColor = faceColor + 0x303030;
            shadowColor.a = alpha;
            offset = 1;
        }
        else
        {
            faceColor = getBaseColor();
            faceColor.a = alpha;
            highlightColor = faceColor + 0x303030;
            highlightColor.a = alpha;
            shadowColor = faceColor - 0x303030;
            shadowColor.a = alpha;
            offset = 0;
        }

        const Rectangle currentClipArea = graphics->getCurrentClipArea();
        graphics->setColor(highlightColor);
        graphics->drawLine(0, 
                           0, 
                           currentClipArea.width - 1, 
                           0);
        graphics->drawLine(0, 
                           1, 
                           0, 
                           currentClipArea.height - 1);
        graphics->setColor(shadowColor);
        graphics->drawLine(currentClipArea.width - 1, 
                           1, 
                           currentClipArea.width - 1, 
                           currentClipArea.height - 1);
        graphics->drawLine(1, 
                           currentClipArea.height - 1, 
                           currentClipArea.width - 2, 
                           currentClipArea.height - 1);

        graphics->setColor(faceColor);
        graphics->fillRectangle(1,
                                1,
                                currentClipArea.width - 2,
                                currentClipArea.height - 2);

        graphics->setColor(getForegroundColor());

        int i;
        int n = currentClipArea.height / 3;
        int dx = currentClipArea.height / 2;
        int dy = (currentClipArea.height * 2) / 3;
        for (i = 0; i < n; i++)
        {
            graphics->drawLine(dx - i + offset,
                               dy - i + offset,
                               dx + i + offset,
                               dy - i + offset);
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
        if (keyEvent.isConsumed())
            return;
        
        Key key = keyEvent.getKey();

        if ((key.getValue() == Key::Enter || key.getValue() == Key::Space)
            && !mDroppedDown)
        {
            dropDown();
            keyEvent.consume();
        }
        else if (key.getValue() == Key::Up)
        {
            setSelected(getSelected() - 1);
            keyEvent.consume();
        }
        else if (key.getValue() == Key::Down)
        {
            setSelected(getSelected() + 1);
            keyEvent.consume();
        }
    }

    void DropDown::mousePressed(MouseEvent& mouseEvent)
    {        
        // If we have a mouse press on the widget.
        if (0 <= mouseEvent.getY()
            && mouseEvent.getY() < getHeight()
            && mouseEvent.getX() >= 0
            && mouseEvent.getX() < getWidth()
            && mouseEvent.getButton() == MouseEvent::Left
            && !mDroppedDown
            && mouseEvent.getSource() == this)
        {
            mPushed = true;
            dropDown();
            requestModalMouseInputFocus();
        }
        // Fold up the listbox if the upper part is clicked after fold down
        else if (0 <= mouseEvent.getY()
                 && mouseEvent.getY() < mFoldedUpHeight
                 && mouseEvent.getX() >= 0
                 && mouseEvent.getX() < getWidth()
                 && mouseEvent.getButton() == MouseEvent::Left
                 && mDroppedDown
                 && mouseEvent.getSource() == this)
        {
            mPushed = false;
            foldUp();
            releaseModalMouseInputFocus();
        }
        // If we have a mouse press outside the widget
        else if (0 > mouseEvent.getY()
                 || mouseEvent.getY() >= getHeight()
                 || mouseEvent.getX() < 0
                 || mouseEvent.getX() >= getWidth())
        {
            mPushed = false;
            foldUp();
        }
    }

    void DropDown::mouseReleased(MouseEvent& mouseEvent)
    {
        if (mIsDragged)
        {
            mPushed = false;
        }

        // Released outside of widget. Can happen when we have modal input focus.
        if ((0 > mouseEvent.getY()
            || mouseEvent.getY() >= getHeight()
            || mouseEvent.getX() < 0
            || mouseEvent.getX() >= getWidth())
            && mouseEvent.getButton() == MouseEvent::Left
            && isModalMouseInputFocused())
        {
            releaseModalMouseInputFocus();

            if (mIsDragged)
            {
                foldUp();
            }
        }
        else if (mouseEvent.getButton() == MouseEvent::Left)
        {
            mPushed = false;
        }

        mIsDragged = false;
    }

    void DropDown::mouseDragged(MouseEvent& mouseEvent)
    {
        mIsDragged = true;

        mouseEvent.consume();
    }

    void DropDown::setListModel(ListModel *listModel)
    {
        mListBox->setListModel(listModel);

        adjustHeight();
    }

    ListModel *DropDown::getListModel() const
    {
        return mListBox->getListModel();
    }

    void DropDown::adjustHeight()
    {
        if (mScrollArea == NULL)
        {
            throw FCN_EXCEPTION("Scroll area has been deleted.");
        }

        if (mListBox == NULL)
        {
            throw FCN_EXCEPTION("List box has been deleted.");
        }

        int listBoxHeight = mListBox->getHeight();
        
        // We add 2 for the border
        int h2 = getFont()->getHeight() + 2;

        _setHeightWithoutNotifying(h2);

        // The addition/subtraction of 2 compensates for the seperation lines
        // seperating the selected element view and the scroll area.

        if (mDroppedDown && getParent())
        {
            int h = getParent()->getChildrenArea().height - getY();

            if (listBoxHeight > h - h2 - 2)
            {
                mScrollArea->setHeight(h - h2 - 2);
                _setHeightWithoutNotifying(h);
            }
            else
            {
                _setHeightWithoutNotifying(listBoxHeight + h2 + 2);
                mScrollArea->setHeight(listBoxHeight);
            }
        }

        mScrollArea->setWidth(getWidth());
        // Resize the ListBox to exactly fit the ScrollArea.
        mListBox->setWidth(mScrollArea->getChildrenArea().width);
        mScrollArea->setPosition(0, 0);
    }

    void DropDown::dropDown()
    {
        if (!mDroppedDown)
        {
            mDroppedDown = true;
            mFoldedUpHeight = getHeight();
            adjustHeight();
            

            if (getParent())
            {
                getParent()->moveToTop(this);
            }
        }

        mListBox->requestFocus();
    }

    void DropDown::foldUp()
    {
        if (mDroppedDown)
        {
            mDroppedDown = false;
            adjustHeight();
            mInternalFocusHandler.focusNone();
        }
    }

    void DropDown::focusLost(const Event& event)
    {
        foldUp();
        mInternalFocusHandler.focusNone();
    }


    void DropDown::death(const Event& event)
    {        
        if (event.getSource() == mScrollArea)
        {
            mScrollArea = NULL;
        }
    }

    void DropDown::action(const ActionEvent& actionEvent)
    {
        foldUp();
        releaseModalMouseInputFocus();
        distributeActionEvent();
    }

    Rectangle DropDown::getChildrenArea()
    {
        if (mDroppedDown)
        {
            // Calculate the children area (with the one pixel border in mind)
            return Rectangle(1, 
                             mFoldedUpHeight + 1, 
                             getWidth() - 2, 
                             getHeight() - mFoldedUpHeight - 2);
        }

        return Rectangle();
    }

    void DropDown::setBaseColor(const Color& color)
    {
        if (mInternalScrollArea)
        {
            mScrollArea->setBaseColor(color);
        }

        if (mInternalListBox)
        {
            mListBox->setBaseColor(color);
        }

        Widget::setBaseColor(color);
    }

    void DropDown::setBackgroundColor(const Color& color)
    {
        if (mInternalScrollArea)
        {
            mScrollArea->setBackgroundColor(color);
        }

        if (mInternalListBox)
        {
            mListBox->setBackgroundColor(color);
        }

        Widget::setBackgroundColor(color);
    }

    void DropDown::setForegroundColor(const Color& color)
    {
        if (mInternalScrollArea)
        {
            mScrollArea->setForegroundColor(color);
        }

        if (mInternalListBox)
        {
            mListBox->setForegroundColor(color);
        }

        Widget::setForegroundColor(color);
    }

	void DropDown::setFont(Font *font)
	{
		if (mInternalScrollArea)
        {
            mScrollArea->setFont(font);
        }

        if (mInternalListBox)
        {
            mListBox->setFont(font);
        }

        Widget::setFont(font);
	}

	void DropDown::mouseWheelMovedUp(MouseEvent& mouseEvent)
	{
        if (isFocused() && mouseEvent.getSource() == this)
        {                   
            mouseEvent.consume();

            if (mListBox->getSelected() > 0)
            {
                mListBox->setSelected(mListBox->getSelected() - 1);
            }
        }
    }

    void DropDown::mouseWheelMovedDown(MouseEvent& mouseEvent)
    {
        if (isFocused() && mouseEvent.getSource() == this)
        {            
            mouseEvent.consume();

            mListBox->setSelected(mListBox->getSelected() + 1);
        }
    }

    void DropDown::setSelectionColor(const Color& color)
    {
        Widget::setSelectionColor(color);
        
        if (mInternalListBox)
        {
            mListBox->setSelectionColor(color);
        }       
    }

    void DropDown::valueChanged(const SelectionEvent& event)
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

        for (iter = mSelectionListeners.begin(); iter != mSelectionListeners.end(); ++iter)
        {
            SelectionEvent event(this);
            (*iter)->valueChanged(event);
        }
    }
}

