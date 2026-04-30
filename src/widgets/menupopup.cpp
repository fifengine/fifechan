// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/widgets/menupopup.hpp"

// Standard library includes
#include <algorithm>
#include <memory>
#include <unordered_map>

// Project headers
#include "fifechan/color.hpp"
#include "fifechan/focushandler.hpp"
#include "fifechan/font.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/widgets/menubar.hpp"
#include "fifechan/widgets/menuitem.hpp"
#include "fifechan/widgets/modalbackdrop.hpp"

namespace fcn
{
    // Default gap values for column layout
    namespace
    {
        constexpr int GAP_ICON_CAPTION     = 6;
        constexpr int GAP_CAPTION_SHORTCUT = 16;
        constexpr int GAP_SHORTCUT_ARROW   = 10;
    } // namespace

    void MenuPopup::layoutItems()
    {
        auto children = getChildren();
        if (children.empty()) {
            return;
        }

        // Get font to use for measurements
        Font const * font = getFont();
        if (font == nullptr) {
            return;
        }

        // === PASS 1: Measure & Aggregate Columns ===
        MenuColumns cols{};

        // Store per-item metrics so pass 2 can reuse exact heights
        std::unordered_map<MenuItem*, MenuItemMetrics> metricsMap;

        for (auto* child : children) {
            if (child == nullptr) {
                continue;
            }

            auto* mi = dynamic_cast<MenuItem*>(child);
            if (mi == nullptr) {
                continue;
            }

            if (mi->getType() == MenuItem::Type::Separator) {
                continue;
            }

            auto m = mi->measure(*font);

            // store metrics for later (pass 2)
            metricsMap[mi] = m;

            cols.iconW     = std::max(cols.iconW, m.iconW);
            cols.captionW  = std::max(cols.captionW, m.captionW);
            cols.shortcutW = std::max(cols.shortcutW, m.shortcutW);
            cols.arrowW    = std::max(cols.arrowW, m.arrowW);
        }

        // Determine actual gaps (collapse if column is empty)
        int const gapIconCaption     = cols.iconW > 0 ? GAP_ICON_CAPTION : 0;
        int const gapCaptionShortcut = (cols.captionW > 0 && cols.shortcutW > 0) ? GAP_CAPTION_SHORTCUT : 0;
        int const gapShortcutArrow   = (cols.shortcutW > 0 && cols.arrowW > 0) ? GAP_SHORTCUT_ARROW : 0;

        // === Compute Content Width ===
        int const contentW = cols.iconW + gapIconCaption + cols.captionW + gapCaptionShortcut + cols.shortcutW +
                             gapShortcutArrow + cols.arrowW;

        // === PASS 2: Layout Items ===
        // Compute X positions for each column
        int const xIcon     = 0;
        int const xCaption  = xIcon + cols.iconW + gapIconCaption;
        int const xShortcut = xCaption + cols.captionW + gapCaptionShortcut;
        int const xArrow    = xShortcut + cols.shortcutW + gapShortcutArrow;

        // Create column layout struct
        ColumnLayout layout{};
        layout.xIcon     = xIcon;
        layout.xCaption  = xCaption;
        layout.xShortcut = xShortcut;
        layout.xArrow    = xArrow;
        layout.cols      = cols;

        // Layout each item — use measured heights when available
        int contentH = 0;
        int y        = 0;

        for (auto* child : children) {
            if (child == nullptr) {
                continue;
            }

            int h = child->getHeight();

            if (auto* mi = dynamic_cast<MenuItem*>(child)) {
                auto it = metricsMap.find(mi);
                if (it != metricsMap.end()) {
                    // prefer measured height (accounts for icon fonts)
                    h = it->second.height;
                }
            }

            // Set the child's frame using setPosition and setSize
            child->setPosition(0, y);
            child->setSize(contentW, h);

            // Apply column layout to MenuItems
            if (auto* mi = dynamic_cast<MenuItem*>(child)) {
                mi->layoutColumns(layout);
            }

            y += h;
            contentH += h;
        }

        // === Compute Final Popup Size ===
        // Get border and padding
        int const leftBorder = ((getBorderSides() & Widget::BORDER_LEFT) != 0U) ? static_cast<int>(getBorderSize()) : 0;
        int const rightBorder =
            ((getBorderSides() & Widget::BORDER_RIGHT) != 0U) ? static_cast<int>(getBorderSize()) : 0;
        int const topBorder = ((getBorderSides() & Widget::BORDER_TOP) != 0U) ? static_cast<int>(getBorderSize()) : 0;
        int const bottomBorder =
            ((getBorderSides() & Widget::BORDER_BOTTOM) != 0U) ? static_cast<int>(getBorderSize()) : 0;

        int const contentPaddingW = leftBorder + rightBorder + getPaddingLeft() + getPaddingRight();
        int const contentPaddingH = topBorder + bottomBorder + getPaddingTop() + getPaddingBottom();

        int const popupWidth  = contentW + contentPaddingW;
        int const popupHeight = contentH + contentPaddingH;

        // Apply final size
        setSize(popupWidth, popupHeight);
    }

