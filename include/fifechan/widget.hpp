/***************************************************************************
 *   Copyright (C) 2012 by the fifechan team                               *
 *   http://fifechan.github.com/fifechan                                   *
 *   This file is part of fifechan.                                        *
 *                                                                         *
 *   fifechan is free software; you can redistribute it and/or             *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA          *
 ***************************************************************************/

/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef FCN_WIDGET_HPP
#define FCN_WIDGET_HPP

#include <list>
#include <string>

#include "fifechan/color.hpp"
#include "fifechan/rectangle.hpp"
#include "fifechan/size.hpp"
#include "fifechan/widgetlistener.hpp"

namespace fcn
{
    class ActionListener;
    class DeathListener;
    class DefaultFont;
    class FocusHandler;
    class FocusListener;
    class Font;
    class Graphics;
    class KeyInput;
    class KeyListener;
    class MouseInput;
    class MouseListener;
    class VisibilityEventHandler;
    class WidgetListener;

    /**
     * Abstract class for widgets of Guichan. It contains basic functions 
     * every widget should have.
     *
     * NOTE: Functions begining with underscore "_" should not
     *       be overloaded unless you know what you are doing.
     *
     * @author Olof Naessén
     * @author Per Larsson
     */
    class FCN_CORE_DECLSPEC Widget
    {
    public:
        /**
         * Constructor. Resets member variables. Noteable, a widget is not
         * focusable as default, therefore, widgets that are supposed to be
         * focusable should overide this default in their own constructor.
         */
        Widget();

        /**
         * Default destructor.
         */
        virtual ~Widget();

        /**
         * Draws the widget. The call to draw is initiated by the widget's
         * parent. The graphics object is set up so that all drawing is relative 
         * to the widget, i.e coordinate (0,0) is the top left corner of the widget. 
         * It is not possible to draw outside of a widget's dimension. If a widget
         * has children, the parent's draw function will always be called before
         * the children's draw functions are called.
         *
         * NOTE: A widget with children won't draw its children unless the
         *       children area given by Widget::getChildrenArea returns a
         *       none empty rectangle inside the widgets dimension. The children
         *       area is considered relative to the widget's position.
         *
         * @param graphics A graphics object to draw with.
         * @see getChildrenArea
         */
        virtual void draw(Graphics* graphics) = 0;

        /**
         * Called when a widget is given a chance to draw a frame around itself.
         * The frame is not considered a part of the widget, it only allows a frame
         * to be drawn around the widget, thus a frame will never be included when
         * calculating if a widget should receive events from user input. Also
         * a widget's frame will never be included when calculating a widget's 
         * position.
         * 
         * The size of the frame is calculated using the widget's frame size. 
         * If a widget has a frame size of 10 pixels than the area the drawFrame 
         * function can draw to will be the size of the widget with an additional 
         * extension of 10 pixels in each direction.
         *
         * An example when drawFrame is a useful function is if a widget needs
         * a glow around itself.
         *
         * @param graphics A graphics object to draw with.
         * @see drawOutline, drawBorder
         */
        virtual void drawFrame(Graphics* graphics);

        /**
         * Called when a widget is given a chance to draw a outline around itself.
         * The outline is not considered as part of the widget, it only allows a outline
         * to be drawn around the widget, thus a outline will never be included when
         * calculating if a widget should receive events from user input. Also
         * a widget's outline will never be included when calculating a widget's 
         * position.
         * 
         * The size of the outline is calculated using the widget's outline size. 
         * If a widget has a outline size of 10 pixels than the area the drawOutline 
         * function can draw to will be the size of the widget with an additional 
         * extension of 10 pixels in each direction.
         *
         * An example when drawOutline is a useful function is if a widget needs
         * a glow around itself.
         *
         * @param graphics A graphics object to draw with.
         * @see setOutlineSize, getOutlineSize
         */
        virtual void drawOutline(Graphics* graphics);

        /**
         * Called when a widget have a border.
         *
         * @param graphics A graphics object to draw with.
         * @see setBorderSize, getBorderSize
         */
        virtual void drawBorder(Graphics* graphics);

        /**
         * Sets the size of the widget's outline. The outline is not considered as part of 
         * the widget, it only allows a outline to be drawn around the widget, thus a outline 
         * will never be included when calculating if a widget should receive events 
         * from user input. Also a widget's outline will never be included when calculating
         * a widget's position.
         *
         * A outline size of 0 means that the widget has no outline. The default outline size
         * is 0.
         *
         * @param size The size of the widget's outline.
         * @see getOutlineSize, drawOutline
         */
        void setOutlineSize(unsigned int size);

        /**
         * Gets the size of the widget's outline. The outline is not considered as part of 
         * the widget, it only allows a outline to be drawn around the widget, thus a outline 
         * will never be included when calculating if a widget should receive events 
         * from user input. Also a widget's outline will never be included when calculating
         * a widget's position.
         *
         * A outline size of 0 means that the widget has no outline. The default outline size
         * is 0.
         *
         * @return The size of the widget's outline.
         * @see setOutlineSize, drawOutline
         */
        unsigned int getOutlineSize() const;

        /**
         * Sets the size of the widget's border. The border is considered as part of 
         * the widget.
         *
         * A border size of 0 means that the widget has no border. The default border size
         * is 0.
         *
         * @param size The size of the widget's border.
         * @see getBorderSize, drawBorder
         */
        void setBorderSize(unsigned int size);

