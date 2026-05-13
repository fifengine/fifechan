// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/widgets/menubar.hpp"

// Standard library includes
#include <string>

// Project headers
#include "fifechan/graphics.hpp"
#include "fifechan/widgets/menuitem.hpp"
#include "fifechan/widgets/menupopup.hpp"

namespace fcn
{
    MenuBar::MenuBar()
    {
        setFixedHeight(24);
        setOpaque(true);
        setBorderBottom(1, getBorderStyle());
        // Register as mouse listener to handle clicks on menu bar
        addMouseListener(this);
    }

    Widget* MenuBar::addMenu(std::string const & text, MenuPopup* popup)
    {
        auto* item = new MenuItem(text);

        item->setType(MenuItem::Type::Submenu);

        // Register MenuBar as an action listener so clicks on the top-level
        // menu item are routed to MenuBar::action.
        item->addActionListener(this);

        if (popup != nullptr) {
            item->setSubmenu(popup);
            popup->setParentMenuItem(item);
        }

        // If a popup is supplied, add it to the top-level container so
        // it becomes part of the widget tree and can be shown/hidden.
        if (popup != nullptr) {
            Container* topContainer = nullptr;
            if (auto* p = getTop()) {
                topContainer = dynamic_cast<Container*>(p);
            }
            if (topContainer != nullptr) {
                // Ensure popup is hidden initially
                popup->setVisible(false);
                // Add popup at position 0,0 initially (will be repositioned when shown)
                topContainer->add(popup, 0, 0);
            }
        }

        add(item);
        return item;
    }

    void MenuBar::closeAll()
    {
        if (mOpenMenu != nullptr) {
            mOpenMenu->hide();
            mOpenMenu = nullptr;
        }
    }

    bool MenuBar::isMenuOpen() const
    {
        return mOpenMenu != nullptr;
    }

    MenuPopup* MenuBar::getOpenMenu() const
    {
        return mOpenMenu;
    }

    void MenuBar::action(ActionEvent const & event)
    {
        // Handle menu item click
        auto* source = dynamic_cast<MenuItem*>(event.getSource());

        if (source == nullptr) {
            return;
        }

        if (source->getSubmenu() != nullptr) {
            MenuPopup* popup = source->getSubmenu();

            if (mOpenMenu == popup) {
                // Close already open menu (ModalScope in popup->hide() handles modal pop)
                popup->hide();
                // Restore focus to MenuBar after closing
                requestFocus();
                mOpenMenu = nullptr;
            } else {
                // Close previously open menu
                if (mOpenMenu != nullptr) {
                    mOpenMenu->hide();
                }

                // Request focus on MenuBar before opening menu
                requestFocus();

                // Open new menu at absolute coordinates
                // Get absolute position from the menu item's position + parent's position chain
                int ax = source->getX();
                int ay = source->getY();

                Widget const * parent = source->getParent();

                while (parent != nullptr) {
                    ax += parent->getX();
                    ay += parent->getY();
                    parent = parent->getParent();
                }
                // Add the menu bar's position too
                ax += getX();
                ay += getY();
                popup->show(ax, ay + source->getHeight());
                mOpenMenu = popup;
            }
        }
    }

    void MenuBar::keyPressed(KeyEvent& event)
    {
        // Handle keyboard navigation in menu bar
        Key const key = event.getKey();

        // ESC closes any open menu
        if (key.getValue() == fcn::ESCAPE) {
            if (mOpenMenu != nullptr) {
                mOpenMenu->hide();
                // Restore focus to MenuBar when ESC closes menu
                requestFocus();
                mOpenMenu = nullptr;
                event.consume();
            }
            return;
        }

        // Left/Right to navigate between menu items
        if (key.getValue() == fcn::LEFT || key.getValue() == fcn::RIGHT) {
            unsigned const childCount = getChildrenCount();
            if (childCount == 0) {
                return;
            }

            if (key.getValue() == fcn::LEFT) {
                mSelectedIndex = (mSelectedIndex - 1 + static_cast<int>(childCount)) % static_cast<int>(childCount);
            } else {
                mSelectedIndex = (mSelectedIndex + 1) % static_cast<int>(childCount);
            }

            // Focus the selected menu item
            Widget* child = getChild(mSelectedIndex);
            if (child != nullptr) {
                child->requestFocus();
                event.consume();
            }
            return;
        }

        // Enter or Down to open the selected menu
        if (key.getValue() == fcn::RETURN || key.getValue() == fcn::DOWN) {
            if (mSelectedIndex >= 0 && mSelectedIndex < static_cast<int>(getChildrenCount())) {
                Widget* child = getChild(mSelectedIndex);
                if (auto const * menuItem = dynamic_cast<MenuItem*>(child)) {
                    if (menuItem->getSubmenu() != nullptr) {
                        action(ActionEvent(this, ""));
                    }
                }
            } else {
                // If no item selected, select the first one
                mSelectedIndex = 0;
            }
            event.consume();
            return;
        }
    }

    void MenuBar::keyReleased(KeyEvent& event)
    {
        // No action needed on key release
    }

    void MenuBar::mousePressed(MouseEvent& event)
    {
        // If a menu is already open, handle clicks specially:
        // - Clicking the same MenuItem that opened the popup: do nothing here so
        //   the MenuItem's action can toggle the popup.
        // - Clicking another MenuItem: close current popup but DON'T consume the
        //   event so the other MenuItem's action can open its popup.
        // - Clicking elsewhere on the MenuBar: close current popup and consume
        //   the event.
        if (mOpenMenu != nullptr) {
            Widget* target = getWidgetAt(event.getX(), event.getY());
            if (auto const * mi = dynamic_cast<MenuItem*>(target)) {
                if (mi->getSubmenu() == mOpenMenu) {
                    // Let the MenuItem's action handler toggle the popup.
                    return;
                }

                // Clicked another menu item: close current menu and allow the
                // click to propagate so the new menu's action can open it.
                mOpenMenu->hide();
                mOpenMenu = nullptr;
                return;
            }

            // Clicked not on a menu item: close and consume the event.
            mOpenMenu->hide();
            requestFocus();
            mOpenMenu = nullptr;
            event.consume();
        }
    }

    void MenuBar::draw(Graphics* graphics)
    {
        // Draw background and a fine 1px bottom border line
        if (isOpaque()) {
            graphics->setColor(getBackgroundColor());
            graphics->fillRectangle(Rectangle(0, 0, getWidth(), getHeight()));
        }

        // Draw children (Container::draw will call drawBorder when border size > 0)
        Container::draw(graphics);
    }
} // namespace fcn
