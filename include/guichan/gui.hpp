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

#ifndef GCN_GUI_HPP
#define GCN_GUI_HPP

#include <string>

#include "guichan/input.hpp"
#include "guichan/platform.hpp"
#include "guichan/widget.hpp"

extern "C"
{
  /**
   * This is a C function which returns the Guichan version.
   * It can be used to check for Guichan with autotools.
   *
   * @return the version of Guichan.
   */
  extern char* gcnGuichanVersion();
}

namespace gcn
{
	// The following comment will appear in the doxygen main page.
  /**
	 * @mainpage
	 * @section Introduction
	 * This documentation is mostly intended as a reference to the API. If you want to get started with Guichan, we suggest you check out the programs in the examples directory of the Guichan release.
	 * @n
	 * @n
	 * This documentation is, and will always be, work in progress. If you find any errors, typos or inconsistencies, or if you feel something needs to be explained in more detail - don't hesitate to tell us.
	 */
	
	 /**
	 * This is the main class for Guichan. It is the core of the a gui.
	 * It holds a special widget called the top widget. For more then
	 * one widget in your Gui, top widget should be a container of some sort.
	 * For the Gui to function properly you need to set one Graphics object
	 * and one Input object.
	 */
  class GCN_CORE_DECLSPEC Gui
  {
  public:

    /**
     * Constructor.
     */
    Gui();

    /**
     * Destructor.
     */
    ~Gui();

    /**
     * Sets the top widget of the gui.
     *
     * @param top the top widget.
     */
    void setTop(Widget* top);

    /**
     * @return the top widget. NULL if no top widget exists.
     * @see setTop
     */
    Widget* getTop() const;

    /**
     * Sets the Graphics object to use for drawing.
     *
     * @param graphics the Graphics object to use for drawing.
     * @see Graphics, SDLGraphics, OpenGLGraphics, AllegroGraphics
     * @todo Explain about the Graphics object in this comment. (Briefly)
     */
    void setGraphics(Graphics* graphics);

    /**
     *  @return the Graphics object used for drawing. NULL if no
     *          Graphics object exists.
     */
    Graphics* getGraphics() const;

    /**
     *  Sets the Input object to use for input handling.
     *
     * @param input the Input object to use for input handling.
     * @param see Input, SDLInput, AllegroInput
     * @todo Explain about the Input object in this comment. (Briefly)
     * @todo Maybe change the tab button to be configurable.
     */
    void setInput(Input* input);
    
    /**
     *  @return the input object used for handling input. NULL if no
     *          Input object exists.
     */
    Input* getInput() const;
    
    /**
     * Performs the Gui:s logic by calling all logic functions
		 * down in the Gui heirarchy. Logic can be just about anything
		 * like adjusting a Widgets size or doing some calculations.
		 *
		 * NOTE: Logic also deals with user input (Mouse and Keyboard)
		 *       for Widgets.
     */
    void logic();

    /**
     * Draws the whole Gui by calling draw functions down in the
		 * Gui hierarchy.
     */
    void draw();

	/**
	 * Focus none of the Widgets in the Gui.
	 */
    void focusNone();

	/**
	 * Toggle the use of the tab key to focus widgets.
	 * By default, tabbing is enabled.
	 *
	 * @param tabbing set to false if you want to disable tabbing
	 */
	void setTabbingEnabled(bool tabbing);

	/**
	 * Checks if tabbing is enabled.
	 *
	 * @return true if tabbing is enabled
	 */
	bool isTabbingEnabled();
			
  protected:
    bool mTopHasMouse;
		bool mTabbing;
		
    Widget* mTop;
    Graphics* mGraphics;
    Input* mInput;
    FocusHandler* mFocusHandler;
    
  }; // end class Gui
  
} // end gcn

#endif // end GCN_GUI_HPP

/* yakslem  - "Women, it's a constant struggle."
 * finalman - "Yes, but sometimes they succeed with their guesses."
 * yaklsem  - "...eh...I was talking about love."
 * finalman - "Oh...ok..."
 * An awkward silence followed.
 */