        /**
         * Gets the size of the widget's border. The border is considered as part of 
         * the widget.
         *
         * A border size of 0 means that the widget has no border. The default border size
         * is 0.
         *
         * @return The size of the widget's border.
         * @see setBorderSize, drawBorder
         */
        unsigned int getBorderSize() const;

        /**
         * Sets all 4 margins to one value.
         * The margin clears an area around an element (outside the border).
         * The margin does not have a background color, and is completely transparent.
         * It is also possible to use negative values, to overlap content.

         * @param margin The margin of the widget.
         * @see setMarginTop, setMarginRight, setMarginBottom, setMarginRight
         */
        void setMargin(int margin);

        /**
         * Sets the top margin.
         * The margin clears an area around an element (outside the border).
         * The margin does not have a background color, and is completely transparent.
         * It is also possible to use negative values, to overlap content.

         * @param margin The top margin of the widget.
         * @see getMarginTop
         */
        void setMarginTop(int margin);
        
        /**
         * Gets the top margin.
         * The margin clears an area around an element (outside the border).
         * The margin does not have a background color, and is completely transparent.
         * It is also possible to use negative values, to overlap content.

         * @return The top margin of the widget.
         * @see setMarginTop
         */
        int getMarginTop() const;

        /**
         * Sets the right margin.
         * The margin clears an area around an element (outside the border).
         * The margin does not have a background color, and is completely transparent.
         * It is also possible to use negative values, to overlap content.

         * @param margin The right margin of the widget.
         * @see getMarginRight
         */
        void setMarginRight(int margin);

        /**
         * Gets the right margin.
         * The margin clears an area around an element (outside the border).
         * The margin does not have a background color, and is completely transparent.
         * It is also possible to use negative values, to overlap content.

         * @return The right margin of the widget.
         * @see setMarginRight
         */
        int getMarginRight() const;

        /**
         * Sets the bottom margin.
         * The margin clears an area around an element (outside the border).
         * The margin does not have a background color, and is completely transparent.
         * It is also possible to use negative values, to overlap content.

         * @param margin The bottom margin of the widget.
         * @see getMarginBottom
         */
        void setMarginBottom(int margin);

        /**
         * Gets the bottom margin.
         * The margin clears an area around an element (outside the border).
         * The margin does not have a background color, and is completely transparent.
         * It is also possible to use negative values, to overlap content.

         * @return The bottom margin of the widget.
         * @see setMarginBottom
         */
        int getMarginBottom() const;

        /**
         * Sets the left margin.
         * The margin clears an area around an element (outside the border).
         * The margin does not have a background color, and is completely transparent.
         * It is also possible to use negative values, to overlap content.

         * @param margin The left margin of the widget.
         * @see getMarginLeft
         */
        void setMarginLeft(int margin);

        /**
         * Gets the left margin.
         * The margin clears an area around an element (outside the border).
         * The margin does not have a background color, and is completely transparent.
         * It is also possible to use negative values, to overlap content.

         * @return The left margin of the widget.
         * @see setMarginLeft
         */
        int getMarginLeft() const;

        /**
         * Sets all 4 paddings to one value.
         * The padding clears an area around the content (inside the border) of
         * an element. The padding is affected by the background color of the element.

         * @param padding The top padding of the widget.
         * @see setPaddingTop, setPaddingRight, setPaddingBottom, setPaddingLeft
         */
        void setPadding(unsigned int padding);

        /**
         * Sets the top padding.
         * The padding clears an area around the content (inside the border) of
         * an element. The padding is affected by the background color of the element.

         * @param padding The top padding of the widget.
         * @see getPaddingTop
         */
        void setPaddingTop(unsigned int padding);

        /**
         * Gets the top padding.
         * The padding clears an area around the content (inside the border) of
         * an element. The padding is affected by the background color of the element.

         * @return The top padding of the widget.
         * @see setPaddingTop
         */
        unsigned int getPaddingTop() const;

        /**
         * Sets the right padding.
         * The padding clears an area around the content (inside the border) of
         * an element. The padding is affected by the background color of the element.

         * @param padding The right padding of the widget.
         * @see getPaddingRight
         */
        void setPaddingRight(unsigned int padding);

        /**
         * Gets the right padding.
         * The padding clears an area around the content (inside the border) of
         * an element. The padding is affected by the background color of the element.

         * @return The right padding of the widget.
         * @see setPaddingRight
         */
        unsigned int getPaddingRight() const;

        /**
         * Sets the bottom padding.
         * The padding clears an area around the content (inside the border) of
         * an element. The padding is affected by the background color of the element.

         * @param padding The bottom padding of the widget.
         * @see getPaddingBottom
         */
        void setPaddingBottom(unsigned int padding);

        /**
         * Gets the bottom padding.
         * The padding clears an area around the content (inside the border) of
         * an element. The padding is affected by the background color of the element.

         * @return The bottom padding of the widget.
         * @see setPaddingBottom
         */
        unsigned int getPaddingBottom() const;

        /**
         * Sets the left padding.
         * The padding clears an area around the content (inside the border) of
         * an element. The padding is affected by the background color of the element.

         * @param padding The left padding of the widget.
         * @see getPaddingLeft
         */
        void setPaddingLeft(unsigned int padding);

        /**
         * Gets the left padding.
         * The padding clears an area around the content (inside the border) of
         * an element. The padding is affected by the background color of the element.

         * @return The left padding of the widget.
         * @see setPaddingLeft
         */
        unsigned int getPaddingLeft() const;

