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

#include "guichan/widgets/slider.hpp"
#include "guichan/mouseinput.hpp"

namespace gcn
{
	Slider::Slider(double scaleEnd)
	{
		mMouseDrag = false;
		mMarkerPosition = 0;
		mMarkerWidth = 10;
		
		mScaleStart = 0;
		mScaleEnd = scaleEnd;
		
		setFocusable(true);
		setBorderSize(1);
		
		addMouseListener(this);
		addKeyListener(this);
	}

	Slider::Slider(double scaleStart, double scaleEnd)
	{
		mMouseDrag = false;
		mMarkerPosition = 0;
		mMarkerWidth = 10;
		
		mScaleStart = scaleStart;
		mScaleEnd = scaleEnd;
		
		setFocusable(true);
		setBorderSize(1);
		
		addMouseListener(this);
		addKeyListener(this);
	}

	void Slider::setScale(double scaleStart, double scaleEnd)
	{
		mScaleStart = scaleStart;
		mScaleEnd = scaleEnd;
	}

	double Slider::getScaleStart()
	{
		return mScaleStart;
	}

	void Slider::setScaleStart(double scaleStart)
	{
		mScaleStart = scaleStart;
	}

	double Slider::getScaleEnd()
	{
		return mScaleEnd;
	}

	void Slider::setScaleEnd(double scaleEnd)
	{
		mScaleEnd = scaleEnd;
	}

	void Slider::draw(gcn::Graphics* graphics)
	{
		Color shadowColor = getBaseColor() - 0x101010;
		int alpha = getBaseColor().a;		
 		shadowColor.a = alpha;
				
		graphics->setColor(shadowColor);
		graphics->fillRectangle(gcn::Rectangle(0,0,getWidth(),getHeight()));

		drawMarker(graphics);
	}

	void Slider::drawBorder(gcn::Graphics* graphics)
	{
		Color faceColor = getBaseColor();
		Color highlightColor, shadowColor;
		int alpha = getBaseColor().a;
		int width = getWidth() + getBorderSize() * 2 - 1;
		int height = getHeight() + getBorderSize() * 2 - 1;
		highlightColor = faceColor + 0x303030;
		highlightColor.a = alpha;
		shadowColor = faceColor - 0x303030;
		shadowColor.a = alpha;
		
		unsigned int i;
		for (i = 0; i < getBorderSize(); ++i)
		{
			graphics->setColor(shadowColor);
			graphics->drawLine(i,i, width - i, i); 
			graphics->drawLine(i,i, i, height - i); 
			graphics->setColor(highlightColor);
			graphics->drawLine(width - i,i, width - i, height - i); 
			graphics->drawLine(i,height - i, width - i, height - i); 
		}
	}
	
	void Slider::drawMarker(gcn::Graphics* graphics)
	{
		gcn::Color faceColor = getBaseColor();
		Color highlightColor, shadowColor;
		int alpha = getBaseColor().a;
		highlightColor = faceColor + 0x303030;
		highlightColor.a = alpha;
		shadowColor = faceColor - 0x303030;
		shadowColor.a = alpha;
		
		int x = mMarkerPosition;
		
		graphics->setColor(faceColor);	
		
		graphics->fillRectangle(gcn::Rectangle(x + 1, 1, mMarkerWidth - 2, getHeight() - 2));

 		graphics->setColor(highlightColor);
 		graphics->drawLine(x, 0, x + mMarkerWidth - 1,0);
 		graphics->drawLine(x, 0, x, getHeight() - 1);
 		graphics->setColor(shadowColor);
 		graphics->drawLine(x + mMarkerWidth - 1, 1, x + mMarkerWidth - 1, getHeight() - 1);
 		graphics->drawLine(x + 1, getHeight() - 1, x + mMarkerWidth - 1, getHeight() - 1);

		if (hasFocus())
		{
			graphics->setColor(getForegroundColor());
			graphics->drawRectangle(Rectangle(x + 2, 2, mMarkerWidth - 4, getHeight() - 4));
		}
	}
	
	void Slider::mousePress(int x, int y, int button)
	{
		if (button == gcn::MouseInput::LEFT
				&& x >= 0 && x <= getWidth()
				&& y >= 0 && y <= getHeight())
		{
			setMarkerPosition(x - mMarkerWidth / 2);			
			mMouseDrag = true;
			generateAction();
		}
		else
		{
			mMouseDrag = false;
		}
	}
	
	void Slider::mouseRelease(int x, int y, int button)
	{
		mMouseDrag = false;
	}
	
	void Slider::lostFocus()
	{
		mMouseDrag = false;
	}
	
	void Slider::mouseMotion(int x, int y)
	{
		if (mMouseDrag)
		{
			setMarkerPosition(x - mMarkerWidth / 2);
			generateAction();
		}
	}
	
	void Slider::setValue(double value)
	{
		int w = getWidth() - mMarkerWidth;
			
		int m = (int)(((mScaleStart - value) * w)
			/ (mScaleStart + mScaleEnd));

		setMarkerPosition(m);
	}
	
	double Slider::getValue()
	{
		double pos = (double)mMarkerPosition / (getWidth() - mMarkerWidth);

		return (1.0 - pos) * mScaleStart + pos * mScaleEnd;
	}

	void Slider::setMarkerPosition(int x)
	{
		int w = getWidth() - mMarkerWidth;
		
		if (x < 0)
		{
			mMarkerPosition = 0;
			return;
		}

		if (x > w)
		{
			mMarkerPosition = w;
			return;
		}
		
		mMarkerPosition = x;		
	}

	int Slider::getMarkerWidth()
	{
		return mMarkerWidth;
	}

	void Slider::setMarkerWidth(int width)
	{
		mMarkerWidth = width;
		setValue(getValue());
	}

	void Slider::keyPress(const Key& key)
	{
		if (key.getValue() == Key::RIGHT)
		{
			setMarkerPosition(mMarkerPosition + 1);
			generateAction();
		}
		else if (key.getValue() == Key::LEFT)
		{
			setMarkerPosition(mMarkerPosition - 1);
			generateAction();
		}
	}
	
} // end gcn