    MenuPopup::MenuPopup()
    {
        // MenuPopup positions children itself via layoutItems(); use AutoSize
        // so Container::resizeToContent() respects the children's dimensions
        // rather than reflowing them with the Vertical layout policy.
        setLayout(LayoutPolicy::AutoSize);
        setOpaque(true);
        // Use built-in widget border so children are laid out inside
        // the border (prevents children from drawing over it).
        setBorderSize(1);
        setBorderStyle(getBorderStyle());

        // Provide a small horizontal padding so items don't touch the popup
        // border; increase slightly for better visual spacing.
        setPaddingLeft(16);
        setPaddingRight(16);

        // Register for mouse events so the popup can handle clicks
        // that occur outside the popup (modal dismissal).
        addMouseListener(this);

        // Initial size: content area 150x10 plus border on both sides
        setSize(150 + (static_cast<int>(getBorderSize()) * 2), 10 + (static_cast<int>(getBorderSize()) * 2));
    }

    void MenuPopup::show(int x, int y)
    {
        // Find the top-level container and reparent the popup to it
        // to ensure proper z-ordering (always on top of other widgets).
        Container* topContainer = nullptr;

        // Try to find top container
        if (mParentMenuItem != nullptr) {
            topContainer = dynamic_cast<Container*>(mParentMenuItem->getTop());
        }
        if (topContainer == nullptr) {
            topContainer = dynamic_cast<Container*>(getTop());
        }

        // If currently in a different parent, remove from that parent first
        Widget* currentParent = getParent();
        if (currentParent != nullptr && currentParent != topContainer) {
            if (auto* parentContainer = dynamic_cast<Container*>(currentParent)) {
                parentContainer->remove(this);
            }
        }

        // Add to top container FIRST before calling moveToTop().
        // moveToTop() throws if the widget is not already a child of the container.
        if (topContainer != nullptr && currentParent != topContainer) {
            topContainer->add(this);
        }

        // Set position (now relative to topContainer since we just added it)
        setPosition(x, y);

        // Now move to top for proper z-ordering (widget is guaranteed to be a child)
        if (topContainer != nullptr) {
            topContainer->moveToTop(this);
        }

        // Set visible and ensure on top
        setVisible(true);
        mVisible = true;

        // Use RAII ModalScope so the modal is popped automatically when
        // the popup is hidden or destroyed. Only create for root menus.
        if (_getFocusHandler() != nullptr && mParentMenu == nullptr) {
            mModalScope = std::make_unique<FocusHandler::ModalScope>(_getFocusHandler(), this, this);

            // Request focus on the popup for keyboard navigation
            requestFocus();

            if (topContainer != nullptr) {
                // Create backdrop, size it to cover top and add it behind menus.
                ModalBackdrop* backdrop = new ModalBackdrop(this);
                backdrop->setPosition(0, 0);
                backdrop->setSize(topContainer->getWidth(), topContainer->getHeight());
                topContainer->add(backdrop);
                // Place backdrop above other top-level widgets so it captures
                // clicks outside the menu, then ensure the popup stays on top.
                topContainer->moveToTop(backdrop);
                topContainer->moveToTop(this);
                mBackdrop = backdrop;
            }
        } else if (mParentMenu != nullptr) {
            // request focus for submenu
            requestFocus();
        }
    }

    void MenuPopup::hide()
    {
        setVisible(false);
        mVisible = false;

        // Restore focus to parent MenuItem/MenuBar before clearing modal
        if (mParentMenuItem != nullptr) {
            mParentMenuItem->requestFocus();
        }

        // Hide any open child submenu first
        if (mOpenChild != nullptr) {
            mOpenChild->hide();
            mOpenChild = nullptr;
        }

        // Remove backdrop if we created one for the root popup
        if (mBackdrop != nullptr) {
            if (mBackdrop->getParent() != nullptr) {
                if (auto* parentContainer = dynamic_cast<Container*>(mBackdrop->getParent())) {
                    parentContainer->remove(mBackdrop);
                }
            }
            delete mBackdrop;
            mBackdrop = nullptr;
        }

        // Release RAII modal scope for root popup if present
        if (mModalScope != nullptr) {
            mModalScope.reset();
        }
    }