        /**
         * Called for all widgets in the gui each time Gui::logic is called.
         * You can do logic stuff here like playing an animation.
         *
         * @see Gui::logic
         */
        virtual void logic() { }

        /**
         * Gets the widget's parent container.
         *
         * @return The widget's parent container. NULL if the widget
         *         has no parent.
         */
        virtual Widget* getParent() const;

        /**
         * Gets the top widget, or top parent, of this widget.
         *
         * @return The top widget, or top parent, for this widget. NULL if no top widget
         *         exists (that is this widget doesn't have a parent).
         */
        virtual Widget* getTop() const;

        /**
         * Sets the width of the widget.
         *
         * @param width The width of the widget.
         * @see getWidth, setHeight, getHeight, setSize,
         *      setDimension, getDimensi
         */
        void setWidth(int width);

        /**
         * Gets the width of the widget.
         *
         * @return The width of the widget.
         * @see setWidth, setHeight, getHeight, setSize,
         *      setDimension, getDimension
         */
        int getWidth() const;

        /**
         * Sets the height of the widget.
         *
         * @param height The height of the widget.
         * @see getHeight, setWidth, getWidth, setSize,
         *      setDimension, getDimension
         */
        void setHeight(int height);

        /**
         * Gets the height of the widget.
         *
         * @return The height of the widget.
         * @see setHeight, setWidth, getWidth, setSize,
         *      setDimension, getDimension
         */
        int getHeight() const;

        /**
         * Sets the size of the widget.
         *
         * @param width The width of the widget.
         * @param height The height of the widget.
         * @see setWidth, setHeight, getWidth, getHeight,
         *      setDimension, getDimension
         */
        void setSize(int width, int height);

        /**
         * Sets the x coordinate of the widget. The coordinate is
         * relateive to the widget's parent.
         *
         * @param x The x coordinate of the widget.
         * @see getX, setY, getY, setPosition, setDimension, getDimension
         */
        void setX(int x);

        /**
         * Gets the x coordinate of the widget. The coordinate is
         * relative to the widget's parent.
         *
         * @return The x coordinate of the widget.
         * @see setX, setY, getY, setPosition, setDimension, getDimension
         */
        int getX() const;

        /**
         * Sets the y coordinate of the widget. The coordinate is
         * relative to the widget's parent.
         *
         * @param y The y coordinate of the widget.
         * @see setY, setX, getX, setPosition, setDimension, getDimension
         */
        void setY(int y);

        /**
         * Gets the y coordinate of the widget. The coordinate is
         * relative to the widget's parent.
         *
         * @return The y coordinate of the widget.
         * @see setY, setX, getX, setPosition, setDimension, getDimension
         */
        int getY() const;

        /**
         * Sets position of the widget. The position is relative
         * to the widget's parent.
         *
         * @param x The x coordinate of the widget.
         * @param y The y coordinate of the widget.
         * @see setX, getX, setY, getY, setDimension, getDimension
         */
        void setPosition(int x, int y);

        /**
         * Sets the dimension of the widget. The dimension is
         * relative to the widget's parent.
         *
         * @param dimension The dimension of the widget.
         * @see getDimension, setX, getX, setY, getY, setPosition
         */
        void setDimension(const Rectangle& dimension);

        /**
         * Gets the dimension of the widget. The dimension is
         * relative to the widget's parent.
         *
         * @return The dimension of the widget.
         * @see getDimension, setX, getX, setY, getY, setPosition
         */
        const Rectangle& getDimension() const;

        /**
         * Gets how many childs the widget have.
         *
         * @return The children count of the widget.
         */
        unsigned int getChildrenCount() const;

        /**
         * Gets how many visible childs the widget have.
         *
         * @return The visible children count of the widget.
         */
        unsigned int getVisibleChildrenCount() const;

        /**
         * Sets the minimal dimension of the widget.
         *
         * @param size The minimal size of the widget.
         * @see getMinSize
         */
        void setMinSize(const Size& size);

        /**
         * Gets the minimal dimension of the widget.
         *
         * @return The minimal size of the widget.
         * @see setMinSize
         */
        const Size& getMinSize() const;

        /**
         * Sets the maximal dimension of the widget.
         *
         * @param size The maximal size of the widget.
         * @see getMaxSize
         */
        void setMaxSize(const Size& size);

        /**
         * Gets the maximal dimension of the widget.
         *
         * @return The maximal size of the widget.
         * @see setMaxSize
         */
        const Size& getMaxSize() const;

        /**
         * Sets the dimension of the widget to a fixed size.
         *
         * @param size The fixed size of the widget.
         * @see getFixedSize, isFixedSize
         */
        void setFixedSize(const Size& size);

        /**
         * Gets the fixed size of the widget.
         *
         * @return The fixed size of the widget.
         * @see setFixedSize, isFixedSize
         */
        const Size& getFixedSize() const;

        /**
         * Gets if the widget use a fixed size.
         *
         * @return True if the widget use a fixed size, otherwise false.
         * @see setFixedSize, getFixedSize
         */
        bool isFixedSize() const;

        /**
         * Sets the widget to be fosusable, or not.
         *
         * @param focusable True if the widget should be focusable,
         *                  false otherwise.
         * @see isFocusable
         */
        void setFocusable(bool focusable);

         /**
         * Checks if a widget is focsable.
         *
         * @return True if the widget should be focusable, false otherwise.
         * @see setFocusable
         */
        bool isFocusable() const;

