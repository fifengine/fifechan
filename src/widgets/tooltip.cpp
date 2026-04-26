// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/widgets/tooltip.hpp"

// Standard library includes
#include <algorithm>

// Project headers (subdirs before local)
#include "fifechan/graphics.hpp"

namespace fcn
{

    Tooltip::Tooltip() : mWidgetId(0), mIsHovering(false), mHoverTimer(0), mModifierState(0), mIsExtended(false)
    {
        // Tooltips are typically transparent, handled in draw()
    }

    void Tooltip::setSpec(TooltipSpec spec)
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
            bool altPressed = (mModifierState & 0x100) != 0; // KMOD_ALT
            if (mSpec.modifierBehavior.enabled && altPressed) {
                mIsExtended = true;
            } else {
                mIsExtended = false;
            }

            generateContent();
        }
    }

    void Tooltip::generateContent()
    {
        if (!mIsHovering) {
            mCurrentContent.clear();
            return;
        }

        if (mIsExtended && mSpec.modifierBehavior.modifiedContent) {
            generateExtendedContent();
        } else if (mSpec.content) {
            generateNormalContent();
        } else {
            mCurrentContent.clear();
        }
    }

    const std::string & Tooltip::getCurrentContent() const
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
        if (mSpec.modifierBehavior.modifiedContent) {
            mCurrentContent = mSpec.modifierBehavior.modifiedContent(mWidgetId);
        } else {
            mCurrentContent.clear();
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

        // Render: only draw background + text; the backends can override
        graphics->setColor(Color(40, 40, 60, 230)); // Semi-transparent dark background
        graphics->fillRectangle(getDimension());

        graphics->setColor(Color(150, 150, 150)); // Light gray border
        graphics->drawRectangle(getDimension());

        // Text rendering would go here
        // Draw placeholder filled rectangles representing text lines.
        Rectangle dim  = getDimension();
        int lineHeight = 18;
        int padding    = 8;

        // Simple representation: draw 3 colored lines for text
        graphics->setColor(Color(200, 200, 200));
        for (size_t i = 0; i < 3; ++i) {
            Rectangle lineRect(
                dim.x + padding,
                dim.y + padding + static_cast<int>(i) * lineHeight,
                dim.width - padding * 2,
                lineHeight - 2);
            graphics->fillRectangle(lineRect);
        }
    }

} // namespace fcn
