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

#ifndef GCN_WIDGET_HPP
#define GCN_WIDGET_HPP

#include <list>
#include <string>

#include "guichan/actionlistener.hpp"
#include "guichan/color.hpp"
#include "guichan/defaultfont.hpp"
#include "guichan/font.hpp"
#include "guichan/graphics.hpp"
#include "guichan/keyinput.hpp"
#include "guichan/keylistener.hpp"
#include "guichan/mouseinput.hpp"
#include "guichan/mouselistener.hpp"
#include "guichan/rectangle.hpp"
#include "guichan/platform.hpp"

namespace gcn
{
	/*
	 * This class definiton exists for the widget class to compile.
	 * For more information regarding class FocusHandler please see
	 * the FocusHandler header file.
	 */
	class FocusHandler;

	/*
	 * This class definiton exists for the widget class to compile.
	 * For more information regarding class BasicContainre please see
	 * the BasicContainer header file.
	 */
	class BasicContainer;

	/**
	 * This is the base class for all widgets. It is abstract.
	 * It handles the common logic for all widgets such as
	 * user input.
	 *
	 * NOTE: Functions begining with underscore "_" should not
	 * be overloaded unless you know what you are doing.
	 */
	class GCN_CORE_DECLSPEC Widget
	{
	public:
		/**
		 * Constructor. Resets member variables. Noteable, a widget
		 * is not focusable as default, therefore, widgets that are
		 * supposed to be focusable should overide this default in
		 * their own constructor.
		 */
		Widget();
    
		/**
		 * Default destructor.
		 */
		virtual ~Widget();

		/**
		 * This function should draw the widget.
		 * It is called by the parent widget when it is time
		 * for the widget to draw itself. The graphics object
		 * is set up so that all drawing is relative to the
		 * widget, i.e coordinate (0,0) is the top-left corner
		 * of the widget. It is not possible to draw outside of
		 * a widgets dimension.
		 */
		virtual void draw(Graphics* graphics) = 0;

		/**
		 * This function should draw a widgets border. A border is drawn
		 * around a widget. The width and height of the border is therefore
		 * the widgets height+2*bordersize. Think of a painting that has
		 * a certain size, the border surrounds the painting. 
		 */
		virtual void drawBorder(Graphics* graphics) { }
		
		/**
		 * This functions gets called for all widgets in the gui
		 * each time Gui::logic is called. You can do logic stuff
		 * here like playing an animation.
		 *
		 * @see Gui
		 */
		virtual void logic() { }
    
		/**
		 * @return a pointer to the widgets parent container. Returns
		 *         null if the widget has no parent for example if the
		 *         widget is the gui top widget.
		 */
		virtual BasicContainer* getParent() const;
    
		/**
		 * @param width the widget width in pixels
		 */
		virtual void setWidth(int width);

		/**
		 * @return the widget with in pixels
		 */
		virtual int getWidth();
    
		/**
		 * @param height the widget height in pixels
		 */
		virtual void setHeight(int height);

		/**
		 * @return the widget height in pixels
		 */
		virtual int getHeight();

		/**
		 * Sets the size of the widget.
		 *
		 * @param width the width
		 * @param height the height
		 */
		virtual void setSize(int width, int height);
		
		/**
		 * @param x the widgets x coordinate relative to its
		 *          container.
		 */
		virtual void setX(int x);

		/**
		 * @return the widgets x coordinate relative to its
		 *         container.
		 */
		virtual int getX();

		/**
		 * @param y the widgets y coordinate relative to its
		 *          container.
		 */
		virtual void setY(int y);

		/**
		 * @return the widgets y coordinate relative to its
		 *         container.
		 */
		virtual int getY();

		/**
		 * @param x the widgets x coordinate relative to its
		 *          container.
		 * @param y the widgets y coordinate relative to its
		 *          container.
		 */
		virtual void setPosition(int x, int y);
    
