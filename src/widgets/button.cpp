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

#include "guichan/widgets/button.hpp"
#include "guichan/mouseinput.hpp"
#include <iostream>

namespace gcn
{
  Button::Button()
  {
    addMouseListener(this);
    addKeyListener(this);
    adjustSize();
    
  } // end Button
  
  Button::Button(const std::string& text)
  {
    mText = text;
    setFocusable(true);
    adjustSize();
    
    mMouseDown = false;
    mKeyDown = false;

    addMouseListener(this);
    addKeyListener(this);
    
  } // end Button

  void Button::setText(const std::string& text)
  {
    mText = text;
    
  } // end setText
  
  void Button::draw(Graphics* graphics)
  {
    graphics->setFont(getFont());

    Color faceColor = getBaseColor();
    Color highlightColor, shadowColor;

    if ((hasMouse() && mMouseDown) || mKeyDown)
    {
      faceColor = faceColor - 0x303030;
      highlightColor = faceColor - 0x303030;
      shadowColor = faceColor + 0x303030;      
    }
    else
    {
      highlightColor = faceColor + 0x303030;
      shadowColor = faceColor - 0x303030;      
    }

    graphics->setColor(faceColor);
    graphics->fillRectangle(Rectangle(1, 1, getDimension().width-1, getHeight() - 1));
    
    graphics->setColor(highlightColor);
    graphics->drawLine(0, 0, getWidth() - 1, 0);
    graphics->drawLine(0, 1, 0, getHeight() - 1);
    
    graphics->setColor(shadowColor);
    graphics->drawLine(getWidth() - 1, 1, getWidth() - 1, getHeight() - 1);
    graphics->drawLine(1, getHeight() - 1, getWidth() - 1, getHeight() - 1);

    graphics->setColor(getForegroundColor());
    if ((hasMouse() && mMouseDown) || mKeyDown)
    {
      graphics->drawText(mText, 5, 5);
    }
    else
    {
      graphics->drawText(mText, 4, 4);

      if (hasFocus())
      {
        graphics->drawRectangle(Rectangle(2, 2, getDimension().width - 4, getDimension().height - 4));
      }      
    }
    
  } // end draw
  
  void Button::adjustSize()
  {
    setWidth(getFont()->getWidth(mText) + 8);
    setHeight(getFont()->getHeight() + 8);

  } // end adjustSize

  void Button::mouseClick(int x, int y, int button, int count)
  {
    if (button == MouseInput::LEFT)
    {
      generateAction();
    }

  } // end mouseClick

  void Button::mousePress(int x, int y, int button)
  {
    if (button == MouseInput::LEFT)
    {      
      mMouseDown = true;
    }

  } // end mousePress

  void Button::mouseRelease(int x, int y, int button)
  {
    if (button == MouseInput::LEFT)
    {      
      mMouseDown = false;
    }

  } // end mouseRelease
  
  void Button::keyPress(const Key& key)
  {
    if (key.getValue() == Key::ENTER || key.getValue() == Key::SPACE)
    {
      mKeyDown = true;
   }

  } // end keyPress

  void Button::keyRelease(const Key& key)
  {
    if ((key.getValue() == Key::ENTER || key.getValue() == Key::SPACE) && mKeyDown)
    {
      mKeyDown = false;
      generateAction();
    }

  } // end keyRelease

  void Button::lostFocus()
  {
    mMouseDown = false;
    mKeyDown = false;

  } // end lostFocus
  
} // end gcn
