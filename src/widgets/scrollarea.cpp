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

#include <iostream>

namespace gcn
{
	
	ScrollArea::ScrollArea()
	{
		mVScroll = 0;
		mHScroll = 0;
		mHPolicy = SHOW_AUTO;
		mVPolicy = SHOW_AUTO;
		mouseOverContent = false;
		mVBarVisible = true;
		mHBarVisible = true;
		mScrollbarWidth = 12;
		mContent = NULL;
		mUpButtonPressed = false;
		mDownButtonPressed = false;
		mLeftButtonPressed = false;
		mRightButtonPressed = false;

		mVerticalMarkerPressed = false;
		mVerticalMarkerMousePosition = 0;
		mHorizontalMarkerPressed = false;
		mHorizontalMarkerMousePosition = 0;

		setFocusable(true);
		setTabable(true);
		addMouseListener(this);

	} // end ScrollArea

	ScrollArea::ScrollArea(Widget *content)
	{
		mVScroll = 0;
		mHScroll = 0;
		mHPolicy = SHOW_AUTO;
		mVPolicy = SHOW_AUTO;
		mouseOverContent = false;
		mVBarVisible = true;
		mHBarVisible = true;
		mScrollbarWidth = 12;
		mContent = NULL;
		mUpButtonPressed = false;
		mDownButtonPressed = false;
		mLeftButtonPressed = false;
		mRightButtonPressed = false;
		mVerticalMarkerPressed = false;
		mVerticalMarkerMousePosition = 0;
		mHorizontalMarkerPressed = false;
		mHorizontalMarkerMousePosition = 0;

		setContent(content);
		setFocusable(true);
		setTabable(true);
		checkPolicies();
		addMouseListener(this);

	} // end ScrollArea

	ScrollArea::ScrollArea(Widget *content, unsigned int hPolicy, unsigned int vPolicy)
	{
		mVScroll = 0;
		mHScroll = 0;
		mHPolicy = hPolicy;
		mVPolicy = vPolicy;
		mouseOverContent = false;
		mScrollbarWidth = 12;
		mContent = content;
		mUpButtonPressed = false;
		mDownButtonPressed = false;
		mLeftButtonPressed = false;
		mRightButtonPressed = false;
		mVerticalMarkerPressed = false;
		mVerticalMarkerMousePosition = 0;
		mHorizontalMarkerPressed = false;
		mHorizontalMarkerMousePosition = 0;

		setFocusable(true);
		setTabable(true);
		checkPolicies();
		addMouseListener(this);
    
	} // end ScrollArea

	ScrollArea::~ScrollArea()
	{
		setContent(NULL);
		
	} // end ~ScrollArea
	
	void ScrollArea::setContent(Widget* widget)
	{
		if (mContent)
		{
			mContent->_setFocusHandler(NULL);
			mContent->_setParent(NULL);
		}
    
		mContent = widget;

		if (mContent != NULL)
		{
			mContent->_setFocusHandler(_getFocusHandler());
			mContent->_setParent(this);
		}
	
	} // end setContent
  
	Widget* ScrollArea::getContent()
	{
		return mContent;

	} // end getContent

	void ScrollArea::setHorizontalScrollPolicy(unsigned int hPolicy)
	{
		mHPolicy = hPolicy;
		checkPolicies();

	} // end setHorizontalScrollPolicy

	unsigned int ScrollArea::getHorizontalScrollPolicy()
	{
		return mHPolicy;

	} // end getHorizontalScrollPolicy

	void ScrollArea::setVerticalScrollPolicy(unsigned int vPolicy)
	{
		mVPolicy = vPolicy;
		checkPolicies();

	} // end setVerticalScrollPolicy

	unsigned int ScrollArea::getVerticalScrollPolicy()
	{
		return mVPolicy;

	} // end getVerticalScrollPolicy

	void ScrollArea::setScrollPolicy(unsigned int hPolicy, unsigned int vPolicy)
	{
		mHPolicy = hPolicy;
		mVPolicy = vPolicy;
		checkPolicies();

	} // end setScrollPolicy

