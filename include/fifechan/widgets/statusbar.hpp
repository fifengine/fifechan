// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_STATUSBAR_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_STATUSBAR_HPP_

// Standard library includes
#include <memory>
#include <string>

// Platform config include
#include "fifechan/platform.hpp"

// Project headers (subdirs before local)
#include "fifechan/widgets/horizontalbar.hpp"

namespace fcn
{
    /**
     * A status bar widget that displays information at the bottom of a window.
     *
     * @ingroup widgets
     */
    class FIFEGUI_API StatusBar : public HorizontalBar
    {
    public:
        /**
         * Constructor.
         */
        StatusBar();

        ~StatusBar() override = default;

        StatusBar(StatusBar const &)            = delete;
        StatusBar& operator=(StatusBar const &) = delete;
        StatusBar(StatusBar&&)                  = delete;
        StatusBar& operator=(StatusBar&&)       = delete;

        /**
         * Sets the status text.
         *
         * @param text The text to display.
         * @see getText
         */
        void setText(std::string const & text);

        /**
         * Gets the status text.
         *
         * @return The status text.
         * @see setText
         */
        std::string const & getText() const;

        /**
         * Access to the icon area (fixed size, left).
         */
        Container* getIconArea() const;

        /**
         * Access to the left (flexible) area.
         */
        Container* getLeftArea() const;

        /**
         * Access to the right (fixed) area.
         */
        Container* getRightArea() const;

        /**
         * Enables or disables shrink priority handling.
         *
         * When enabled, the status bar will shrink the leftArea first
         * when width is constrained, before truncating text.
         *
         * @param enabled True to enable shrink priority, false to disable.
         * @see isShrinkPriorityEnabled
         */
        void setShrinkPriorityEnabled(bool enabled);

        /**
         * Checks if shrink priority handling is enabled.
         *
         * @return True if shrink priority is enabled.
         * @see setShrinkPriorityEnabled
         */
        bool isShrinkPriorityEnabled() const;

        /**
         * Enables or disables truncation with ellipsis.
         *
         * When enabled and text needs to be truncated, "..." will be
         * shown at the end of the truncated text.
         *
         * @param enabled True to enable truncation with ellipsis.
         * @see isTruncateWithEllipsis
         */
        void setTruncateWithEllipsis(bool enabled);

        /**
         * Checks if truncation with ellipsis is enabled.
         *
         * @return True if truncation with ellipsis is enabled.
         * @see setTruncateWithEllipsis
         */
        bool isTruncateWithEllipsis() const;

    protected:
        /**
         * Draws the status bar.
         */
        void draw(Graphics* graphics) override;

    private:
        /**
         * Status text.
         */
        std::string mText;

        std::unique_ptr<Container> mIconArea;
        std::unique_ptr<Container> mLeftArea;
        std::unique_ptr<Container> mRightArea;

        /**
         * Enable shrink priority handling.
         */
        bool mShrinkPriorityEnabled{false};

        /**
         * Enable truncation with ellipsis.
         */
        bool mTruncateWithEllipsis{true};
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_STATUSBAR_HPP_