    // cppcheck-suppress duplInheritedMember
    bool MenuPopup::isVisible() const
    {
        return mVisible;
    }

    void MenuPopup::setParentMenuItem(Widget* parent)
    {
        mParentMenuItem = parent;
    }

    Widget* MenuPopup::getParentMenuItem() const
    {
        return mParentMenuItem;
    }

    MenuPopup* MenuPopup::getParentMenu() const
    {
        return mParentMenu;
    }

    void MenuPopup::setParentMenu(MenuPopup* parent)
    {
        mParentMenu = parent;
    }

    void MenuPopup::addItem(Widget* item)
    {
        // Add the item to the popup
        add(item);

        // Let the item size itself according to font/padding
        try {
            item->adjustSize();
        } catch (...) {
            // ignore if widget doesn't implement adjustSize
        }

        // Perform two-pass column-based layout
        layoutItems();

        // Position children according to the container's layout (vertical stacking).
        // Call resizeToContent with recursion=false so children keep the sizes
        // assigned by layoutItems() (we don't want each MenuItem to re-measure
        // itself and overwrite the column-computed widths).
        resizeToContent(false);

        // Listen for actions from items (so we can close the menu on selection)
        auto* mi = dynamic_cast<MenuItem*>(item);
        if (mi != nullptr && mi->getType() != MenuItem::Type::Separator) {
            mi->addActionListener(this);
        }
    }

    void MenuPopup::addSeparator()
    {
        // Create a separator as a special MenuItem
        auto* sep = new MenuItem("");
        sep->setType(MenuItem::Type::Separator);
        sep->setHeight(2);
        add(sep);

        // Perform two-pass column-based layout
        layoutItems();

        // Ensure children are laid out after recomputing sizes. Avoid recursive
        // resize so children don't overwrite sizes set by layoutItems.
        resizeToContent(false);
    }

    void MenuPopup::draw(Graphics* graphics)
    {
        int const w = getWidth();
        int const h = getHeight();

        // Draw shadow
        graphics->setColor(Color(50, 50, 50, 100));
        graphics->fillRectangle(Rectangle(3, 3, w, h));

        // Draw background - always visible (white)
        graphics->setColor(Color(255, 255, 255, 255));
        graphics->fillRectangle(Rectangle(0, 0, w, h));

        // Draw children and built-in borders (Container::draw will call
        // drawBorder when border size > 0).
        Container::draw(graphics);
    }

    void MenuPopup::mousePressed(MouseEvent& event)
    {
        // Handle click on backdrop to close
        int const relx = event.getX();
        int const rely = event.getY();

        // Coordinates from MouseEvent are relative to the widget the
        // listener is registered to (this popup). Convert to absolute
        // coordinates for hit-testing against top-level children.
        int absx = relx;
        int absy = rely;
        int px   = 0;
        int py   = 0;
        getAbsolutePosition(px, py);
        absx += px;
        absy += py;

        if (!contains(relx, rely)) {
            // If the click landed on a top-level MenuBar/MenuItem, allow the
            // event to propagate so the MenuBar can toggle the popup. Otherwise
            // hide the popup and consume the event.
            bool clickHitsMenuBar          = false;
            Container const * topContainer = dynamic_cast<Container*>(getTop());
            if (topContainer != nullptr) {
                for (unsigned i = 0; i < topContainer->getChildrenCount(); ++i) {
                    Widget* child = topContainer->getChild(i);
                    if (child == nullptr || child == this) {
                        continue;
                    }
                    int cx = 0;
                    int cy = 0;
                    child->getAbsolutePosition(cx, cy);
                    if (absx >= cx && absx < cx + child->getWidth() && absy >= cy && absy < cy + child->getHeight()) {
                        if (dynamic_cast<MenuBar*>(child) != nullptr || dynamic_cast<MenuItem*>(child) != nullptr) {
                            clickHitsMenuBar = true;
                            break;
                        }
                    }
                }
            }

            if (clickHitsMenuBar) {
                // Let the MenuBar/MenuItem handle the click (toggle behavior).
                return;
            }

            hide();
            event.consume();
        }
    }