		/**
		 * @param dimension the widgets dimension relative to is
		 *                  container.
		 * @see Rectangle
		 */
		virtual void setDimension(const Rectangle& dimension);

		/**
		 * Sets the size of the border, or the width if you so like.
		 * The size is the number of pixels that the border extends
		 * outside the widget. Border size = 0 means no border.		 
		 *
		 * @param borderSize
		 * @see drawBorder
		 */
		virtual void setBorderSize(unsigned int borderSize);

		/**
		 * Gets the size of the border, or the width if you so like.
		 * The size is the number of pixels that the border extends
		 * outside the widget. Border size = 0 means no border.		 			 
		 *
		 * @return the size of the border
		 * @see drawBorder		 
		 */
		virtual unsigned int getBorderSize();
		
		/**
		 * @return the widgets dimension relative to its
		 *         container.
		 * @see Rectangle
		 */
		virtual const Rectangle& getDimension();

		/**
		 * @param focusable indicates whether the widget is focusable
		 *                  or not.
		 */
		void setFocusable(bool focusable);

		/**
		 * @return true if the widget is focusable.
		 */
		bool isFocusable();

		/**
		 * @return true if the widget currently has focus.
		 */
		bool hasFocus();

		/**
		 * Called if the widget looses focus.
		 */
		virtual void lostFocus() { };
    
		/**
		 * Called if the widget recieves focus.       
		 */
		virtual void gotFocus() { };
    
		/**
		 * @return true if the widget currently has the mouse.
		 */
		virtual bool hasMouse();
    
		/**
		 * @param visible indicates whether the widget is visible or
		 *                not.
		 */
		void setVisible(bool visible);

		/**
		 * @return true if the widget is visible.
		 */
		bool isVisible();

		/**
		 * Sets the widget's base color. (the background
		 * colors for buttons and containers).
		 *
		 * @param color the foreground color.
		 * @see Color
		 */
		virtual void setBaseColor(const Color& color);

		/**
		 * @return the widgets foreground color.
		 * @see Color
		 */
		virtual const Color& getBaseColor() const;
    
		/**
		 * Sets the widget's foreground color.
		 *
		 * @param color the foreground color.
		 * @see Color
		 */
		virtual void setForegroundColor(const Color& color);

		/**
		 * @return the widgets foreground color.
		 * @see Color
		 */
		virtual const Color& getForegroundColor() const;
    
		/**
		 * Sets the widget's background color.
		 *
		 * @param color the background color.
		 * @see Color
		 */
		virtual void setBackgroundColor(const Color& color);

		/**
		 * @return the widgets background color.
		 * @see Color
		 */
		virtual const Color& getBackgroundColor() const;    
            
		/**
		 * This function is used internally be the gui to handle all
		 * mouse messages. Don't call or overload it unless you know
		 * what you are doing.
		 *
		 * @param mouseInput
		 * @see MouseInput
		 */
		virtual void _mouseInputMessage(const MouseInput& mouseInput);

		/**
		 * This function is used internally be the gui to handle all
		 * key messages. Don't call or overload it unless you know
		 * what you are doing.
		 *
		 * @param keyInput
		 * @see KeyInput
		 */
		virtual void _keyInputMessage(const KeyInput& keyInput);

		/**
		 * This function is used internally be the gui to handle
		 * mouse in messages. Don't call this function unless you
		 * know what you are doing.
		 */
		virtual void _mouseInMessage();

		/**
		 * This function is used internally be the gui to handle
		 * mouse out messages. Don't call this function unless you
		 * know what you are doing.
		 */
		virtual void _mouseOutMessage();
    
		/**
		 * This function requests focus for the widget. If the widget
		 * is focusable it will get focus. Otherwise, this function
		 * does nothing.
		 */
		void requestFocus();

		/**
		 * This function requests the container the widgets in to
		 * move the widget to the top.
		 */
		void requestMoveToTop();

		/**
		 * This function requests the container the widgets in to
		 * move the widget to the bottom.
		 */
		void requestMoveToBottom();