        /**
         * Checks if the widget is focused.
         *
         * @return True if the widget is focused, false otherwise.
         */
        virtual bool isFocused() const;

        /**
         * Sets the widget to enabled, or not. A disabled
         * widget will never recieve mouse or key events.
         *
         * @param enabled True if widget should be enabled,
         *                false otherwise.
         * @see isEnabled
         */
        void setEnabled(bool enabled);

        /**
         * Checks if the widget is enabled. A disabled
         * widget will never recieve mouse or key events.
         *
         * @return True if widget is enabled, false otherwise.
         * @see setEnabled
         */
        bool isEnabled() const;

        /**
         * Sets the widget to be visible, or not.
         *
         * @param visible True if widget should be visible, false otherwise.
         * @see isVisible
         */
        void setVisible(bool visible);

        /**
         * Checks if the widget is visible.
         *
         * @return True if widget is be visible, false otherwise.
         * @see setVisible
         */
        bool isVisible() const;

        /**
         * Checks if the widget setting is visible.
         *
         * @return True if widget setting is be visible, false otherwise.
         */
        bool isSetVisible() const;

        /**
         * Sets the base color of the widget.
         *
         * @param color The baseground color.
         * @see getBaseColor
         */
        void setBaseColor(const Color& color);

        /**
         * Gets the base color.
         *
         * @return The base color.
         * @see setBaseColor
         */
        const Color& getBaseColor() const;

        /**
         * Sets the foreground color.
         *
         * @param color The foreground color.
         * @see getForegroundColor
         */
        void setForegroundColor(const Color& color);

        /**
         * Gets the foreground color.
         *
         * @see setForegroundColor
         */
        const Color& getForegroundColor() const;

        /**
         * Sets the background color.
         *
         * @param color The background Color.
         * @see setBackgroundColor
         */
        void setBackgroundColor(const Color& color);

        /**
         * Gets the background color.
         *
         * @see setBackgroundColor
         */
        const Color& getBackgroundColor() const;

        /**
         * Sets the selection color.
         *
         * @param color The selection color.
         * @see getSelectionColor
         */
        void setSelectionColor(const Color& color);

        /**
         * Gets the selection color.
         *
         * @return The selection color.
         * @see setSelectionColor
         */
        const Color& getSelectionColor() const;
        
        /**
         * Sets the outline color.
         *
         * @param color The outline color.
         * @see getOutlineColor
         */
        void setOutlineColor(const Color& color);

        /**
         * Gets the outline color.
         *
         * @return The outline color.
         * @see setOutlineColor
         */
        const Color& getOutlineColor() const;

        /**
         * Sets the border color.
         *
         * @param color The border color.
         * @see getBorderColor
         */
        void setBorderColor(const Color& color);

        /**
         * Gets the border color.
         *
         * @return The border color.
         * @see setBorderColor
         */
        const Color& getBorderColor() const;

        /**
         * Requests focus for the widget. A widget will only recieve focus
         * if it is focusable.
         *
         */
        virtual void requestFocus();

        /**
         * Requests a move to the top in the parent widget.
         *
         */
        virtual void requestMoveToTop();

        /**
         * Requests a move to the bottom in the parent widget.
         *
         */
        virtual void requestMoveToBottom();

        /**
         * Called whenever a widget should draw itself. The function will
         * set up clip areas and call the draw function for this widget
         * and for all its children.
         *
         * WARNING: This function is used internally and should not
         *          be called or overloaded unless you know what you
         *          are doing.
         */
        virtual void _draw(Graphics* graphics);

         /**
         * Called whenever a widget should perform logic. The function will
         * call the logic function for this widget and for all its children.
         *
         * WARNING: This function is used internally and should not
         *          be called or overloaded unless you know what you
         *          are doing.
         */
        virtual void _logic();

        /**
         * Sets the focus handler to be used.
         *
         * WARNING: This function is used internally and should not
         *          be called or overloaded unless you know what you
         *          are doing.
         *
         * @param focusHandler The focus handler to use.
         * @see _getFocusHandler
         */
        virtual void _setFocusHandler(FocusHandler* focusHandler);
        
        /**
         * Gets the focus handler used.
         *
         * WARNING: This function is used internally and should not
         *          be called or overloaded unless you know what you
         *          are doing.
         *
         * @return The focus handler used.
         * @see _setFocusHandler
         */
        virtual FocusHandler* _getFocusHandler();
        
        /**
         * Adds an action listener to the widget. When an action event 
         * is fired by the widget the action listeners of the widget 
         * will get notified.
         *
         * @param actionListener The action listener to add.
         * @see removeActionListener
         */
        void addActionListener(ActionListener* actionListener);

        /**
         * Removes an added action listener from the widget.
         *
         * @param actionListener The action listener to remove.
         * @see addActionListener
         */
        void removeActionListener(ActionListener* actionListener);

        /**
         * Adds a death listener to the widget. When a death event is 
         * fired by the widget the death listeners of the widget will 
         * get notified.
         *
         * @param deathListener The death listener to add.
         * @see removeDeathListener
         */
        void addDeathListener(DeathListener* deathListener);

        /**
         * Removes an added death listener from the widget.
         *
         * @param deathListener The death listener to remove.
         * @see addDeathListener
         */
        void removeDeathListener(DeathListener* deathListener);

        /**
         * Adds a mouse listener to the widget. When a mouse event is 
         * fired by the widget the mouse listeners of the widget will 
         * get notified.
         *
         * @param mouseListener The mouse listener to add.
         * @see removeMouseListener
         */
        void addMouseListener(MouseListener* mouseListener);

