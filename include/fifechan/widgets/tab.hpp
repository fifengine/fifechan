// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_TAB_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_TAB_HPP_

#include <map>
#include <string>

#include "fifechan/mouselistener.hpp"
#include "fifechan/platform.hpp"
#include "fifechan/widgets/container.hpp"

namespace fcn
{
    class TabbedArea;

    /**
     * An implementation of a simple tab to be used in a tabbed area.
     *
     * @see TabbedArea
     */
    class FIFEGUI_API Tab : public MouseListener, public Container
    {
    public:
        Tab();

        virtual ~Tab();

        /**
         * Sets the tabbed area the tab should be a part of.
         *
         * @param tabbedArea The tabbed area the tab should be a part of.
         * @see getTabbedArea
         */
        void setTabbedArea(TabbedArea* tabbedArea);

        /**
         * Gets the tabbed are the tab is a part of.
         *
         * @return The tabbed are the tab is a part of.
         * @see setTabbedArea
         */
        TabbedArea* getTabbedArea();

        // Inherited from Widget

        /**
         * Adjusts the size of the tab to fit the caption. If this tab was
         * added to a TabbedArea, it will also adjust the tab positions.
         */
        virtual void adjustSize();

        virtual Rectangle getChildrenArea();

        virtual void draw(Graphics* graphics);

        // Inherited from MouseListener

        virtual void mouseEntered(MouseEvent& mouseEvent);

        virtual void mouseExited(MouseEvent& mouseEvent);

    protected:
        /**
         * True if the tab has the mouse, false otherwise.
         */
        bool mHasMouse;

        /**
         * Holds the tabbed area the tab is a part of.
         */
        TabbedArea* mTabbedArea;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_TAB_HPP_
