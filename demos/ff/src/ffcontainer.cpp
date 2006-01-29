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

#include "ffcontainer.hpp"
#include <cmath>
#include <SDL/SDL.h>

int FFContainer::mInstances = 0;
gcn::Image *FFContainer::mCornerUL = 0;
gcn::Image *FFContainer::mCornerUR = 0;
gcn::Image *FFContainer::mCornerDL = 0;
gcn::Image *FFContainer::mCornerDR = 0;
gcn::Image *FFContainer::mHorizontal = 0;
gcn::Image *FFContainer::mVertical = 0;

FFContainer::FFContainer()
{
	if (mInstances == 0)
	{
		mCornerUL = new gcn::Image("images/cornerul.png");
		mCornerUR = new gcn::Image("images/cornerur.png");
		mCornerDL = new gcn::Image("images/cornerdl.png");
		mCornerDR = new gcn::Image("images/cornerdr.png");
		mHorizontal = new gcn::Image("images/horizontal.png");
		mVertical = new gcn::Image("images/vertical.png");				
	}

	mInstances++;
	
 	mRealWidth = 0;
	mRealHeight = 0;
	mTime = -1;
	mShow = true;
	Container::setWidth(0);
	Container::setHeight(0);	
	mSlideTarget = 0;
	mCurrentSlide = 0;
	setBorderSize(0);
}

FFContainer::~FFContainer()
{
	mInstances--;

	if (mInstances == 0)
	{
		delete mCornerUL;
		delete mCornerUR;
		delete mCornerDL;
		delete mCornerDR;
		delete mHorizontal;
		delete mVertical;		
	}
}

void FFContainer::draw(gcn::Graphics* graphics)
{
	int i;
	
	if (isOpaque())
	{
		double height = (mRealHeight - 8) / 16.0;
		gcn::Color c(0x7070FF);		
	
		for (i = 0; i<16; ++i)
		{
			graphics->setColor(c * (1.0 - i / 18.0));
			graphics->fillRectangle(gcn::Rectangle(4, (int)(i*height+4), getWidth()-8, (int)((i*height)+height)));
		}
	}

	graphics->pushClipArea(gcn::Rectangle(0, mCurrentSlide, getWidth(), getHeight()));
	drawChildren(graphics);
	graphics->popClipArea();
	
	for (i = 5; i < getHeight()-10; i+=5)
	{
		graphics->drawImage(mVertical, 0, i);
		graphics->drawImage(mVertical, getWidth()-4, i);		
	}
	graphics->drawImage(mVertical, 0, 0, 0, i, 4, getHeight()-5-i);
	graphics->drawImage(mVertical, 0, 0, getWidth()-4, i, 4, getHeight()-5-i);	

	for (i = 5; i < getWidth()-10; i+=5)
	{
		graphics->drawImage(mHorizontal, i, 0);
		graphics->drawImage(mHorizontal, i, getHeight()-4);		
	}
	graphics->drawImage(mHorizontal, 0, 0, i, 0, getWidth()-5-i, 4);
	graphics->drawImage(mHorizontal, 0, 0, i, getHeight()-4, getWidth()-5-i, 4);	
	
	graphics->drawImage(mCornerUL, 0, 0);
	graphics->drawImage(mCornerUR, getWidth()-5, 0);	
	graphics->drawImage(mCornerDL, 0, getHeight()-5);
	graphics->drawImage(mCornerDR, getWidth()-5, getHeight()-5);	
}

void FFContainer::logic()
{
	if (mTime < 0)
	{
		mTime = SDL_GetTicks();
	}

	int deltaTime = SDL_GetTicks() - mTime;
	mTime = SDL_GetTicks();	

	if (!mShow)
	{
		Container::setWidth(getWidth() - deltaTime);

		if (getWidth() < 0)
		{
			Container::setWidth(0);
		}

		Container::setHeight(getHeight() - deltaTime);

		if (getHeight() < 0)
		{
			Container::setHeight(0);
		}

		if (getHeight() == 0 && getWidth() == 0)
		{
			Container::setVisible(false);
		}
	}
	else
	{
		if (getWidth() < mRealWidth)
		{
			Container::setWidth(getWidth() + deltaTime);
			
			if (getWidth() > mRealWidth)
			{
				Container::setWidth(mRealWidth);
			}
		}
		else if (getWidth() > mRealWidth)
		{
			Container::setWidth(getWidth() - deltaTime);
			
			if (getWidth() < mRealWidth)
			{
				Container::setWidth(mRealWidth);
			}
		}
		
		if (getHeight() < mRealHeight)
		{
			Container::setHeight(getHeight() + deltaTime);
			
			if (getHeight() > mRealHeight)
			{
				Container::setHeight(mRealHeight);
			}
		}
		else if (getHeight() > mRealHeight)
		{
			Container::setHeight(getHeight() - deltaTime);
			
			if (getHeight() < mRealHeight)
			{
				Container::setHeight(mRealHeight);
			}
		}
	}

	if (mCurrentSlide < mSlideTarget)
	{
		mCurrentSlide += deltaTime;
		if (mCurrentSlide > mSlideTarget)
		{
			mCurrentSlide = mSlideTarget;
		}
	}

	if (mCurrentSlide > mSlideTarget)
	{
		mCurrentSlide -= deltaTime;
		if (mCurrentSlide < mSlideTarget)
		{
			mCurrentSlide = mSlideTarget;
		}
	}

	Container::logic();
}

void FFContainer::setDimension(const gcn::Rectangle &dimension)
{
	setPosition(dimension.x, dimension.y);
	setWidth(dimension.width);
	setHeight(dimension.height);	
}

void FFContainer::setVisible(bool visible)
{
	mShow = visible;
	
	if (visible)
	{
		Container::setVisible(true);
	}
}

void FFContainer::setWidth(int width)
{
//	Container::setWidth(0);
	mRealWidth = width;
}

void FFContainer::setHeight(int height)
{
// 	Container::setHeight(0);
	mRealHeight = height;
}

void FFContainer::slideContentTo(int y)
{
	mSlideTarget = y;
}
