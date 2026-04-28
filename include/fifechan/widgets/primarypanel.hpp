// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_PRIMARYPANEL_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_PRIMARYPANEL_HPP_

// Platform config include
#include "fifechan/platform.hpp"

// Standard library includes
#include <string>

// Project headers (subdirs before local)
#include "fifechan/widgets/panel.hpp"

namespace fcn
{
    /**
     * A primary (left-side) panel widget.
     *
     * PrimaryPanel is a panel positioned on the left edge of the window.
     * It is typically used for explorer views, file trees, or tools.
     *
     * @ingroup widgets
     */
    class FIFEGUI_API PrimaryPanel : public Panel
    {
    public:
        /**
         * Constructor.
         *
         * @param title The title to display in the panel title bar.
         */
        explicit PrimaryPanel(std::string const & title = "Primary");

        ~PrimaryPanel() override;

        PrimaryPanel(PrimaryPanel const &)            = delete;
        PrimaryPanel& operator=(PrimaryPanel const &) = delete;
        PrimaryPanel(PrimaryPanel&&)                  = delete;
        PrimaryPanel& operator=(PrimaryPanel&&)       = delete;

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

#endif // INCLUDE_FIFECHAN_WIDGETS_PRIMARYPANEL_HPP_
