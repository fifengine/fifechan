// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_MENUPOPUP_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_MENUPOPUP_HPP_

// Standard library includes
#include <memory>

// Platform config include
#include "fifechan/platform.hpp"

// Project headers (subdirs before local)
#include "fifechan/focushandler.hpp"
#include "fifechan/listeners/actionlistener.hpp"
#include "fifechan/listeners/focuslistener.hpp"
#include "fifechan/listeners/keylistener.hpp"
#include "fifechan/listeners/mouselistener.hpp"
#include "fifechan/widgets/container.hpp"

namespace fcn
{
    class ModalBackdrop;
    /**
     * A menu popup widget that displays a dropdown menu.
     *
     * MenuPopup is a modal overlay that appears below or above a menu item.
     * It handles keyboard navigation, hover-to-open for submenus, and click-outside dismissal.
     *
     * @ingroup widgets
     */
    class FIFEGUI_API MenuPopup :
        public Container,
        public MouseListener,
        public KeyListener,
        public FocusListener,
        public ActionListener
    {
    public:
        /**
         * Constructor.
         */
        MenuPopup();

        ~MenuPopup() override = default;

        MenuPopup(MenuPopup const &)            = delete;
        MenuPopup& operator=(MenuPopup const &) = delete;
        MenuPopup(MenuPopup&&)                  = delete;
        MenuPopup& operator=(MenuPopup&&)       = delete;

        /**
         * Shows the popup at a specific position.
         *
         * @param x The x position.
         * @param y The y position.
         */
        void show(int x, int y);

        /**
         * Hides the popup.
         */
        void hide();

        /**
         * Checks if the popup is visible.
         *
         * @return True if visible.
         */
        // cppcheck-suppress duplInheritedMember
        bool isVisible() const;

        /**
         * Sets the parent menu item that opened this popup.
         *
         * @param parent The parent MenuItem.
         * @see getParentMenuItem
         */
        void setParentMenuItem(Widget* parent);

        /**
         * Gets the parent menu item.
         *
         * @return The parent MenuItem, or nullptr.
         * @see setParentMenuItem
         */
        Widget* getParentMenuItem() const;

        /**
         * Gets the parent MenuPopup (for nested menus).
         *
         * @return The parent MenuPopup, or nullptr.
         */
        MenuPopup* getParentMenu() const;

        /**
         * Sets the parent MenuPopup (for nested menus).
         *
         * @param parent The parent popup.
         */
        void setParentMenu(MenuPopup* parent);

        /**
         * Adds a menu item to the popup.
         *
         * @param item The menu item to add.
         */
        void addItem(Widget* item);

        /**
         * Adds a separator to the popup.
         */
        void addSeparator();

        // Inherited from Widget

        void draw(Graphics* graphics) override;

    protected:
        // Inherited from MouseListener

        void mousePressed(MouseEvent& event) override;
        void mouseReleased(MouseEvent& event) override;
        void mouseEntered(MouseEvent& event) override;
        void mouseExited(MouseEvent& event) override;

        // Inherited from KeyListener

        void keyPressed(KeyEvent& event) override;
        void keyReleased(KeyEvent& event) override;

        // Inherited from FocusListener

        void focusLost(Event const & event) override;

        // Inherited from ActionListener

        void action(ActionEvent const & actionEvent) override;

    private:
        /**
         * Performs two-pass column-based layout.
         * Pass 1: Measures all items and aggregates column widths.
         * Pass 2: Lays out each item with column positions.
         */
        void layoutItems();

        /**
         * Whether the popup is visible.
         */
        // cppcheck-suppress duplInheritedMember
        bool mVisible{false};

        /**
         * Parent menu item that opened this popup.
         */
        Widget* mParentMenuItem{nullptr};

        /**
         * Parent popup (for nested menus).
         */
        MenuPopup* mParentMenu{nullptr};

        /**
         * Currently opened child submenu (if any).
         */
        MenuPopup* mOpenChild{nullptr};

        /**
         * Backdrop widget added to the GUI top when this is a root popup.
         * Clicks on the backdrop will close the menu.
         */
        ModalBackdrop* mBackdrop{nullptr};

        /**
         * Hovered child index for keyboard navigation.
         */
        int mHoverIndex{-1};

        /**
         * RAII modal scope for root popups.
         */
        std::unique_ptr<FocusHandler::ModalScope> mModalScope;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_MENUPOPUP_HPP_