        /**
         * Removes an added mouse listener from the widget.
         *
         * @param mouseListener The mouse listener to remove.
         * @see addMouseListener
         */
        void removeMouseListener(MouseListener* mouseListener);

        /**
         * Adds a key listener to the widget. When a key event is 
         * fired by the widget the key listeners of the widget will 
         * get notified.
         *
         * @param keyListener The key listener to add.
         * @see removeKeyListener
         */
        void addKeyListener(KeyListener* keyListener);

        /**
         * Removes an added key listener from the widget.
         *
         * @param keyListener The key listener to remove.
         * @see addKeyListener
         */
        void removeKeyListener(KeyListener* keyListener);

        /**
         * Adds a focus listener to the widget. When a focus event is 
         * fired by the widget the key listeners of the widget will 
         * get notified.
         *
         * @param focusListener The focus listener to add.
         * @see removeFocusListener
         */
        void addFocusListener(FocusListener* focusListener);

        /**
         * Removes an added focus listener from the widget.
         *
         * @param focusListener The focus listener to remove.
         * @see addFocusListener
         */
        void removeFocusListener(FocusListener* focusListener);

        /**
         * Adds a widget listener to the widget. When a widget event is 
         * fired by the widget the key listeners of the widget will 
         * get notified.
         *
         * @param widgetListener The widget listener to add.
         * @see removeWidgetListener
         */
        void addWidgetListener(WidgetListener* widgetListener);

        /**
         * Removes an added widget listener from the widget.
         *
         * @param widgetListener The widget listener to remove.
         * @see addWidgetListener
         */
        void removeWidgetListener(WidgetListener* widgetListener);

        /**
         * Sets the action event identifier of the widget. The identifier is
         * used to be able to identify which action has occured.
         *
         * NOTE: An action event identifier should not be used to identify a
         *       certain widget but rather a certain event in your application.
         *       Several widgets can have the same action event identifer.
         *
         * @param actionEventId The action event identifier.
         * @see getActionEventId
         */
        void setActionEventId(const std::string& actionEventId);

        /**
         * Gets the action event identifier of the widget.
         *
         * @return The action event identifier of the widget.
         * @see setActionEventId
         */
        const std::string& getActionEventId() const;

        /**
         * Gets the absolute position on the screen for the widget.
         *
         * @param x The absolute x coordinate will be stored in this parameter.
         * @param y The absolute y coordinate will be stored in this parameter.
         */
        virtual void getAbsolutePosition(int& x, int& y) const;

        /**
         * Sets the parent of the widget. A parent must be a BasicContainer.
         *
         * WARNING: This function is used internally and should not
         *          be called or overloaded unless you know what you
         *          are doing.
         *
         * @param parent The parent of the widget.
         * @see getParent
         */
        virtual void _setParent(Widget* parent);

        /**
         * Gets the font set for the widget. If no font has been set, 
         * the global font will be returned. If no global font has been set, 
         * the default font will be returend.
         *
         * @return The font set for the widget.
         * @see setFont, setGlobalFont
         */
        Font *getFont() const;

        /**
         * Sets the global font to be used by default for all widgets.
         *
         * @param font The global font.
         * @see getGlobalFont
         */
        static void setGlobalFont(Font* font);

        /**
         * Sets the font for the widget. If NULL is passed, the global font 
         * will be used.
         *
         * @param font The font to set for the widget.
         * @see getFont
         */
        void setFont(Font* font);

        /**
         * Called when the font has changed. If the change is global,
         * this function will only be called if the widget doesn't have a
         * font already set.
         *
         */
        virtual void fontChanged() { }

        /**
         * Checks if a widget exists or not, that is if it still exists
         * an instance of the object.
         *
         * @param widget The widget to check.
         * @return True if an instance of the widget exists, false otherwise.
         */
        static bool widgetExists(const Widget* widget);

        /**
         * Checks if tab in is enabled. Tab in means that you can set focus
         * to this widget by pressing the tab button. If tab in is disabled
         * then the focus handler will skip this widget and focus the next
         * in its focus order.
         *
         * @return True if tab in is enabled, false otherwise.
         * @see setTabInEnabled
         */
        bool isTabInEnabled() const;

        /**
         * Sets tab in enabled, or not. Tab in means that you can set focus
         * to this widget by pressing the tab button. If tab in is disabled
         * then the FocusHandler will skip this widget and focus the next
         * in its focus order.
         *
         * @param enabled True if tab in should be enabled, false otherwise.
         * @see isTabInEnabled
         */
        void setTabInEnabled(bool enabled);

        /**
         * Checks if tab out is enabled. Tab out means that you can lose
         * focus to this widget by pressing the tab button. If tab out is
         * disabled then the FocusHandler ignores tabbing and focus will
         * stay with this widget.
         *
         * @return True if tab out is enabled, false otherwise.
         * @see setTabOutEnabled
         */
        bool isTabOutEnabled() const;

        /**
         * Sets tab out enabled. Tab out means that you can lose
         * focus to this widget by pressing the tab button. If tab out is
         * disabled then the FocusHandler ignores tabbing and focus will
         * stay with this widget.
         *
         * @param enabled True if tab out should be enabled, false otherwise.
         * @see isTabOutEnabled
         */
        void setTabOutEnabled(bool enabled);

