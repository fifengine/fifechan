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

#ifndef GCN_FONT_HPP
#define GCN_FONT_HPP

#include "guichan/graphics.hpp"

namespace gcn
{
  /**
   * This is the abstract base class for fonts. It can be overloaded
   * to represent any type of font.
   *
   * @see ImageFont
   */
  class Font
  {
  public:

    /**
     * Destructor.
     */
    virtual ~Font(){}

    /**
     * Gets a width of a glyph.
     *
     * @param glyph the glyph which width will be returned
     * @return the width of a glyph 
     */
    virtual int getWidth(unsigned char glyph) const = 0;

    /**
     * Gets the width of a string. The width of a string is not necesserily
     * the sum of all the widths of its glyphs. 
     *
     * @param text the string of which width will be returned
     * @return the width of a string 
     */
    virtual int getWidth(const std::string& text) const;

    /**
     * Gets the height of the glyphs.
     *
     * @return the height of the glyphs
     */
    virtual int getHeight() const = 0;

    /**
     * Use this function to retrive a string index (for a character in a
     * string) at a certain x position. This function is especially useful
     * when a mouse clicks in a TextField and you want to know which
     * character was clicked.
     *
     *
     * @return the string index at coordinate x.
     */
    virtual int getStringIndexAt(const std::string& text, int x);
    
    /**
     * Draws a glyph.
     *
     * NOTE: You normally won't use this function to draw text since
     *       the Graphics class contains better functions for drawing
     *       text.
     *
     * @param graphics a graphics object to be used for drawing
     * @param glyph a glyph to draw
     * @param x the x coordinate where to draw the glyph
     * @param y the y coordinate where to draw the glyph
     * @see Graphics
     */
    virtual int drawGlyph(Graphics* graphics, unsigned char glyph, int x, int y) = 0;
    
    /**
     * Draws a string.
     * 
     * NOTE: You normally won't use this function to draw text since
     *       the Graphics class contains better functions for drawing
     *       text.
     *
     * @param graphics a graphics object to be used for drawing
     * @param text the string to draw
     * @param x the x coordinate where to draw the string
     * @param y the y coordinate where to draw the string
     * @see Graphics
     */
    virtual void drawString(Graphics* graphics, const std::string& text, const int x, const int y);    

  }; // end Font
  
} // end gcn

#endif // end GCN_FONT_HPP
