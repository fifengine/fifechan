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

#include "guichan/widgets/checkbox.hpp"

namespace gcn
{
	
	CheckBox::CheckBox()
	{
		setMarked(false);
		
		setFocusable(true);
		addMouseListener(this);
		addKeyListener(this);

	} // end CheckBox
    
	CheckBox::CheckBox(const std::string &caption, bool marked)
	{
		setCaption(caption);
		setMarked(marked);

		setFocusable(true);
		addMouseListener(this);
		addKeyListener(this);

		adjustSize();

	} // end CheckBox
  
	void CheckBox::logic()
	{

	} // end logic
  
	void CheckBox::draw(Graphics* graphics)
	{
		drawBox(graphics);

		graphics->setFont(getFont());
		graphics->setColor(getForegroundColor());

		graphics->drawText(mCaption, getHeight() + getHeight() / 2, 0);
		
	} // end draw
  
	void CheckBox::drawBox(Graphics *graphics)
	{
		int h = getHeight() - 1;

		Color faceColor = getBaseColor();
		Color highlightColor = faceColor + 0x303030;
		Color shadowColor = faceColor - 0x303030;      

		graphics->setColor(shadowColor);
		graphics->drawLine(0, 0, h, 0);
		graphics->drawLine(0, 1, 0, h);

		graphics->setColor(highlightColor);
		graphics->drawLine(h, 1, h, h);
		graphics->drawLine(1, h, h - 1, h);

		graphics->setColor(getBackgroundColor());
		graphics->fillRectangle(Rectangle(1, 1, h - 1, h - 1));

		graphics->setColor(getForegroundColor());
    
		if (mMarked)
		{
			graphics->drawLine(2, 2, h - 2, h - 2);
			graphics->drawLine(3, 2, h - 2, h - 3);
			graphics->drawLine(2, 3, h - 3, h - 2);
      
			graphics->drawLine(2, h - 2, h - 2, 2);
			graphics->drawLine(2, h - 3, h - 3, 2);
			graphics->drawLine(3, h - 2, h - 2, 3);      
		}

		if (hasFocus())
		{
			graphics->drawRectangle(Rectangle(2, 2, h - 3, h - 3));
		}
		
	} // end drawBox
  
	bool CheckBox::isMarked() const
	{
		return mMarked;
		
	} // end isMarked
	
	void CheckBox::setMarked(bool marked)
	{
		mMarked = marked;

	} // end setMarked

	const std::string &CheckBox::getCaption() const
	{
		return mCaption;

	} // end getCaption

	void CheckBox::setCaption(const std::string caption)
	{
		mCaption = caption;

	} // end setCaption
  
	void CheckBox::keyPress(const Key& key)
	{
		if (key.getValue() == Key::ENTER ||
			key.getValue() == Key::SPACE)
		{
			toggle();
		}

	} // end keyPress
  
	void CheckBox::mouseClick(int x, int y, int button, int count)
	{
		if (button == MouseInput::LEFT)
		{
			toggle();
		}

	} // end mouseClick
  
	void CheckBox::adjustSize()
	{
		int height = getFont()->getHeight();

		setHeight(height);
		setWidth(getFont()->getWidth(mCaption) + height + height/2);

	} // end adjustSize

	void CheckBox::toggle()
	{
		mMarked = !mMarked;
		generateAction();

	} // end toggle
  
} // end gcn

