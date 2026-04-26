// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

#ifndef FIFEGUI_TOOLTIP_H
#define FIFEGUI_TOOLTIP_H

#include <functional>
#include <string>

#include "fifechan/widget.hpp"

namespace fcn
{

    /**
     * Tooltip trigger type
     */
    enum class TooltipTrigger : uint8_t
    {
        Hover
    };

    /**
     * Tooltip placement relative to cursor
     */
    enum class TooltipPlacement : uint8_t
    {
        Cursor,
        Right,
        Bottom
    };

    /**
     * Tooltip modifier behavior (for an ALT-key extended view)
     */
    struct TooltipModifierBehavior
    {
        bool enabled = false;
        int modifier = 0x100; // KMOD_ALT from SDL
        std::function<std::string(int widgetId)> modifiedContent;
    };

    /**
     * Tooltip specification (data + behavior)
     */
    struct TooltipSpec
    {
        std::function<std::string(int widgetId)> content;
        int delayMs                = 300;
        TooltipTrigger trigger     = TooltipTrigger::Hover;
        TooltipPlacement placement = TooltipPlacement::Cursor;
        TooltipModifierBehavior modifierBehavior;
    };

    /**
     * A tooltip widget.
     *
     * This widget is not meant to be used directly, but managed by a TooltipManager.
     *
     * It handles:
     * - hover state
     * - content generation based on widget ID
     * - rendering
     * - the Tooltip content can be extended
     *   - by holding a modifier key (e.g., ALT), if configured in TooltipSpec.
     */
    class Tooltip : public Widget
    {
    public:
        Tooltip();
        virtual ~Tooltip() override = default;

        // Set tooltip specification
        void setSpec(TooltipSpec spec);
        TooltipSpec const & getSpec() const;

        // Widget ID this tooltip is attached to
        void setWidgetId(int id);
        int getWidgetId() const;

        // Hover state management
        void startHover();
        void endHover();
        bool isHovering() const;
        void update(int deltaMs, int modifierState);

        // Content generation
        void generateContent();
        const std::string & getCurrentContent() const;
        bool isExtendedView() const;

        // Rendering
        virtual void draw(Graphics* graphics) override;

    private:
        void generateNormalContent();
        void generateExtendedContent();

        TooltipSpec mSpec;
        int mWidgetId{0};
        bool mIsHovering{false};
        int mHoverTimer{0};
        int mModifierState{0};

        std::string mCurrentContent;
        bool mIsExtended{false};
    };

} // namespace fcn

#endif // FIFEGUI_TOOLTIP_H