        /**
         * Requests modal focus. When a widget has modal focus, only that
         * widget and it's children may recieve input.
         *
         * @throws Exception if another widget already has modal focus.
         * @see releaseModalFocus, isModalFocused
         */
        virtual void requestModalFocus();

        /**
         * Requests modal mouse input focus. When a widget has modal input focus
         * that widget will be the only widget receiving input even if the input
         * occurs outside of the widget and no matter what the input is.
         *
         * @throws Exception if another widget already has modal focus.
         * @see releaseModalMouseInputFocus, isModalMouseInputFocused
         */
        virtual void requestModalMouseInputFocus();

        /**
         * Releases modal focus. Modal focus will only be released if the
         * widget has modal focus.
         *
         * @see requestModalFocus, isModalFocused
         */
        virtual void releaseModalFocus();

        /**
         * Releases modal mouse input focus. Modal mouse input focus will only
         * be released if the widget has modal mouse input focus.
         *
         * @see requestModalMouseInputFocus, isModalMouseInputFocused
         */
        virtual void releaseModalMouseInputFocus();

        /**
         * Checks if the widget or it's parent has modal focus.
         *
         * @return True if the widget has modal focus, false otherwise.
         * @see requestModalFocus, releaseModalFocus
         */
        virtual bool isModalFocused() const;

        /**
         * Checks if the widget or it's parent has modal mouse input focus.
         *
         * @return True if the widget has modal mouse input focus, false
         *         otherwise.
         * @see requestModalMouseInputFocus, releaseModalMouseInputFocus
         */
        virtual bool isModalMouseInputFocused() const;

        /**
         * Gets a widget at a certain position in the widget.
         * This function is used to decide which gets mouse input,
         * thus it can be overloaded to change that behaviour.
         *
         * NOTE: This always returns NULL if the widget is not
         *       a container.
         *
         * @param x The x coordinate of the widget to get.
         * @param y The y coordinate of the widget to get.
         * @param exclude Widget to exclude from search, if NULL
         *            no widgets get excluded.
         * @return The widget at the specified coodinate, NULL
         *         if no widget is found.
         */
        virtual Widget *getWidgetAt(int x, int y, Widget* exclude = NULL);

        /**
         * Gets all widgets inside a certain area of the widget.
         *
         * NOTE: This always returns an emtpy list if the widget is not
         *       a container.
         *
         * @param area The area to check.
         * @param ignore If supplied, this widget will be ignored.
         * @return A list of widgets. An empty list if no widgets was found.
         */
        virtual std::list<Widget*> getWidgetsIn(const Rectangle& area, 
                                                Widget* ignore = NULL);

        /**
         * Gets the mouse listeners of the widget.
         *
         * @return The mouse listeners of the widget.
         */
        virtual const std::list<MouseListener*>& _getMouseListeners();

        /**
         * Gets the key listeners of the widget.
         *
         * @return The key listeners of the widget.
         */
        virtual const std::list<KeyListener*>& _getKeyListeners();

        /**
         * Gets the focus listeners of the widget.
         *
         * @return The focus listeners of the widget.
         */
        virtual const std::list<FocusListener*>& _getFocusListeners();

        /**
         * Gets the area of the widget occupied by the widget's children.
         * By default this method returns an empty rectangle as not all
         * widgets are containers. If you want to make a container this
         * method should return the area where the children resides. This
         * method is used when drawing children of a widget when computing
         * clip rectangles for the children.
         *
         * NOTE: The returned rectangle should be relative to the widget,
         *       i.e a rectangle with x and y coordinate (0,0) and with
         *       width and height the same as the widget will let the 
         *       children draw themselves in the whole widget.
         *
         * An example of a widget that overloads this method is ScrollArea.
         * A ScrollArea has a view of its contant and that view is the
         * children area. The size of a ScrollArea's children area might
         * vary depending on if the scroll bars of the ScrollArea is shown
         * or not.
         *
         * @return The area of the widget occupied by the widget's children.
         */
        virtual Rectangle getChildrenArea();

        /**
         * Gets the internal focus handler used.
         *
         * @return the internalFocusHandler used. If no internal focus handler
         *         is used, NULL will be returned.
         * @see setInternalFocusHandler
         */
        virtual FocusHandler* _getInternalFocusHandler();

        /**
         * Sets the internal focus handler. An internal focus handler is
         * needed if both a widget in the widget and the widget itself
         * should be foucsed at the same time.
         *
         * @param focusHandler The internal focus handler to be used.
         * @see getInternalFocusHandler
         */
        void setInternalFocusHandler(FocusHandler* internalFocusHandler);

        /**
         * Moves a widget to the top of this widget. The moved widget will be
         * drawn above all other widgets in this widget.
         *
         * This method is safe to call at any time.
         *
         * @param widget The widget to move to the top.
         * @see moveToBottom
         */
        virtual void moveToTop(Widget* widget);

        /**
         * Moves a widget in this widget to the bottom of this widget.
         * The moved widget will be drawn below all other widgets in this widget.
         *
         * This method is safe to call at any time.
         *
         * @param widget The widget to move to the bottom.
         * @see moveToTop
         */
        virtual void moveToBottom(Widget* widget);

        /**
         * Focuses the next widget in the widget.
         * 
         * @see moveToBottom
         */
        virtual void focusNext();

        /**
         * Focuses the previous widget in the widget.
         *
         * @see moveToBottom
         */
        virtual void focusPrevious();

        /**
         * Tries to show a specific part of a widget by moving it. Used if the
         * widget should act as a container.
         *
         * @param widget The target widget.
         * @param area The area to show.
         */
        virtual void showWidgetPart(Widget* widget, Rectangle area);

