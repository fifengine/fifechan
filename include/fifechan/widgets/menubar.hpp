// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_MENUBAR_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_MENUBAR_HPP_

// Platform config include
#include "fifechan/platform.hpp"

// Project headers (subdirs before local)
#include "fifechan/listeners/actionlistener.hpp"
#include "fifechan/listeners/keylistener.hpp"
#include "fifechan/listeners/mouselistener.hpp"
#include "fifechan/widgets/horizontalbar.hpp"

namespace fcn
{
    class MenuPopup;

    /**
     * A menu bar widget that displays menus at the top of a window.
     *
     * MenuBar displays horizontal top-level menu items. Clicking on an item
     * opens the corresponding MenuPopup dropdown.
     *
     * @ingroup widgets
     */
    class FIFEGUI_API MenuBar : public HorizontalBar, public ActionListener, public KeyListener, public MouseListener
    {
    public:
        /**
         * Constructor.
         */
        MenuBar();

        ~MenuBar() override;

        MenuBar(MenuBar const &)            = delete;
        MenuBar& operator=(MenuBar const &) = delete;
        MenuBar(MenuBar&&)                  = delete;
        MenuBar& operator=(MenuBar&&)       = delete;

        /**
         * Adds a menu item to the menu bar.
         *
         * @param text The menu text (e.g., "File", "Edit").
         * @param popup The popup to open when clicked (can be nullptr for non-dropdown items).
         * @return The created MenuItem.
         */
        Widget* addMenu(std::string const & text, MenuPopup* popup = nullptr);

        /**
         * Closes all open menus.
         */
        void closeAll();

        /**
         * Checks if any menu is currently open.
         *
         * @return True if a menu is open.
         */
        bool isMenuOpen() const;

        /**
         * Gets the currently open menu popup.
         *
         * @return The open popup, or nullptr if none.
         */
        MenuPopup* getOpenMenu() const;

    protected:
        // Inherited from KeyListener
        void keyPressed(KeyEvent& event) override;
        void keyReleased(KeyEvent& event) override;

        // Inherited from MouseListener
        void mousePressed(MouseEvent& event) override;

        /**
         * Draws the menu bar (background and bottom border).
         */
        void draw(Graphics* graphics) override;
        // Inherited from ActionListener

        void action(ActionEvent const & event) override;

    private:
        /**
         * Currently open menu popup.
         */
        MenuPopup* mOpenMenu{nullptr};

        /**
         * Currently selected menu item index (-1 if none).
         */
        int mSelectedIndex{-1};
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_MENUBAR_HPP_
