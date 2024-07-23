// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/widgets/tabbedarea.hpp"

#include <algorithm>

#include "fifechan/exception.hpp"
#include "fifechan/focushandler.hpp"
#include "fifechan/font.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/widgets/tab.hpp"

namespace fcn
{
    TabbedArea::TabbedArea() : mSelectedTab(NULL), mOpaque(false)
    {
        setFocusable(true);
        addKeyListener(this);
        addMouseListener(this);

        mTabContainer = new Container();
        mTabContainer->setOpaque(false);
        mTabContainer->setLayout(Container::Horizontal);
        mWidgetContainer = new Container();
        mWidgetContainer->setLayout(Container::Vertical);
        add(mTabContainer);
        add(mWidgetContainer);
    }

    TabbedArea::~TabbedArea()
    {
        remove(mTabContainer);
        remove(mWidgetContainer);

        delete mTabContainer;
        delete mWidgetContainer;

        for (unsigned int i = 0; i < mTabsToDelete.size(); i++) {
            delete mTabsToDelete[i];
        }
    }

    void TabbedArea::addTab(Tab* tab, Widget* widget)
    {
        tab->setTabbedArea(this);
        tab->addActionListener(this);
        if (tab->getLayout() == Container::Absolute) {
            tab->setLayout(getLayout());
        }
        mTabContainer->add(tab);
        mTabs.push_back(std::pair<Tab*, Widget*>(tab, widget));

        if (mSelectedTab == NULL) {
            setSelectedTab(tab);
        } else {
            adaptLayout();
        }
    }

