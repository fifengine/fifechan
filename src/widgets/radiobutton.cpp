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

#include "guichan/widgets/radiobutton.hpp"

namespace gcn
{
	RadioButton::GroupMap RadioButton::mGroupMap;
    
	RadioButton::RadioButton()
	{
		setMarked(false);

		setFocusable(true);
		addMouseListener(this);
		addKeyListener(this);        

	} // end RadioButton
    
	RadioButton::RadioButton(const std::string &caption,
							 const std::string &group,                           
							 bool marked)
	{
		setCaption(caption);
		setGroup(group);
		setMarked(marked);

		setFocusable(true);
		addMouseListener(this);
		addKeyListener(this);

		adjustSize();

	} // end RadioButton
      
	RadioButton::~RadioButton()
	{
		// remove us from the group list
		setGroup("");

	} // end ~RadioButton
    
	void RadioButton::logic()
	{
    
	} // end logic
    
	void RadioButton::draw(Graphics* graphics)
	{
		drawBox(graphics);

		graphics->setFont(getFont());
		graphics->setColor(getForegroundColor());
    
		graphics->drawText(mCaption, getHeight() + getHeight() / 2, 0);      

	} // end draw

	void RadioButton::drawBox(Graphics *graphics)
	{
		int h;

		if (getHeight()%2 == 0)
		{
			h = getHeight() - 2;
		}
		else
		{
			h = getHeight() - 1;
		}
    
		Color faceColor = getBaseColor();
		Color highlightColor = faceColor + 0x303030;
		Color shadowColor = faceColor - 0x303030;      
    
		graphics->setColor(getBackgroundColor());

		int i;
		int hh = (h + 1) / 2;
    
		for (i = 1; i <= hh; ++i)
		{
			graphics->drawLine(hh - i + 1,
							   i,
							   hh + i - 1,
							   i);
		}
     
		for (i = 1; i < hh; ++i)
		{
			graphics->drawLine(hh - i + 1,
							   h - i,
							   hh + i - 1,
							   h - i);
		}
     
		graphics->setColor(shadowColor);
		graphics->drawLine(hh, 0, 0, hh);
		graphics->drawLine(hh + 1, 1, h - 1, hh - 1);
     
		graphics->setColor(highlightColor);
		graphics->drawLine(1, hh + 1, hh, h);
		graphics->drawLine(hh + 1, h - 1, h, hh);
         
		graphics->setColor(getForegroundColor());

		int hhh = hh - 3;
		if (mMarked)
		{
			for (i = 0; i < hhh; ++i)
			{         
				graphics->drawLine(hh - i, 4 + i, hh + i, 4 + i);
			}
			for (i = 0; i < hhh; ++i)
			{         
				graphics->drawLine(hh - i, h - 4 - i, hh + i, h - 4 -  i);
			}

		}

		if (hasFocus())
		{
			graphics->drawLine(hh , 2, 2, hh);
			graphics->drawLine(2 , hh, hh, h - 2);
			graphics->drawLine(hh , h - 2, h - 2, hh);
			graphics->drawLine(h - 2 , hh, hh, 2);
		}  

	} // end drawBox
    
	bool RadioButton::isMarked() const
	{
		return mMarked;

	} // end isMarked
    
	void RadioButton::setMarked(bool marked)
	{
		if (marked && mGroup != "")
		{
			GroupIterator iter, iterEnd;
			iterEnd = mGroupMap.upper_bound(mGroup);

			for (iter = mGroupMap.lower_bound(mGroup);
				 iter != iterEnd;
				 iter++)
			{
				if (iter->second->isMarked())
				{
					iter->second->setMarked(false);
				}
			}             
		}

		mMarked = marked;

	} // end setMarked

	const std::string &RadioButton::getCaption() const
	{
		return mCaption;

	} // end getCaption

	void RadioButton::setCaption(const std::string caption)
	{
		mCaption = caption;

	} // end setCaption

	void RadioButton::keyPress(const Key& key)
	{
		if (key.getValue() == Key::ENTER ||
			key.getValue() == Key::SPACE)
		{
			setMarked(true);
			generateAction();
		}    

	} // end keyPress  

	void RadioButton::mouseClick(int x, int y, int button, int count)
	{
		if (button == MouseInput::LEFT)
		{
			setMarked(true);
			generateAction();
		}  

	} // end mouseClick

	void RadioButton::setGroup(const std::string &group)
	{
		if (mGroup != "")
		{
			GroupIterator iter, iterEnd;
			iterEnd = mGroupMap.upper_bound(mGroup);

			for (iter = mGroupMap.lower_bound(mGroup);
				 iter != iterEnd;
				 iter++)
			{
				if (iter->second == this)
				{
					mGroupMap.erase(iter);
					break;
				}
			}             
		}

		if (group != "")
		{
			mGroupMap.insert(
				std::pair<std::string, RadioButton *>(group, this));
		}

		mGroup = group;

	} // end setGroup

	const std::string &RadioButton::getGroup() const
	{
		return mGroup;

	} // end getGroup

	void RadioButton::adjustSize()
	{
		int height = getFont()->getHeight();

		setHeight(height);
		setWidth(getFont()->getWidth(mCaption) + height + height/2);    

	} // end adjustSize
      
} // end gcn
