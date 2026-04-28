// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/widgets/menuitem.hpp"

// Project headers
#include "fifechan/font.hpp"
#include "fifechan/graphics.hpp"

namespace fcn
{
    MenuItem::MenuItem(std::string const & text) : mType(Type::Action)
    {
        setCaption(text);
        setBorderSize(0);
    }

    MenuItem::~MenuItem() { }

    void MenuItem::setType(Type type)
    {
        mType = type;
    }

    MenuItem::Type MenuItem::getType() const
    {
        return mType;
    }

    void MenuItem::setSubmenu(MenuPopup* popup)
    {
        mSubmenu = popup;
        if (popup) {
            mType = Type::Submenu;
        }
    }

    MenuPopup* MenuItem::getSubmenu() const
    {
        return mSubmenu;
    }

    void MenuItem::setText(std::string const & text)
    {
        setCaption(text);
    }

    std::string const & MenuItem::getText() const
    {
        return getCaption();
    }

    void MenuItem::setIcon(Image const * image)
    {
        mIcon = image;
    }

    Image const * MenuItem::getIcon() const
    {
        return mIcon;
    }

    void MenuItem::setIconGlyph(std::string const & glyph, Font* font)
    {
        mIconGlyph     = glyph;
        mIconGlyphFont = font;
    }

    std::string const & MenuItem::getIconGlyph() const
    {
        return mIconGlyph;
    }

    void MenuItem::setIconGlyphFont(Font* font)
    {
        mIconGlyphFont = font;
    }

    Font* MenuItem::getIconGlyphFont() const
    {
        return mIconGlyphFont;
    }

    void MenuItem::setShortcut(std::string const & shortcut)
    {
        mShortcut = shortcut;
    }

    std::string const & MenuItem::getShortcut() const
    {
        return mShortcut;
    }

    void MenuItem::setChecked(bool checked)
    {
        mChecked = checked;
    }

    bool MenuItem::isChecked() const
    {
        return mChecked;
    }

    void MenuItem::setEnabled(bool enabled)
    {
        mEnabled = enabled;
    }

    bool MenuItem::isEnabled() const
    {
        return mEnabled;
    }

    void MenuItem::draw(Graphics* graphics)
    {
        // Skip if separator
        if (mType == Type::Separator) {
            return;
        }

        // Draw background on hover
        if (mHasMouse && mEnabled) {
            graphics->setColor(getSelectionColor());
            graphics->fillRectangle(Rectangle(0, 0, getWidth(), getHeight()));
        }

        // Use column layout if available, otherwise use legacy positioning
        if (mColumnLayout) {
            // Compute vertical text position for the main font
            int const textY =
                getPaddingTop() + ((getHeight() - getPaddingTop() - getPaddingBottom() - getFont()->getHeight()) / 2);

            // Draw icon glyph (if present) or image/checkmark in icon column
            if (!mIconGlyph.empty()) {
                Font* iconFont = mIconGlyphFont ? mIconGlyphFont : getFont();
                graphics->setFont(iconFont);
                graphics->setColor(getForegroundColor());
                int const iconY = getPaddingTop() +
                                  ((getHeight() - getPaddingTop() - getPaddingBottom() - iconFont->getHeight()) / 2);
                graphics->drawText(mIconGlyph, mColumnLayout->xIcon, iconY, Graphics::Alignment::Left);
            } else if (mIcon) {
                int iconY = (getHeight() - mIcon->getHeight()) / 2;
                graphics->drawImage(mIcon, mColumnLayout->xIcon, iconY);
            } else if (mType == Type::Checkable && mChecked) {
                graphics->setFont(getFont());
                graphics->setColor(getForegroundColor());
                graphics->drawText("[x]", mColumnLayout->xIcon, textY, Graphics::Alignment::Left);
            }

            // Draw caption (left-aligned in caption column) using main font
            if (!mCaption.empty()) {
                graphics->setFont(getFont());
                graphics->setColor(getForegroundColor());
                graphics->drawText(mCaption, mColumnLayout->xCaption, textY, Graphics::Alignment::Left);
            }

            // Draw shortcut (right-aligned in shortcut column)
            if (!mShortcut.empty()) {
                graphics->setFont(getFont());
                graphics->setColor(getForegroundColor());
                graphics->drawText(
                    mShortcut,
                    mColumnLayout->xShortcut + mColumnLayout->cols.shortcutW,
                    textY,
                    Graphics::Alignment::Right);
            }

            // Draw arrow for submenu items (right-aligned)
            if (mType == Type::Submenu) {
                graphics->setFont(getFont());
                graphics->setColor(getForegroundColor());
                graphics->drawText(
                    " ", mColumnLayout->xArrow + mColumnLayout->cols.arrowW, textY, Graphics::Alignment::Right);
            }
        } else {
            // Legacy rendering (fallback when no column layout)
            int textX = 4;

            // Draw icon if present
            if (!mIconGlyph.empty()) {
                Font* iconFont = mIconGlyphFont ? mIconGlyphFont : getFont();
                graphics->setFont(iconFont);
                int const iconY = getPaddingTop() +
                                  ((getHeight() - getPaddingTop() - getPaddingBottom() - iconFont->getHeight()) / 2);
                graphics->setColor(getForegroundColor());
                graphics->drawText(mIconGlyph, 4, iconY, Graphics::Alignment::Left);
                textX += iconFont->getWidth(mIconGlyph) + 4;
            } else if (mIcon) {
                graphics->drawImage(mIcon, 4, (getHeight() - mIcon->getHeight()) / 2);
                textX += mIcon->getWidth() + 4;
            }

            // Ensure font and color are set for text rendering
            graphics->setFont(getFont());
            graphics->setColor(getForegroundColor());

            // Compute vertical text position similar to Button::draw so text is
            // vertically centered relative to the font metrics and padding.
            int const textY =
                getPaddingTop() + ((getHeight() - getPaddingTop() - getPaddingBottom() - getFont()->getHeight()) / 2);

            // Draw text
            if (!mCaption.empty()) {
                graphics->drawText(mCaption, textX, textY, Graphics::Alignment::Left);
            }

            // Draw shortcut text (right-aligned)
            if (!mShortcut.empty()) {
                graphics->drawText(mShortcut, getWidth() - 4, textY, Graphics::Alignment::Right);
            }

            // Draw checkmark for checked items
            if (mType == Type::Checkable && mChecked) {
                graphics->setColor(getForegroundColor());
                graphics->drawText("[x]", 4, textY, Graphics::Alignment::Left);
            }

            // Draw arrow for submenu items
            if (mType == Type::Submenu) {
                graphics->setColor(getForegroundColor());
                graphics->drawText(" ", getWidth() - 4, textY, Graphics::Alignment::Right);
            }
        }
    }