        /**
         * Sets an id of a widget. An id can be useful if a widget needs to be
         * identified in a container. For example, if widgets are created by an
         * XML document, a certain widget can be retrieved given that the widget
         * has an id.
         *
         * @param id The id to set to the widget.
         * @see getId, BasicContainer::findWidgetById
         */
        void setId(const std::string& id);

        /**
         * Gets the id of a widget. An id can be useful if a widget needs to be
         * identified in a container. For example, if widgets are created by an
         * XML document, a certain widget can be retrieved given that the widget
         * has an id.
         *
         * @param id The id to set to the widget.
         * @see setId, BasicContainer::findWidgetById
         */
        const std::string& getId() const;

        /**
         * Shows a certain part of a widget in the widget's parent.
         * Used when widgets want a specific part to be visible in
         * its parent. An example is a TextArea that wants a specific
         * part of its text to be visible when a TextArea is a child
         * of a ScrollArea.
         *
         * @param rectangle The rectangle to be shown.
         */
        virtual void showPart(Rectangle rectangle);
    
        /**
         * Sets the visibility event handler to be used.
         * 
         * WARNING: This function is used internally and should not
         *          be called unless you know what you
         *          are doing.
         * 
         * FIXME We don't like the visibility handler being static
         *        but we leave it as is for the moment, until we
         *        come up a better solution.
         * 
         * @param visibilityEventHandler The visibility event handler to be used.
         */
        static void _setVisibilityEventHandler(VisibilityEventHandler* visibilityEventHandler);
        
        /**
         * Gets the visibility event handler of this widget.
         * 
         * WARNING: This function is used internally and should not
         *          be called unless you know what you
         *          are doing.
         * 
         * FIXME  We don't like the visibility handler being static
         *        but we leave it as is for the moment, until we
         *        come up a better solution.
         */
        static VisibilityEventHandler* _getVisibilityEventHandler();
        
        /**
         * Sets the widget to vertical expandable.
         *
         * @param expand True if the widget can be vertical expanded, otherwise false.
         * @see isVerticalExpand
         */
        void setVerticalExpand(bool expand);

        /**
         * Gets if widget is vertical expandable.
         *
         * @return True if the widget can be vertical expanded, otherwise false.
         * @see setVerticalExpand
         */
        bool isVerticalExpand() const;

        /**
         * Sets the widget to horizontal expandable.
         *
         * @param expand True if the widget can be horizontal expanded, otherwise false.
         * @see isHorizontalExpand
         */
        void setHorizontalExpand(bool expand);

        /**
         * Gets if widget is horizontal expandable.
         *
         * @return True if the widget can be horizontal expanded, otherwise false.
         * @see setHorizontalExpand
         */
        bool isHorizontalExpand() const;

        /**
         * Execute the layouting. 
         * In case you want to relayout a visible widget. This function will
         * automatically perform the layout adaption from the top-most layouted widget.
         */
        virtual void adaptLayout();

        /**
         * Resizes the widget's size to fit the content exactly,
         * calls recursively all childs.
         */
        virtual void resizeToContent() {}

        /**
         * Resizes the widget's size to fit the content exactly.
         */
        virtual void adjustSize() {}

        /**
         * Expands the child widgets to the size of this widget,
         * calls recursively all childs.
         */
        virtual void expandContent() {}

        /**
         * Helper function to decide if we need to layout.
         */
        virtual bool isLayouted() { return false; }

    protected:
        /**
         * Distributes an action event to all action listeners
         * of the widget.
         *
         */
        void distributeActionEvent();

        /**
         * Distributes resized events to all of the widget's listeners.
         *
         */
        void distributeResizedEvent();

        /**
         * Distributes moved events to all of the widget's listeners.
         *
         */
        void distributeMovedEvent();

        /**
         * Distributes hidden events to all of the widget's listeners.
         *
         */
        void distributeHiddenEvent();

        /**
         * Distributes shown events to all of the widget's listeners.
         *
         */
        void distributeShownEvent();
        
        /**
         * Distributes ancestor moved events to all of the widget's listeners.
         * All children will also distribute the same event.
         * 
         * @param ancestor Ancestor widget that was moved.
         */
        void distributeAncestorMovedEvent(Widget* ancestor);
        
        /**
         * Distributes ancestor hidden events to all of the widget's listeners.
         * All children will also distribute the same event.
         * 
         * @param ancestor Ancestor widget that was hidden.
         */
        void distributeAncestorHiddenEvent(Widget* ancestor);
        
        /**
         * Distributes ancestor shown events to all of the widget's listeners.
         * All children will also distribute the same event.
         * 
         * @param ancestor Ancestor widget that was shown.
         */
        void distributeAncestorShownEvent(Widget* ancestor);
        
        /**
         * Adds a child to the widget.
         *
         * THIS METHOD IS NOT SAFE TO CALL INSIDE A WIDGETS LOGIC FUNCTION
         * INSIDE ANY LISTER FUNCTIONS!
         *
         * @param widget The widget to add.
         * @see remove, clear
         */
        void add(Widget* widget);

        /**
         * Removes a child from the widget.
         *
         * THIS METHOD IS NOT SAFE TO CALL INSIDE A WIDGETS LOGIC FUNCTION
         * INSIDE ANY LISTER FUNCTIONS!
         *
         * @param widget The widget to remove.
         * @see add, clear
         */
        virtual void remove(Widget* widget);

