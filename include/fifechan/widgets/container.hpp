/***************************************************************************
 *   Copyright (c) 2017-2019 by the fifechan team                               *
 *   https://github.com/fifengine/fifechan                                 *
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

#ifndef FCN_CONTAINER_HPP
#define FCN_CONTAINER_HPP

#include <list>

#include "fifechan/containerlistener.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/platform.hpp"
#include "fifechan/widget.hpp"

namespace fcn
{
    /**
     * An implementation of a container able to contain other widgets. A widget's 
     * position in the container is relative to the container itself and not the screen. 
     * A container is the most common widget to use as the Gui's top widget as makes the Gui
     * able to contain more than one widget.
     *
     * @see Gui::setTop
     */
    class FCN_CORE_DECLSPEC Container: public Widget
    {
    public:
        enum LayoutPolicy {
            Absolute,
            AutoSize,
            Vertical,
            Horizontal,
            Circular
        };

        /**
         * Constructor. A container is opauqe as default, if you want a
         * none opaque container call setQpaque(false).
         *
         * @see setOpaque, isOpaque
         */
        Container();

        /**
         * Destructor.
         */
        virtual ~Container();

        /**
         * Sets the container to be opaque or not. If the container
         * is opaque its background will be drawn, if it's not opaque
         * its background will not be drawn, and thus making the container
         * completely transparent.
         *
         * NOTE: This is not the same as to set visibility. A non visible
         *       container will not itself nor will it draw its content.
         *
         * @param opaque True if the container should be opaque, false otherwise.
         * @see isOpaque
         */
        virtual void setOpaque(bool opaque);

        /**
         * Checks if the container is opaque or not.
         *
         * @return True if the container is opaque, false otherwise.
         * @see setOpaque
         */
        virtual bool isOpaque() const;

        /**
         * Adds a widget to the container.
         *
         * @param widget The widget to add.
         * @see remove, clear
         */
        virtual void add(Widget* widget);

        /**
         * Adds a widget to the container and also specifies the widget's
         * position in the container. The position is relative to the container
         * and not relative to the screen.
         *
         * @param widget The widget to add.
         * @param x The x coordinate for the widget.
         * @param y The y coordinate for the widget.
         * @see remove, clear
         */
        virtual void add(Widget* widget, int x, int y);

        /**
         * Removes a widget from the Container.
         *
         * @param widget The widget to remove.
         * @throws Exception when the widget has not been added to the
         *                   container.
         * @see add, clear
         */
        virtual void remove(Widget* widget);

        /**
         * Clears the container of all widgets.
         *
         * @see add, remove
         */
        virtual void clear();

        /**
         * Finds a widget given an id.
         *
         * @param id The id to find a widget by.
         * @return A widget with a corrosponding id, NULL if no widget 
         *         is found.
         * @see Widget::setId
         */
        virtual Widget* findWidgetById(const std::string &id);

        /**
         * Adds a container listener to the container. When a widget is
         * added or removed an event will be sent to all container 
         * listeners of the container
         *
         * @param containerListener The container listener to add.
         */
        void addContainerListener(ContainerListener* containerListener);

        /**
         * Removes a container listener from the container.
         *
         * @param containerListener The container listener to remove.
         */
        void removeContainerListener(ContainerListener* containerListener);

        /**
         * Returns the children of the container.
         *
         * @return The children of the container.
         */
        const std::list<Widget*>& getChildren() const;

        // Inherited from Widget

        virtual void resizeToContent(bool recursiv=true);
        virtual void adjustSize();
        virtual void expandContent(bool recursiv=true);
        virtual void draw(Graphics* graphics);
        virtual Rectangle getChildrenArea();
        virtual bool isLayouted() { return mLayout != Absolute; };

        /**
         * Sets the layout of the container.
         * @see LayoutPolicy
         *
         * @param policy The LayoutPolicy of the container.
         * @see getLayout
         */
        virtual void setLayout(LayoutPolicy policy);

        /**
         * Gets the layout of the container.
         * @see LayoutPolicy
         *
         * @return The LayoutPolicy of the container.
         * @see setLayout
         */
        virtual LayoutPolicy getLayout() const;

        /**
         * If enabled, the free space is distributed in a way that the size of the
         * childrens will be equal (if possible).
         * Otherwise the free space will evenly distributed.
         *
         * @param uniform Indicates if uniform size is enabled or not.
         * @see isUniformSize
         */
        virtual void setUniformSize(bool uniform);

        /**
         * True if the container tries to expand the childs
         * to a uniform size.
         *
         * @return True if uniform size is set, otherwise false.
         * @see setUniformSize
         */
        virtual bool isUniformSize() const;

        /**
         * Set the vertical spacing between rows.
         *
         * @param verticalSpacing spacing in pixels.
         * @see getVerticalSpacing
         */
        virtual void setVerticalSpacing(unsigned int spacing);

        /**
         * Get the vertical spacing between rows.
         *
         * @return vertical spacing in pixels.
         * @see setVerticalSpacing
         */
        virtual unsigned int getVerticalSpacing() const;

        /**
         * Set the horizontal spacing between columns.
         *
         * @param horizontalSpacing spacing in pixels.
         * @see getHorizontalSpacing
         */
        virtual void setHorizontalSpacing(unsigned int spacing);

        /**
         * Get the horizontal spacing between rows.
         *
         * @return horizontal spacing in pixels.
         * @see setHorizontalSpacing
         */
        virtual unsigned int getHorizontalSpacing() const;

        void setBackgroundWidget(Widget* widget);
        Widget* getBackgroundWidget();

    protected:
        /**
         * Distributes a widget added container event to all container listeners
         * of the container.
         * 
         * @param source The source widget of the event.
         */
        void distributeWidgetAddedEvent(Widget* source);

        /**
         * Distributes a widget removed container event to all container listeners
         * of the container.
         * 
         * @param source The source widget of the event.
         */
        void distributeWidgetRemovedEvent(Widget* source);

        /**
         * True if the container is opaque, false otherwise.
         */
        bool mOpaque;

        /**
         * Typdef.
         */ 
        typedef std::list<ContainerListener*> ContainerListenerList;
        
        /**
         * The container listeners of the container.
         */
        ContainerListenerList mContainerListeners;

        /**
         * Typedef.
         */
        typedef ContainerListenerList::iterator ContainerListenerIterator;

        /**
         * Layout
         */
        LayoutPolicy mLayout;

        /**
         * Indicates if the childs should be expanded to a uniform size
         */
        bool mUniform;

        /**
         * VerticalSpacing
         */
        unsigned int mVerticalSpacing;

        /**
         * HorizontalSpacing
         */
        unsigned int mHorizontalSpacing;

        Widget* mBackgroundWidget;
    };
}

#endif // end FCN_CONTAINER_HPP
