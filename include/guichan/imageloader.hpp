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

#ifndef GCN_IMAGELOADER_HPP
#define GCN_IMAGELOADER_HPP

#include "guichan/color.hpp"
#include <string>

namespace gcn
{
  class Image;
  
  /**
   * This is an abstract class used to load images in guichan.
   */
  class ImageLoader
  {
  public:
    
    virtual ~ImageLoader() { }

    /**
     * Prepares an image for reading. After you have called this function
     * you can retrieve information about it and edit it.
     *
     * @param filename the image file to prepare
     * @throws Exception when called without having finalized or disposed to
     *                   last image or when unable to load the image.
     * @see load, finalize, discard
     */
    virtual void prepare(const std::string& filename) = 0;

    /**
     * This function frees an image
     *
     * NOTE: There is generally no reason to call this function as
     *       it is called upon by the Image object when destroying an Image.
     *
     * @param filename the file to be freed and removed.
     * @throws Exception when image points to null.
     * @see Image, load
     */
    virtual void free(Image* image) = 0;

    /**
     * This function returns a pointer raw data of an image, the raw
     * data is in 32 bit RGBA format. The funcion will not free a prepared
     * image, so finalize or discard should be used afterwards.
     *
     * @return a pointer to the raw image data
     */
    virtual void* getRawData() = 0;
    
    /**
     * This function finalizes an image meaning it will return the image
     * data.
     *
     * @return a pointer to the image data.
     * @throws Exception when no image has been prepared.
     * @see prepare, discard
     */
    virtual void* finalize() = 0;

    /**
     * This function discards a prepared image.
     *
     * @throws Exception when no image has been prepared.
     * @see prepare, finalize
     */
    virtual void discard() = 0;
    
    /**
     * @return the height of the image.
     * @throws Exception if no image have been prepared.
     */
    virtual int getHeight() const = 0;

    /**
     * @return the width of the image.
     * @throws Exception if no image have been prepared.
     */
    virtual int getWidth() const = 0;

    /**
     * @param x the x coordinate.
     * @param y the y coordinate.
     * @return the color of the pixel.
     */
    virtual Color getPixel(int x, int y) = 0;

    /**
     * @param x the x coordinate.
     * @param y the y coordinate.
     * @param color the color of the pixel to put.
     */
    virtual void putPixel(int x, int y, const Color& color) = 0;

    
  }; // end ImageLoader
  
} // end gcn

#endif // end GCN_IMAGELOADER_HPP