    void MenuItem::mousePressed(MouseEvent& mouseEvent)
    {
        // Call parent Button handler
        Button::mousePressed(mouseEvent);
    }

    void MenuItem::mouseReleased(MouseEvent& mouseEvent)
    {
        // Call parent Button handler
        Button::mouseReleased(mouseEvent);
    }

    MenuItemMetrics MenuItem::measure(Font const & font) const
    {
        MenuItemMetrics m;

        // For separators, return minimal dimensions
        if (mType == Type::Separator) {
            m.height = 2;
            return m;
        }

        // Icon width: image, glyph, or checkmark space
        if (mIcon) {
            m.iconW = mIcon->getWidth();
        } else if (!mIconGlyph.empty()) {
            if (mIconGlyphFont) {
                m.iconW = mIconGlyphFont->getWidth(mIconGlyph);
            } else {
                m.iconW = font.getWidth(mIconGlyph);
            }
        } else if (mType == Type::Checkable) {
            // Space for "[x]" checkmark
            m.iconW = font.getWidth("[x]");
        }

        // Caption width
        if (!mCaption.empty()) {
            m.captionW = font.getWidth(mCaption);
        }

        // Shortcut width
        if (!mShortcut.empty()) {
            m.shortcutW = font.getWidth(mShortcut);
        }

        // Arrow width for submenus
        if (mType == Type::Submenu) {
            m.arrowW = font.getWidth(" >") + 8; // Add some padding for the arrow
        }

        // Height: max of icon height or font height, plus vertical padding
        int contentH = font.getHeight();
        if (mIcon) {
            contentH = std::max(contentH, mIcon->getHeight());
        }
        if (!mIconGlyph.empty()) {
            int iconFontH = mIconGlyphFont ? mIconGlyphFont->getHeight() : font.getHeight();
            contentH      = std::max(contentH, iconFontH);
        }
        // Add vertical padding (top + bottom)
        int vpad = getPaddingTop() + getPaddingBottom();
        m.height = contentH + vpad;

        return m;
    }

    void MenuItem::layoutColumns(ColumnLayout const & layout)
    {
        mColumnLayout = std::make_unique<ColumnLayout>(layout);
    }
} // namespace fcn