	void ScrollArea::setVerticalScrollAmount(int vScroll)
	{
		int max = getVerticalMaxScroll();

		mVScroll = vScroll;
    
		if (vScroll > max)
		{
			mVScroll = max;
		}
    
		if (vScroll < 0)
		{
			mVScroll = 0;
		}

	} // end setVerticalScrollAmount

	int ScrollArea::getVerticalScrollAmount()
	{
		return mVScroll;

	} // end getVerticalScrollAmount

	void ScrollArea::setHorizontalScrollAmount(int hScroll)
	{
		int max = getHorizontalMaxScroll();
    
		mHScroll = hScroll;
	
		if (hScroll > max)
		{
			mHScroll = max;
		}
		else if (hScroll < 0)
		{
			mHScroll = 0;
		}

	} // end setHorizontalScrollAmount

	int ScrollArea::getHorizontalScrollAmount()
	{
		return mHScroll;

	} // end getHorizontalScrollAmount

	void ScrollArea::setScrollAmount(int hScroll, int vScroll)
	{
		setHorizontalScrollAmount(hScroll);
		setVerticalScrollAmount(vScroll);

	} // end setScrollAmount

	int ScrollArea::getHorizontalMaxScroll()
	{
		checkPolicies();

		if (mContent == NULL)
		{
			return 0;
		}
        
		int value = mContent->getWidth() - getContentDimension().width;

		if (value < 0)
		{
			return 0;
		}

		return value;

	} // end getHorizontalMaxScroll

	int ScrollArea::getVerticalMaxScroll()
	{
		checkPolicies();

		if (mContent == NULL)
		{
			return 0;
		}

		int value;
    
		value = mContent->getHeight() - getContentDimension().height;
    
		if (value < 0)
		{
			return 0;
		}

		return value;

	} // getVerticalMaxScroll

	void ScrollArea::setScrollbarWidth(int width)
	{
		if (width > 0)
		{
			mScrollbarWidth = width;
		}
		else
		{
			throw GCN_EXCEPTION("ScrollArea::setScrollbarWidth. width should be greater then 0");
		}

	} // end setScrollbarWidth

	int ScrollArea::getScrollbarWidth()
	{
		return mScrollbarWidth;

	} // end getScrollbarWidth

	void ScrollArea::_setFocusHandler(FocusHandler* focusHandler)
	{
		BasicContainer::_setFocusHandler(focusHandler);

		if (mContent)
		{
			mContent->_setFocusHandler(focusHandler);
		}

	} // end _setFocusHandler

	void ScrollArea::_mouseInputMessage(const MouseInput &mouseInput)
	{
		BasicContainer::_mouseInputMessage(mouseInput);
    
		if (getContentDimension().isPointInRect(mouseInput.x, mouseInput.y))
		{
			if (mContent != NULL)
			{
				if (!mContent->hasMouse())
				{
					mContent->_mouseInMessage();          
				}
        
				// Only send mousemessages to the content if it is not focused
				// (focused widgets always receive mousemessages)
				if (!mContent->hasFocus())
				{          
					MouseInput mi = mouseInput;
          
					mi.x -= mContent->getX();
					mi.y -= mContent->getY();

					if (mi.x >= mContent->getWidth())
					{
						mi.x = mContent->getWidth() - 1;
					}

					if (mi.y >= mContent->getHeight())
					{
						mi.y = mContent->getHeight() - 1;
					}          
          
					mContent->_mouseInputMessage(mi);
				}
			}      
		}
		else
		{
			if (mContent && mContent->hasMouse())
			{
				mContent->_mouseOutMessage();
			}     
		}

	} // end _mouseInputMessage

	void ScrollArea::_mouseOutMessage()
	{
		if (mContent && mContent->hasMouse())
		{
			mContent->_mouseOutMessage();
		}
	
		BasicContainer::_mouseOutMessage();

	} // end _mouseOutMessage
  
