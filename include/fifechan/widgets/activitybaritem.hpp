// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_ACTIVITYBARITEM_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_ACTIVITYBARITEM_HPP_

// Standard library includes
#include <string>

// Platform config include
#include "fifechan/platform.hpp"

// Project headers (subdirs before local)
#include "fifechan/listeners/widgetlistener.hpp"
#include "fifechan/widgets/togglebutton.hpp"

namespace fcn
{
    /**
     * A toggle button item for use in an ActivityBar.
     *
     * ActivityBarItem is a toggle button that shows/hides a panel.
     * It has icon-only display and supports selected/unselected states.
     *
     * @ingroup widgets
     */
    class FIFEGUI_API ActivityBarItem : public ToggleButton
    {
    public:
        /**
         * Constructor.
         *
         * @param icon The icon to display (can be image path or emoji).
         * @param tooltip The tooltip text.
         */
        explicit ActivityBarItem(std::string const & icon, std::string const & tooltip = "", Widget* panel = nullptr);

        ~ActivityBarItem() override;

        ActivityBarItem(ActivityBarItem const &)            = delete;
        ActivityBarItem& operator=(ActivityBarItem const &) = delete;
        ActivityBarItem(ActivityBarItem&&)                  = delete;
        ActivityBarItem& operator=(ActivityBarItem&&)       = delete;

        /**
         * Sets the target panel that this item controls.
         *
         * @param panel The panel widget to show/hide.
         * @see getPanel
         */
        void setPanel(Widget* panel);

        /**
         * Gets the target panel that this item controls.
         *
         * @return The panel widget, or nullptr if not set.
         * @see setPanel
         */
        Widget* getPanel() const;

        /**
         * Sets the panel visibility and updates button state accordingly.
         *
         * @param visible True to show the panel, false to hide.
         * @see setPanel
         * @see isSelected
         */
        void setPanelVisible(bool visible);

        /**
         * Sets the default size for activity bar items.
         *
         * @param size The size in pixels (width and height).
         */
        void setSize(int size);

        /**
         * Gets the default size.
         *
         * @return The size in pixels.
         */
        int getSize() const;

        // Inherited from ToggleButton

        void setSelected(bool selected) override;
        void toggleSelected() override;

    protected:
        // Inherited from WidgetListener (via ToggleButton/Button)

        void widgetHidden(Event const & event) override;
        void widgetShown(Event const & event) override;

        /**
         * Panel that this item controls.
         */
        Widget* mPanel{nullptr};

        /**
         * Default size for items.
         */
        int mSize{40};

    private:
        /**
         * Syncs button selected state to panel visibility.
         */
        void syncButtonToPanel();

        /**
         * Syncs panel visibility to button selected state.
         */
        void syncPanelToButton();
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_ACTIVITYBARITEM_HPP_
