// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_ACTIVITYBAR_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_ACTIVITYBAR_HPP_

// Platform config include
#include "fifechan/platform.hpp"

// Project headers (subdirs before local)
#include "fifechan/listeners/actionlistener.hpp"
#include "fifechan/widgets/container.hpp"

namespace fcn
{
    /**
     * An activity bar widget that displays toggle buttons on the side of a window.
     *
     * The ActivityBar is a vertical bar that contains toggle buttons
     * for showing/hiding panels (like a sidebar toggle in IDEs).
     *
     * @ingroup widgets
     */
    class FIFEGUI_API ActivityBar : public Container, public ActionListener
    {
    public:
        /**
         * Constructor.
         *
         * @param width The width of the activity bar.
         */
        explicit ActivityBar(int width = 48);

        ~ActivityBar() override;

        ActivityBar(ActivityBar const &)            = delete;
        ActivityBar& operator=(ActivityBar const &) = delete;
        ActivityBar(ActivityBar&&)                  = delete;
        ActivityBar& operator=(ActivityBar&&)       = delete;

        /**
         * Sets the width of the activity bar.
         *
         * @param width The width in pixels.
         * @see getWidth
         */
        void setWidth(int width) override;

        /**
         * Gets the width of the activity bar.
         *
         * @return The width in pixels.
         * @see setWidth
         */
        int getWidth() const;

        /**
         * Sets the spacing between items.
         *
         * @param spacing The spacing in pixels.
         * @see getSpacing
         */
        void setSpacing(unsigned int spacing);

        /**
         * Gets the spacing between items.
         *
         * @return The spacing in pixels.
         * @see setSpacing
         */
        unsigned int getSpacing() const;

        /**
         * Adds an action listener to the activity bar.
         *
         * @param listener The listener to add.
         */
        void addActionListener(ActionListener* listener);

        /**
         * Removes an action listener from the activity bar.
         *
         * @param listener The listener to remove.
         */
        void removeActionListener(ActionListener* listener);

        /**
         * Shows all panels (toggles all buttons to selected state).
         */
        void showAll();

        /**
         * Hides all panels (toggles all buttons to unselected state).
         */
        void hideAll();

        // Inherited from Container

        void resizeToContent(bool recursion = true) override;
        void adjustSize() override;
        void draw(Graphics* graphics) override;

    protected:
        // Inherited from ActionListener

        void action(ActionEvent const & event) override;

    private:
        /**
         * Width of the activity bar.
         */
        int mWidth;

        /**
         * Spacing between items.
         */
        unsigned int mSpacing{4};

        /**
         * List of action listeners.
         */
        std::list<ActionListener*> mActionListeners;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_ACTIVITYBAR_HPP_
