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

#ifndef GCN_WINDOW_HPP
#define GCN_WINDOW_HPP

#include <string>

#include "guichan/platform.hpp"
#include "guichan/basiccontainer.hpp"

namespace gcn
{
    /**
     * This is a movable window which can contain another widget.
     */
	class GCN_CORE_DECLSPEC Window : public BasicContainer,
                                     public MouseListener
	{
	public:
		/**
		 * Constructor.
		 */
		Window();

		/**
		 * Constructor.
		 *
		 * @param caption the caption of the window.
		 */
		Window(const std::string& caption);

        /**
         * Constructor.
         *
         * @param content the content widget.
		 * @param caption the caption of the window.
         */
        Window(Widget* content, const std::string& caption = "");

        /**
         * Destructor.
         */
        virtual ~Window();
    
		/**
		 * Set the caption of the window.
		 *
		 * @param caption the caption of the button.
		 */
		virtual void setCaption(const std::string& caption);
		
		/**
		 * @return the caption of the window.		 
		 */
		virtual const std::string& getCaption() const;

		/**
		 * Set the alignment for the caption.
		 *
		 * @param alignemnt Graphics::LEFT, Graphics::CENTER or Graphics::RIGHT
		 * @see Graphics
		 */
		virtual void setAlignment(unsigned int alignment);

		/**
		 * Get the alignment for the caption.
		 *
		 * @return alignment of caption.
		 */
		virtual unsigned int getAlignment() const;
		
        /**
         * Set the content widget in the window.
         *
         * @param widget the contant widget.
         */
		virtual void setContent(Widget* widget);

        /**
         * Get the content widget in the window.
         *
         * @return the contant widget.
         */
		virtual Widget* getContent() const;

        /**
         * Set the padding of the window which is the distance
         * between the window border and the content.
         *
         * @param padding the padding value.
         */
        void setPadding(unsigned int padding);

        /**
         * Get the padding of the window.
         *
         * @return the padding value.
         */
        unsigned int getPadding() const;

        /**
         * Set the title bar height.
         *
         * @param title the title height value.
         */      
        void setTitleBarHeight(unsigned int height);

        /**
         * Get the title bar height.
         *
         * @return the title bar height.
         */
        unsigned int getTitleBarHeight();

        /**
         * Set the window to be moveble.
         *
         * @param movable true or false.
         */    
        void setMovable(bool movable);

        /**
         * Check if the window is movable.
         *
         * @return true or false.
         */    
        bool isMovable() const;

        /**
         * Resize the window to fit the content.
         */
        void resizeToContent();

        /**
         * Set the window to be opaque. If it's not opaque,
         * the content area will not be filled with a color.
         *
         * @param opaque true or false.
         */
        void setOpaque(bool opaque);

        /**
         * Check if the window is opaque.
         *
         * @return true or false.
         */
        bool isOpaque();

    
        // Inherited from BasicContainer

        virtual void moveToTop(Widget* widget);

        virtual void moveToBottom(Widget* widget);

        virtual void getDrawSize(int& width, int& height, Widget* widget);

        virtual void _announceDeath(Widget *widget);      
    
    
		// Inherited from Widget
		
		virtual void draw(Graphics* graphics);

		virtual void drawBorder(Graphics* graphics);			

        virtual void _mouseInputMessage(const MouseInput &mouseInput);
    
        virtual void _mouseOutMessage();
    
        virtual void _setFocusHandler(FocusHandler* focusHandler);
        
    
		// Inherited from MouseListener	 

		virtual void mousePress(int x, int y, int button);

		virtual void mouseRelease(int x, int y, int button);

        virtual void mouseMotion(int x, int y);
    
	protected:
        /**
         * Moves the content to the top left corner of the window,
         * uses getContentDimension to get the offset
         */
        virtual void repositionContent();

        /**
         * Gets the area in the window that the content occupies
         */
        virtual Rectangle getContentDimension();
    
		std::string mCaption;
		unsigned int mAlignment;
		Widget* mContent;
        unsigned int mPadding;
        unsigned int mTitleBarHeight;
        bool mMouseDrag;
        int mMouseXOffset;
        int mMouseYOffset;
        bool mMovable;
        bool mOpaque;
    
	}; // end Window
  
} // end gcn

#endif // end GCN_WINDOW_HPP
