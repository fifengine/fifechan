// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/widgets/tooltip.hpp"

// Standard library includes
#include <algorithm>
#include <cassert>
#include <string>

// Project headers (subdirs before local)
#include "fifechan/graphics.hpp"

namespace fcn
{

    Tooltip::Tooltip()
    {
        // Tooltips should not draw a container background.
        // They only render their own background, then
        // let child widgets (like a Label) draw the text.
        setOpaque(false);
    }

    void Tooltip::setSpec(TooltipSpec const & spec)
    {
        mSpec = spec;
    }

    TooltipSpec const & Tooltip::getSpec() const
    {
        return mSpec;
    }

    void Tooltip::setWidgetId(int id)
    {
        mWidgetId = id;
    }

    int Tooltip::getWidgetId() const
    {
        return mWidgetId;
    }

    void Tooltip::startHover()
    {
        mIsHovering = true;
        mHoverTimer = 0;
    }

    void Tooltip::endHover()
    {
        mIsHovering = false;
        mHoverTimer = 0;
        mIsExtended = false;
    }

    bool Tooltip::isHovering() const
    {
        return mIsHovering;
    }

    void Tooltip::update(int deltaMs, int modifierState)
    {
        mModifierState = modifierState;

        if (mIsHovering) {
            mHoverTimer += deltaMs;

            // Check if we should show extended content (ALT key)
            bool const altPressed = (mModifierState & 0x100) != 0; // KMOD_ALT
            mIsExtended           = mSpec.modifierBehavior.enabled && altPressed;

            generateContent();
        }
    }

    void Tooltip::generateContent()
    {
        if (!mIsHovering) {
            mCurrentContent.clear();
            return;
        }

        // Generate normal content first, then extended content if active.
        generateNormalContent();

        if (mIsExtended) {
            generateExtendedContent();
        }
    }

    std::string const & Tooltip::getCurrentContent() const
    {
        return mCurrentContent;
    }

    bool Tooltip::isExtendedView() const
    {
        return mIsExtended;
    }

    void Tooltip::generateNormalContent()
    {
        if (mSpec.content) {
            mCurrentContent = mSpec.content(mWidgetId);
        } else {
            mCurrentContent.clear();
        }
    }

    void Tooltip::generateExtendedContent()
    {
        if (!mSpec.modifierBehavior.modifiedContent) {
            return;
        }

        std::string modified = mSpec.modifierBehavior.modifiedContent(mWidgetId);

        // TODO: consider a more robust way to combine normal and modified content
        // If modified begins with the normal content, strip the duplicate prefix and
        // any leading newlines so the extended text appears underneath.
        if (!mCurrentContent.empty() && !modified.empty()) {
            if (modified.starts_with(mCurrentContent)) { // modified starts with normal
                modified.erase(0, mCurrentContent.size());
                while (!modified.empty() && (modified.front() == '\n' || modified.front() == '\r')) {
                    modified.erase(modified.begin());
                }
            }
        }

        if (mCurrentContent.empty()) {
            mCurrentContent = modified;
        } else if (!modified.empty()) {
            mCurrentContent = mCurrentContent + "\n" + modified;
        }
    }

    void Tooltip::draw(Graphics* graphics)
    {
        if (!mIsHovering || mHoverTimer < mSpec.delayMs) {
            return; // Don't render if not ready
        }

        if (mCurrentContent.empty()) {
            return;
        }

        // Render: draw background + border relative to this widget (graphics is relative)
        Rectangle const dim(0, 0, getWidth(), getHeight());

        // Choose background color to indicate extended view
        if (mIsExtended) {
            graphics->setColor(Color(60, 40, 80, 230));
        } else {
            graphics->setColor(Color(40, 40, 60, 230));
        }
        graphics->fillRectangle(dim);

        graphics->setColor(Color(150, 150, 150));
        graphics->drawRectangle(dim);

        // Draw child widgets (e.g., Label)
        // they will render text within this widget's coordinate space
        Container::draw(graphics);
    }

    Rectangle Tooltip::getChildrenArea()
    {
        // While the tooltip is not hovered long enough or not in delay,
        // do not expose the children area, so Widget::_draw() will skip
        // drawing children.
        // Once ready, fall back to the normal container children area.
        if (!mIsHovering || mHoverTimer < mSpec.delayMs) {
            return {0, 0, 0, 0};
        }
        return Container::getChildrenArea();
    }

} // namespace fcn