	void ScrollArea::mousePress(int x, int y, int button)
	{
		if (getUpButtonDimension().isPointInRect(x, y))
		{
			setVerticalScrollAmount(getVerticalScrollAmount() - 10);
			mUpButtonPressed = true;
		}
		else if (getDownButtonDimension().isPointInRect(x, y))
		{
			setVerticalScrollAmount(getVerticalScrollAmount() + 10);
			mDownButtonPressed = true;
		}
		else if (getLeftButtonDimension().isPointInRect(x, y))
		{
			setHorizontalScrollAmount(getHorizontalScrollAmount() - 10);
			mLeftButtonPressed = true;
		}
		else if (getRightButtonDimension().isPointInRect(x, y))
		{
			setHorizontalScrollAmount(getHorizontalScrollAmount() + 10);
			mRightButtonPressed = true;
		}		
		else if (getVerticalMarkerDimension().isPointInRect(x, y))
		{
			mVerticalMarkerPressed = true;
			mVerticalMarkerMousePosition = y - getVerticalMarkerDimension().y;
		}
		else if (getHorizontalMarkerDimension().isPointInRect(x, y))
		{
			mHorizontalMarkerPressed = true;
			mHorizontalMarkerMousePosition = x - getHorizontalMarkerDimension().x;
		}
    
	} // end mousePress

	void ScrollArea::mouseRelease(int x, int y, int button)
	{
    mUpButtonPressed = false;
    mDownButtonPressed = false;
    mLeftButtonPressed = false;
    mRightButtonPressed = false;
    mVerticalMarkerPressed = false;
    mHorizontalMarkerPressed = false;

    if (button == MouseInput::LEFT && hasFocus() && mContent)
    {
      mContent->requestFocus();
    }
      
  } // end mouseRelease

  void ScrollArea::mouseMotion(int x, int y)
  {
    if (mVerticalMarkerPressed)
    {
      int pos = y - getVerticalBarDimension().y  - mVerticalMarkerMousePosition;
      int length = getVerticalMarkerDimension().height;

      Rectangle barDim = getVerticalBarDimension();
	  
      if ((barDim.height - length) > 0)
      {
        setVerticalScrollAmount((getVerticalMaxScroll() * pos)
                                / (barDim.height - length));
      }
      else
      {
        setVerticalScrollAmount(0);
      }	 
    }
    if (mHorizontalMarkerPressed)
    {
      int pos = x - getHorizontalBarDimension().x  - mHorizontalMarkerMousePosition;
      int length = getHorizontalMarkerDimension().width;

      Rectangle barDim = getHorizontalBarDimension();
	  
      if ((barDim.width - length) > 0)
      {
        setHorizontalScrollAmount((getHorizontalMaxScroll() * pos)
                                  / (barDim.width - length));
      }
      else
      {
        setHorizontalScrollAmount(0);
      }	 
    }

  }

  void ScrollArea::draw(Graphics *graphics)
  {
    graphics->setColor(getBackgroundColor());
    graphics->fillRectangle(getContentDimension());
    
    Color lightColor = getBaseColor() + 0x303030;
    Color shadowColor = getBaseColor() - 0x303030;      
    
    graphics->setColor(shadowColor);
    graphics->drawLine(0, 0, getWidth() - 1, 0);
    graphics->drawLine(0, 1, 0, getHeight() - 1);
    
    graphics->setColor(lightColor);
    graphics->drawLine(getWidth() - 1, 1, getWidth() - 1, getHeight() - 1);
    graphics->drawLine(1, getHeight() - 1, getWidth() - 1, getHeight() - 1);
    
    if (mVBarVisible)
    {
      drawUpButton(graphics);
      drawDownButton(graphics);
      drawVBar(graphics);
      drawVMarker(graphics);
    }
    
    if (mHBarVisible)
    {
      drawLeftButton(graphics);
      drawRightButton(graphics);
      drawHBar(graphics);
      drawHMarker(graphics);
    }
    
    if (mHBarVisible && mVBarVisible)
    {
      graphics->setColor(getBaseColor());
      graphics->fillRectangle(Rectangle(getWidth() - mScrollbarWidth - 1,
                                        getHeight() - mScrollbarWidth - 1,
                                        mScrollbarWidth,
                                        mScrollbarWidth));
    }

    if (mContent)
    {
      // We adjust x and y to account for the border
      Rectangle contdim = mContent->getDimension();
      contdim.x -= getContentDimension().x;
      contdim.y -= getContentDimension().y;
        
      graphics->pushClipArea(getContentDimension());
      graphics->pushClipArea(contdim);
      mContent->draw(graphics);
      graphics->popClipArea();
      graphics->popClipArea();
    }
  } // end draw

