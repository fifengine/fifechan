/*
 *    _aaaa,  _aa.  sa,  aaa              _aaaa,_  ac  .aa.   .aa.  .aa,  _a, sa
 *  .wWV!!!T  |Wm;  dQ[  $WF            _mWT!"?Y  ]QE  :Q#:   ]QW[  :WWk. ]Q[ dW
 * .jWf       :WW: .dQ[  dQ[           .mW(       )WE  :Q#:  .mSQh. :mWQa.]W[ dQ
 * |QW:       :Wm;  mQ[  dQ[           ]Qk        )Qmi_aQW:  <B:$Qc :WBWQ()W[ dQ
 * |W#:  .ww  ;WW;  dQ[  dQ[  .......  ]Qk        )QB?YYW#:  jf ]Qp.:mE)Qm]Q[ )W
 * +WQ;  :Wm  |Wm; .mQ[  dQ[ :qgggggga ]Qm.       ]WE  :Q# :=QasuQm;:Wk 3QQW[ )Y
 *  ]Wmi.:Wm  +$Q; .mW(  dQ[  !"!!"!!^ dQk,  ._   ]WE  :Q# :3D"!!$Qc.Wk -$WQ[   
 *   "?????? ` "?!=m?!   ??'            -??????!  -?!  -?? -?'   "?"-?"  "??' "?
 *
 * Copyright (c) 2004 darkbits                              Js_./
 * Per Larsson a.k.a finalman                          _RqZ{a<^_aa
 * Olof Naessén a.k.a jansem/yakslem                _asww7!uY`>  )\a//
 *                                                 _Qhm`] _f "'c  1!5m
 * Visit: http://guichan.darkbits.org             )Qk<P ` _: :+' .'  "{[
 *                                               .)j(] .d_/ '-(  P .   S
 * License: (BSD)                                <Td/Z <fP"5(\"??"\a.  .L
 * Redistribution and use in source and          _dV>ws?a-?'      ._/L  #'
 * binary forms, with or without                 )4d[#7r, .   '     )d`)[
 * modification, are permitted provided         _Q-5'5W..j/?'   -?!\)cam'
 * that the following conditions are met:       j<<WP+k/);.        _W=j f
 * 1. Redistributions of source code must       .$%w\/]Q  . ."'  .  mj$
 *    retain the above copyright notice,        ]E.pYY(Q]>.   a     J@\
 *    this list of conditions and the           j(]1u<sE"L,. .   ./^ ]{a
 *    following disclaimer.                     4'_uomm\.  )L);-4     (3=
 * 2. Redistributions in binary form must        )_]X{Z('a_"a7'<a"a,  ]"[
 *    reproduce the above copyright notice,       #}<]m7`Za??4,P-"'7. ).m
 *    this list of conditions and the            ]d2e)Q(<Q(  ?94   b-  LQ/
 *    following disclaimer in the                <B!</]C)d_, '(<' .f. =C+m
 *    documentation and/or other materials      .Z!=J ]e []('-4f _ ) -.)m]'
 *    provided with the distribution.          .w[5]' _[ /.)_-"+?   _/ <W"
 * 3. Neither the name of darkbits nor the     :$we` _! + _/ .        j?
 *    names of its contributors may be used     =3)= _f  (_yQmWW$#(    "
 *    to endorse or promote products derived     -   W,  sQQQQmZQ#Wwa]..
 *    from this software without specific        (js, \[QQW$QWW#?!V"".
 *    prior written permission.                    ]y:.<\..          .
 *                                                 -]n w/ '         [.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT       )/ )/           !
 * HOLDERS AND CONTRIBUTORS "AS IS" AND ANY         <  (; sac    ,    '
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING,               ]^ .-  %
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF            c <   r
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR            aga<  <La
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE          5%  )P'-3L
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR        _bQf` y`..)a
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          ,J?4P'.P"_(\?d'.,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES               _Pa,)!f/<[]/  ?"
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT      _2-..:. .r+_,.. .
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     ?a.<%"'  " -'.a_ _,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION)                     ^
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "guichan/widgets/dropdown.hpp"

#include <iostream>

namespace gcn
{
  
  DropDown::DropDown()
  {
    mDroppedDown = false;
    mPushed = false;
    
    setWidth(100);
    setFocusable(true);

    mDefaultScrollArea = new ScrollArea();
    mDefaultScrollArea->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
    mDefaultListBox = new ListBox();

    mScrollArea = mDefaultScrollArea;
    mScrollArea->_setFocusHandler(&mFocusHandler);

    mListBox = mDefaultListBox;    
    mScrollArea->setContent(mListBox);
        
    addMouseListener(this);
    addKeyListener(this);
    adjustHeight();    
  }
    
  DropDown::DropDown(ListModel *listModel)
  {
    setWidth(100);
    setFocusable(true);
    mDroppedDown = false;
    mPushed = false;    
    
    mDefaultScrollArea = new ScrollArea();
    mDefaultScrollArea->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
    mDefaultListBox = new ListBox();

    mScrollArea = mDefaultScrollArea;
    mListBox = mDefaultListBox;
    
    mScrollArea->setContent(mListBox);
    mScrollArea->_setFocusHandler(&mFocusHandler);
    
    setListModel(listModel);
    
    if (mListBox->getSelected() < 0)
    {
      mListBox->setSelected(0);
    }

    addMouseListener(this);
    addKeyListener(this);
    adjustHeight();
  }
  
  DropDown::DropDown(ListModel *listModel,
                     ScrollArea *scrollArea,
                     ListBox *listBox)
  {
    setWidth(100);
    setFocusable(true);
    mDroppedDown = false;
    mPushed = false;
    
    mDefaultScrollArea = NULL;
    mDefaultListBox = NULL;

    mScrollArea = scrollArea;
    mScrollArea->_setFocusHandler(&mFocusHandler);

    mListBox = listBox;    
    mScrollArea->setContent(mListBox);
    
    setListModel(listModel);

    if (mListBox->getSelected() < 0)
    {
      mListBox->setSelected(0);
    }

    addMouseListener(this);
    addKeyListener(this);
    adjustHeight();    
  }

  DropDown::~DropDown()
  {
//    mScrollArea->_setFocusHandler(NULL);    

    if (!mDefaultScrollArea)
    {
      delete mDefaultScrollArea;
    }

    if (!mDefaultListBox)
    {
      delete mDefaultListBox;
    }
  }
  
  void DropDown::logic()
  {
    mScrollArea->logic();    
  }
    
  void DropDown::draw(Graphics* graphics)
  {
    int h;

    if (mDroppedDown)
    {
      h = mOldH;
    }
    else
    {
      h = getHeight();
    }
    
    Color faceColor = getBaseColor();
    Color highlightColor = faceColor + 0x303030;
    Color shadowColor = faceColor - 0x303030;      
    
    graphics->setColor(getBackgroundColor());
    graphics->fillRectangle(Rectangle(1, 1, getWidth() - 2, h + 4));
   
    graphics->setColor(highlightColor);           
    graphics->drawLine(1, h-1, getWidth()-1, h-1);
    graphics->drawLine(getWidth()-1, 1, getWidth()-1, h - 1);

    graphics->setColor(shadowColor);
    graphics->drawLine(0, 0, 0, h);
    graphics->drawLine(0, 0, getWidth()-1, 0);

    graphics->setColor(getForegroundColor());
    graphics->setFont(getFont());
    
    if (mListBox->getListModel() && mListBox->getSelected() >= 0)
    {
      graphics->drawText(mListBox->getListModel()->getElementAt(mListBox->getSelected()), 2, 2);
    }

    drawButton(graphics);
            
    if (mDroppedDown)
    {
      graphics->pushClipArea(mScrollArea->getDimension());
      mScrollArea->draw(graphics);
      graphics->popClipArea();
    }
  }

  void DropDown::drawButton(Graphics *graphics)
  {
    Color faceColor, highlightColor, shadowColor;
    int offset;

    if (mPushed)
    {
      faceColor = getBaseColor() - 0x303030;
      highlightColor = faceColor - 0x303030;
      shadowColor = faceColor + 0x303030;
      offset = 1;
    }
    else
    {
      faceColor = getBaseColor();
      highlightColor = faceColor + 0x303030;
      shadowColor = faceColor - 0x303030;
      offset = 0;
    }

    int h;
    if (mDroppedDown)
    {
      h = mOldH - 2;
    }
    else
    {
      h = getHeight() - 2;
    }
    int x = getWidth() - h - 1;
    int y = 1;    

    graphics->setColor(faceColor);
    graphics->fillRectangle(Rectangle(x+1, y+1, h-2, h-2));

    graphics->setColor(highlightColor);
    graphics->drawLine(x, y, x+h-1, y);
    graphics->drawLine(x, y+1, x, y+h-1);

    graphics->setColor(shadowColor);
    graphics->drawLine(x+h-1, y+1, x+h-1, y+h-1);
    graphics->drawLine(x+1, y+h-1, x+h-2, y+h-1);

    graphics->setColor(getForegroundColor());
    
    int i;
    int hh = h / 3;
    int hx = x + h / 2;
    int hy = y + (h * 2) / 3;
    for (i=0; i<hh; i++)
    {
      graphics->drawLine(hx - i + offset,
                         hy - i + offset,
                         hx + i + offset,
                         hy - i + offset);
    }     
  }
    
  int DropDown::getSelected()
  {
    return mListBox->getSelected();
  }
    
  void DropDown::setSelected(int selected)
  {
    if (selected >= 0)
    {
      mListBox->setSelected(selected);
    }
  }

  void DropDown::keyPress(const Key& key)
  {
    
  }
  
  void DropDown::mousePress(int x, int y, int button)
  {
    if (button == MouseInput::LEFT && hasMouse() && !mDroppedDown)
    {      
      mPushed = true;
      dropDown();
    }    
  }

  void DropDown::mouseRelease(int x, int y, int button)
  {
    if (button == MouseInput::LEFT)
    {      
      mPushed = false;
    }

  } // end mouseRelease
  
  void DropDown::setListModel(ListModel *listModel)
  {
    mListBox->setListModel(listModel);
    
    if (mListBox->getSelected() < 0)
    {
      mListBox->setSelected(0);
    }
    
    adjustHeight();
  }
  
  ListModel *DropDown::getListModel()
  {
    return mListBox->getListModel();
  }

  void DropDown::setScrollArea(ScrollArea *scrollArea)
  {
    mScrollArea->_setFocusHandler(NULL);
    mScrollArea = scrollArea;
    mScrollArea->_setFocusHandler(&mFocusHandler);
    mScrollArea->setContent(mListBox);
    adjustHeight();
  }

  ScrollArea *DropDown::getScrollArea()
  {
    return mScrollArea;
  }
  
  void DropDown::setListBox(ListBox *listBox)
  {
    listBox->setSelected(mListBox->getSelected());
    listBox->setListModel(mListBox->getListModel());
    mListBox = listBox;
    
    mScrollArea->setContent(mListBox);
    
    if (mListBox->getSelected() < 0)
    {
      mListBox->setSelected(0);
    }
  }

  ListBox *DropDown::getListBox()
  {
    return mListBox;
  }
  
  void DropDown::adjustHeight()
  {
    int listBoxHeight = mListBox->getHeight();
    int h2 = getFont()->getHeight() + 2;

    setHeight(h2);
    
    if (mDroppedDown && getParent())
    {
      int h = getParent()->getHeight() - getY();
      
      if (listBoxHeight + 2 > h - h2)
      {
        mScrollArea->setHeight(h - h2);
        setHeight(h);
      }
      else
      {
        setHeight(listBoxHeight + h2 + 2);
        mScrollArea->setHeight(listBoxHeight + 2);
      }
    }

    mScrollArea->setWidth(getWidth());
    mScrollArea->setPosition(0, h2);
  }

  void DropDown::dropDown()
  {
    if (!mDroppedDown)
    {
      mDroppedDown = true;
      mOldH = getHeight();
      adjustHeight();
    }
  }

  void DropDown::foldUp()
  {
    if (mDroppedDown)
    {
      mDroppedDown = false;
      mFocusHandler.focusNone();
      adjustHeight();
    }
  }

  void DropDown::_mouseInputMessage(const MouseInput &mouseInput)
  {
    BasicContainer::_mouseInputMessage(mouseInput);
    
    if (mDroppedDown)
    {
      if (mouseInput.y >= mOldH)
      {
        MouseInput mi = mouseInput;
        mi.y -= mScrollArea->getY();        
        mScrollArea->_mouseInputMessage(mi);
      }
    }
  }

  void DropDown::lostFocus()
  {
    foldUp();
  }
  
} // end gcn

