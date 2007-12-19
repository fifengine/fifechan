/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004, 2005, 2006, 2007 Olof Naessén and Per Larsson
 *
 *                                                         Js_./
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

#ifndef GCN_BUTTON_HPP
#define GCN_BUTTON_HPP

#include <string>

#include "guichan/focuslistener.hpp"
#include "guichan/keylistener.hpp"
#include "guichan/mouseevent.hpp"
#include "guichan/mouselistener.hpp"
#include "guichan/platform.hpp"
#include "guichan/widget.hpp"

namespace gcn
{
    /**
     * An implementation of a regular clickable button. A button is capable of
     * displaying a caption. 
     *
     * If a button is clicked an action event will be sent to all action listener's 
     * of the button.
     *
     * @see ImageButton
     */
    class GCN_CORE_DECLSPEC Button : public Widget,
                                     public MouseListener,
                                     public KeyListener,
                                     public FocusListener
    {
    public:
        /**
         * Constructor.
         */
        Button();

        /**
         * Constructor. The button will be automatically resized
         * to fit it's caption.
         *
         * @param caption The caption of the button.
         */
        Button(const std::string& caption);

        /**
         * Sets the caption of the button. It's advisable to call
         * adjustSize after setting of the caption to adjust the
         * button's size to fit the caption.
         *
         * @param caption The caption of the button.
         * @see getCaption, adjustSize
         */
        void setCaption(const std::string& caption);

        /**
         * Gets the caption of the button.
         *
         * @return The caption of the button.
         */
        const std::string& getCaption() const;

        /**
         * Sets the alignment for the caption. The alignment is relative
         * to the center of the button.
         *
         * @param alignment Graphics::LEFT, Graphics::CENTER or Graphics::RIGHT
         * @see getAlignment, Graphics
         */
        void setAlignment(unsigned int alignment);

        /**
         * Gets the alignment for the caption.
         *
         * @return Alignment of caption. Graphics::LEFT, Graphics::CENTER or Graphics::RIGHT.
         * @see setAlignment, Graphics
         */
        unsigned int getAlignment() const;

        /**
         * Sets the spacing between the border of this button and its caption.
         *
         * @param spacing The default value for spacing is 4 and can be changed 
         *                using this method.
         * @see getSpacing
         */
        void setSpacing(unsigned int spacing);

        /**
         * Gets the spacing between the border of this button and its caption.
         *
         * @return spacing.
         * @see setSpacing
         */
        unsigned int getSpacing() const;

        /**
         * Adjusts the button's size to fit the caption.
         */
        void adjustSize();

        /**
         * Checks if the button is pressed. Convenient method to use
         * when overloading the draw method of the button.
         *
         * @return True if the button is pressed, false otherwise.
         */
        bool isPressed() const;


        //Inherited from Widget

        virtual void draw(Graphics* graphics);

        virtual void drawBorder(Graphics* graphics);


        // Inherited from FocusListener

        virtual void focusLost(const Event& event);


        // Inherited from MouseListener

        virtual void mousePressed(MouseEvent& mouseEvent);

        virtual void mouseReleased(MouseEvent& mouseEvent);

        virtual void mouseEntered(MouseEvent& mouseEvent);

        virtual void mouseExited(MouseEvent& mouseEvent);

        virtual void mouseDragged(MouseEvent& mouseEvent);


        // Inherited from KeyListener

        virtual void keyPressed(KeyEvent& keyEvent);

        virtual void keyReleased(KeyEvent& keyEvent);

    protected:
        /**
         * Holds the caption of the button.
         */
        std::string mCaption;

        /**
         * True if the mouse is ontop of the button, false otherwise.
         */
        bool mHasMouse;

        /**
         * True if a key has been pressed, false otherwise.
         */
        bool mKeyPressed;

        /**
         * True if a mouse has been pressed, false otherwise.
         */
        bool mMousePressed;

        /**
         * Holds the alignment of the caption.
         */
        unsigned int mAlignment;

        /**
         * Holds the spacing between the border and the caption.
         */
        unsigned int mSpacing;
    };
}

#endif // end GCN_BUTTON_HPP