  void ScrollArea::drawHBar(Graphics* graphics)
  {
    Rectangle dim = getHorizontalBarDimension();

    graphics->pushClipArea(dim);

    graphics->setColor(getBaseColor() - 0x101010);
    graphics->fillRectangle(Rectangle(0, 0, dim.width, dim.height));

    graphics->setColor(getBaseColor() - 0x303030);
    graphics->drawLine(0, 0, dim.width, 1);

    graphics->popClipArea();
	
  } // end drawHBar  
  
  void ScrollArea::drawVBar(Graphics* graphics)
  {
    Rectangle dim = getVerticalBarDimension();

    graphics->pushClipArea(dim);

    graphics->setColor(getBaseColor() - 0x101010);
    graphics->fillRectangle(Rectangle(0, 0, dim.width, dim.height));
    
    graphics->setColor(getBaseColor() - 0x303030);
    graphics->drawLine(0, 0, 1, dim.height);
		
    graphics->popClipArea();

  } // end drawVBar
  
  void ScrollArea::drawUpButton(Graphics* graphics)
  {
    Rectangle dim = getUpButtonDimension();
    graphics->pushClipArea(dim);
	
    Color lightColor;
    Color shadowColor;
    Color faceColor;
    int offset;
    
    if (mUpButtonPressed)
    {
      faceColor = getBaseColor() - 0x303030;      
      lightColor = getBaseColor() - 0x606060;
      shadowColor = getBaseColor();
      offset = 1;
    }
    else
    {
      lightColor = getBaseColor() + 0x303030;
      shadowColor = getBaseColor() - 0x303030;
      faceColor = getBaseColor();
      offset = 0;
    }

    graphics->setColor(faceColor);      
    graphics->fillRectangle(Rectangle(0, 0, dim.width, dim.height));

    graphics->setColor(lightColor);
    graphics->drawLine(0, 0, dim.width - 1, 0);
    graphics->drawLine(0, 1, 0, dim.height - 1);
    
    graphics->setColor(shadowColor);
    graphics->drawLine(dim.width - 1, 0, dim.width - 1, dim.height - 1);
    graphics->drawLine(1, dim.height - 1, dim.width - 1, dim.height - 1);
    
    graphics->setColor(getForegroundColor());
    
    int i;
    int w = dim.height / 2;
    int h = w / 2 + 2;
    for (i = 0; i < w / 2; ++i)
    {
      graphics->drawLine(w - i + offset,
                         i + h + offset,
                         w + i + offset,
                         i + h + offset);
    }
    
    graphics->popClipArea();

  } // end drawUpButton

  void ScrollArea::drawDownButton(Graphics* graphics)
  {    
    Rectangle dim = getDownButtonDimension();
    graphics->pushClipArea(dim);

    Color lightColor;
    Color shadowColor;
    Color faceColor;
    int offset;
    
    if (mDownButtonPressed)
    {
      faceColor = getBaseColor() - 0x303030;      
      lightColor = getBaseColor() - 0x606060;
      shadowColor = getBaseColor();
      offset = 1;
    }
    else
    {
      lightColor = getBaseColor() + 0x303030;
      shadowColor = getBaseColor() - 0x303030;
      faceColor = getBaseColor();
      offset = 0;
    }
    
    graphics->setColor(faceColor);      
    graphics->fillRectangle(Rectangle(0, 0, dim.width, dim.height));

    graphics->setColor(lightColor);
    graphics->drawLine(0, 0, dim.width - 1, 0);
    graphics->drawLine(0, 1, 0, dim.height - 1);
    
    graphics->setColor(shadowColor);
    graphics->drawLine(dim.width - 1, 0, dim.width - 1, dim.height - 1);
    graphics->drawLine(1, dim.height - 1, dim.width - 1, dim.height - 1);
    
    graphics->setColor(getForegroundColor());
    
    int i;
    int w = dim.height / 2;
    int h = w + 1;
    for (i = 0; i < w / 2; ++i)
    {
      graphics->drawLine(w - i + offset,
                         -i + h + offset,
                         w + i + offset,
                         -i + h + offset);      
    }
    
    graphics->popClipArea();

  } // end drawDownButton

