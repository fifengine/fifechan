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

#ifndef GCN_DROPDOWN_HPP
#define GCN_DROPDOWN_HPP

#include "guichan/basiccontainer.hpp"
#include "guichan/focushandler.hpp"
#include "guichan/keylistener.hpp"
#include "guichan/listmodel.hpp"
#include "guichan/mouselistener.hpp"
#include "guichan/platform.hpp"
#include "guichan/widgets/listbox.hpp"
#include "guichan/widgets/scrollarea.hpp"

namespace gcn
{
	/**
	 * This is a drop down box (you know, the ones you can drop down and
	 * select between different values, like a list box). It is one of the
	 * most complicated widgets you will find in gui-chan. For drawing the
	 * dropped down box it uses one ScrollArea and one ListBox. It also
	 * uses an internal FocusHandler to handle the focus of the internal
	 * ScollArea and ListBox.
	 *	 
	 * It uses a ListModel to look up it's values, just like the ListBox.
	 */
	class GCN_CORE_DECLSPEC DropDown :
		public BasicContainer,
		public MouseListener,
		public KeyListener,
		public ActionListener
	{
	public:
		/**
		 * Constructor.
		 */
		DropDown();

		/**
		 * Contructor.
		 *
		 * @param listModel the ListModel to be used.
		 * @see ListModel.
		 */		 
		DropDown(ListModel *listModel);
		
		/**
		 * Contructor.
		 *
		 * @param listModel the ListModel to be used.
		 * @param scrollArea the ScrollArea to be used.
		 * @param listBox the listBox to be used.
		 * @see ListModel, ScrollArea, ListBox.
		 */		 
		DropDown(ListModel *listModel,
				 ScrollArea *scrollArea,
				 ListBox *listBox);

		/**
		 * Destructor.
		 */
		virtual ~DropDown();
    
		/**
		 * @return the selected element.
		 */
		virtual int getSelected();

		/**
		 * Set the selected element.
		 *
		 * @param selected the number of the element to be selected.
		 */
		virtual void setSelected(int selected);

		/**
		 * Sets the ListModel to be used.
		 *
		 * @param listModel the ListModel to be used.
		 * @see ListModel
		 */
		virtual void setListModel(ListModel *listModel);

		/**
		 * @return the ListModel used.
		 * @see ListModel
		 */
		virtual ListModel *getListModel();

		/**
		 * Sets the ScrollArea to be used.
		 *
		 * @param scrollArea the ScrollArea to be used.
		 * @see ScrollArea
		 */
		virtual void setScrollArea(ScrollArea* scrollArea);

		/**
		 * @return the ScrollArea used.
		 * @see ScrollArea
		 */
		virtual ScrollArea *getScrollArea();

		/**
		 * Sets the ListBox to be used.
		 *
		 * @param listBox the ListBox to be used.
		 * @see ListBox
		 */
		virtual void setListBox(ListBox* listBox);

		/**
		 * @return the ListBox used.
		 * @see ListBox
		 */
		virtual ListBox *getListBox();

		/**
		 * Adjusts the height of the DropDown fitting its parents
		 * height.
		 */
		virtual void adjustHeight();


		// Inherited from Widget
		
		virtual void draw(Graphics* graphics);

		virtual void drawBorder(Graphics* graphics);
		
		virtual void _keyInputMessage(const KeyInput& keyInput);
		
		virtual void logic();
		
		virtual void _mouseInputMessage(const MouseInput &mouseInput);

		virtual void lostFocus();

		virtual void setBaseColor(const Color& color);

		virtual void setBackgroundColor(const Color& color);

		virtual void setForegroundColor(const Color& color);
		
		// Inherited from BasicContainer

		virtual void moveToTop(Widget* widget);

		virtual void moveToBottom(Widget* widget);

		virtual void _announceDeath(Widget* widget);

		virtual void getDrawSize(int& width, int& height, Widget* widget);


		// Inherited from ActionListener
		
		virtual void action(const std::string& eventId);


		// Inherited from KeyListener
		
		virtual void keyPress(const Key& key);


		// Inherited from MouseListener
		
		virtual void mousePress(int x, int y, int button);
    
		virtual void mouseRelease(int x, int y, int button);    

		
	protected:
		/**
		 * Draws the button with the little down arrow.
		 *
		 *
		 * @param graphics a Graphics object.
		 */
		virtual void drawButton(Graphics *graphics);

		/**
		 * Sets the DropDown widget to dropped-down mode.
		 */
		virtual void dropDown();

		/**
		 * Sets the DropDown widget to folded-up mode.
		 */
		virtual void foldUp();
    
		bool mDroppedDown;
		bool mPushed;
		int mOldH;
		ScrollArea* mScrollArea;
		ListBox* mListBox;
		ScrollArea* mDefaultScrollArea;
		ListBox* mDefaultListBox;
		FocusHandler mFocusHandler;
    
	}; // end DropDown
  
} // end gcn

#endif // end GCN_DROPDOWN_HPP
