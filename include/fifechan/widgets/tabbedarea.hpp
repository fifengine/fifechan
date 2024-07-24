// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_TABBEDAREA_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_TABBEDAREA_HPP_

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "fifechan/actionlistener.hpp"
#include "fifechan/keylistener.hpp"
#include "fifechan/mouselistener.hpp"
#include "fifechan/platform.hpp"
#include "fifechan/widget.hpp"
#include "fifechan/widgets/container.hpp"

namespace fcn
{
    class Tab;

    /**
     * An implementation of a tabbed area where a user can display a widget by
     * selecting a tab.
     *
     */
    class FIFEGUI_API TabbedArea : public ActionListener, public KeyListener, public MouseListener, public Widget
    {
        friend class Tab;

    public:
        TabbedArea();

        virtual ~TabbedArea();

        /**
         * Sets the tabbed area to be opaque or not. If the tabbed area is
         * opaque its background will be drawn, if it's not opaque its
         * background will not be drawn. By default, a tabbed area is not
         * opaque.
         *
         * The tabbed area's background is normally only visible behind the
         * tabs, since the container holding the tab contents is opaque by
         * default.
         *
         * @param opaque True if the tabbed area should be opaque, false
         *               otherwise.
         * @see isOpaque
         */
        void setOpaque(bool opaque);

        /**
         * Checks if the tabbed area is opaque or not.
         *
         * @return true if the tabbed area is opaque, false otherwise.
         * @see setOpaque
         */
        bool isOpaque() const;

        /**
         * Adds a tab to the tabbed area. The tab will not be deleted by the
         * tabbed area when it is removed.
         *
         * @param tab The tab widget for the tab.
         * @param widget The widget to view when the tab is selected.
         * @see removeTab, removeTabWithIndex
         */
        virtual void addTab(Tab* tab, Widget* widget);

        /**
         * Removes a tab from the tabbed area.
         *
         * @param index The index of the tab to remove.
         * @see addTab
         */
        virtual void removeTabWithIndex(unsigned int index);

        /**
         * Removes a tab from the tabbed area.
         *
         * @param index The tab to remove.
         * @see addTab
         */
        virtual void removeTab(Tab* tab);

        /**
         * Returns the number of tabs in this tabbed area.
         *
         */
        int getNumberOfTabs() const;

        /**
         * Checks if a tab given an index is selected or not.
         *
         * @param index The index of the tab to check.
         * @return True if the tab is selected, false otherwise.
         * @see setSelectedTab
         */
        virtual bool isTabSelected(unsigned int index) const;

        /**
         * Checks if a tab is selected or not.
         *
         * @param index The tab to check.
         * @return True if the tab is selected, false otherwise.
         * @see setSelectedTab
         */
        virtual bool isTabSelected(Tab* tab) const;

        /**
         * Sets a tab given an index to be selected.
         *
         * @param index The index of the tab to be selected.
         * @see isTabSelected, getSelectedTab
         */
        virtual void setSelectedTab(unsigned int index);

        /**
         * Sets a tab to be selected or not.
         *
         * @param index The tab to be selected.
         * @see isTabSelected, getSelectedTab
         */
        virtual void setSelectedTab(Tab* tab);

        /**
         * Gets the index of the selected tab.
         *
         * @return The index of the selected tab.
         *         If no tab is selected -1 will be returned.
         * @see isTabSelected, setSelectedTab
         */
        virtual int getSelectedTabIndex() const;

        /**
         * Gets the selected tab.
         *
         * @return The selected tab.
         * @see isTabSelected, setSelectedTab
         */
        Tab* getSelectedTab() const;

        // Inherited from Widget

        virtual void resizeToContent(bool recursiv = true);

        virtual void expandContent(bool recursiv = true);

        /**
         * Adjusts the size of the tab container and the widget container.
         */
        virtual void adjustSize();

        virtual Rectangle getChildrenArea();

        virtual void draw(Graphics* graphics);

        void setWidth(int width);

        void setHeight(int height);

        void setSize(int width, int height);

        void setDimension(Rectangle const & dimension);

        void setBaseColor(Color const & color);

        void setBackgroundWidget(Widget* widget);
        Widget* getBackgroundWidget();

        /**
         * Sets the layout of the tabbedarea.
         * @see LayoutPolicy
         *
         * @param policy The LayoutPolicy of the tabbedarea.
         * @see getLayout
         */
        void setLayout(Container::LayoutPolicy policy);

        /**
         * Gets the layout of the tabbedarea.
         * @see LayoutPolicy
         *
         * @return The LayoutPolicy of the tabbedarea.
         * @see setLayout
         */
        Container::LayoutPolicy getLayout() const;

        /**
         * If enabled, the free space is distributed in a way that the size of the
         * childrens will be equal (if possible).
         * Otherwise the free space will evenly distributed.
         *
         * @param uniform Indicates if uniform size is enabled or not.
         * @see isUniformSize
         */
        virtual void setUniformSize(bool uniform);

        /**
         * True if the tab container tries to expand the childs
         * to a uniform size.
         *
         * @return True if uniform size is set, otherwise false.
         * @see setUniformSize
         */
        virtual bool isUniformSize() const;

        /**
         * Set the vertical spacing between rows.
         *
         * @param verticalSpacing spacing in pixels.
         * @see getVerticalSpacing
         */
        virtual void setVerticalSpacing(unsigned int spacing);

        /**
         * Get the vertical spacing between rows.
         *
         * @return vertical spacing in pixels.
         * @see setVerticalSpacing
         */
        virtual unsigned int getVerticalSpacing() const;

        /**
         * Set the horizontal spacing between columns.
         *
         * @param horizontalSpacing spacing in pixels.
         * @see getHorizontalSpacing
         */
        virtual void setHorizontalSpacing(unsigned int spacing);

        /**
         * Get the horizontal spacing between rows.
         *
         * @return horizontal spacing in pixels.
         * @see setHorizontalSpacing
         */
        virtual unsigned int getHorizontalSpacing() const;

        // Inherited from ActionListener

        void action(ActionEvent const & actionEvent);

        // Inherited from DeathListener

        virtual void death(Event const & event);

        // Inherited from KeyListener

        virtual void keyPressed(KeyEvent& keyEvent);

        // Inherited from MouseListener

        virtual void mousePressed(MouseEvent& mouseEvent);

    protected:
        /**
         * Adjusts the positions of the tabs.
         */
        void adjustTabPositions();

        /**
         * Holds the selected tab.
         */
        Tab* mSelectedTab;

        /**
         * Holds the container for the tabs.
         */
        Container* mTabContainer;

        /**
         * Holds the container for the widgets.
         */
        Container* mWidgetContainer;

        /**
         * Holds a vector of tabs to delete in the destructor.
         * A tab that is to be deleted is a tab that has been
         * internally created by the tabbed area.
         */
        std::vector<Tab*> mTabsToDelete;

        /**
         * A map between a tab and a widget to display when the
         * tab is selected.
         */
        std::vector<std::pair<Tab*, Widget*>> mTabs;

        /**
         * True if the tabbed area is opaque, false otherwise.
         */
        bool mOpaque;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_TABBEDAREA_HPP_