  void ScrollArea::drawLeftButton(Graphics* graphics)
  {
    Rectangle dim = getLeftButtonDimension();
    graphics->pushClipArea(dim);

    Color lightColor;
    Color shadowColor;
    Color faceColor;
    int offset;
    
    if (mLeftButtonPressed)
    {
      faceColor = getBaseColor() - 0x303030;      
      lightColor = getBaseColor() - 0x606060;
      shadowColor = getBaseColor();
      offset = 1;
    }
    else
    {
      lightColor = getBaseColor() + 0x303030;
      shadowColor = getBaseColor() - 0x303030;
      faceColor = getBaseColor();
      offset = 0;
    }
    
    graphics->setColor(faceColor);      
    graphics->fillRectangle(Rectangle(0, 0, dim.width, dim.height));

    graphics->setColor(lightColor);
    graphics->drawLine(0, 0, dim.width - 1, 0);
    graphics->drawLine(0, 1, 0, dim.height - 1);
    
    graphics->setColor(shadowColor);
    graphics->drawLine(dim.width - 1, 0, dim.width - 1, dim.height - 1);
    graphics->drawLine(1, dim.height - 1, dim.width - 1, dim.height - 1);
    
    graphics->setColor(getForegroundColor());

    int i;
    int w = dim.width / 2;
    int h = w - 2;
    for (i = 0; i < w / 2; ++i)
    {
      graphics->drawLine(i + h + offset,
                         w - i + offset,
                         i + h + offset,
                         w + i + offset);      
    }
    
    graphics->popClipArea();

  } // end drawLeftButton

  void ScrollArea::drawRightButton(Graphics* graphics)
  {
    Rectangle dim = getRightButtonDimension();
    graphics->pushClipArea(dim);

    Color lightColor;
    Color shadowColor;
    Color faceColor;
    int offset;
    
    if (mRightButtonPressed)
    {
      faceColor = getBaseColor() - 0x303030;      
      lightColor = getBaseColor() - 0x606060;
      shadowColor = getBaseColor();
      offset = 1;
    }
    else
    {
      lightColor = getBaseColor() + 0x303030;
      shadowColor = getBaseColor() - 0x303030;
      faceColor = getBaseColor();
      offset = 0;
    }
    
    graphics->setColor(faceColor);      
    graphics->fillRectangle(Rectangle(0, 0, dim.width, dim.height));

    graphics->setColor(lightColor);
    graphics->drawLine(0, 0, dim.width - 1, 0);
    graphics->drawLine(0, 1, 0, dim.height - 1);
    
    graphics->setColor(shadowColor);
    graphics->drawLine(dim.width - 1, 0, dim.width - 1, dim.height - 1);
    graphics->drawLine(1, dim.height - 1, dim.width - 1, dim.height - 1);
    
    graphics->setColor(getForegroundColor());
    
    int i;
    int w = dim.width / 2;
    int h = w + 1;
    for (i = 0; i < w / 2; ++i)
    {
      graphics->drawLine(-i + h + offset,
                         w - i + offset,
                         -i + h + offset,
                         w + i + offset);      
    }

    graphics->popClipArea();
    
  } // end drawRightButton


