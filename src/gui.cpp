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

#include "guichan/gui.hpp"
#include "guichan/focushandler.hpp"
#include "guichan/exception.hpp"

#include <iostream>

namespace gcn
{
  
  Gui::Gui()
  {
    mTop = NULL;
    mInput = NULL;
    mGraphics = NULL;
    mFocusHandler = new FocusHandler();

  } // end Gui

  Gui::~Gui()
  {
    delete mFocusHandler;

  } // end ~Gui
  
  void Gui::setTop(Widget* top)
  {
    if (mTop)
    {
      mTop->_setFocusHandler(NULL);
    }
    if (top)
    {
      top->_setFocusHandler(mFocusHandler);
    }
    
    mTop = top;

  } // end setTop

  Widget* Gui::getTop() const
  {
    return mTop;

  } // end getTop

  void Gui::setGraphics(Graphics* graphics)
  {
    mGraphics = graphics;

  } // end setGraphics

  Graphics* Gui::getGraphics() const
  {
    return mGraphics;

  } // end getGraphics

  void Gui::setInput(Input* input)
  {
    mInput = input;

  } // end setInput
  
  Input* Gui::getInput() const
  {
    return mInput;

  } // end getInput

  void Gui::logic()
  {
    if (!mTop)
    {
      throw GCN_EXCEPTION("Gui::logic. No top widget set");
    }

    if(mInput)
    {
      while (!mInput->isMouseQueueEmpty())
      {
        //TODO: adjust x and y depending on top position
        MouseInput mi = mInput->dequeueMouseInput();
        //TODO: adjust x and y depending on widget position
        if (mFocusHandler->getFocused())
        {
          int xOffset, yOffset;
          mFocusHandler->getFocused()->getAbsolutePosition(xOffset, yOffset);
          MouseInput mio = mi;
          mio.x -= xOffset;
          mio.y -= yOffset;
          mFocusHandler->getFocused()->_mouseInputMessage(mio);
        }
        
        mTop->_mouseInputMessage(mi);

      }
    }
    mTop->logic();

  } // end logic

  void Gui::draw()
  {
    if (!mTop)
    {
      throw GCN_EXCEPTION("Gui::draw. No top widget set");
    }
    if (!mGraphics)
    {
      throw GCN_EXCEPTION("Gui::draw. No graphics set");
    }

    mGraphics->_beginDraw();
    mGraphics->pushClipArea(mTop->getDimension());    
    mTop->draw(mGraphics);
    mGraphics->popClipArea();
    mGraphics->_endDraw();
    
  } // end draw
  
} // end gcn