		/**
		 * This function sets the focus handler to be used by this
		 * widget. Should not be called or overloaded unless you
		 * know what you are doing.
		 *
		 * @param focusHandler a pointer to a focus handler.
		 * @see FocusHandler
		 */
		virtual void _setFocusHandler(FocusHandler* focusHandler);

		/**
		 * This function gets the focus handler used by this
		 * widget. Should not be called or overloaded unless you
		 * know what you are doing.
		 *
		 * @return the focus handler.
		 */
		virtual FocusHandler* _getFocusHandler();

		/**
		 * This function adds the action listener to the widget.
		 * When an action is triggered, the widget calls its
		 * action listeners. An action can be just about anything,
		 * for example a click on a button or enter pressed in an
		 * edit box.
		 *
		 * @param actionListener the action listener to add.
		 * @see ActionListener
		 */
		void addActionListener(ActionListener* actionListener);
    
		/**
		 * This function removes an action listener from the widget.
		 *
		 * @param actionListener the action listener to remove.
		 * @see ActionListener
		 */
		void removeActionListener(ActionListener* actionListener);

		/**
		 * This function adds a mouse listener to the widget.
		 * When a mouse message is recieved its sent to the
		 * mouse listeners.
		 *
		 * @param mouseListener the mouse listener to add.
		 * @see MouseListener
		 */
		void addMouseListener(MouseListener* mouseListener);
    
		/**
		 * This function removes a mouse listener from the widget.
		 *
		 * @param mouseListener the mouse listener to remove.
		 * @see MouseListener
		 */
		void removeMouseListener(MouseListener* mouseListener);

		/**
		 * This function adds a key listener to the widget.
		 * When a key message is recieved its sent to the 
		 * key listeners.
		 *
		 * @param keyListener the key listener to add.
		 * @see KeyListener
		 */
		void addKeyListener(KeyListener* keyListener);

		/**
		 * This function removes a key listener to the widget.
		 *
		 * @param keyListener the key listener to remove.
		 * @see KeyListener
		 */
		void removeKeyListener(KeyListener* keyListener);

		/**
		 * This function sets the widgets event identifier. An event
		 * identifier is used with action events. If this widget
		 * sends an action event, the widgets event identifier is
		 * sent along with the action so that the action listener can
		 * determine which widget sent the action event.
		 *
		 * NOTE: An event identifier should not be used to identify
		 * a certain widget but rather a certain event in your
		 * application. Several widgets can have the same event
		 * identifer.
		 * 
		 * @param eventId the event identifier
		 * @see ActionListener, getEventId, sendActionEvent
		 */
		void setEventId(const std::string& eventId);

		/**
		 * This function returns the widgets event identifier. An
		 * event identifier is used with action events. If this
		 * widget sends an action event, the widgets event identifier
		 * is sent along with the action so that the action listener
		 * can determine which widget sent the action event.
		 *
		 * @return the widgets event identifier
		 */
		virtual const std::string& getEventId() const;

		/**
		 * This function sends an action event to the widgets action
		 * listener. An action can be just about anything, for
		 * example a click on a button or enter pressed in an edit
		 * box.
		 *
		 * @see setEventId, getEventId, ActionListener
		 */
		void sendActionEvent();

		/**
		 * Gets the absolute position on the screen for the widget,
		 *
		 * @param x absolute x coordinate will be stored in this parameter
		 * @param y absolute y coordinate will be stored in this parameter
		 */
		void getAbsolutePosition(int& x, int& y);
    
		/**
		 * This function sets the widgets parent. It should not be
		 * called unless you know what you are doing.
		 *
		 * @param widget the parent widget. 
		 */
		void _setParent(BasicContainer* parent);

		/**
		 * This function returns the font used by this widget. If no 
		 * font has been set, the global font will be returned instead.
		 * If no global font has been set either, it will fall back on an
		 * ugly default.
		 *
		 * @return the currently used font.
		 * @see Font, DefaultFont
		 */
		Font *getFont() const;
    
