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

#include "guichan/sdl/sdlinput.hpp"

namespace gcn
{
  void SDLInput::init()
  {
    SDL_EnableUNICODE(1);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

  } // end init
  
  bool SDLInput::isKeyQueueEmpty()
  {
    return mKeyInputQueue.empty();

  } // end isKeyQueueEmpty
  
  KeyInput SDLInput::dequeueKeyInput()
  {
    KeyInput keyInput;
    
    if (mKeyInputQueue.empty())
    {
      // TODO
      // Add exception instead of return statement
      keyInput.setType(KeyInput::EMPTY);
      return keyInput;
    }
    
    keyInput = mKeyInputQueue.front();
    mKeyInputQueue.pop();

    return keyInput;
    
  } // end dequeueKeyInput

  bool SDLInput::isMouseQueueEmpty()
  {
    return mMouseInputQueue.empty();

  } // end isMouseQueueEmpty

  MouseInput SDLInput::dequeueMouseInput()
  {
    MouseInput mouseInput;
    
    if (mMouseInputQueue.empty())
    {
      // TODO
      // Add exception instead of return statement
      mouseInput.setType(MouseInput::EMPTY);
      return mouseInput;
    }
    
    mouseInput = mMouseInputQueue.front();
    mMouseInputQueue.pop();

    return mouseInput;
    
  } // end dequeueMouseInput
  
  int SDLInput::getMouseX()
  {
    return mMouseX;

  } // end getMouseX 

  int SDLInput::getMouseY()
  {
    return mMouseY;

  } // end getMouseY

  void SDLInput::getMousePosition(int& x, int& y)
  {
    x = mMouseX;
    y = mMouseY;

  } // end getMousePosition

  void SDLInput::pollInput()
  {
    mMouseMotion = false;

  } // end pollInput
  
  bool SDLInput::isMouseMoved()
  {
    return mMouseMotion;

  } // end isMouseMoved

  void SDLInput::pushInput(SDL_Event event)
  {
    Key key;
    KeyInput keyInput;
    MouseInput mouseInput;
    
    switch (event.type)
    {
      case SDL_KEYDOWN:
        key.setAscii(convertKeyCharacter(event.key.keysym.unicode));
        keyInput.setKey(key);
        keyInput.setType(KeyInput::PRESS);        
        mKeyInputQueue.push(keyInput);
        break;
      case SDL_KEYUP:
        key.setAscii(convertKeyCharacter(event.key.keysym.unicode));
        keyInput.setKey(key);
        keyInput.setType(KeyInput::RELEASE);
        mKeyInputQueue.push(keyInput);
        break;
      case SDL_MOUSEBUTTONDOWN:
        mouseInput.x = event.button.x;
        mouseInput.y = event.button.y;
        mouseInput.setButton(convertMouseButton(event.button.button));
        mouseInput.setType(MouseInput::PRESS);
        mMouseInputQueue.push(mouseInput);
        break;
      case SDL_MOUSEBUTTONUP:
        mouseInput.x = event.button.x;
        mouseInput.y = event.button.y;
        mouseInput.setButton(convertMouseButton(event.button.button));
        mouseInput.setType(MouseInput::RELEASE);
        mMouseInputQueue.push(mouseInput);
        break;
      case SDL_MOUSEMOTION:
        mouseInput.x = event.button.x;
        mouseInput.y = event.button.y;
        mouseInput.setButton(MouseInput::EMPTY);
        mouseInput.setType(MouseInput::MOTION);
        mMouseInputQueue.push(mouseInput);
        mMouseX = event.button.x;
        mMouseY = event.button.y;
        mMouseMotion = true;
        break;
    } // end switch

  } // end pushInput
  
  int SDLInput::convertMouseButton(int button)
  {
    switch (button)
    {
      case SDL_BUTTON_LEFT:
        return MouseInput::LEFT;
        break;
      case SDL_BUTTON_RIGHT:
        return MouseInput::RIGHT;
        break;
      case SDL_BUTTON_MIDDLE:
        return MouseInput::MIDDLE;
        break;
      case SDL_BUTTON_WHEELUP:
        return MouseInput::WHEEL_UP;
        break;
      case SDL_BUTTON_WHEELDOWN:
        return MouseInput::WHEEL_DOWN;
        break;
    } 

    //TODO: Exception!
    
    return 0;

  } // end convertMouseButton

  unsigned char SDLInput::convertKeyCharacter(unsigned int unicode)
  {
    if (unicode > 255)
    {
      return 0;
    }
    else
    {
      return (unsigned char)unicode;
    }    

  } // end convertKeyCharacter

} // end gcn
