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

#include "guichan/exception.hpp"
#include "guichan/widgets/container.hpp"

namespace gcn
{

	Container::Container()
	{
		mWidgetWithMouse = NULL;
		mOpaque = true;

	} // end Container

	Container::~Container()
	{
		clear();
		
	} // end ~Container
	
	void Container::logic()
	{
		logicChildren();

	} // end logic
   
	void Container::draw(Graphics* graphics)
	{
		if (mOpaque)
		{
			graphics->setColor(getBaseColor());
			graphics->fillRectangle(Rectangle(0, 0, getDimension().width, getDimension().height));
		}
    
		drawChildren(graphics);

	} // draw

	void Container::logicChildren()
	{
		WidgetIterator iter;
		for (iter = mWidgets.begin(); iter != mWidgets.end(); iter++)
		{
			(*iter)->logic();
		}

	} // end logicChildren
  
	void Container::drawChildren(Graphics* graphics)
	{
		WidgetIterator iter;
		for (iter = mWidgets.begin(); iter != mWidgets.end(); iter++)
		{
			if ((*iter)->isVisible())
			{
				graphics->pushClipArea((*iter)->getDimension());
				(*iter)->draw(graphics);
				graphics->popClipArea();
			}
		}

	} // end drawChildren

	void Container::setOpaque(bool opaque)
	{
		mOpaque = opaque;

	} // end setOpaque

	bool Container::isOpaque() const
	{
		return mOpaque;

	} // end isOpaque

	void Container::moveToTop(Widget* widget)
	{
		WidgetIterator iter;
		for (iter = mWidgets.begin(); iter != mWidgets.end(); iter++)
		{
			if (*iter == widget)
			{
				mWidgets.erase(iter);
				mWidgets.push_back(widget);
				return;
			}
		}

		throw GCN_EXCEPTION("Container::moveToTop. There is no such widget in this container");

	} // end moveToTop

	void Container::moveToBottom(Widget* widget)
	{
		WidgetIterator iter;
		for (iter = mWidgets.begin(); iter != mWidgets.end(); iter++)
		{
			if (*iter == widget)
			{
				mWidgets.erase(iter);
				mWidgets.push_front(widget);
				return;
			}
		}

		throw GCN_EXCEPTION("Container::moveToBottom. There is no such widget in this container");

	} // end moveToBottom

	void Container::_announceDeath(Widget *widget)
	{
		if (mWidgetWithMouse == widget)
		{
			mWidgetWithMouse = NULL;
		}
		  
		WidgetIterator iter;
		for (iter = mWidgets.begin(); iter != mWidgets.end(); iter++)
		{
			if (*iter == widget)
			{
				mWidgets.erase(iter);
				return;
			}
		}

		throw GCN_EXCEPTION("Container::_announceDeath. There is no such widget in this container");
		  
	} // end _announceDeath
	
	void Container::getDrawSize(int& width, int& height, Widget* widget)
	{
		WidgetIterator iter;
		bool contains = false;

		for (iter = mWidgets.begin(); iter != mWidgets.end(); iter++)
		{
			if (widget == *iter)
			{
				contains = true;
				break;
			}
		}
    
		if (contains)
		{
			Rectangle widgetDim = widget->getDimension();
			Rectangle dim = getDimension();

			width = widgetDim.width;
			height = widgetDim.height;

			if (widgetDim.x < 0)
			{
				width += widgetDim.x;
			}

			if (widgetDim.y < 0)
			{
				height += widgetDim.y;
			}

			if (widgetDim.x + widgetDim.width > dim.width)
			{
				width -= (widgetDim.x + widgetDim.width) - dim.width;
			}

			if (widgetDim.y + widgetDim.height > dim.height)
			{
				height -= (widgetDim.y + widgetDim.height) - dim.height;
			}

			if (width < 0)
			{
				width = 0;
			}

			if (height < 0)
			{
				height = 0;
			}
		}
		else
		{
			throw GCN_EXCEPTION("Container::getDrawSize. Widget not in container");
		}
    
	} // end getDrawSize
  
	void Container::add(Widget* widget)
	{
		mWidgets.push_back(widget);
		widget->_setFocusHandler(_getFocusHandler());
		widget->_setParent(this);
		// TODO: Give it the mousetypehandler too!

	} // add

	void Container::add(Widget* widget, int x, int y)
	{
		widget->setPosition(x, y);
		add(widget);

	} // end add

	void Container::remove(Widget* widget)
	{
		if (mWidgetWithMouse == widget)
		{
			mWidgetWithMouse = NULL;
		}
    
		WidgetIterator iter;
		for (iter = mWidgets.begin(); iter != mWidgets.end(); iter++)
		{
			if (*iter == widget)
			{
				mWidgets.erase(iter);
				widget->_setFocusHandler(NULL);
				widget->_setParent(NULL);
				return;
			}
		}

		throw GCN_EXCEPTION("Container::remove. There is no such widget in this container");

	} // end remove

	void Container::clear()
	{ 
		mWidgetWithMouse = NULL;
    
		WidgetIterator iter;

		for (iter = mWidgets.begin(); iter != mWidgets.end(); iter++)
		{      
			(*iter)->_setFocusHandler(NULL);
			(*iter)->_setParent(NULL);
		}
    
		mWidgets.clear();

	} // end clear

	void Container::_setFocusHandler(FocusHandler* focusHandler)
	{
		Widget::_setFocusHandler(focusHandler);
    
		WidgetIterator iter;
		for (iter = mWidgets.begin(); iter != mWidgets.end(); iter++)
		{
			(*iter)->_setFocusHandler(focusHandler);
		}   

	} // end _setFocusHandler
  
	void Container::_mouseInputMessage(const MouseInput &mouseInput)
	{
		Widget* tempWidgetWithMouse = NULL;
    
		WidgetIterator iter;    
		for (iter = mWidgets.begin(); iter != mWidgets.end(); iter++)
		{
			if ((*iter)->getDimension().isPointInRect(mouseInput.x, mouseInput.y))
			{
				tempWidgetWithMouse = (*iter);
			}
		}

		if (tempWidgetWithMouse != mWidgetWithMouse)
		{
			if (mWidgetWithMouse)
			{
				mWidgetWithMouse->_mouseOutMessage();
			}

			if (tempWidgetWithMouse)
			{
				tempWidgetWithMouse->_mouseInMessage();
			}

			mWidgetWithMouse = tempWidgetWithMouse;
		}
    
		if (mWidgetWithMouse && !mWidgetWithMouse->hasFocus())
		{
			MouseInput mi = mouseInput;
			mi.x -= mWidgetWithMouse->getX();
			mi.y -= mWidgetWithMouse->getY();      
			mWidgetWithMouse->_mouseInputMessage(mi);
		}

		if (mWidgetWithMouse == NULL)
		{
			Widget::_mouseInputMessage(mouseInput);
		}

	} //end _mouseInputMessage

	void Container::_mouseOutMessage()
	{
		if (mWidgetWithMouse)
		{
			mWidgetWithMouse->_mouseOutMessage();
			mWidgetWithMouse = NULL;
		}

		Widget::_mouseOutMessage();

	} // end _mouseOutMessage
  
} // end gcn
