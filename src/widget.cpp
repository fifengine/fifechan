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

/*
 * For comments regarding functions please see the header file. 
 */

#include "guichan/widget.hpp"
#include "guichan/focushandler.hpp"
#include <iostream>

namespace gcn
{
  Widget::Widget()
  {
    mParent = NULL;
    mForegroundColor = Color(255,255,255);
    mBackgroundColor = Color(128,128,128);
    mFocusHandler = NULL;
    mMouseListener = NULL;
    mKeyListener = NULL;
    mActionListener = NULL;
    mFocusable = false;
    mClickTimeStamp = 0;
    mClickCount = 0;
    mHasMouse = false;
    mVisible = true;
    mTabable = true;
    
  } // end Widget

  void Widget::_setParent(Widget* parent)
  {
    mParent = parent;

  } // end _setParent

  Widget* Widget::getParent() const
  {
    return mParent;
    
  } // end getParent

  void Widget::setWidth(int width)
  {
    mDimension.width = width;

  } // end setWidth

  int Widget::getWidth()
  {
    return mDimension.width;

  } // end getWidth

  void Widget::setHeight(int height)
  {
    mDimension.height = height;

  } // end getHeight

  int Widget::getHeight()
  {
    return mDimension.height;

  } // end getHeight

  void Widget::setX(int x)
  {
    mDimension.x = x;

  } // end setX

  int Widget::getX()
  {
    return mDimension.x;

  } // end getX

  void Widget::setY(int y)
  {
    mDimension.y = y;

  } // end setY

  int Widget::getY()
  {
    return mDimension.y;

  } // end getY
  
  void Widget::setPosition(int x, int y)
  {
    mDimension.x = x;
    mDimension.y = y;

  } // end setPosition

  void Widget::setDimension(const Rectangle& dimension)
  {
    mDimension = dimension;

  } // end setDimension

  const Rectangle& Widget::getDimension()
  {
    return mDimension;

  } // end getDimension
  
  const std::string& Widget::getEventId() const
  {
    return mEventId;

  } // end getEventId

  void Widget::setEventId(const std::string& eventId)
  {
    mEventId = eventId;

  } // end setEventId
  
  bool Widget::hasFocus()
  {
    return (mFocusHandler->hasFocus(this));
   
  } // end hasFocus

  bool Widget::hasMouse()
  {
    return mHasMouse;

  } // end hasMouse

  void Widget::setFocusable(bool focusable)
  {
    if (!focusable && hasFocus())
    {
      mFocusHandler->focusNone();
    }
    
    mFocusable = focusable;

  } // end setFocusable

  bool Widget::isFocusable()
  {
    return mFocusable && mVisible;

  } // end isFocusable
  
  void Widget::requestFocus()
  {
    if (isFocusable())
    {
      mFocusHandler->requestFocus(this);
    }

  } // end requestFocus

  void Widget::setVisible(bool visible)
  {
    if (!visible && hasFocus())
    {
      mFocusHandler->focusNone();
    }    
    mVisible = visible;

  } // end setVisible
  
  bool Widget::isVisible()
  {
    return mVisible;

  } // end isVisible

  void Widget::setForegroundColor(const Color& color)
  {
    mForegroundColor = color;

  } // end setForegroundColor

  const Color& Widget::getForegroundColor() const
  {
    return mForegroundColor;

  } // end getForegroundColor
  
  void Widget::setBackgroundColor(const Color& color)
  {
    mBackgroundColor = color;

  } // setBackgroundColor

  const Color& Widget::getBackgroundColor() const
  {
    return mBackgroundColor;

  } // end getBackgroundColor

  const std::string& Widget::getMouseType() const
  {
    return mMouseType;

  } // end getMouseType

  void Widget::setMouseType(const std::string &mouseType)
  {
    mMouseType = mouseType;

  } // end setMouseType

  void Widget::_setFocusHandler(FocusHandler* focusHandler)
  {
    if (mFocusHandler)
    {
      mFocusHandler->remove(this);
    }
    
    if (focusHandler)
    {
      focusHandler->add(this);
    }
    
    mFocusHandler = focusHandler;

  } // end _setFocusHandler

