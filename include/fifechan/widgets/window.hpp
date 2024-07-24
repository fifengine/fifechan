// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_WINDOW_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_WINDOW_HPP_

#include <string>

#include "fifechan/mouselistener.hpp"
#include "fifechan/platform.hpp"
#include "fifechan/widgets/container.hpp"

namespace fcn
{
    /**
     * An implementation of a movable window that can contain other widgets.
     */
    class FIFEGUI_API Window : public Container, public MouseListener
    {
    public:
        Window();

        /**
         * Constructor. The window will be automatically resized in height
         * to fit the caption.
         *
         * @param caption the caption of the window.
         */
        explicit Window(std::string const & caption);

        virtual ~Window();

        /**
         * Sets the caption of the window.
         *
         * @param caption The caption of the window.
         * @see getCaption
         */
        void setCaption(std::string const & caption);

        /**
         * Gets the caption of the window.
         *
         * @return the caption of the window.
         * @see setCaption
         */
        std::string const & getCaption() const;

        /**
         * Sets the alignment of the caption.
         *
         * @param alignment The alignment of the caption.
         * @see getAlignment, Graphics
         */
        void setAlignment(Graphics::Alignment alignment);

        /**
         * Gets the alignment of the caption.
         *
         * @return The alignment of caption.
         * @see setAlignment, Graphics
         */
        Graphics::Alignment getAlignment() const;

        /**
         * Sets the title bar height.
         *
         * @param height The title height value.
         * @see getTitleBarHeight
         */
        void setTitleBarHeight(unsigned int height);

        /**
         * Gets the title bar height.
         *
         * @return The title bar height.
         * @see setTitleBarHeight
         */
        unsigned int getTitleBarHeight() const;

        void setInnerBorderSize(unsigned int border);

        unsigned int getInnerBorderSize() const;

        /**
         * Sets the window to be movable or not.
         *
         * @param movable True if the window should be movable, false otherwise.
         * @see isMovable
         */
        void setMovable(bool movable);

        /**
         * Checks if the window is movable.
         *
         * @return True if the window is movable, false otherwise.
         * @see setMovable
         */
        bool isMovable() const;

        /**
         * Sets the window to be opaque or not. An opaque window will draw it's background
         * and it's content. A non opaque window will only draw it's content.
         *
         * @param opaque True if the window should be opaque, false otherwise.
         * @see isOpaque
         */
        void setOpaque(bool opaque);

        /**
         * Checks if the window is opaque.
         *
         * @return True if the window is opaque, false otherwise.
         * @see setOpaque
         */
        bool isOpaque() const;

        virtual void drawInnerBorder(Graphics* graphics);

        // Inherited from BasicContainer

        // virtual void resizeToContent();
        virtual void adjustSize();
        // virtual void expandContent();
        virtual Rectangle getChildrenArea();

        // Inherited from Widget

        virtual void draw(Graphics* graphics);

        // Inherited from MouseListener

        virtual void mousePressed(MouseEvent& mouseEvent);

        virtual void mouseDragged(MouseEvent& mouseEvent);

        virtual void mouseReleased(MouseEvent& mouseEvent);

    protected:
        /**
         * Holds the caption of the window.
         */
        std::string mCaption;

        /**
         * Holds the alignment of the caption.
         */
        Graphics::Alignment mAlignment;

        /**
         * Holds the title bar height of the window.
         */
        unsigned int mTitleBarHeight;

        /**
         * Holds the size of the inner border.
         */
        unsigned int mInnerBorder;

        /**
         * True if the window is movable, false otherwise.
         */
        bool mMovable;

        /**
         * True if the window is opaque, false otherwise.
         */
        bool mOpaque;

        /**
         * Holds a drag offset as an x coordinate where the drag of the window
         * started if the window is being dragged. It's used to move the window
         * correctly when dragged.
         */
        int mDragOffsetX;

        /**
         * Holds a drag offset as an y coordinate where the drag of the window
         * started if the window is being dragged. It's used to move the window
         * correctly when dragged.
         */
        int mDragOffsetY;

        /**
         * True if the window is being moved, false otherwise.
         */
        bool mMoved;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_WINDOW_HPP_
