// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_SECONDARYPANEL_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_SECONDARYPANEL_HPP_

// Platform config include
#include "fifechan/platform.hpp"

// Project headers (subdirs before local)
#include "fifechan/widgets/panel.hpp"

namespace fcn
{
    /**
     * A secondary (right-side) panel widget.
     *
     * SecondaryPanel is a panel positioned on the right edge of the window.
     * It is typically used for properties, outline, or inspector views.
     *
     * @ingroup widgets
     */
    class FIFEGUI_API SecondaryPanel : public Panel
    {
    public:
        /**
         * Constructor.
         *
         * @param title The title to display in the panel title bar.
         */
        explicit SecondaryPanel(std::string const & title = "Secondary");

        ~SecondaryPanel() override;

        SecondaryPanel(SecondaryPanel const &)            = delete;
        SecondaryPanel& operator=(SecondaryPanel const &) = delete;
        SecondaryPanel(SecondaryPanel&&)                  = delete;
        SecondaryPanel& operator=(SecondaryPanel&&)       = delete;

        /**
         * Sets the default width of the panel.
         *
         * @param width The width in pixels.
         * @see getDefaultWidth
         */
        void setDefaultWidth(int width);

        /**
         * Gets the default width of the panel.
         *
         * @return The default width in pixels.
         * @see setDefaultWidth
         */
        int getDefaultWidth() const;

    private:
        /**
         * Default width of the panel.
         */
        int mDefaultWidth{250};
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_SECONDARYPANEL_HPP_
