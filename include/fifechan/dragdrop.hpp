// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_DRAGDROP_HPP
#define INCLUDE_FIFECHAN_DRAGDROP_HPP

#include <fifechan/gui.hpp>
#include <fifechan/widget.hpp>

#include <cstdint>
#include <functional>
#include <memory>
#include <string>

namespace fcn
{

    class Graphics;

    struct DragPoint
    {
        int x, y;
        explicit DragPoint(int x_ = 0, int y_ = 0) : x(x_), y(y_) { }
    };

    enum class DragState : uint8_t
    {
        Idle,
        Dragging
    };

    enum class DropResult : uint8_t
    {
        Accepted,
        Rejected,
        Cancelled
    };

    class DragPayload
    {
    public:
        using RenderCallback = std::function<void(Graphics&, DragPoint const & screenPos)>;

        explicit DragPayload(std::shared_ptr<void> data = nullptr);

        template <typename T>
        std::shared_ptr<T> getShared() const
        {
            return std::static_pointer_cast<T>(m_data);
        }

        template <typename T>
        T* get() const
        {
            return static_cast<T*>(m_data.get());
        }

        void setData(std::shared_ptr<void> data)
        {
            m_data = std::move(data);
        }
        bool hasData() const
        {
            return m_data != nullptr;
        }

        void setGhostRenderer(RenderCallback cb)
        {
            m_ghostRenderer = std::move(cb);
        }
        RenderCallback getGhostRenderer() const
        {
            return m_ghostRenderer;
        }

        void setTooltip(std::string tip)
        {
            m_tooltip = std::move(tip);
        }
        std::string const & getTooltip() const
        {
            return m_tooltip;
        }

    private:
        std::shared_ptr<void> m_data;
        RenderCallback m_ghostRenderer;
        std::string m_tooltip;
    };

    struct DragRenderConfig
    {
        using GhostRenderer     = std::function<void(Graphics&, DragPayload const &, DragPoint const &)>;
        using HighlightRenderer = std::function<void(Graphics&, Widget* target, bool isValid)>;

        GhostRenderer ghost;
        HighlightRenderer highlight;

        static HighlightRenderer defaultHighlight();
    };

    class DragHandler
    {
    public:
        explicit DragHandler(Gui* gui);

        DragState getState() const;
        Widget* getHoveredWidget() const;
        Widget* getActiveDropTarget() const;
        DragPayload const * getPayload() const;

        bool beginDrag(Widget* source, std::unique_ptr<DragPayload> payload, int mouseX, int mouseY);
        bool beginDragFromWidget(Widget* source, int mouseX, int mouseY);

        void update(int mouseX, int mouseY);
        DropResult drop(int mouseX, int mouseY);
        void cancel();

        void render(Graphics& graphics);

        void setRenderConfig(DragRenderConfig config);
        DragRenderConfig const & getRenderConfig() const;

        void setModalWidget(Widget* modal);
        Widget* getModalWidget() const;

        static Widget* findWidgetAt(Widget* root, int x, int y, bool mustBeVisible = true, bool mustBeEnabled = true);

    private:
        void distributeDragLeave();
        void distributeDragEnter(Widget* candidate);
        void updateActiveTarget();

        Gui* m_gui{nullptr};
        DragState m_state{DragState::Idle};
        std::unique_ptr<DragPayload> m_payload;

        Widget* m_sourceWidget{nullptr};
        Widget* m_hoveredWidget{nullptr};
        Widget* m_activeTarget{nullptr};
        Widget* m_modalWidget{nullptr};

        DragPoint m_dragOffset{};
        DragRenderConfig m_renderConfig{};

        DragPoint m_lastMousePos{};
    };

} // namespace fcn

#endif // INCLUDE_FIFECHAN_DRAGDROP_HPP