    void TabbedArea::removeTabWithIndex(unsigned int index)
    {
        if (index >= mTabs.size()) {
            fcn::throwException("No such tab index.", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        removeTab(mTabs[index].first);
    }

    void TabbedArea::removeTab(Tab* tab)
    {
        int tabIndexToBeSelected = -1;

        if (tab == mSelectedTab) {
            int index     = getSelectedTabIndex();
            int mTabsSize = static_cast<int>(mTabs.size());
            if (index == (mTabsSize - 1 && mTabsSize >= 2)) {
                tabIndexToBeSelected = index - 1;
            } else if (index == mTabsSize - 1 && mTabsSize == 1) {
                tabIndexToBeSelected = -1;
            } else {
                tabIndexToBeSelected = index;
            }
        }

        std::vector<std::pair<Tab*, Widget*>>::iterator iter;
        for (iter = mTabs.begin(); iter != mTabs.end(); iter++) {
            if (iter->first == tab) {
                mTabContainer->remove(tab);
                mTabs.erase(iter);
                break;
            }
        }

        std::vector<Tab*>::iterator iter2;
        for (iter2 = mTabsToDelete.begin(); iter2 != mTabsToDelete.end(); iter2++) {
            if (*iter2 == tab) {
                mTabsToDelete.erase(iter2);
                delete tab;
                break;
            }
        }

        if (tabIndexToBeSelected == -1) {
            mSelectedTab = NULL;
            mWidgetContainer->clear();
            adaptLayout();
        } else {
            mWidgetContainer->clear();
            setSelectedTab(tabIndexToBeSelected);
        }
    }

    int TabbedArea::getNumberOfTabs() const
    {
        return mTabs.size();
    }

    bool TabbedArea::isTabSelected(unsigned int index) const
    {
        if (index >= mTabs.size()) {
            fcn::throwException("No such tab index.", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        return mSelectedTab == mTabs[index].first;
    }

    bool TabbedArea::isTabSelected(Tab* tab) const
    {
        return mSelectedTab == tab;
    }

    void TabbedArea::setSelectedTab(unsigned int index)
    {
        if (index >= mTabs.size()) {
            fcn::throwException("No such tab index.", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        setSelectedTab(mTabs[index].first);
    }

    void TabbedArea::setSelectedTab(Tab* tab)
    {
        if (tab == mSelectedTab) {
            return;
        }
        unsigned int i;
        for (i = 0; i < mTabs.size(); i++) {
            if (mTabs[i].first == mSelectedTab) {
                mWidgetContainer->remove(mTabs[i].second);
            }
        }

        for (i = 0; i < mTabs.size(); i++) {
            if (mTabs[i].first == tab) {
                mSelectedTab = tab;
                mWidgetContainer->add(mTabs[i].second);
            }
        }
        adaptLayout();
    }

    int TabbedArea::getSelectedTabIndex() const
    {
        unsigned int i;
        for (i = 0; i < mTabs.size(); i++) {
            if (mTabs[i].first == mSelectedTab) {
                return i;
            }
        }

        return -1;
    }

    Tab* TabbedArea::getSelectedTab() const
    {
        return mSelectedTab;
    }

    void TabbedArea::setOpaque(bool opaque)
    {
        mOpaque = opaque;
    }

    bool TabbedArea::isOpaque() const
    {
        return mOpaque;
    }

    void TabbedArea::setBackgroundWidget(Widget* widget)
    {
        mTabContainer->setBackgroundWidget(widget);
        mWidgetContainer->setBackgroundWidget(widget);
    }

    Widget* TabbedArea::getBackgroundWidget()
    {
        return mTabContainer->getBackgroundWidget();
    }

    void TabbedArea::draw(Graphics* graphics)
    {
        Color const & faceColor = getBaseColor();
        int const alpha         = getBaseColor().a;
        Color highlightColor    = faceColor + 0x303030;
        highlightColor.a        = alpha;
        Color shadowColor       = faceColor - 0x303030;
        shadowColor.a           = alpha;

        // Draw a border.
        graphics->setColor(highlightColor);
        graphics->drawLine(0, mTabContainer->getHeight(), 0, getHeight() - 2);
        graphics->setColor(shadowColor);
        graphics->drawLine(getWidth() - 1, mTabContainer->getHeight() + 1, getWidth() - 1, getHeight() - 1);
        graphics->drawLine(1, getHeight() - 1, getWidth() - 1, getHeight() - 1);

        if (isOpaque()) {
            graphics->setColor(getBaseColor());
            graphics->fillRectangle(1, 1, getWidth() - 2, getHeight() - 2);
        }

        // Draw a line underneath the tabs.
        graphics->setColor(highlightColor);
        graphics->drawLine(1, mTabContainer->getHeight(), getWidth() - 1, mTabContainer->getHeight());

        // If a tab is selected, remove the line right underneath
        // the selected tab.
        if (mSelectedTab != NULL) {
            graphics->setColor(getBaseColor());
            graphics->drawLine(
                mSelectedTab->getX() + 1,
                mTabContainer->getHeight(),
                mSelectedTab->getX() + mSelectedTab->getWidth() - 2,
                mTabContainer->getHeight());
        }

        // drawChildren(graphics);
    }

    Rectangle TabbedArea::getChildrenArea()
    {
        Rectangle rec;
        rec.x      = getBorderSize() + getPaddingLeft();
        rec.y      = getBorderSize() + getPaddingTop();
        rec.width  = getWidth() - 2 * getBorderSize() - getPaddingLeft() - getPaddingRight();
        rec.height = getHeight() - 2 * getBorderSize() - getPaddingTop() - getPaddingBottom();
        return rec;
    }

    void TabbedArea::resizeToContent(bool recursiv)
    {
        if (recursiv) {
            mTabContainer->resizeToContent(recursiv);
            mWidgetContainer->resizeToContent(recursiv);
        }
        resizeToChildren();
        adjustSize();
        adjustTabPositions();
    }

    void TabbedArea::expandContent(bool recursiv)
    {
        if (recursiv) {
            mTabContainer->expandContent(recursiv);
            mWidgetContainer->expandContent(recursiv);
        }
        adjustSize();
        adjustTabPositions();
    }

    void TabbedArea::adjustSize()
    {
        int totalTabWidth  = 0;
        int totalTabHeight = 0;
        int maxTabWidth    = 0;
        int maxTabHeight   = 0;

        Rectangle area = getChildrenArea();
        for (unsigned int i = 0; i < mTabs.size(); i++) {
            totalTabWidth += mTabs[i].first->getWidth();
            totalTabHeight += mTabs[i].first->getHeight();
            if (mTabs[i].first->getWidth() > maxTabWidth) {
                maxTabWidth = mTabs[i].first->getWidth();
            }
            if (mTabs[i].first->getHeight() > maxTabHeight) {
                maxTabHeight = mTabs[i].first->getHeight();
            }
        }

        if (getLayout() == Container::Vertical) {
            mTabContainer->setSize(maxTabWidth, getHeight() - 2);
            mWidgetContainer->setSize(getWidth() - maxTabWidth - 2, getHeight() - 2);
            mWidgetContainer->setPosition(maxTabWidth + 1, 1);
        } else if (getLayout() == Container::Horizontal) {
            mTabContainer->setSize(getWidth() - 2, maxTabHeight);
            mWidgetContainer->setSize(getWidth() - 2, getHeight() - maxTabHeight - 2);
            mWidgetContainer->setPosition(1, maxTabHeight + 1);
        }
    }

    void TabbedArea::adjustTabPositions()
    {
        int maxTabWidth  = 0;
        int maxTabHeight = 0;
        unsigned int i;
        for (i = 0; i < mTabs.size(); i++) {
            if (mTabs[i].first->getWidth() > maxTabWidth) {
                maxTabWidth = mTabs[i].first->getWidth();
            }
            if (mTabs[i].first->getHeight() > maxTabHeight) {
                maxTabHeight = mTabs[i].first->getHeight();
            }
        }

        if (getLayout() == Container::Vertical) {
            int y = 0;
            for (i = 0; i < mTabs.size(); i++) {
                Tab* tab = mTabs[i].first;
                tab->setPosition(maxTabWidth - tab->getWidth(), y);
                y += tab->getHeight();
            }
        } else if (getLayout() == Container::Horizontal) {
            int x = 0;
            for (i = 0; i < mTabs.size(); i++) {
                Tab* tab = mTabs[i].first;
                tab->setPosition(x, maxTabHeight - tab->getHeight());
                x += tab->getWidth();
            }
        }
    }

    void TabbedArea::setWidth(int width)
    {
        // This may seem odd, but we want the TabbedArea to adjust
        // it's size properly before we call Widget::setWidth as
        // Widget::setWidth might distribute a resize event.
        fcn::Rectangle dim = mDimension;
        mDimension.width   = width;
        adjustSize();
        mDimension = dim;
        Widget::setWidth(width);
    }

    void TabbedArea::setHeight(int height)
    {
        // This may seem odd, but we want the TabbedArea to adjust
        // it's size properly before we call Widget::setHeight as
        // Widget::setHeight might distribute a resize event.
        fcn::Rectangle dim = mDimension;
        mDimension.height  = height;
        adjustSize();
        mDimension = dim;
        Widget::setHeight(height);
    }

    void TabbedArea::setSize(int width, int height)
    {
        // This may seem odd, but we want the TabbedArea to adjust
        // it's size properly before we call Widget::setSize as
        // Widget::setSize might distribute a resize event.
        fcn::Rectangle dim = mDimension;
        mDimension.width   = width;
        mDimension.height  = height;
        adjustSize();
        mDimension = dim;
        Widget::setSize(width, height);
    }

    void TabbedArea::setDimension(Rectangle const & dimension)
    {
        // This may seem odd, but we want the TabbedArea to adjust
        // it's size properly before we call Widget::setDimension as
        // Widget::setDimension might distribute a resize event.
        fcn::Rectangle dim = mDimension;
        mDimension         = dimension;
        adjustSize();
        mDimension = dim;
        Widget::setDimension(dimension);
    }

    void TabbedArea::keyPressed(KeyEvent& keyEvent)
    {
        if (keyEvent.isConsumed() || !isFocused()) {
            return;
        }

        if (keyEvent.getKey().getValue() == Key::Left) {
            int index = getSelectedTabIndex();
            index--;

            if (index < 0) {
                return;
            } else {
                setSelectedTab(mTabs[index].first);
            }

            keyEvent.consume();
        } else if (keyEvent.getKey().getValue() == Key::Right) {
            int index = getSelectedTabIndex();
            index++;

            if (index >= static_cast<int>(mTabs.size())) {
                return;
            } else {
                setSelectedTab(mTabs[index].first);
            }

            keyEvent.consume();
        }
    }

    void TabbedArea::mousePressed(MouseEvent& mouseEvent)
    {
        // we ignore that, otherwise the tab can not be pressed
        // because the content consumed the event
        // if (mouseEvent.isConsumed())
        //{
        //    return;
        //}
        if (mouseEvent.getButton() == MouseEvent::Left) {
            Widget* widget = mTabContainer->getWidgetAt(mouseEvent.getX(), mouseEvent.getY());
            Tab* tab       = dynamic_cast<Tab*>(widget);

            if (tab != NULL) {
                setSelectedTab(tab);
            }
        }

        // Request focus only if the source of the event
        // is not focusble. If the source of the event
        // is focused we don't want to steal the focus.
        if (!mouseEvent.getSource()->isFocusable()) {
            requestFocus();
        }
    }

    void TabbedArea::death(Event const & event)
    {
        Tab* tab = dynamic_cast<Tab*>(event.getSource());

        if (tab != NULL) {
            removeTab(tab);
        } else {
            // BasicContainer::death(event);
        }
    }

    void TabbedArea::action(ActionEvent const & actionEvent)
    {
        Widget* source = actionEvent.getSource();
        Tab* tab       = dynamic_cast<Tab*>(source);

        if (tab == NULL) {
            fcn::throwException(
                "Received an action from a widget that's not a tab!",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        setSelectedTab(tab);
    }

    void TabbedArea::setBaseColor(Color const & color)
    {
        Widget::setBaseColor(color);
        mWidgetContainer->setBaseColor(color);
        mTabContainer->setBaseColor(color);
    }

    void TabbedArea::setLayout(Container::LayoutPolicy policy)
    {
        mTabContainer->setLayout(policy);
    }

    Container::LayoutPolicy TabbedArea::getLayout() const
    {
        return mTabContainer->getLayout();
    }

    void TabbedArea::setUniformSize(bool uniform)
    {
        mTabContainer->setUniformSize(uniform);
    }

    bool TabbedArea::isUniformSize() const
    {
        return mTabContainer->isUniformSize();
    }

    void TabbedArea::setVerticalSpacing(unsigned int spacing)
    {
        mTabContainer->setVerticalSpacing(spacing);
    }

    unsigned int TabbedArea::getVerticalSpacing() const
    {
        return mTabContainer->getVerticalSpacing();
    }

    void TabbedArea::setHorizontalSpacing(unsigned int spacing)
    {
        mTabContainer->setHorizontalSpacing(spacing);
    }

    unsigned int TabbedArea::getHorizontalSpacing() const
    {
        return mTabContainer->getHorizontalSpacing();
    }
} // namespace fcn
