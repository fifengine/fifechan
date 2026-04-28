// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_PANEL_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_PANEL_HPP_

// Standard library includes
#include <string>

// Platform config include
#include "fifechan/platform.hpp"

// Project headers (subdirs before local)
#include "fifechan/listeners/mouselistener.hpp"
#include "fifechan/widgets/container.hpp"

namespace fcn
{
    /**
     * Defines the visibility state of a Panel.
     */
    enum class VisibilityState
    {
        Visible,  //!< Fully visible
        Hidden,   //!< Not rendered, no layout space
        Collapsed //!< Minimal width visible (thin strip)
    };

    /**
     * A Panel widget with a title bar and optional close button.
     *
     * A Panel is a Container with a title bar. Can be used as a base
     * for dialogs, inspector panels, or floating windows.
     *
     * @ingroup widgets
     */
    class FIFEGUI_API Panel : public Container, public MouseListener
    {
    public:
        /**
         * Constructor.
         */
        Panel();

        /**
         * Sets the visibility state of the panel.
         *
         * @param state The visibility state to set.
         * @see getVisibilityState
         */
        void setVisibilityState(VisibilityState state);

        /**
         * Gets the visibility state of the panel.
         *
         * @return The current visibility state.
         * @see setVisibilityState
         */
        VisibilityState getVisibilityState() const;

        /**
         * Sets the width when the panel is collapsed.
         *
         * @param width The width in pixels when collapsed (default 10).
         * @see getCollapsedWidth
         */
        void setCollapsedWidth(int width);

        /**
         * Gets the collapsed width.
         *
         * @return The width in pixels when collapsed.
         * @see setCollapsedWidth
         */
        int getCollapsedWidth() const;

        /**
         * Sets whether the panel is visible.
         * Maps to VisibilityState for backward compatibility.
         *
         * @param visible True if panel should be visible, false otherwise.
         * @see isVisible
         */
        void setVisible(bool visible);

        /**
         * Adjusts the panel size based on its visibility state.
         * When collapsed, the width is limited to the collapsed width.
         */
        void adjustSize() override;

        /**
         * Sets the panel title.
         *
         * @param title The title text to display in the title bar.
         * @see getTitle
         */
        void setTitle(std::string const & title);

        /**
         * Gets the panel title.
         *
         * @return The title text.
         * @see setTitle
         */
        std::string const & getTitle() const;

        /**
         * Sets whether the panel has a close button.
         *
         * @param closable True to show close button, false to hide.
         * @see isClosable
         */
        void setClosable(bool closable);

        /**
         * Checks if the panel has a close button.
         *
         * @return True if closable, false otherwise.
         * @see setClosable
         */
        bool isClosable() const;

    protected:
        /**
         * Draws the panel content.
         *
         * @param graphics A graphics object to draw with.
         */
        void draw(Graphics* graphics) override;

        /**
         * @see MouseListener
         */
        void mousePressed(MouseEvent& event) override;

        /**
         * Handles content area mouse clicks.
         * Called when click is in content area (below title bar).
         *
         * @param event The mouse event.
         * @return true if handled.
         */
        virtual bool handleContentClick(MouseEvent& event);

    private:
        std::string mTitle;
        bool mClosable = true;

        Rectangle mCloseButtonBounds;

        VisibilityState mVisibilityState = VisibilityState::Visible;
        int mCollapsedWidth              = 10;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_PANEL_HPP_