    void MenuPopup::mouseReleased(MouseEvent& event)
    {
    }

    void MenuPopup::mouseEntered(MouseEvent& event)
    {
        // Determine which child was entered and update hover/focus
        Widget* src   = event.getSource();
        auto children = getChildren();
        int idx       = 0;
        for (auto* child : children) {
            if (child == src) {
                // Focus the hovered item for visual feedback and keyboard nav
                child->requestFocus();
                mHoverIndex = idx;

                // If it's a MenuItem with a submenu, open it
                if (auto* mi = dynamic_cast<MenuItem*>(child)) {
                    MenuPopup* submenu = mi->getSubmenu();
                    if (submenu != nullptr) {
                        // Close previous child if different
                        if (mOpenChild != nullptr && mOpenChild != submenu) {
                            mOpenChild->hide();
                        }

                        // Attach submenu to this menu and position to the right
                        submenu->setParentMenu(this);
                        submenu->setParentMenuItem(mi);

                        int ax = 0;
                        int ay = 0;
                        mi->getAbsolutePosition(ax, ay);
                        int const sx = ax + mi->getWidth();
                        int const sy = ay;
                        submenu->show(sx, sy);
                        mOpenChild = submenu;
                    }
                }

                break;
            }
            ++idx;
        }
    }

    void MenuPopup::mouseExited(MouseEvent& event)
    {
    }

    void MenuPopup::keyPressed(KeyEvent& event)
    {
        Key const key = event.getKey();
        // ESC closes this menu (and its root will pop modal)
        if (key.getValue() == Key::Escape) {
            // If this menu has a parent, close self; otherwise close root
            hide();
            event.consume();
            return;
        }

        if (mParentMenuItem != nullptr) {
            if (key.getValue() == Key::Up || key.getValue() == Key::Down) {
                auto children = getChildren();
                if (children.empty()) {
                    return;
                }

                if (mHoverIndex < 0) {
                    mHoverIndex = 0;
                }

                if (key.getValue() == Key::Up) {
                    mHoverIndex =
                        (mHoverIndex - 1 + static_cast<int>(children.size())) % static_cast<int>(children.size());
                } else {
                    mHoverIndex = (mHoverIndex + 1) % static_cast<int>(children.size());
                }

                // Advance iterator to the hovered child
                int i = 0;
                for (auto* target : children) {
                    if (i == mHoverIndex) {
                        if (target != nullptr) {
                            target->requestFocus();
                        }
                        break;
                    }
                    ++i;
                }

                event.consume();
                return;
            }

            if (key.getValue() == Key::Right) {
                auto children = getChildren();
                if (mHoverIndex >= 0) {
                    int i = 0;
                    for (auto* child : children) {
                        if (i == mHoverIndex) {
                            if (auto* mi = dynamic_cast<MenuItem*>(child)) {
                                if (mi->getSubmenu() != nullptr) {
                                    MenuPopup* submenu = mi->getSubmenu();
                                    submenu->setParentMenu(this);
                                    submenu->setParentMenuItem(mi);
                                    int ax = 0;
                                    int ay = 0;
                                    mi->getAbsolutePosition(ax, ay);
                                    submenu->show(ax + mi->getWidth(), ay);
                                    mOpenChild = submenu;
                                    event.consume();
                                    return;
                                }
                            }
                            break;
                        }
                        ++i;
                    }
                }
            }

            if (key.getValue() == Key::Left) {
                if (mParentMenu != nullptr) {
                    hide();
                    event.consume();
                    return;
                }
            }
        }
    }

    void MenuPopup::keyReleased(KeyEvent& event)
    {
    }

    void MenuPopup::focusLost(Event const & event)
    {
        // Close when focus is lost
        if (isVisible()) {
            hide();
        }
    }

    void MenuPopup::action(ActionEvent const & event)
    {
        // Handle selection action from child MenuItems.
        auto* source = dynamic_cast<MenuItem*>(event.getSource());
        if (source == nullptr) {
            return;
        }

        // Toggle checkable items
        if (source->getType() == MenuItem::Type::Checkable) {
            source->setChecked(!source->isChecked());
        }

        // Close the entire menu tree: find root and hide it
        MenuPopup* root = this;
        while (root->getParentMenu() != nullptr) {
            root = root->getParentMenu();
        }
        root->hide();
    }
} // namespace fcn
