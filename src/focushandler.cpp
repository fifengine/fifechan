/*      _______   __   __   __   ______   __   __   _______   __   __                 
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\                
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /                 
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /                  
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /                   
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /                    
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/                      
 *
 * Copyright (c) 2004, 2005 darkbits                        Js_./
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

#include "guichan/focushandler.hpp"
#include "guichan/exception.hpp"

#include <iostream>

namespace gcn
{
	FocusHandler::FocusHandler()
	{
		mFocusedWidget = -1;
		mDraggedWidget = -1; 
		mToBeFocused = -1;
		mToBeDragged = -1;
	}

	void FocusHandler::requestFocus(Widget* widget)
	{
		unsigned int i = 0;
    
		for (i = 0; i < mWidgets.size(); ++i)
		{      
			if (mWidgets[i] == widget)
			{
				mToBeFocused = i;                
				return;
			}
		}    
    
		throw GCN_EXCEPTION("FocusHandler::requestFocus. No such widget exists");
	}

	void FocusHandler::requestDrag(Widget* widget)
	{
		unsigned int i = 0;
    
		for (i = 0; i < mWidgets.size(); ++i)
		{      
			if (mWidgets[i] == widget)
			{
				mToBeDragged = i;                
				return;
			}
		}    
    
		throw GCN_EXCEPTION("FocusHandler::requestDragged. No such widget exists");
	}

	Widget* FocusHandler::getFocused() const
	{
		if (mFocusedWidget == -1)
		{
			return NULL;
		}

		return mWidgets[mFocusedWidget];    
	}

	Widget* FocusHandler::getDragged() const
	{
		if (mDraggedWidget == -1)
 		{
 			return NULL;
 		}
		
 		return mWidgets[mDraggedWidget];    
	}

	void FocusHandler::focusNext()
	{
		int focused = mFocusedWidget;
		int i;
		
		if (mWidgets.size() == 0)
		{
			mFocusedWidget = -1;
			return;
		}    

		// i is a counter that ensures that the following loop
		// won't get stuck in an infinite loop
		i = (int)mWidgets.size();
		do
		{
			++mFocusedWidget;
			
			if (i==0)
			{
				mFocusedWidget = -1;
				break;
			}
			
			--i;

			if (mFocusedWidget >= (int)mWidgets.size())
			{
				mFocusedWidget = 0;      
			}

			if (mFocusedWidget == focused)
			{
				return;
			}			
		}
		while (!mWidgets.at(mFocusedWidget)->isFocusable());

		if (focused >= 0)
		{
			mWidgets.at(focused)->lostFocus();
		}
    
		if (mFocusedWidget >= 0)
		{
			mWidgets.at(mFocusedWidget)->gotFocus();
		}
    
	} // end focusNext
    
	void FocusHandler::focusPrevious()
	{
		int focused = mFocusedWidget;
		int i;
		
		if (mWidgets.size() == 0)
		{
			mFocusedWidget = -1;
			return;
		}    

		// i is a counter that ensures that the following loop
		// won't get stuck in an infinite loop
		i = (int)mWidgets.size();
		do
		{
			--mFocusedWidget;

			if (i==0)
			{
				mFocusedWidget = -1;
				break;
			}

			--i;
			
			if (mFocusedWidget <= 0)
			{
				mFocusedWidget = mWidgets.size() - 1;      
			}

			if (mFocusedWidget == focused)
			{
				return;
			}
		}
		while (!mWidgets.at(mFocusedWidget)->isFocusable());

		if (focused >= 0)
		{
			mWidgets.at(focused)->lostFocus();
		}
    
		if (mFocusedWidget >= 0)
		{
			mWidgets.at(mFocusedWidget)->gotFocus();
		}
    
	} // end focusPrevious

	bool FocusHandler::hasFocus(const Widget* widget) const
	{
		if (mFocusedWidget == -1)
		{
			return false;
		}
    
		return mWidgets[mFocusedWidget] == widget;
	}

 	bool FocusHandler::isDragged(const Widget* widget) const
 	{
 		if (mDraggedWidget == -1)
 		{
 			return false;
 		}
    
 		return mWidgets[mDraggedWidget] == widget;
 	}

	void FocusHandler::add(Widget* widget)
	{
		mWidgets.push_back(widget);    
	}
    
	void FocusHandler::remove(Widget* widget)
	{
		int i = 0;
		WidgetIterator iter;
    
		for (iter = mWidgets.begin(); iter != mWidgets.end(); ++iter)
		{
			++i;
      
			if ((*iter) == widget)
			{        
				if (i < mFocusedWidget)
				{
					--mFocusedWidget;
				}
				else if (i == mFocusedWidget)
				{
					mFocusedWidget = -1;
				}
				
// 				if (i < mMouseFocusedWidget)
// 				{
// 					--mMouseFocusedWidget;
// 				}
// 				else if (i == mMouseFocusedWidget)
// 				{
// 					mMouseFocusedWidget = -1;
// 				}

				mWidgets.erase(iter);
		
				return;
			}      
		}    
	}

	void FocusHandler::focusNone()
	{
		if (mFocusedWidget >= 0)
		{
			mWidgets.at(mFocusedWidget)->lostFocus();
		}
    
		mFocusedWidget = -1;    
	}

	void FocusHandler::dragNone()
	{
		mDraggedWidget = -1;
	}
	
	void FocusHandler::tabNext()
	{
		if (mFocusedWidget >= 0)
		{
			if (!mWidgets.at(mFocusedWidget)->isTabOutEnabled())
			{
				return;
			}
		}

		int focused = mFocusedWidget;
		int i;
		
		if (mWidgets.size() == 0)
		{
			mFocusedWidget = -1;
			return;
		}    

		// i is a counter that ensures that the following loop
		// won't get stuck in an infinite loop
		i = (int)mWidgets.size();
		do
		{
			++mFocusedWidget;
			
			if (i==0)
			{
				mFocusedWidget = -1;
				break;
			}
			
			--i;

			if (mFocusedWidget >= (int)mWidgets.size())
			{
				mFocusedWidget = 0;      
			}

			if (mFocusedWidget == focused)
			{
				return;
			}			
		}
		while (!mWidgets.at(mFocusedWidget)->isFocusable() ||
               !mWidgets.at(mFocusedWidget)->isTabInEnabled());

		if (focused >= 0)
		{
			mWidgets.at(focused)->lostFocus();
		}
    
		if (mFocusedWidget >= 0)
		{
			mWidgets.at(mFocusedWidget)->gotFocus();
		}
	} // end tabNext

	void FocusHandler::tabPrevious()
	{
		if (mFocusedWidget >= 0)
		{
			if (!mWidgets.at(mFocusedWidget)->isTabOutEnabled())
			{
				return;
			}
		}
		
		int focused = mFocusedWidget;
		int i;
		
		if (mWidgets.size() == 0)
		{
			mFocusedWidget = -1;
			return;
		}    

		// i is a counter that ensures that the following loop
		// won't get stuck in an infinite loop
		i = (int)mWidgets.size();
		do
		{
			--mFocusedWidget;

			if (i==0)
			{
				mFocusedWidget = -1;
				break;
			}

			--i;
			
			if (mFocusedWidget <= 0)
			{
				mFocusedWidget = mWidgets.size() - 1;      
			}

			if (mFocusedWidget == focused)
			{
				return;
			}
		}
		while (!mWidgets.at(mFocusedWidget)->isFocusable());

		if (focused >= 0)
		{
			mWidgets.at(focused)->lostFocus();
		}
    
		if (mFocusedWidget >= 0)
		{
			mWidgets.at(mFocusedWidget)->gotFocus();
		}    
	} // end tabPrevious

	void FocusHandler::applyChanges()
	{
		if (mToBeFocused >= 0 && mFocusedWidget != mToBeFocused)
		{    
			if (mFocusedWidget >= 0)
			{
				mWidgets.at(mFocusedWidget)->lostFocus();
			}
					
			mWidgets.at(mToBeFocused)->gotFocus();        
			mFocusedWidget = mToBeFocused;        
			mToBeFocused = -1;
		}

 		if (mToBeDragged >= 0 && mDraggedWidget != mToBeDragged)
 		{    
			mDraggedWidget = mToBeDragged;
			mToBeDragged = -1;
 		}				
	}
	
} // end gcn
