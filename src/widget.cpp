/*      _______   __   __   __   ______   __   __   _______   __   __                 
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\                
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /                 
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /                  
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /                   
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /                    
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/                      
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
 * 3. Neither the name of Guichan nor the      :$we` _! + _/ .        j?
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

#include "guichan/basiccontainer.hpp"
#include "guichan/exception.hpp"
#include "guichan/focushandler.hpp"
#include "guichan/widget.hpp"

namespace gcn
{
	Font* Widget::mGlobalFont = NULL;
	DefaultFont Widget::mDefaultFont;
	std::list<Widget*> Widget::mWidgets;

	Widget::Widget()
	{
		mParent = NULL;
		mForegroundColor = Color(0x000000);
		mBackgroundColor = Color(0xffffff);
		mBaseColor = Color(0x808090);
		mBorderSize = 0;
		mFocusHandler = NULL;
		mFocusable = false;
		mClickTimeStamp = 0;
		mClickCount = 0;		
		mHasMouse = false;
		mVisible = true;
		mTabIn = true;
		mTabOut = true;
		
		mCurrentFont = NULL;
		mWidgets.push_back(this);
    
	} // end Widget

	Widget::~Widget()
	{
		if (getParent())
		{
			getParent()->_announceDeath(this);
		}
	
		_setFocusHandler(NULL);
	
		mWidgets.remove(this);
	
	} // end ~Widget

	void Widget::_setParent(BasicContainer* parent)
	{
		mParent = parent;

	} // end _setParent

	BasicContainer* Widget::getParent() const
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

	void Widget::setBorderSize(unsigned int borderSize)
	{
		mBorderSize = borderSize;
	}

	unsigned int Widget::getBorderSize()
	{
		return mBorderSize;
	}
	
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
		if (!mFocusHandler)
		{
			return false;
		}

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
		return mFocusable && isVisible();

	} // end isFocusable
  
	void Widget::requestFocus()
	{
		if (mFocusHandler == NULL)
		{
			throw GCN_EXCEPTION("Widget::requestFocus. No focushandler set (did you add the widget to the gui?)");
		}
		
		if (isFocusable())
		{
			mFocusHandler->requestFocus(this);
		}

	} // end requestFocus

	void Widget::requestMoveToTop()
	{
		if (mParent)
		{
			mParent->moveToTop(this);
		}

	} // end requestMoveToTop

	void Widget::requestMoveToBottom()
	{
		if (mParent)
		{
			mParent->moveToBottom(this);
		}
    
	} // end requestMoveToBottom

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
		if (getParent() == NULL)
		{			
			return mVisible;
		}
		else
		{
			return mVisible && getParent()->isVisible();
		}
			
	} // end isVisible

	void Widget::setBaseColor(const Color& color)
	{
		mBaseColor = color;

	} // end setBaseColor

	const Color& Widget::getBaseColor() const
	{
		return mBaseColor;

	} // end getBaseColor

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

	FocusHandler* Widget::_getFocusHandler()
	{
		return mFocusHandler;
	} // end _setFocusHandler

	void Widget::addActionListener(ActionListener* actionListener)
	{
		mActionListeners.push_back(actionListener);
    
	} // end addActionListener
  
	void Widget::removeActionListener(ActionListener* actionListener)
	{
		mActionListeners.remove(actionListener);
    
	} // end removeActionListener
  
	void Widget::addKeyListener(KeyListener* keyListener)
	{
		mKeyListeners.push_back(keyListener);
    
	} // end addKeyListener
  
	void Widget::removeKeyListener(KeyListener* keyListener)
	{
		mKeyListeners.remove(keyListener);
    
	} // end removeKeyListener
  
	void Widget::addMouseListener(MouseListener* mouseListener)
	{
		mMouseListeners.push_back(mouseListener);
    
	} // end addMouseListener
  
	void Widget::removeMouseListener(MouseListener* mouseListener)
	{
		mMouseListeners.remove(mouseListener);
    
	} // end removeMouseListener
  
	void Widget::_mouseInputMessage(const MouseInput& mouseInput)
	{
		int x = mouseInput.x;
		int y = mouseInput.y;
		int b = mouseInput.getButton();
		int ts = mouseInput.getTimeStamp();

		MouseListenerIterator iter;
    
		switch(mouseInput.getType())
		{
		  case MouseInput::MOTION:
			  for (iter = mMouseListeners.begin(); iter != mMouseListeners.end(); ++iter)
			  {
				  (*iter)->mouseMotion(x, y);
			  }
			  break;
        
		  case MouseInput::PRESS:
			  requestFocus();
        
			  if (b != MouseInput::WHEEL_UP && b != MouseInput::WHEEL_DOWN)
			  {
				  for (iter = mMouseListeners.begin(); iter != mMouseListeners.end(); ++iter)
				  {
					  (*iter)->mousePress(x, y, b);
				  }

				  if (hasMouse())
				  {
					  if (ts - mClickTimeStamp < 300 && mClickButton == b)
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
				  else
				  {
					  mClickButton = 0;
				  }
			  }
			  else if (b == MouseInput::WHEEL_UP)
			  {
				  for (iter = mMouseListeners.begin(); iter != mMouseListeners.end(); ++iter)
				  {
					  (*iter)->mouseWheelUp(x, y);
				  }
			  }
			  else
			  {
				  for (iter = mMouseListeners.begin(); iter != mMouseListeners.end(); ++iter)
				  {
					  (*iter)->mouseWheelDown(x, y);
				  }
			  }
			  break;

		  case MouseInput::RELEASE:
			  if (b != MouseInput::WHEEL_UP && b != MouseInput::WHEEL_DOWN)
			  {
				  for (iter = mMouseListeners.begin(); iter != mMouseListeners.end(); ++iter)
				  {
					  (*iter)->mouseRelease(x, y, b);
				  }
			  }

			  if (mHasMouse)
			  {
				  if (b == mClickButton)
				  {
					  for (iter = mMouseListeners.begin(); iter != mMouseListeners.end(); ++iter)
					  {
						  (*iter)->mouseClick(x, y, b, mClickCount + 1);
					  }
				  }
				  else
				  {
					  mClickButton = 0;
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
		KeyListenerIterator iter;
    
		switch(keyInput.getType())
		{
		  case KeyInput::PRESS:
			  for (iter = mKeyListeners.begin(); iter != mKeyListeners.end(); ++iter)
			  {
				  (*iter)->keyPress(keyInput.getKey());
			  }        
			  break;
        
		  case KeyInput::RELEASE:
			  for (iter = mKeyListeners.begin(); iter != mKeyListeners.end(); ++iter)
			  {
				  (*iter)->keyRelease(keyInput.getKey());
			  }        
			  break;
		}
    
	} // end _keyInputMessage

	void Widget::_mouseInMessage()
	{
		mHasMouse = true;

		MouseListenerIterator iter;
		for (iter = mMouseListeners.begin(); iter != mMouseListeners.end(); ++iter)
		{
			(*iter)->mouseIn();
		}    

	} // end _mouseInMessage

	void Widget::_mouseOutMessage()
	{
		mHasMouse = false;

		MouseListenerIterator iter;
		for (iter = mMouseListeners.begin(); iter != mMouseListeners.end(); ++iter)
		{
			(*iter)->mouseOut();
		}    

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
    
	}
	
	void Widget::generateAction()
	{
		ActionListenerIterator iter;
		for (iter = mActionListeners.begin(); iter != mActionListeners.end(); ++iter)
		{
			(*iter)->action(mEventId);
		}    
    
	} // end generateAction
    
	Font* Widget::getFont() const
	{
		if (mCurrentFont == NULL)
		{
			if (mGlobalFont == NULL)
			{
				return &mDefaultFont;
			}

			return mGlobalFont;
		}    
    
		return mCurrentFont;

	} // end getFont
  
	void Widget::setGlobalFont(Font* font)
	{
		mGlobalFont = font;
        
		std::list<Widget*>::iterator iter;
		for (iter = mWidgets.begin(); iter != mWidgets.end(); ++iter)
		{
			if ((*iter)->mCurrentFont == NULL)
			{
				(*iter)->fontChanged();
			}
		}
    
	} // end setGlobalFont
  
	void Widget::setFont(Font* font)
	{
		mCurrentFont = font;
		fontChanged();
    
	} // end setFont

	bool Widget::widgetExists(const Widget* widget)
	{
		bool result = false;
		
		std::list<Widget*>::iterator iter;
		for (iter = mWidgets.begin(); iter != mWidgets.end(); ++iter)
		{
			if (*iter == widget)
			{
				return true;
			}
		}

		return result;

	} // end widgetExists

	bool Widget::isTabInEnabled() const
	{
		return mTabIn;
	}
		
	void Widget::setTabInEnabled(bool enabled)
	{
		mTabIn = enabled;
	}

	bool Widget::isTabOutEnabled() const
	{
		return mTabOut;
	}
		
	void Widget::setTabOutEnabled(bool enabled)
	{
		mTabOut = enabled;
	}

	void Widget::setSize(int width, int height)
	{
		setWidth(width);
		setHeight(height);
	}
	
} // end gcn
