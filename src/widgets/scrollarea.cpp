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

#include "guichan/widgets/scrollarea.hpp"
#include "guichan/exception.hpp"

namespace gcn
{

  ScrollArea::ScrollArea()
  {
    mVScroll = 0;
    mHScroll = 0;
    mHPolicy = SHOW_ALWAYS;
    mVPolicy = SHOW_ALWAYS;
    mouseOverContent = false;
    mVBarVisible = true;
    mVBarVisible = true;
    mScrollBarWidth = 12;
    mContent = NULL;
  }

  ScrollArea::ScrollArea(Widget *content)
  {
    mVScroll = 0;
    mHScroll = 0;
    mHPolicy = SHOW_ALWAYS;
    mVPolicy = SHOW_ALWAYS;
    mouseOverContent = false;
    mVBarVisible = true;
    mVBarVisible = true;
    mScrollBarWidth = 12;
    mContent = content;
  }

  ScrollArea::ScrollArea(Widget *content, unsigned int hPolicy, unsigned int vPolicy)
  {
    mVScroll = 0;
    mHScroll = 0;
    mHPolicy = hPolicy;
    mVPolicy = vPolicy;
    mouseOverContent = false;
    mScrollBarWidth = 12;
    mContent = content;
    checkPolicies();
  }

  ScrollArea::~ScrollArea()
  {

  }

  void ScrollArea::setContent(Widget* widget)
  {
    mContent = widget;
  }

  Widget* ScrollArea::getContent()
  {
    return mContent;
  }

  void ScrollArea::setHorizontalScrollPolicy(unsigned int hPolicy)
  {
    mHPolicy = hPolicy;
    checkPolicies();
  }

  unsigned int ScrollArea::getHorizontalScrollPolicy()
  {
    return mHPolicy;
  }

  void ScrollArea::setVerticalScrollPolicy(unsigned int vPolicy)
  {
    mVPolicy = vPolicy;
    checkPolicies();
  }

  unsigned int ScrollArea::getVerticalScrollPolicy()
  {
    return mVPolicy;
  }

  void ScrollArea::setScrollPolicy(unsigned int hPolicy, unsigned int vPolicy)
  {
    mHPolicy = hPolicy;
    mVPolicy = vPolicy;
    checkPolicies();
  }

  void ScrollArea::setVerticalScrollAmount(int vScroll)
  {
    int max = getVerticalMaxScroll();
    
    if (vScroll <= max)
    {
      mVScroll = vScroll;
    }
    else if (vScroll < 0)
    {
      mVScroll = 0;
    }
    else
    {
      mVScroll = max;
    }
  }

  int ScrollArea::getVerticalScrollAmount()
  {
    return mVScroll;
  }

  void ScrollArea::setHorizontalScrollAmount(int hScroll)
  {
    int max = getHorizontalMaxScroll();
    
    if (hScroll <= max)
    {
      mHScroll = hScroll;
    }
    else if (hScroll < 0)
    {
      mHScroll = 0;
    }
    else
    {
      mHScroll = max;
    }
  }

  int ScrollArea::getHorizontalScrollAmount()
  {
    return mHScroll;
  }

  void ScrollArea::setScrollAmount(int hScroll, int vScroll)
  {
    setHorizontalScrollAmount(hScroll);
    setVerticalScrollAmount(vScroll);
  }

  int ScrollArea::getHorizontalMaxScroll()
  {
    checkPolicies();

    if (mContent == NULL)
    {
      return 0;
    }

    int value;
    
    if (mVBarVisible)
    {
      value = mContent->getWidth() - getWidth() + mScrollBarWidth;
    }
    else
    {
      value = mContent->getWidth() - getWidth();
    }

    if (value < 0)
    {
      return 0;
    }

    return value;
  }

  int ScrollArea::getVerticalMaxScroll()
  {
    checkPolicies();

    if (mContent == NULL)
    {
      return 0;
    }

    int value;
    
    if (mHBarVisible)
    {
      value = mContent->getHeight() - getHeight() + mScrollBarWidth;
    }
    else
    {
      value = mContent->getHeight() - getHeight();
    }

    if (value < 0)
    {
      return 0;
    }

    return value;
  }

  void ScrollArea::setScrollbarWidth(int width)
  {
    if (width > 0)
    {
      mScrollBarWidth = width;
    }
    else
    {
      throw GCN_EXCEPTION("ScrollArea::setScrollbarWidth. width should be greater than 0");
    }
  }

  int ScrollArea::getScrollbarWidth()
  {
    return mScrollBarWidth;
  }

