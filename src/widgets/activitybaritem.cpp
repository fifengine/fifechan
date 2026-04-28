// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Project headers
#include "fifechan/widgets/activitybaritem.hpp"

// Standard library includes
#include <string>

// Project headers (subdirs before local)
#include "fifechan/widgets/label.hpp"

namespace fcn
{
    ActivityBarItem::ActivityBarItem(std::string const & icon, std::string const & tooltip, Widget* panel) :
        mSize(40), mPanel(panel)
    {
        setCaption(icon);
        Widget::setSize(mSize, mSize);

        // Register as widget listener on the panel if provided
        if (mPanel) {
            mPanel->addWidgetListener(this);
        }
    }

    ActivityBarItem::~ActivityBarItem()
    {
        // Remove ourselves as a widget listener from the panel
        if (mPanel) {
            mPanel->removeWidgetListener(this);
        }
    }

    void ActivityBarItem::setPanel(Widget* panel)
    {
        // Remove ourselves as a widget listener from the old panel
        if (mPanel) {
            mPanel->removeWidgetListener(this);
        }

        mPanel = panel;

        // Register as widget listener on the new panel
        if (mPanel) {
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

    void ActivityBarItem::setSelected(bool selected)
    {
        ToggleButton::setSelected(selected);
        // Sync panel visibility with button state
        if (mPanel) {
            mPanel->setVisible(selected);
        }
    }

    void ActivityBarItem::toggleSelected()
    {
        ToggleButton::toggleSelected();
        // Toggle panel visibility
        if (mPanel) {
            mPanel->setVisible(isSelected());
        }
    }

    void ActivityBarItem::setPanelVisible(bool visible)
    {
        // Set panel visibility directly
        if (mPanel) {
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