  void ScrollArea::drawVMarker(Graphics* graphics)
  {
    Rectangle dim = getVerticalMarkerDimension();
    graphics->pushClipArea(dim);
	
    graphics->setColor(getBaseColor());      

    graphics->fillRectangle(Rectangle(1, 1, dim.width - 1, dim.height - 1));
	
    graphics->setColor(getBaseColor() + 0x303030);
    graphics->drawLine(0, 0, dim.width - 1, 0);
    graphics->drawLine(0, 1, 0, dim.height - 1);
	
    graphics->setColor(getBaseColor() - 0x303030);	
    graphics->drawLine(1, dim.height - 1, dim.width - 1, dim.height - 1);
    graphics->drawLine(dim.width - 1, 0, dim.width - 1, dim.height - 1);

    graphics->popClipArea();

  } // end drawVMarker

  void ScrollArea::drawHMarker(Graphics* graphics)
  {
    Rectangle dim = getHorizontalMarkerDimension();
    graphics->pushClipArea(dim);
	
    graphics->setColor(getBaseColor());      

    graphics->fillRectangle(Rectangle(1, 1, dim.width - 1, dim.height - 1));
	
    graphics->setColor(getBaseColor() + 0x303030);
    graphics->drawLine(0, 0, dim.width - 1, 0);
    graphics->drawLine(0, 1, 0, dim.height - 1);
	
    graphics->setColor(getBaseColor() - 0x303030);	
    graphics->drawLine(1, dim.height - 1, dim.width - 1, dim.height - 1);
    graphics->drawLine(dim.width - 1, 0, dim.width - 1, dim.height - 1);

    graphics->popClipArea();

  } // end drawVMarker
  
  void ScrollArea::logic()
  {
    checkPolicies();

    setVerticalScrollAmount(getVerticalScrollAmount());
    setHorizontalScrollAmount(getHorizontalScrollAmount());

    if (mContent)
    {
      mContent->setPosition(-mHScroll + getContentDimension().x,
                            -mVScroll + getContentDimension().y);
      mContent->logic();
    }
    
  } // end logic

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

	void ScrollArea::_announceDeath(Widget *widget)
	{
		if (widget == mContent)
		{
			mContent = NULL;
			checkPolicies();
		}
		else
		{
			throw GCN_EXCEPTION("ScrollArea::_announceDeath. called by not-child");
		}
	}

  void ScrollArea::getDrawSize(int& width, int& height, Widget* widget)
  {
    if (mContent == widget)
    {
      width = getContentDimension().width;
      height = getContentDimension().height;
    }
    else
    {
      throw GCN_EXCEPTION("ScrollArea::getDrawSize. Widget not in scrollarea");
    }
    
  } // end getDrawSize
  
  void ScrollArea::drawContent(Graphics* graphics)
  {
    if (mContent)
    {
      mContent->draw(graphics);
    }
  }
  