  void ScrollArea::_setFocusHandler(FocusHandler* focusHandler)
  {
    BasicContainer::_setFocusHandler(focusHandler);

    if (mContent)
    {
      mContent->_setFocusHandler(focusHandler);
    }
  }

  void ScrollArea::_mouseInputMessage(const MouseInput &mouseInput)
  {
    BasicContainer::_mouseInputMessage(mouseInput);

    // todo: fix
  }

  void ScrollArea::_mouseOutMessage()
  {
    BasicContainer::_mouseOutMessage();

    // todo: fix
  }
    
  void ScrollArea::draw(Graphics *graphics)
  {
    graphics->setColor(getBackgroundColor());
    graphics->fillRectangle(Rectangle(0, 0, getWidth(), getHeight()));

    Color lightColor = getBaseColor() + 0x303030;
    Color shadowColor = getBaseColor() - 0x303030;      

    graphics->setColor(shadowColor);
    graphics->drawLine(0, 0, getWidth() - 1, 0);
    graphics->drawLine(0, 1, 0, getHeight() - 1);
    
    graphics->setColor(lightColor);
    graphics->drawLine(getWidth() - 1, 1, getWidth() - 1, getHeight() - 1);
    graphics->drawLine(1, getHeight() - 1, getWidth() - 1, getHeight() - 1);
    
    if (mVBarVisible && mHBarVisible)
    {
      drawVBar(graphics,
               Rectangle(getWidth() - mScrollBarWidth - 1, 1,
                         mScrollBarWidth, getHeight() - 2 - mScrollBarWidth),
               Rectangle(getWidth() - mScrollBarWidth - 1, mScrollBarWidth,
                         mScrollBarWidth, (getHeight() - 2 - mScrollBarWidth*3) / 2));      
      drawHBar(graphics,
               Rectangle(1, getHeight() - mScrollBarWidth - 1,
                         getWidth() - 2 - mScrollBarWidth, mScrollBarWidth), 
               Rectangle(mScrollBarWidth, getHeight() - mScrollBarWidth - 1,
                         (getWidth() - 2 - mScrollBarWidth*3) / 2 , mScrollBarWidth));      
    }
    else if (mVBarVisible && !mHBarVisible)
    {
      drawVBar(graphics,
               Rectangle(getWidth() - mScrollBarWidth - 1, 1,
                         mScrollBarWidth, getHeight() - 2),
               Rectangle(getWidth() - mScrollBarWidth - 1, 1,
                         mScrollBarWidth , (getHeight() - 2) / 2));
    }
    else if (mHBarVisible && !mVBarVisible)
    {
      drawHBar(graphics,
               Rectangle(1, getHeight() - mScrollBarWidth - 1,
                         getWidth() - 2, mScrollBarWidth), 
               Rectangle(1, getHeight() - mScrollBarWidth - 1,
                         (getWidth() - 2 ) / 2, mScrollBarWidth));
    }


    if (mVBarVisible)
    {
      graphics->setColor(lightColor);
      graphics->drawLine(getWidth() - mScrollBarWidth - 1, 1,
                         getWidth(), 1);
      graphics->drawLine(getWidth() - mScrollBarWidth - 1, 1,
                         getWidth() - mScrollBarWidth, mScrollBarWidth);
      
      graphics->setColor(shadowColor);
      graphics->drawLine(getWidth() - 2, 1,
                         getWidth() - 1, mScrollBarWidth);
      graphics->drawLine(getWidth() - mScrollBarWidth, mScrollBarWidth - 1,
                         getWidth() - 2, mScrollBarWidth - 1);
      
      graphics->setColor(getForegroundColor());
      
      int i;
      int w = mScrollBarWidth / 2;
      int h = w / 2 + 2;
      for (i = 0; i < w / 2; ++i)
      {
        graphics->drawLine(getWidth() - w - i - 1,
                           i + h,
                           getWidth() - w + i - 1,
                           i + h);
      }

      graphics->setColor(lightColor);
      graphics->drawLine(getWidth() - mScrollBarWidth - 1, getHeight() - mScrollBarWidth*2,
                         getWidth(), getHeight() - mScrollBarWidth*2);
//       graphics->drawLine(getWidth() - mScrollBarWidth - 1, getHeight() - mScrollBarWidth,
//                          getWidth() - mScrollBarWidth, getHeight());
      
//       graphics->setColor(shadowColor);
//       graphics->drawLine(getWidth() - 2, getHeight() - mScrollBarWidth*2,
//                          getWidth() - 1, getHeight() - mScrollBarWidth);
//       graphics->drawLine(getWidth() - mScrollBarWidth, getHeight() - 1 - mScrollBarWidth,
//                          getWidth() - 2, getHeight() - 1 - mScrollBarWidth);

      graphics->setColor(getForegroundColor());
      
      h = w / 2 + 2;
      for (i = 0; i < w / 2; ++i)
      {
        graphics->drawLine(getWidth() - w - i - 1,
                           getHeight() -i + h - mScrollBarWidth*2,
                           getWidth() - w + i - 1,
                           getHeight() -i + h - mScrollBarWidth*2);
      }
      
      
    }
  }

