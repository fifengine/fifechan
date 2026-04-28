// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_MENUITEM_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_MENUITEM_HPP_

// Standard library includes
#include <memory>
#include <string>

// Platform config include
#include "fifechan/platform.hpp"

// Project headers (subdirs before local)
#include "fifechan/image.hpp"
#include "fifechan/listeners/actionlistener.hpp"
#include "fifechan/widgets/button.hpp"

namespace fcn
{
    class Font;
    class MenuPopup;

    /**
     * Layout metrics for a single menu item (content only, no padding/borders).
     */
    struct MenuItemMetrics
    {
        int iconW{0};     // Width of icon column (or checkmark space)
        int captionW{0};  // Width of caption text
        int shortcutW{0}; // Width of shortcut text
        int arrowW{0};    // Width of submenu arrow
        int height{0};    // Height of the item
    };

    /**
     * Aggregated column metrics for all items in a menu.
     */
    struct MenuColumns
    {
        int iconW{0};     // Maximum icon width across all items
        int captionW{0};  // Maximum caption width across all items
        int shortcutW{0}; // Maximum shortcut width across all items
        int arrowW{0};    // Maximum arrow width across all items
    };

    /**
     * Column layout specification passed to items for rendering.
     */
    struct ColumnLayout
    {
        int xIcon;        // X position of icon column
        int xCaption;     // X position of caption column
        int xShortcut;    // X position of shortcut column
        int xArrow;       // X position of arrow column
        MenuColumns cols; // Column widths
    };

    /**
     * A menu item widget for use in menus.
     *
     * MenuItem displays a single item in a menu. It can be:
     * - An action item (triggers callback when clicked)
     * - A submenu item (opens MenuPopup on hover)
     * - A separator (non-interactive divider)
     * - A checkable item (toggleable state)
     *
     * @ingroup widgets
     */
    class FIFEGUI_API MenuItem : public Button
    {
    public:
        /**
         * The type of menu item.
         *
         * Action: Regular clickable item that triggers an action.
         * Submenu: Opens a submenu when hovered.
         * Separator: Non-interactive divider (a line between items).
         * Checkable: An item that can be toggled on/off (e.g. with a checkmark).
         */
        enum class Type
        {
            Action,
            Submenu,
            Separator,
            Checkable
        };

        /**
         * Constructor.
         *
         * @param text The menu item text.
         */
        explicit MenuItem(std::string const & text);

        ~MenuItem() override;

        MenuItem(MenuItem const &)            = delete;
        MenuItem& operator=(MenuItem const &) = delete;
        MenuItem(MenuItem&&)                  = delete;
        MenuItem& operator=(MenuItem&&)       = delete;

        /**
         * Sets the menu item type.
         *
         * @param type The type of menu item.
         * @see getType
         */
        void setType(Type type);

        /**
         * Gets the menu item type.
         *
         * @return The type of menu item.
         * @see setType
         */
        Type getType() const;

        /**
         * Sets the submenu popup (for submenu-type items).
         *
         * @param popup The submenu popup.
         * @see getSubmenu
         */
        void setSubmenu(MenuPopup* popup);

        /**
         * Gets the submenu popup.
         *
         * @return The submenu popup, or nullptr if not a submenu.
         * @see setSubmenu
         */
        MenuPopup* getSubmenu() const;

        /**
         * Sets the shortcut text (displayed on the right).
         *
         * @param shortcut The shortcut text (e.g., "Ctrl+S").
         * @see getShortcut
         */
        void setShortcut(std::string const & shortcut);

        /**
         * Gets the shortcut text.
         *
         * @return The shortcut text.
         * @see setShortcut
         */
        std::string const & getShortcut() const;

        /**
         * Text API aliases for compatibility.
         */
        void setText(std::string const & text);
        std::string const & getText() const;

        /**
         * Icon support (optional image to render at the left).
         */
        void setIcon(Image const * image);
        Image const * getIcon() const;

        /**
         * Icon glyph support using a font (e.g. OpenMoji emoji glyphs).
         * The glyph is rendered using the provided font when available.
         */
        void setIconGlyph(std::string const & glyph, Font* font = nullptr);
        std::string const & getIconGlyph() const;
        void setIconGlyphFont(Font* font);
        Font* getIconGlyphFont() const;

        /**
         * Sets whether the item is checked (for checkable items).
         *
         * @param checked True if checked.
         * @see isChecked
         */
        void setChecked(bool checked);

        /**
         * Checks if the item is checked.
         *
         * @return True if checked.
         * @see setChecked
         */
        bool isChecked() const;

        /**
         * Sets whether the item is enabled.
         *
         * @param enabled True if enabled.
         * @see isEnabled
         */
        void setEnabled(bool enabled);

        /**
         * Checks if the item is enabled.
         *
         * @return True if enabled.
         * @see setEnabled
         */
        bool isEnabled() const;

        // Override draw for custom rendering
        void draw(Graphics* graphics) override;

        // Override mouse events for debug tracing
        void mousePressed(MouseEvent& mouseEvent) override;
        void mouseReleased(MouseEvent& mouseEvent) override;

        /**
         * Measures the content metrics of this menu item.
         *
         * @param font The font to use for text measurements.
         * @return MenuItemMetrics containing content-only dimensions (no padding/borders).
         */
        MenuItemMetrics measure(Font const & font) const;

        /**
         * Applies column layout positions for rendering.
         *
         * @param layout The column layout specification.
         */
        void layoutColumns(ColumnLayout const & layout);

    protected:
        /**
         * Type of menu item.
         */
        Type mType{Type::Action};

        /**
         * Submenu popup (for submenu items).
         */
        MenuPopup* mSubmenu{nullptr};

        /**
         * Shortcut text.
         */
        std::string mShortcut;

        /**
         * Optional icon image.
         */
        Image const * mIcon{nullptr};

        /**
         * Optional icon glyph (emoji or glyph string) and font to render it.
         */
        std::string mIconGlyph;
        Font* mIconGlyphFont{nullptr};

        /**
         * Checked state.
         */
        bool mChecked{false};

        /**
         * Enabled state.
         */
        bool mEnabled{true};

        /**
         * Cached column layout for rendering.
         */
        std::unique_ptr<ColumnLayout> mColumnLayout;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_MENUITEM_HPP_