        /**
         * Clears the widget from all its children.
         *
         * THIS METHOD IS NOT SAFE TO CALL INSIDE A WIDGETS LOGIC FUNCTION
         * INSIDE ANY LISTER FUNCTIONS!
         *
         * @see remove, clear
         */
        virtual void clear();

        /**
         * Finds a widget given an id. This function can be useful
         * when implementing a GUI generator for Guichan, such as
         * the ability to create a Guichan GUI from an XML file.
         *
         * @param id The id to find a widget by.
         * @return The widget with the corrosponding id, 
         *         NULL of no widget is found.
         *
         */
        virtual Widget* findWidgetById(const std::string& id);

        /**
         * Resizes the widget to fit it's children exactly.
         *
         */
        void resizeToChildren();

        /**
         * Checks the size against the size constraints. Used by setDimension.
         *
         */
        void calculateSize();

        /**
         * Gets the children of the widget.
         *
         * @return A list of the widgets children.
         */
        const std::list<Widget*>& getChildren() const;

        /**
         * Holds the mouse listeners of the widget.
         */
        std::list<MouseListener*> mMouseListeners;

        /**
         * Holds the key listeners of the widget.
         */
        std::list<KeyListener*> mKeyListeners;

        /** 
         * Holds the action listeners of the widget.
         */
        std::list<ActionListener*> mActionListeners;

        /**
         * Holds the death listeners of the widget.
         */ 
        std::list<DeathListener*> mDeathListeners;

        /**
         * Holds the focus listeners of the widget.
         */
        std::list<FocusListener*> mFocusListeners;

        /**
         * Holds the widget listeners of the widget.
         */
        std::list<WidgetListener*> mWidgetListeners;

        /**
         * Holds the foreground color of the widget.
         */
        Color mForegroundColor;

        /**
         * Holds the background color of the widget.
         */
        Color mBackgroundColor;

        /**
         * Holds the base color of the widget.
         */
        Color mBaseColor;

        /**
         * Holds the selection color of the widget.
         */
        Color mSelectionColor;

        /**
         * Holds the outline color of the widget.
         */
        Color mOutlineColor;

        /**
         * Holds the border color of the widget.
         */
        Color mBorderColor;

        /**
         * Holds the focus handler used by the widget.
         */
        FocusHandler* mFocusHandler;

        /**
         * Holds the focus handler used by the widget. NULL
         * if no internal focus handler is used.
         */
        FocusHandler* mInternalFocusHandler;
        
        /**
         * Holds the parent of the widget. NULL if the widget
         * has no parent.
         */
        Widget* mParent;

        /**
         * Holds the dimension of the widget.
         */
        Rectangle mDimension;

        /** 
         * Holds the outline size of the widget.
         */
        unsigned int mOutlineSize;

        /** 
         * Holds the border size of the widget.
         */
        unsigned int mBorderSize;

        /** 
         * Holds the top margin of the widget.
         */
        int mMarginTop;

        /** 
         * Holds the top right of the widget.
         */
        int mMarginRight;

        /** 
         * Holds the bottom margin of the widget.
         */
        int mMarginBottom;

        /** 
         * Holds the left margin of the widget.
         */
        int mMarginLeft;

        /** 
         * Holds the top padding of the widget.
         */
        unsigned int mPaddingTop;

        /** 
         * Holds the right padding of the widget.
         */
        unsigned int mPaddingRight;

        /** 
         * Holds the bottom padding of the widget.
         */
        unsigned int mPaddingBottom;

        /** 
         * Holds the left padding of the widget.
         */
        unsigned int mPaddingLeft;

        /**
         * Holds the action event of the widget.
         */
        std::string mActionEventId;

        /**
         * True if the widget focusable, false otherwise.
         */
        bool mFocusable;

        /**
         * True if the widget visible, false otherwise.
         */
        bool mVisible;

        /**
         * True if the widget has tab in enabled, false otherwise.
         */
        bool mTabIn;

        /**
         * True if the widget has tab in enabled, false otherwise.
         */
        bool mTabOut;

        /**
         * True if the widget is enabled, false otherwise.
         */
        bool mEnabled;

        /**
         * Holds the id of the widget.
         */
        std::string mId;

        /**
         * Holds the font used by the widget.
         */
        Font* mCurrentFont;

        /**
         * Holds the min size.
         */
        Size mMinSize;

        /**
         * Holds the min size.
         */
        Size mMaxSize;

        /**
         * Holds the fixed size.
         */
        Size mFixedSize;

        /**
         * True if the widget used a fixed size.
         */
        bool mIsFixedSize;

        /**
         * True if the widget can be vertical expanded.
         */
        bool mVExpand;

        /**
         * True if the widget can be horizontal expanded.
         */
        bool mHExpand;
        /**
         * Holds the default font used by the widget.
         */
        static DefaultFont mDefaultFont;

        /**
         * Holds the global font used by the widget.
         */
        static Font* mGlobalFont;

        /**
         * Holds a list of all instances of widgets.
         */
        static std::list<Widget*> mWidgetInstances;
        
        /**
         * Holds the visibility event handler used by the widgets.
         * 
         * FIXME We don't like the visibility handler being static
         *        but we leave it as is for the moment, until we
         *        come up a better solution.
         */
        static VisibilityEventHandler* mVisibilityEventHandler;

        /**
         * Holds all children of the widget.
         */
        std::list<Widget*> mChildren;
    };
}

#endif // end FCN_WIDGET_HPP
