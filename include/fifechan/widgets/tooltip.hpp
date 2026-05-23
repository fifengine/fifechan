// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef FIFEGUI_TOOLTIP_H
#define FIFEGUI_TOOLTIP_H

#include <functional>
#include <string>

// Platform config include (for FIFEGUI_API)
#include "fifechan/platform.hpp"
#include "fifechan/widgets/container.hpp"

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
            /**
             * True if modifier-based extended content is enabled.
             */
            bool enabled = false;

            /**
             * Modifier key mask used to trigger extended content (SDL mask value).
             */
            int modifier = 0x100; // KMOD_ALT from SDL

            /**
             * Function that returns modified content when the modifier is active.
             * The argument is the widget id for which to generate content.
             */
            std::function<std::string(int widgetId)> modifiedContent;
    };

    /**
     * Tooltip specification (data + behavior)
     */
    struct TooltipSpec
    {
            /**
             * Function that generates tooltip content for a widget id.
             */
            std::function<std::string(int widgetId)> content;

            /**
             * Delay in milliseconds before showing the tooltip.
             */
            int delayMs = 300;

            /**
             * Trigger mechanism for the tooltip (e.g., hover).
             */
            TooltipTrigger trigger = TooltipTrigger::Hover;

            /**
             * Preferred placement for the tooltip relative to cursor/widget.
             */
            TooltipPlacement placement = TooltipPlacement::Cursor;

            /**
             * Behavior configuration for modifier-extended content.
             */
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
    class FIFEGUI_API Tooltip : public Container
    {
        public:
            /**
             * Constructor.
             */
            Tooltip();

            /**
             * Destructor.
             */
            ~Tooltip() override = default;

            Tooltip(Tooltip const &)            = delete;
            Tooltip& operator=(Tooltip const &) = delete;
            Tooltip(Tooltip&&)                  = delete;
            Tooltip& operator=(Tooltip&&)       = delete;

            /**
             * Set the tooltip specification describing content and behavior.
             */
            void setSpec(TooltipSpec const & spec);

            /**
             * Get the current tooltip specification.
             */
            TooltipSpec const & getSpec() const;

            /**
             * Attach this tooltip to a widget id.
             */
            void setWidgetId(int id);

            /**
             * Get the widget id this tooltip is attached to.
             */
            int getWidgetId() const;

            /**
             * Begin hover tracking for the attached widget.
             */
            void startHover();

            /**
             * End hover tracking for the attached widget.
             */
            void endHover();

            /**
             * Query whether the tooltip is currently hovering.
             */
            bool isHovering() const;

            /**
             * Periodic update to advance timers and compute modifier state.
             *
             * @param deltaMs Milliseconds since last update.
             * @param modifierState Current modifier key state mask.
             */
            void update(int deltaMs, int modifierState);

            /**
             * Generate tooltip content based on `mSpec` and widget id.
             */
            void generateContent();

            /**
             * Get the currently generated content string.
             */
            std::string const & getCurrentContent() const;

            /**
             * True if the tooltip is showing extended (modifier) content.
             */
            bool isExtendedView() const;

            /**
             * Draw the tooltip.
             */
            void draw(Graphics* graphics) override;

            /**
             * Compute the children area for layout.
             */
            Rectangle getChildrenArea() override;

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
