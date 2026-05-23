// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_DRAGDROP_HPP
#define INCLUDE_FIFECHAN_DRAGDROP_HPP

// Standard library includes
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <utility>

// Platform config include
#include "fifechan/platform.hpp"

// Project headers (subdirs before local)
#include <fifechan/gui.hpp>
#include <fifechan/widget.hpp>

namespace fcn
{

    class Graphics;

    /**
     * Simple 2D integer point used for drag offsets and positions.
     */
    struct DragPoint
    {
            /**
             * X coordinate.
             */
            int x;

            /**
             * Y coordinate.
             */
            int y;

            /**
             * Construct a point with optional coordinates.
             */
            explicit DragPoint(int x_ = 0, int y_ = 0) : x(x_), y(y_)
            {
            }
    };

    /**
     * State of the drag controller.
     */
    enum class DragState : uint8_t
    {
        /** No drag is active. */
        Idle,

        /** A drag operation is in progress. */
        Dragging
    };

    /**
     * Result returned after attempting to drop payload onto a target.
     */
    enum class DropResult : uint8_t
    {
        /** Target accepted the drop. */
        Accepted,

        /** Target rejected the drop. */
        Rejected,

        /** Drop was cancelled before delivery. */
        Cancelled
    };

    /**
     * Container for arbitrary data carried during a drag operation.
     *
     * Users can attach arbitrary shared data and optionally provide
     * a ghost renderer and tooltip for visual feedback during drag.
     */
    class FIFEGUI_API DragPayload
    {
        public:
            /**
             * Callback used to render a ghost image at a screen position.
             */
            using RenderCallback = std::function<void(Graphics&, DragPoint const & screenPos)>;

            /**
             * Construct a payload.
             */
            explicit DragPayload(std::shared_ptr<void> data = nullptr);

            /**
             * Get shared pointer to stored data.
             */
            template <typename T>
            std::shared_ptr<T> getShared() const
            {
                return std::static_pointer_cast<T>(m_data);
            }

            /**
             * Get raw pointer to stored data.
             */
            template <typename T>
            T* get() const
            {
                return static_cast<T*>(m_data.get());
            }

            /**
             * Set the stored data.
             */
            void setData(std::shared_ptr<void> data)
            {
                m_data = std::move(data);
            }
            /**
             * True if payload stores data.
             */
            bool hasData() const
            {
                return m_data != nullptr;
            }

            /**
             * Set the ghost renderer callback.
             */
            void setGhostRenderer(RenderCallback cb)
            {
                m_ghostRenderer = std::move(cb);
            }
            /**
             * Get the ghost renderer callback.
             */
            RenderCallback getGhostRenderer() const
            {
                return m_ghostRenderer;
            }

            /**
             * Set tooltip text for the payload.
             */
            void setTooltip(std::string const & tip)
            {
                m_tooltip = tip;
            }
            /**
             * Get the tooltip text.
             */
            std::string const & getTooltip() const
            {
                return m_tooltip;
            }

        private:
            /**
             * Stored arbitrary data.
             */
            std::shared_ptr<void> m_data;

            /**
             * Ghost renderer callback.
             */
            RenderCallback m_ghostRenderer;

            /**
             * Tooltip text.
             */
            std::string m_tooltip;
    };

    /**
     * Configuration for how drag visuals are rendered.
     *
     * Holds a ghost renderer used to draw a floating representation
     * of the payload and a highlight renderer used to mark candidate
     * drop targets.
     */
    struct DragRenderConfig
    {
            /**
             * Renderer used to draw the ghost image.
             */
            using GhostRenderer = std::function<void(Graphics&, DragPayload const &, DragPoint const &)>;

            /**
             * Renderer used to draw a highlight on candidate targets.
             */
            using HighlightRenderer = std::function<void(Graphics&, Widget* target, bool isValid)>;

            /**
             * Renderer used to draw the ghost.
             */
            GhostRenderer ghost;

            /**
             * Renderer used to draw a highlight on targets.
             */
            HighlightRenderer highlight;

            /**
             * Default highlight renderer.
             */
            static HighlightRenderer defaultHighlight();
    };

    /**
     * Manages drag and drop operations for a `Gui` instance.
     *
     * Tracks drag state, active payload, hovered widget and
     * provides rendering hooks for ghost and highlight visuals.
     */
    class FIFEGUI_API DragHandler
    {
        public:
            /**
             * Create a drag handler for a GUI.
             */
            explicit DragHandler(Gui* gui);

            /**
             * Get the current drag state.
             */
            DragState getState() const;

            /**
             * Get the widget currently hovered by the drag.
             */
            Widget* getHoveredWidget() const;

            /**
             * Get the active drop target widget.
             */
            Widget* getActiveDropTarget() const;

            /**
             * Get the active payload.
             */
            DragPayload const * getPayload() const;

            /**
             * Start a drag with the given payload.
             */
            bool beginDrag(Widget* source, std::unique_ptr<DragPayload> payload, int mouseX, int mouseY);

            /**
             * Start a drag from a widget without explicit payload.
             */
            bool beginDragFromWidget(Widget* source, int mouseX, int mouseY);

            /**
             * Update drag position.
             */
            void update(int mouseX, int mouseY);

            /**
             * Drop at the given position.
             */
            DropResult drop(int mouseX, int mouseY);

            /**
             * Cancel the current drag.
             */
            void cancel();

            /**
             * Render drag related visuals.
             */
            void render(Graphics& graphics);

            /**
             * Set how drag visuals are rendered.
             */
            void setRenderConfig(DragRenderConfig config);

            /**
             * Get the current render config.
             */
            DragRenderConfig const & getRenderConfig() const;

            /**
             * Set a modal widget to ignore hits.
             */
            void setModalWidget(Widget* modal);

            /**
             * Get the modal widget if any.
             */
            Widget* getModalWidget() const;

            /**
             * Find a widget at the given coordinates.
             */
            static Widget* findWidgetAt(
                Widget* root, int x, int y, bool mustBeVisible = true, bool mustBeEnabled = true);

        private:
            /**
             * Distribute a leave event to targets.
             */
            void distributeDragLeave();

            /**
             * Distribute an enter event to a candidate.
             */
            void distributeDragEnter(Widget* candidate);

            /**
             * Update the active drop target.
             */
            void updateActiveTarget();

            /**
             * Associated GUI instance.
             */
            Gui* m_gui{nullptr};

            /**
             * Current drag state.
             */
            DragState m_state{DragState::Idle};

            /**
             * Active payload for the drag.
             */
            std::unique_ptr<DragPayload> m_payload;

            /**
             * Source widget for the drag.
             */
            Widget* m_sourceWidget{nullptr};

            /**
             * Widget currently hovered.
             */
            Widget* m_hoveredWidget{nullptr};

            /**
             * Widget that will receive the drop.
             */
            Widget* m_activeTarget{nullptr};

            /**
             * Modal widget that blocks events.
             */
            Widget* m_modalWidget{nullptr};

            /**
             * Drag offset from mouse.
             */
            DragPoint m_dragOffset;

            /**
             * How drag visuals are rendered.
             */
            DragRenderConfig m_renderConfig{};

            /**
             * Last mouse position seen.
             */
            DragPoint m_lastMousePos;
    };

} // namespace fcn

#endif // INCLUDE_FIFECHAN_DRAGDROP_HPP