		/**
		 * Sets the global font to be used by default for all widgets.
		 *
		 * @param font the global font
		 * @see Font
		 */
		static void setGlobalFont(Font* font);
    
		/**
		 * Sets the font for this widget. Pass NULL to this function
		 * to use the global font.
		 *
		 * @param font the font. If NULL, the global font will be used.
		 * @see setGlobalFont
		 */
		virtual void setFont(Font* font);

		/**
		 * Use this function to trap font changes. It is called
		 * whenever a widgets font is chanegd. 
		 * If the change is global, this function will only be called
		 * if the widget don't have a font of its own.
		 */
		virtual void fontChanged() { }

		/**
		 * Checks whether a widget exists or not, that is if it still exists
		 * an instance of the object.
		 *
		 *  @param widget the widget to check
		 */
		static bool widgetExists(const Widget* widget);

		/**
		 * Tab in means that you can set focus to this widget by pressing
		 * the tab button. If tab in is disabled then the FocusHandler will
		 * skip this widget and focus the next in its focus order.
		 *
		 * @return true if tab is enabled for the widget.
		 */
		virtual bool isTabInEnabled() const;
		
		/**
		 * Tab in means that you can set focus to this widget by pressing
		 * the tab button. If tab in is disabled then the FocusHandler will
		 * skip this widget and focus the next in its focus order.
		 *
		 * @param enabled true if tab in should be enabled.
		 */
		virtual void setTabInEnabled(bool enabled);

		/**
		 * Tab out means that you can lose focus to this widget by pressing
		 * the tab button. If tab out is disabled then the FocusHandler ignores
		 * tabbing and focus will stay with this widget,
		 *
		 * @return true if tab out is enabled for the widget.
		 */
		virtual bool isTabOutEnabled() const;
		
		/**
		 * Tab out means that you can lose focus to this widget by pressing
		 * the tab button. If tab out is disabled then the FocusHandler ignores
		 * tabbing and focus will stay with this widget,
		 *
		 * @param enabled true if tab out should be enabled.
		 */
		virtual void setTabOutEnabled(bool enabled);

	protected:
		/**
		 * This function generates an action to the widgets action listeners.
		 */
		void generateAction();
      
    
		typedef std::list<MouseListener*> MouseListenerList;
		MouseListenerList mMouseListeners;
		typedef MouseListenerList::iterator MouseListenerIterator;
    
		typedef std::list<KeyListener*> KeyListenerList;
		KeyListenerList mKeyListeners;
		typedef KeyListenerList::iterator KeyListenerIterator;
        
	private:
		typedef std::list<ActionListener*> ActionListenerList;
		ActionListenerList mActionListeners;
		typedef ActionListenerList::iterator ActionListenerIterator;

		Color mForegroundColor;
		Color mBackgroundColor;
		Color mBaseColor;
		FocusHandler* mFocusHandler;
		BasicContainer* mParent;
		Rectangle mDimension;
		unsigned int mBorderSize;
		std::string mEventId;
		int mClickTimeStamp;
		int mClickCount;
		int mClickButton;
		bool mHasMouse;
		bool mFocusable;
		bool mVisible;
		bool mTabIn;
		bool mTabOut;
		
		Font* mCurrentFont;
		static DefaultFont mDefaultFont;
		static Font* mGlobalFont;
		static std::list<Widget*> mWidgets;

	}; // end Widget
  
} // end gcn

#endif // end GCN_WIDGET_HPP

/*
 * yakslem  - "I have a really great idea! Why not have an
 *             interesting and funny quote or story at the
 *             end of every source file."
 * finalman - "Yeah - good idea! .... do you know any funny
 *             quotes?"
 * yakslem  - "Well.. I guess not. I just thought it would be
 *             fun to tell funny quotes."
 * finalman - "That's not a very funny quote. But i believe
 *             pointless quotes are funny in their own pointless
 *             way."
 * yakslem  - "...eh...ok..."
 */