  void ScrollArea::checkPolicies()
  {
    int w = getWidth() - 2;
    int h = getHeight() - 2;
    
    mHBarVisible = false;
    mVBarVisible = false;
    
    
    if (!mContent)
    {
      mHBarVisible = (mHPolicy == SHOW_ALWAYS);
      mVBarVisible = (mVPolicy == SHOW_ALWAYS);
      return;
    }
    
    if (mHPolicy == SHOW_AUTO &&
        mVPolicy == SHOW_AUTO)
    {
      if (mContent->getWidth() <= w
          && mContent->getHeight() <= h)
      {
        mHBarVisible = false;
        mVBarVisible = false;
      }

      if (mContent->getWidth() > w)
      {
        mHBarVisible = true;
      }

      if ((mContent->getHeight() > h)
          || (mHBarVisible && mContent->getHeight() > h - mScrollbarWidth))
      {
        mVBarVisible = true;
      }

      if (mVBarVisible && mContent->getWidth() > w - mScrollbarWidth)
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
          mHBarVisible = mContent->getWidth() > w;
        }
        else // (mVPolicy == SHOW_ALWAYS)
        {
          mHBarVisible = mContent->getWidth() > w - mScrollbarWidth;
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
          mVBarVisible = mContent->getHeight() > h;
        }
        else // (mHPolicy == SHOW_ALWAYS)
        {
          mVBarVisible = mContent->getHeight() > h - mScrollbarWidth;
        }
        break;
      default:
        throw GCN_EXCEPTION("ScrollArea::checkPolicies. Vertical scroll policy invalid");
    }
  }

  Rectangle ScrollArea::getUpButtonDimension()
  {
    if (!mVBarVisible)
    {
      return Rectangle(0, 0, 0, 0);
    }
	
    return Rectangle(getWidth() - mScrollbarWidth - 1,
                     1,
                     mScrollbarWidth,
                     mScrollbarWidth);

  } // end getUpButtonDimension

  Rectangle ScrollArea::getDownButtonDimension()
  {
    if (!mVBarVisible)
    {
      return Rectangle(0, 0, 0, 0);
    }

    if (mVBarVisible && mHBarVisible)
    {
      return Rectangle(getWidth() - mScrollbarWidth - 1,
                       getHeight() - mScrollbarWidth*2 - 1,
                       mScrollbarWidth,
                       mScrollbarWidth);
    }
	
    return Rectangle(getWidth() - mScrollbarWidth - 1,
                     getHeight() - mScrollbarWidth - 1,
                     mScrollbarWidth,
                     mScrollbarWidth);      
	
  } // end getDownButtonDimension


  Rectangle ScrollArea::getLeftButtonDimension()
  {
    if (!mHBarVisible)
    {
      return Rectangle(0, 0, 0, 0);
    }
	
    return Rectangle(1,
                     getHeight() - mScrollbarWidth - 1,
                     mScrollbarWidth,
                     mScrollbarWidth);
	
  } // end getLeftButtonDimension

  Rectangle ScrollArea::getRightButtonDimension()
  {
    if (!mHBarVisible)
    {
      return Rectangle(0, 0, 0, 0);
    }

    if (mVBarVisible && mHBarVisible)
    {
      return Rectangle(getWidth() - mScrollbarWidth*2 - 1,
                       getHeight() - mScrollbarWidth - 1,
                       mScrollbarWidth,
                       mScrollbarWidth);
    }

    return Rectangle(getWidth() - mScrollbarWidth - 1,
                     getHeight() - mScrollbarWidth - 1,
                     mScrollbarWidth,
                     mScrollbarWidth);
		  
  } // end getRightButtonDimension

  Rectangle ScrollArea::getContentDimension()
  {
    if (mVBarVisible && mHBarVisible)
    {
      return Rectangle(1, 1, getWidth() - mScrollbarWidth - 2, 
                       getHeight() - mScrollbarWidth - 2);
    }

    if (mVBarVisible)
    {
      return Rectangle(1, 1, getWidth() - mScrollbarWidth - 2, getHeight() - 2);
    }

    if (mHBarVisible)
    {
      return Rectangle(1, 1, getWidth() - 2, getHeight() - mScrollbarWidth - 2);
    }
	
    return Rectangle(1, 1, getWidth() - 2, getHeight() - 2);

  } // end getContentDimension

  Rectangle ScrollArea::getVerticalBarDimension()
  {
    if (!mVBarVisible)
    {
      return Rectangle(0, 0, 0, 0);
    }

    if (mHBarVisible)
    {
      return Rectangle(getWidth() - mScrollbarWidth - 1,
                       getUpButtonDimension().height + 1,
                       mScrollbarWidth,
                       getHeight() 
                       - getUpButtonDimension().height
                       - getDownButtonDimension().height
                       - mScrollbarWidth - 2);
    }
	
    return Rectangle(getWidth() - mScrollbarWidth - 1,
                     getUpButtonDimension().height + 1,
                     mScrollbarWidth,
                     getHeight() 
                     - getUpButtonDimension().height
                     - getDownButtonDimension().height
                     - 2);

  } // end getVerticalBarDimension

  Rectangle ScrollArea::getHorizontalBarDimension()
  {
    if (!mHBarVisible)
    {
      return Rectangle(0, 0, 0, 0);
    }

    if (mVBarVisible)
    {
      return Rectangle(getLeftButtonDimension().width + 1,
                       getHeight() - mScrollbarWidth - 1,
                       getWidth() 
                       - getLeftButtonDimension().width
                       - getRightButtonDimension().width
                       - mScrollbarWidth
                       - 2, 
                       mScrollbarWidth);
    }

    return Rectangle(getLeftButtonDimension().width + 1,
                     getHeight() - mScrollbarWidth - 1,
                     getWidth() 
                     - getLeftButtonDimension().width
                     - getRightButtonDimension().width
                     - 2,
                     mScrollbarWidth);

  } // end getHorizontalBarDimension

  Rectangle ScrollArea::getVerticalMarkerDimension()
  {
    if (!mVBarVisible)
    {
      return Rectangle(0, 0, 0, 0);
    }

    int length, pos;
    Rectangle barDim = getVerticalBarDimension();

    if (mContent && mContent->getHeight() != 0)
    {	  
      length = (barDim.height * getContentDimension().height) 
        / mContent->getHeight();
    }
    else
    {
      length = barDim.height;
    }

    if (length < mScrollbarWidth)
    {
      length = mScrollbarWidth;
    }

    if (length > barDim.height)
    {
      length = barDim.height;
    }

    if (getVerticalMaxScroll() != 0)
    {
      pos = ((barDim.height - length) * getVerticalScrollAmount())
        / getVerticalMaxScroll();
    }
    else
    {
      pos = 0;
    }
	
    return Rectangle(barDim.x, barDim.y + pos, mScrollbarWidth, length);

  } // end getVerticalMarkerDimension

  Rectangle ScrollArea::getHorizontalMarkerDimension()
  {
    if (!mHBarVisible)
    {
      return Rectangle(0, 0, 0, 0);
    }

    int length, pos;
    Rectangle barDim = getHorizontalBarDimension();

    if (mContent && mContent->getWidth() != 0)
    {	  
      length = (barDim.width * getContentDimension().width) 
        / mContent->getWidth();
    }
    else
    {
      length = barDim.width;
    }

    if (length < mScrollbarWidth)
    {
      length = mScrollbarWidth;
    }

    if (length > barDim.width)
    {
      length = barDim.width;
    }
	
    if (getHorizontalMaxScroll() != 0)
    {
      pos = ((barDim.width - length) * getHorizontalScrollAmount())
        / getHorizontalMaxScroll();
    }
    else
    {
      pos = 0;
    }

    return Rectangle(barDim.x + pos, barDim.y, length, mScrollbarWidth);

  } // end getHorizontalMarkerDimension

  void ScrollArea::scrollToRectangle(const Rectangle& rectangle)
  {
    Rectangle contentDim = getContentDimension();
    
    if (rectangle.x + rectangle.width
        > getHorizontalScrollAmount() + contentDim.width)
    {
      setHorizontalScrollAmount(rectangle.x + rectangle.width - contentDim.width);
    }
    
    if (rectangle.y + rectangle.height
        > getVerticalScrollAmount() + contentDim.height)
    {
      setVerticalScrollAmount(rectangle.y + rectangle.height - contentDim.height);
    }

    if (rectangle.x < getHorizontalScrollAmount())
    {
      setHorizontalScrollAmount(rectangle.x);
    }
    
    if (rectangle.y < getVerticalScrollAmount())
    {
      setVerticalScrollAmount(rectangle.y);
    }
    
  } // end scrollToRectangle

  void ScrollArea::mouseWheelUp(int x, int y)
  {    
    if (hasMouse())
    {
      setVerticalScrollAmount(getVerticalScrollAmount() - getContentDimension().height / 8);
    }
    
  } // end mouseWheelUp

  void ScrollArea::mouseWheelDown(int x, int y)
  {    
    if (hasMouse())
    {
      setVerticalScrollAmount(getVerticalScrollAmount() + getContentDimension().height / 8);
    }
    
  } // end mouseWheelDown

} // end gcn

/*
 * Wow! This is a looooong source file. 1295 lines!
 */
