/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004, 2005, 2006 Olof Naessén and Per Larsson
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

#ifndef GCN_BASICCONTAINER_HPP
#define GCN_BASICCONTAINER_HPP

#include <list>

#include "guichan/platform.hpp"
#include "guichan/widget.hpp"

namespace gcn
{
    /**
     * Containers base class. Contains basic container functions every
     * container should have. Containers should inherit from this class and
     * implement it's functions.
     *
     * @see Container
     */
    class GCN_CORE_DECLSPEC BasicContainer : public Widget
    {
    public:
        /**
         * Constructor
         */
        BasicContainer();

        /**
         * Destructor
         */
        virtual ~BasicContainer();

        /**
         * Moves a Widget to the top of the BasicContainer. The widget will be
         * drawn above all other Widgets in the BasicContainer.
         *
         * @param widget the Widget to move.
         */
        virtual void moveToTop(Widget* widget);

        /**
         * Moves a widget to the bottom of the BasicContainer. The Widget will
         * be drawn below all other Widgets in the BasicContainer.
         *
         * @param widget the Widget to move.
         */
        virtual void moveToBottom(Widget* widget);

        /**
         * Called when a child of the BasicContainer gets destroyed.
         *
         * @param widget the destroyed Widget.
         */
        virtual void _announceDeath(Widget *widget);

        /**
         * Gets the subarea of the BasicContainer that the children
         * occupy.
         *
         * @return the subarea as a Rectangle.
         */
        virtual Rectangle getChildrenArea();

        /**
         * Focuses the next Widget in the BasicContainer.
         */
        virtual void focusNext();

        /**
         * Focuses the previous Widget in the BasicContainer.
         */
        virtual void focusPrevious();

        /**
         * Gets a widget from a certain position in the container.
         * This function is used to decide which gets mouse input,
         * thus it can be overloaded to change that behaviour.
         *
         * @param x the x coordinate.
         * @param y the y coordinate.
         * @return the widget at the specified coodinate, or NULL
         *         if no such widget exists.
         */
        virtual Widget *getWidgetAt(int x, int y);

        /**
         * Tries to show a specific part of a Widget by moving it.
         *
         * @param widget the target Widget.
         * @param area the area to show.
         */
        virtual void showWidgetPart(Widget* widget, Rectangle area);

        /**
         * Mouse policies to be used with setMouseInputPolicy().
         *
         * NEVER - Mouse input will never be sent the BasicContainer but to 
         *         children of the BasicContainer.
         * ALWAYS - Mouse inpuyt will always be sent to the BasicContainer
         *          as well as to the children of the BasicContainer.
         * NOT_ON_CHILD - Mouse input will not be sent to the BasicContainer if
         *                the mouse input occurs over a child.
         * NOT_IN_CHILDREN_AREA -  Mouse input will not be sent to the
         *                         BasicContainer if the mouse input occurs 
         *                         over the children area.
         *
         */
        enum
        {
            NEVER,
            ALWAYS,
            NOT_ON_CHILD,
            NOT_IN_CHILDREN_AREA
        };
        
        
        // Inherited from Widget

        virtual void logic();

        virtual void _setFocusHandler(FocusHandler* focusHandler);

        virtual void _keyInputMessage(const KeyInput& keyInput);

    protected:
        /**
         * Adds a widget.
         *
         * @param widget the Widget to add.
         */
        virtual void add(Widget* widget);

        /**
         * Removes a widget.
         *
         * @param widget the Widget to remove.
         */
        virtual void remove(Widget* widget);

        /**
         * Clears the BasicContainer of widgets.
         */
        virtual void clear();

        /**
         * Draws children widgets.
         *
         * @param graphics a Graphics object to draw with.
         */
        virtual void drawChildren(Graphics* graphics);

        /**
         * Calls logic for children widgets.
         */
        virtual void logicChildren();

        /**
         * Sets the mouse input policy. The default policy is NOT_ON_CHILD.
         *
         * @param policy the mouse input policy. See enum.
         */
        virtual void setMouseInputPolicy(unsigned int policy);

        /**
         * Gets the mouse input policy.
         *
         * @return the mouse input policy. See enum.
         */
        virtual unsigned int getMouseInputPolicy();

        /**
         * Gets the internal FocusHandler used.
         *
         * @return the internalFocusHandler used. If no internal FocusHandler
         *         is used, NULL will be returned.
         */
        virtual FocusHandler* getInternalFocusHandler();

        /**
         * Sets the internal FocusHandler. An internal focushandler is
         * needed if both a widget in the container and the container
         * should be foucsed at once.
         *
         * @param focusHandler the FocusHandler to be used.
         */
        virtual void setInternalFocusHandler(FocusHandler* focusHandler);

        typedef std::list<Widget *> WidgetList;
        typedef WidgetList::iterator WidgetListIterator;
        typedef WidgetList::reverse_iterator WidgetListReverseIterator;

        WidgetList mWidgets;
        Widget *mWidgetWithMouse;
        unsigned int mMouseInputPolicy;
        FocusHandler* mInternalFocusHandler;
    };
}

#endif // end GCN_BASICCONTAINER_HPP
