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

#ifndef GCN_KEY_HPP
#define GCN_KEY_HPP

#include <string>
#include "guichan/platform.hpp"

namespace gcn
{
  /**
   * This represents a key or a character.
   */
  class DECLSPEC Key
  {
  public:
    /**
     * Constructor.
     */
    Key();
    
    /**
     * Constructor.
     *
     * @param value the ascii or enum value for the key.
     */
    Key(int value);
    
    /**
     * @return true if the key is a letter, number or whitespace.
     */
    bool isCharacter() const;

    /**
     * @return true if the key is a number (0-9).
     */
    bool isNumber() const;
    
    /**
     * @return true if the key is a letter (a-z,A-Z). 
     */
    bool isLetter() const;

    /**
     * @return true if shift was pressed at the same time as the key.
     */
    bool isShiftPressed() const;
    
    /**
     * Sets the shift pressed flag.
     *
     * @param pressed the shift flag value.
     */
    void setShiftPressed(bool pressed);

    /**
     * @return true if control was pressed at the same time as the key.
     */    
    bool isControlPressed() const;
    
    /**
     * Sets the control pressed flag.
     *
     * @param pressed the control flag value.
     */
    void setControlPressed(bool pressed);

    /**
     * @return true if alt was pressed at the same time as the key.
     */
    bool isAltPressed() const;

    /**
     * Sets the alt pressed flag.
     *
     * @param pressed the alt flag value.
     */
    void setAltPressed(bool pressed);

    /**
     * @return true if meta was pressed at the same time as the key.
     */
    bool isMetaPressed() const;

    /**
     * Sets the meta pressed flag.
     *
     * @param pressed the meta flag value.
     */
    void setMetaPressed(bool pressed);
    
    /**
     * @return true if key pressed at the numeric pad.
     */
    bool isNumericPad() const;

    /**
     * Sets the numeric pad flag.
     *
     * @param numpad the numeric pad flag value.
     */
    void setNumericPad(bool numpad);
    
    /**
     * @return the value of the key, an ascii value if exists
     *         otherwise an enum value will be returned.
     */
    int getValue() const;

    /**
     * Sets the value of the key. An ascii value or an enum value.
     *
     * @param value the key value.
     */
    void setValue(int value);

    /**
     * An enum with key values.
     */
    enum
    {
      SPACE              = ' ',
      TAB                = '\t', 
      ENTER              = '\n',   
      LEFT_ALT           = 1000,
      RIGHT_ALT,
      LEFT_SHIFT,
      RIGHT_SHIFT,
      LEFT_CONTROL,
      RIGHT_CONTROL,
      LEFT_META,
      RIGHT_META,
      LEFT_SUPER,
      RIGHT_SUPER,
      INSERT,
      HOME,
      PAGE_UP,
      DELETE,
      END,
      PAGE_DOWN,
      ESCAPE,
      CAPS_LOCK,
      BACKSPACE,
      F1,
      F2,
      F3,
      F4,
      F5,
      F6,
      F7,
      F8,
      F9,
      F10,
      F11,
      F12,
      F13,
      F14,
      F15,
      PRINT_SCREEN,
      SCROLL_LOCK,
      PAUSE,
      NUM_LOCK,
      ALT_GR,
      LEFT,
      RIGHT,
      UP,
      DOWN      
    };
    
  protected:
    int mValue;
    bool mShiftPressed;
    bool mControlPressed;
    bool mAltPressed;
    bool mMetaPressed;
    bool mNumericPad;
    
  }; // end Key

} // end gcn

#endif // end GCN_KEY_HPP
