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

#ifndef GCN_SCROLLAREA_HPP
#define GCN_SCROLLAREA_HPP

#include "guichan/basiccontainer.hpp"
#include "guichan/mouselistener.hpp"

#include <string>

namespace gcn
{
  /**
   * This is a scroll area
   */
  class ScrollArea: public BasicContainer, public MouseListener
  {
  public:
    /**
     *
     */
    ScrollArea();

    /**
     *
     */    
    ScrollArea(Widget *content);

    /**
     *
     */    
    ScrollArea(Widget *content, unsigned int hPolicy, unsigned int vPolicy);

    /**
     *
     */    
    virtual ~ScrollArea() { };

    /**
     *
     */    
    virtual void setContent(Widget* widget);

    /**
     *
     */    
    virtual Widget* getContent();

    /**
     *
     */    
    virtual void setHorizontalScrollPolicy(unsigned int hPolicy);

    /**
     *
     */    
    virtual unsigned int getHorizontalScrollPolicy();

    /**
     *
     */    
    virtual void setVerticalScrollPolicy(unsigned int vPolicy);

    /**
     *
     */    
    virtual unsigned int getVerticalScrollPolicy();

    /**
     *
     */    
    virtual void setScrollPolicy(unsigned int hPolicy, unsigned int vPolicy);

    /**
     *
     */    
    virtual void setVerticalScrollAmount(int vScroll);

    /**
     *
     */    
    virtual int getVerticalScrollAmount();

    /**
     *
     */    
    virtual void setHorizontalScrollAmount(int hScroll);

    /**
     *
     */    
    virtual int getHorizontalScrollAmount();

    /**
     *
     */    
    virtual void setScrollAmount(int hScroll, int vScroll);

    /**
     *
     */    
    virtual int getHorizontalMaxScroll();

    /**
     *
     */    
    virtual int getVerticalMaxScroll();

    /**
     *
     */
    virtual void setScrollbarWidth(int width);

    /**
     *
     */
    virtual int getScrollbarWidth();

    virtual void _setFocusHandler(FocusHandler* focusHandler);

    virtual void _mouseInputMessage(const MouseInput &mouseInput);

    virtual void _mouseOutMessage();

    virtual void mousePress(int x, int y, int button);
    
    virtual void mouseRelease(int x, int y, int button);
    
    virtual void mouseMotion(int x, int y);

    virtual void mouseWheelUp(int x, int y);

    virtual void mouseWheelDown(int x, int y);
    
    virtual void draw(Graphics *graphics);

    /**
     *
     */
    virtual void drawUpButton(Graphics *graphics);
    
    /**
     *
     */
    virtual void drawDownButton(Graphics *graphics);

    /**
     *
     */
    virtual void drawLeftButton(Graphics *graphics);
    
    /**
     *
     */
    virtual void drawRightButton(Graphics *graphics);

    virtual void logic();

    virtual void moveToTop(Widget* widget);

    virtual void moveToBottom(Widget* widget);

    virtual void getDrawSize(int& width, int& height, Widget* widget);

    /**
     *
     */
    virtual void scrollToRectangle(const Rectangle& rectangle);

    /**
     *
     */
    enum 
    {
      SHOW_ALWAYS,
      SHOW_NEVER,
      SHOW_AUTO
    };
    
  protected:
    /**
     *
     */
    virtual void drawContent(Graphics* graphics);

    /**
     *
     */
    virtual void drawVBar(Graphics* graphics);

    /**
     *
     */
    virtual void drawHBar(Graphics* graphics);

    virtual void drawVMarker(Graphics* graphics);

    virtual void drawHMarker(Graphics* graphics);

    /**
     *
     */
    virtual void checkPolicies();

    /**
     *
     */
    virtual Rectangle getUpButtonDimension();

    /**
     *
     */
    virtual Rectangle getDownButtonDimension();

    /**
     *
     */
    virtual Rectangle getLeftButtonDimension();

    /**
     *
     */
    virtual Rectangle getRightButtonDimension();

    /**
     *
     */
    virtual Rectangle getContentDimension();

    /**
     *
     */
    virtual Rectangle getVerticalBarDimension();

    /**
     *
     */
    virtual Rectangle getHorizontalBarDimension();

    /**
     *
     */
    virtual Rectangle getVerticalMarkerDimension();
    
    /**
     *
     */
    virtual Rectangle getHorizontalMarkerDimension();
    
    
    Widget *mContent;
    int mVScroll;
    int mHScroll;
    int mScrollbarWidth;
    unsigned int mHPolicy;
    unsigned int mVPolicy;
    bool mouseOverContent;
    bool mVBarVisible;
    bool mHBarVisible;
    bool mUpButtonPressed;
    bool mDownButtonPressed;
    bool mLeftButtonPressed;
    bool mRightButtonPressed;
    bool mVerticalMarkerPressed;
    int mVerticalMarkerMousePosition;
    bool mHorizontalMarkerPressed;
    int mHorizontalMarkerMousePosition;

    bool mContentHasMouse;

  }; // end ScrollArea

} // end gcn

#endif // end GCN_SCROLLAREA_HPP