  void ScrollArea::logic()
  {
    // todo?
  }

  void ScrollArea::moveToTop(Widget* widget)
  {
    if (widget == mContent)
    {
      if (getParent())
      {
        getParent()->moveToTop(this);
      }
    }
    else
    {
      throw GCN_EXCEPTION("ScrollArea::moveToTop. only a ScrollArea's content may be moved to top");
    }
  }

  void ScrollArea::moveToBottom(Widget* widget)
  {
    if (widget == mContent)
    {
      if (getParent())
      {
        getParent()->moveToBottom(this);
      }
    }
    else
    {
      throw GCN_EXCEPTION("ScrollArea::moveToBottom. only a ScrollArea's content may be moved to bottom");
    }
  }
  
  void ScrollArea::drawContent(Graphics* graphics)
  {
    if (mContent)
    {
      // todo: fix clip
      mContent->draw(graphics);
    }
  }

  void ScrollArea::drawVBar(Graphics* graphics,
                            Rectangle position,
                            Rectangle markerPosition)
  {
    graphics->setColor(getBaseColor());
    graphics->fillRectangle(position);
    graphics->setColor(getForegroundColor());
    graphics->fillRectangle(markerPosition);
  }

  void ScrollArea::drawHBar(Graphics* graphics,
                            Rectangle position,
                            Rectangle markerPosition)
  {
    graphics->setColor(getBaseColor());
    graphics->fillRectangle(position);
    graphics->setColor(getForegroundColor());
    graphics->fillRectangle(markerPosition);
  }

  void ScrollArea::checkPolicies()
  {
    mHBarVisible = false;
    mVBarVisible = false;
    
    if (!mContent)
    {
      mHBarVisible = (mHPolicy == SHOW_ALWAYS);
      mVBarVisible = (mVPolicy == SHOW_ALWAYS);
    }
    
    if (mHPolicy == SHOW_AUTO &&
        mVPolicy == SHOW_AUTO)
    {
      if (mContent->getWidth() <= getWidth()
          && mContent->getHeight() <= getHeight())
      {
        mHBarVisible = false;
        mVBarVisible = false;
      }

      if (mContent->getWidth() > getWidth())
      {
        mHBarVisible = true;
      }

      if ((mContent->getHeight() > getHeight())
          || (mHBarVisible && mContent->getHeight() > getHeight() - mScrollBarWidth))
      {
        mVBarVisible = true;
      }

      if (mVBarVisible && mContent->getWidth() > getWidth() - mScrollBarWidth)
      {
        mHBarVisible = true;
      }
      
      return;
    }
    
    switch (mHPolicy)
    {
      case SHOW_NEVER:
        mHBarVisible = false;
        break;

      case SHOW_ALWAYS:
        mHBarVisible = true;
        break;

      case SHOW_AUTO:
        if (mVPolicy == SHOW_NEVER)
        {
          mHPolicy = mContent->getWidth() > getWidth();
        }
        else // (mVPolicy == SHOW_ALWAYS)
        {
          mHPolicy = mContent->getWidth() > getWidth() - mScrollBarWidth;
        }
        break;

      default:
        throw GCN_EXCEPTION("ScrollArea::checkPolicies. Horizontal scroll policy invalid");
    }

    switch (mVPolicy)
    {
      case SHOW_NEVER:
        mVBarVisible = false;
        break;

      case SHOW_ALWAYS:
        mVBarVisible = true;
        break;

      case SHOW_AUTO:
        if (mHPolicy == SHOW_NEVER)
        {
          mVPolicy = mContent->getHeight() > getHeight();
        }
        else // (mHPolicy == SHOW_ALWAYS)
        {
          mVPolicy = mContent->getHeight() > getHeight() - mScrollBarWidth;
        }

      default:
        throw GCN_EXCEPTION("ScrollArea::checkPolicies. Vertical scroll policy invalid");
    }
  }
}
