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

#include "guichan/keyinput.hpp"
#include "guichan/mouseinput.hpp"
#include "guichan/widgets/textfield.hpp"

namespace gcn
{
  TextField::TextField()
  {
    mCaretPosition = 0;
    mXScroll = 0;

    setFocusable(true);

    addMouseListener(this);
    addKeyListener(this);
    adjustHeight();
    
  } // end TextField
  
  TextField::TextField(const std::string& text)
  {
    mCaretPosition = 0;
    mXScroll = 0;
    
    mText = text;
    adjustSize();
    
    setFocusable(true);    
  
    addMouseListener(this);
    addKeyListener(this);
    
  } // end TextField

  void TextField::setText(const std::string& text)
  {
    if(text.size() < mCaretPosition )
    {
      mCaretPosition = text.size();
    }
    
    mText = text;
    
  } // end setText
  
  void TextField::draw(Graphics* graphics)
  {
		int alpha = getBaseColor().a;
    Color faceColor = getBackgroundColor();
    Color shadowColor = getBaseColor() - 0x303030;
		shadowColor.a = alpha;
    Color highlightColor = getBaseColor() + 0x303030;
		highlightColor.a = alpha;
		
    graphics->setColor(faceColor);
    graphics->fillRectangle(Rectangle(1, 1, getWidth() - 2, getHeight() - 2));
    
    graphics->setColor(shadowColor);
    graphics->drawLine(0, 0, getWidth() - 1, 0);
    graphics->drawLine(0, 1, 0, getHeight() - 1);
    
    graphics->setColor(highlightColor);
    graphics->drawLine(getWidth() - 1, 1, getWidth() - 1, getHeight() - 1);
    graphics->drawLine(1, getHeight() - 1, getWidth() - 1, getHeight() - 1);

    if (hasFocus())
    {      
      drawCaret(graphics, getFont()->getWidth(mText.substr(0, mCaretPosition)) + 1 - mXScroll);
    }
    
    graphics->setColor(getForegroundColor());
    graphics->setFont(getFont());
    graphics->drawText(mText, 2 - mXScroll, 2);
    
  } // end draw

  void TextField::drawCaret(Graphics* graphics, int x)
  {
    graphics->setColor(getForegroundColor());
    graphics->drawLine(x, getHeight() - 3, x, 2);
    
  } // end drawCaret
  
  void TextField::mousePress(int x, int y, int button)
  {
    if (hasMouse() && button == MouseInput::LEFT)
    {
      mCaretPosition = getFont()->getStringIndexAt(mText, x + mXScroll);
      fixScroll();
    }
      
  } // end mousePress
  
  void TextField::keyPress(const Key& key)
  {
    if (key.getValue() == Key::LEFT && mCaretPosition > 0)
    {
      --mCaretPosition;
    }

    else if (key.getValue() == Key::RIGHT && mCaretPosition < mText.size())
    {
      ++mCaretPosition;
    }

	else if (key.getValue() == Key::DELETE && mCaretPosition < mText.size())
    {
      mText.erase(mCaretPosition, 1);
    }

	else if (key.getValue() == Key::BACKSPACE && mCaretPosition > 0)
    {
      mText.erase(mCaretPosition - 1, 1);
      --mCaretPosition;
    }

	else if (key.getValue() == Key::ENTER)
    {
      generateAction();
    }

	else if (key.getValue() == Key::HOME)
    {
      mCaretPosition = 0;
    }    

	else if (key.getValue() == Key::END)
    {
      mCaretPosition = mText.size();
    }    

	else if (key.isCharacter())
    {
      mText.insert(mCaretPosition, std::string(1,(char)key.getValue()));
      ++mCaretPosition;
    }

    fixScroll();
    
  } // end keyPress

  void TextField::keyRelease(const Key& key)
  {

  } // end keyRelease

  void TextField::lostFocus()
  {

  } // end lostFocus

  void TextField::adjustSize()
  {
    setWidth(getFont()->getWidth(mText) + 4);
    adjustHeight();

    fixScroll();
    
  } // end adjustSize
  
  void TextField::adjustHeight()
  {
    setHeight(getFont()->getHeight() + 4);
    
  } // end adjustHeight

  void TextField::fixScroll()
  {
    if (hasFocus())
    {
      int caretX = getFont()->getWidth(mText.substr(0, mCaretPosition));

      if (caretX - mXScroll > getWidth() - 6)
      {
        mXScroll = caretX - getWidth() + 6;
      }
      else if (caretX - mXScroll < getFont()->getWidth(' '))
      {
        mXScroll = caretX - getFont()->getWidth(' ');
        if (mXScroll < 0)
        {
          mXScroll = 0;
        }
      }
    }

  } // end fixScroll

  void TextField::setCaretPosition(unsigned int position)
  {
    if (position > mText.size())
    {
      mCaretPosition = mText.size();
    }
    else
    {    
      mCaretPosition = position;
    }

    fixScroll();
    
  } // end setCaretPosition

  unsigned int TextField::getCaretPosition() const
  {
    return mCaretPosition;
    
  } // end getCaretPosition

  const std::string& TextField::getText() const
  {
    return mText;
    
  } // end getText
  
  void TextField::fontChanged()
  {
    fixScroll();

  } // end fontChanged

} // end gcn