  void Widget::setActionListener(ActionListener* actionListener)
  {
    mActionListener = actionListener;

  } // end setActionListener

  void Widget::setKeyListener(KeyListener* keyListener)
  {
    mKeyListener = keyListener;

  } // end setKeyListener
  
  void Widget::_mouseInputMessage(const MouseInput& mouseInput)
  {
    int x = mouseInput.x;
    int y = mouseInput.y;
    int b = mouseInput.getButton();
    int ts = mouseInput.getTimeStamp();
      
    switch(mouseInput.getType())
    {
      case MouseInput::MOTION:
        mouseMotionMessage(x, y);
        if (mMouseListener)
        {
          mMouseListener->mouseMotionEvent(mEventId, x, y);
        }
        break;
        
      case MouseInput::PRESS:
        requestFocus();
        
        if (b != MouseInput::WHEEL_UP && b != MouseInput::WHEEL_DOWN)
        {
          mousePressMessage(x, y, b);
          if (mMouseListener)
          {
            mMouseListener->mousePressEvent(mEventId, x, y, b);
          }

          // TODO: fajntjuna det här
          if (ts - mClickTimeStamp < 100 && mClickButton == b)
          {
            mClickCount++;
          }
          else
          {
            mClickCount = 0;
          }
          mClickButton = b;
          mClickTimeStamp = ts;
        }
        else if (b == MouseInput::WHEEL_UP)
        {
          mouseWheelUpMessage(x, y);
          if (mMouseListener)
          {
            mMouseListener->mouseWheelUpEvent(mEventId, x, y);
          }                    
        }
        else
        {
          mouseWheelDownMessage(x, y);
          if (mMouseListener)
          {
            mMouseListener->mouseWheelDownEvent(mEventId, x, y);
          }                    
        }
        break;

      case MouseInput::RELEASE:
        if (b != MouseInput::WHEEL_UP && b != MouseInput::WHEEL_DOWN)
        {
          mouseReleaseMessage(x, y, b);
          if (mMouseListener)
          {
            mMouseListener->mouseReleaseEvent(mEventId, x, y, b);
          }
        }

        if (mHasMouse)
        {
          if (b == mClickButton)
          {
            mouseClickMessage(x, y, b, mClickCount+1);
            if (mMouseListener)
            {
              mMouseListener->mouseClickEvent(mEventId, x, y, b, mClickCount+1);
            }
          }
          else
          {
            mouseClickMessage(x, y, b, 1);
            if (mMouseListener)
            {
              mMouseListener->mouseClickEvent(mEventId, x, y, b, 1);
            }
            mClickCount = 0;            
          }
        }
        else
        {
          mClickCount = 0;
          mClickTimeStamp = 0;
        }
        break;
    }

  } // end _mouseInputMessage

  void Widget::_keyInputMessage(const KeyInput& keyInput)
  {
    switch(keyInput.getType())
    {
      case KeyInput::PRESS:
        keyPressMessage(keyInput.getKey());
        if (mKeyListener)
        {
          mKeyListener->keyPressEvent(mEventId, keyInput.getKey());
        }           
        break;
        
      case KeyInput::RELEASE:
        keyReleaseMessage(keyInput.getKey());
        if (mKeyListener)
        {
          mKeyListener->keyReleaseEvent(mEventId, keyInput.getKey());
        }        
        break;
    }

  } // end _keyInputMessage

  void Widget::_mouseInMessage()
  {
    mHasMouse = true;
    mouseInMessage();

  } // end _mouseInMessage

  void Widget::_mouseOutMessage()
  {
    mHasMouse = false;
    mouseOutMessage();

  } // end _mouseOutMessage

  void Widget::getAbsolutePosition(int& x, int& y)
  {
    if (getParent() == NULL)
    {
      x = mDimension.x;
      y = mDimension.y;
      return;
    }

    int parentX;
    int parentY;

    getParent()->getAbsolutePosition(parentX, parentY);

    x = parentX + mDimension.x;
    y = parentY + mDimension.y;
    
  } // end getAbsolutDimension

  bool Widget::isTabable() const
  {
    return mTabable;
    
  } // end isTabable

  void Widget::setTabable(bool tabable)
  {
    mTabable = tabable;
    
  } // end setTabable
  
} // end gcn
