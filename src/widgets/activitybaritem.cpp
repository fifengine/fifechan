// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Project headers
#include "fifechan/widgets/activitybaritem.hpp"

// Standard library includes
#include <memory>
#include <string>

// Project headers (subdirs before local)
#include "fifechan/widgets/label.hpp"
#include "fifechan/widgets/tooltip.hpp"

namespace fcn
{
    ActivityBarItem::ActivityBarItem(std::string const & icon, std::string const & tooltip, Widget* panel) :
        mPanel(panel), mTooltip(tooltip)
    {
        setCaption(icon);
        Widget::setSize(mSize, mSize);

        // Register as widget listener on the panel if provided
        if (mPanel != nullptr) {
            mPanel->addWidgetListener(this);
        }
    }

    ActivityBarItem::~ActivityBarItem()
    {
        // Remove ourselves as a widget listener from the panel
        if (mPanel != nullptr) {
            mPanel->removeWidgetListener(this);
        }
    }

    void ActivityBarItem::setPanel(Widget* panel)
    {
        // Remove ourselves as a widget listener from the old panel
        if (mPanel != nullptr) {
            mPanel->removeWidgetListener(this);
        }

        mPanel = panel;

        // Register as widget listener on the new panel
        if (mPanel != nullptr) {
            mPanel->addWidgetListener(this);
        }
    }

    Widget* ActivityBarItem::getPanel() const
    {
        return mPanel;
    }

    void ActivityBarItem::setSize(int size)
    {
        mSize = size;
        Widget::setSize(size, size);
    }

    int ActivityBarItem::getSize() const
    {
        return mSize;
    }

    void ActivityBarItem::setTooltip(std::string const & tooltip)
    {
        mTooltip = tooltip;
    }

    std::string const & ActivityBarItem::getTooltip() const
    {
        return mTooltip;
    }

    void ActivityBarItem::mouseEntered(MouseEvent& mouseEvent)
    {
        // Call base class implementation
        Button::mouseEntered(mouseEvent);

        // Show tooltip if we have tooltip text
        if (!mTooltip.empty()) {
            // Lazily create tooltip widget if needed
            if (!mTooltipWidget) {
                mTooltipWidget = std::make_unique<Tooltip>();
                mTooltipWidget->setSize(150, 30); // Default size, can be adjusted

                // Set up tooltip spec with content callback
                TooltipSpec spec;
                spec.content = [this](int /*widgetId*/) {
                    return mTooltip;
                };
                spec.delayMs = 500; // 500ms delay before showing
                mTooltipWidget->setSpec(spec);
            }

            // Position tooltip relative to this widget (offset to the right)
            int const tooltipX = getWidth() + 5; // 5px offset to the right
            int const tooltipY = 0;
            mTooltipWidget->setPosition(tooltipX, tooltipY);

            // Start hover state
            mTooltipWidget->startHover();

            // Add tooltip as a child of this widget
            add(mTooltipWidget.get());
        }
    }

    void ActivityBarItem::mouseExited(MouseEvent& mouseEvent)
    {
        // Call base class implementation
        Button::mouseExited(mouseEvent);

        // Hide tooltip if it exists
        if (mTooltipWidget) {
            mTooltipWidget->endHover();

            // Remove tooltip from this widget
            remove(mTooltipWidget.get());
        }
    }

    void ActivityBarItem::setSelected(bool selected)
    {
        ToggleButton::setSelected(selected);
        // Sync panel visibility with button state
        if (mPanel != nullptr) {
            mPanel->setVisible(selected);
        }
    }

    void ActivityBarItem::toggleSelected()
    {
        ToggleButton::toggleSelected();
        // Toggle panel visibility
        if (mPanel != nullptr) {
            mPanel->setVisible(isSelected());
        }
    }

    void ActivityBarItem::setPanelVisible(bool visible)
    {
        // Set panel visibility directly
        if (mPanel != nullptr) {
            mPanel->setVisible(visible);
        }

        // Update button selected state to match
        ToggleButton::setSelected(visible);

        // Fire action event so listeners know something changed
        distributeActionEvent();
    }

    void ActivityBarItem::widgetHidden(Event const & event)
    {
        // Only react to our own panel's visibility changes
        if (event.getSource() == mPanel) {
            // Sync button state to match panel (which is now hidden)
            if (isSelected()) {
                ToggleButton::setSelected(false);
            }
        }
    }

    void ActivityBarItem::widgetShown(Event const & event)
    {
        // Only react to our own panel's visibility changes
        if (event.getSource() == mPanel) {
            // Sync button state to match panel (which is now visible)
            if (!isSelected()) {
                ToggleButton::setSelected(true);
            }
        }
    }

} // namespace fcn
