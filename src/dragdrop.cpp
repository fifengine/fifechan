// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include <fifechan/dragdrop.hpp>

// Standard library includes
#include <memory>
#include <utility>

// Platform config include
#include "fifechan/platform.hpp"

// Project headers (subdirs before local)
#include <fifechan/events/dragevent.hpp>
#include <fifechan/graphics.hpp>
#include <fifechan/listeners/droptargetlistener.hpp>
#include <fifechan/widget.hpp>

namespace fcn
{

    // Merge: provide DragPayload implementation here to avoid a separate translation unit.
    DragPayload::DragPayload(std::shared_ptr<void> data) : m_data(std::move(data)) { }

    DragHandler::DragHandler(Gui* gui) : m_gui(gui) { }

    DragState DragHandler::getState() const
    {
        return m_state;
    }

    Widget* DragHandler::getHoveredWidget() const
    {
        return m_hoveredWidget;
    }

    Widget* DragHandler::getActiveDropTarget() const
    {
        return m_activeTarget;
    }

    DragPayload const * DragHandler::getPayload() const
    {
        return m_payload.get();
    }

    bool DragHandler::beginDrag(Widget* source, std::unique_ptr<DragPayload> payload, int mouseX, int mouseY)
    {
        if (m_state == DragState::Dragging || !payload) {
            return false;
        }
        m_sourceWidget = source;
        m_payload      = std::move(payload);
        m_state        = DragState::Dragging;
        m_lastMousePos = DragPoint(mouseX, mouseY);
        return true;
    }

    bool DragHandler::beginDragFromWidget(Widget* /*source*/, int /*mouseX*/, int /*mouseY*/)
    {
        // Convenience: apps may implement IDragSource; left as TODO for implementation
        return false;
    }

    void DragHandler::update(int mouseX, int mouseY)
    {
        if (m_state != DragState::Dragging) {
            return;
        }
        m_lastMousePos = DragPoint(mouseX, mouseY);

        if (m_gui == nullptr) {
            return;
        }

        Widget* candidate = m_gui->getWidgetAt(mouseX, mouseY);

        if (candidate != m_hoveredWidget) {
            // Hover changed
            m_hoveredWidget = candidate;
            updateActiveTarget();
        } else {
            // same hovered widget, forward hover to active target if any
            if (m_activeTarget != nullptr) {
                int absX = 0;
                int absY = 0;
                m_activeTarget->getAbsolutePosition(absX, absY);
                int const localX = mouseX - absX;
                int const localY = mouseY - absY;
                DragEvent hoverEvt(
                    m_activeTarget, m_payload.get(), DragEvent::Type::Hover, localX, localY, mouseX, mouseY);
                m_activeTarget->distributeDragHover(hoverEvt);
            }
        }
    }

    DropResult DragHandler::drop(int /*mouseX*/, int /*mouseY*/)
    {
        if (m_state != DragState::Dragging) {
            return DropResult::Cancelled;
        }
        if (m_activeTarget != nullptr) {
            int const mouseX = m_lastMousePos.x;
            int const mouseY = m_lastMousePos.y;
            int absX         = 0;
            int absY         = 0;
            m_activeTarget->getAbsolutePosition(absX, absY);
            int const localX = mouseX - absX;
            int const localY = mouseY - absY;
            DragEvent dropEvt(m_activeTarget, m_payload.get(), DragEvent::Type::Drop, localX, localY, mouseX, mouseY);
            m_activeTarget->distributeDragDrop(dropEvt);
            m_payload.reset();
            m_state        = DragState::Idle;
            m_activeTarget = nullptr;
            return DropResult::Accepted;
        }

        m_payload.reset();
        m_state = DragState::Idle;
        return DropResult::Cancelled;
    }

    void DragHandler::cancel()
    {
        if (m_state != DragState::Dragging) {
            return;
        }
        // notify active target about leave if needed
        if (m_activeTarget != nullptr) {
            distributeDragLeave();
            m_activeTarget = nullptr;
        }
        m_payload.reset();
        m_state = DragState::Idle;
    }

    void DragHandler::render(Graphics& /*graphics*/)
    {
        // If a ghost renderer is supplied in m_renderConfig, call it here.
    }

    void DragHandler::setRenderConfig(DragRenderConfig config)
    {
        m_renderConfig = std::move(config);
    }

    DragRenderConfig const & DragHandler::getRenderConfig() const
    {
        return m_renderConfig;
    }

    void DragHandler::setModalWidget(Widget* modal)
    {
        m_modalWidget = modal;
    }
    Widget* DragHandler::getModalWidget() const
    {
        return m_modalWidget;
    }

    Widget* DragHandler::findWidgetAt(Widget* root, int x, int y, bool mustBeVisible, bool mustBeEnabled)
    {
        // Defer to Widget's existing hit test - this helper is a convenience placeholder.
        if (root == nullptr) {
            return nullptr;
        }
        return root->getWidgetAt(x, y);
    }

    void DragHandler::distributeDragLeave()
    {
        if (m_activeTarget == nullptr || m_payload == nullptr) {
            return;
        }

        int const mouseX = m_lastMousePos.x;
        int const mouseY = m_lastMousePos.y;
        int absX         = 0;
        int absY         = 0;
        m_activeTarget->getAbsolutePosition(absX, absY);
        int const localX = mouseX - absX;
        int const localY = mouseY - absY;
        DragEvent leaveEvt(m_activeTarget, m_payload.get(), DragEvent::Type::Leave, localX, localY, mouseX, mouseY);
        m_activeTarget->distributeDragLeave(leaveEvt);
    }

    void DragHandler::distributeDragEnter(Widget* candidate)
    {
        if (candidate == nullptr || m_payload == nullptr) {
            return;
        }

        int const mouseX = m_lastMousePos.x;
        int const mouseY = m_lastMousePos.y;
        int absX         = 0;
        int absY         = 0;
        candidate->getAbsolutePosition(absX, absY);
        int const localX = mouseX - absX;
        int const localY = mouseY - absY;
        DragEvent enterEvt(candidate, m_payload.get(), DragEvent::Type::Enter, localX, localY, mouseX, mouseY);
        bool const accepted = candidate->distributeDragEnter(enterEvt);
        if (accepted) {
            m_activeTarget = candidate;
        }
    }

    void DragHandler::updateActiveTarget()
    {
        // If we have an active target but no hovered widget, fire leave.
        if (m_activeTarget != nullptr && m_hoveredWidget != m_activeTarget) {
            distributeDragLeave();
            m_activeTarget = nullptr;
        }

        // If we have a hovered widget and no active target, try to enter it.
        if (m_hoveredWidget != nullptr && m_activeTarget == nullptr) {
            distributeDragEnter(m_hoveredWidget);
        }
    }

} // namespace fcn
