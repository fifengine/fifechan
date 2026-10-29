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
#include <fifechan/color.hpp>
#include <fifechan/events/dragevent.hpp>
#include <fifechan/graphics.hpp>
#include <fifechan/listeners/droptargetlistener.hpp>
#include <fifechan/rectangle.hpp>
#include <fifechan/widget.hpp>

namespace fcn
{
    namespace
    {
        /**
         * True if `widget` is `ancestor` or one of its descendants.
         */
        bool isWithin(Widget const * widget, Widget const * ancestor)
        {
            for (Widget const * current = widget; current != nullptr; current = current->getParent()) {
                if (current == ancestor) {
                    return true;
                }
            }
            return false;
        }

        /**
         * Convert screen coordinates to coordinates local to `widget`.
         */
        DragPoint toLocal(Widget const * widget, int screenX, int screenY)
        {
            int absoluteX = 0;
            int absoluteY = 0;
            widget->getAbsolutePosition(absoluteX, absoluteY);
            return DragPoint(screenX - absoluteX, screenY - absoluteY);
        }
    } // namespace

    // Merge: provide DragPayload implementation here to avoid a separate translation unit.
    DragPayload::DragPayload(std::shared_ptr<void> data) : m_data(std::move(data))
    {
    }

    DragHandler::DragHandler(Gui* gui) : m_gui(gui)
    {
    }

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

    Widget* DragHandler::getSourceWidget() const
    {
        return m_sourceWidget;
    }

    bool DragHandler::beginDrag(Widget* source, std::unique_ptr<DragPayload> payload, int mouseX, int mouseY)
    {
        if (m_state == DragState::Dragging || !payload) {
            return false;
        }
        m_sourceWidget      = source;
        m_payload           = std::move(payload);
        m_state             = DragState::Dragging;
        m_lastMousePos      = DragPoint(mouseX, mouseY);
        m_hoveredWidget     = nullptr;
        m_lastHoveredWidget = nullptr;
        m_activeTarget      = nullptr;
        return true;
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

        Widget* candidate = nullptr;
        Widget* const top = m_gui->getTop();
        if (top != nullptr) {
            int absoluteX = 0;
            int absoluteY = 0;
            top->getAbsolutePosition(absoluteX, absoluteY);
            candidate = findWidgetAt(top, mouseX - absoluteX, mouseY - absoluteY, true, true);
        }

        // A modal widget confines drops to itself and its descendants.
        if (candidate != nullptr && m_modalWidget != nullptr && !isWithin(candidate, m_modalWidget)) {
            candidate = nullptr;
        }

        m_hoveredWidget           = candidate;
        bool const hoveredChanged = candidate != m_lastHoveredWidget;
        updateActiveTarget(hoveredChanged);

        if (m_activeTarget != nullptr) {
            DragPoint const local = toLocal(m_activeTarget, mouseX, mouseY);
            DragEvent hoverEvt(
                m_activeTarget, m_payload.get(), DragEvent::Type::Hover, local.x, local.y, mouseX, mouseY);
            m_activeTarget->distributeDragHover(hoverEvt);
        }
    }

    DropResult DragHandler::drop(int mouseX, int mouseY)
    {
        if (m_state != DragState::Dragging) {
            return DropResult::Cancelled;
        }

        // Refresh the target from the coordinates actually used for the drop.
        update(mouseX, mouseY);

        DropResult result = DropResult::Cancelled;
        if (m_activeTarget != nullptr) {
            DragPoint const local = toLocal(m_activeTarget, mouseX, mouseY);
            DragEvent dropEvt(m_activeTarget, m_payload.get(), DragEvent::Type::Drop, local.x, local.y, mouseX, mouseY);
            bool const accepted = m_activeTarget->distributeDragDrop(dropEvt);
            result              = accepted ? DropResult::Accepted : DropResult::Rejected;

            // The drag ends here, so the target is no longer hovered. Send the
            // same exit notification as cancel() so listeners can clear hover
            // state in one place, regardless of whether the drop was accepted.
            distributeDragLeave();
        }

        m_payload.reset();
        m_state             = DragState::Idle;
        m_activeTarget      = nullptr;
        m_hoveredWidget     = nullptr;
        m_lastHoveredWidget = nullptr;
        m_sourceWidget      = nullptr;
        return result;
    }

    void DragHandler::cancel()
    {
        if (m_state != DragState::Dragging) {
            return;
        }
        if (m_activeTarget != nullptr) {
            distributeDragLeave();
        }
        m_payload.reset();
        m_state             = DragState::Idle;
        m_activeTarget      = nullptr;
        m_hoveredWidget     = nullptr;
        m_lastHoveredWidget = nullptr;
        m_sourceWidget      = nullptr;
    }

    void DragHandler::render(Graphics& graphics)
    {
        if (m_state != DragState::Dragging || m_payload == nullptr) {
            return;
        }

        if (m_renderConfig.ghost) {
            m_renderConfig.ghost(graphics, *m_payload, m_lastMousePos);
        } else if (auto const ghost = m_payload->getGhostRenderer(); ghost) {
            ghost(graphics, m_lastMousePos);
        }

        if (m_activeTarget != nullptr) {
            auto const highlight =
                m_renderConfig.highlight ? m_renderConfig.highlight : DragRenderConfig::defaultHighlight();
            highlight(graphics, m_activeTarget, true);
        }
    }

    void DragHandler::setRenderConfig(DragRenderConfig config)
    {
        m_renderConfig = std::move(config);
    }

    DragRenderConfig DragHandler::getRenderConfig() const
    {
        return m_renderConfig;
    }

    DragRenderConfig::HighlightRenderer DragRenderConfig::defaultHighlight()
    {
        // Signature is fixed by HighlightRenderer; a const pointer would not match.
        // cppcheck-suppress constParameterPointer
        return [](Graphics& graphics, Widget* target, bool isValid) {
            if (target == nullptr) {
                return;
            }

            int x = 0;
            int y = 0;
            target->getAbsolutePosition(x, y);
            graphics.setColor(isValid ? Color(96, 200, 128, 220) : Color(220, 96, 96, 220));
            graphics.drawRectangle(Rectangle(x, y, target->getWidth(), target->getHeight()));
        };
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
        if (root == nullptr) {
            return nullptr;
        }
        if (mustBeVisible && !root->isVisible()) {
            return nullptr;
        }
        if (mustBeEnabled && !root->isEnabled()) {
            return nullptr;
        }

        // `root` receives coordinates in its own space, so its bounds start at (0, 0).
        if (x < 0 || y < 0 || x >= root->getWidth() || y >= root->getHeight()) {
            return nullptr;
        }

        Rectangle const area = root->getChildrenArea();
        if (!area.isContaining(x, y)) {
            // The point is on the widget itself (border or padding), not its children.
            return root;
        }

        int const childX = x - area.x;
        int const childY = y - area.y;

        auto const & children = root->getChildren();
        for (auto iter = children.rbegin(); iter != children.rend(); ++iter) {
            Widget* const child = *iter;
            if (child == nullptr) {
                continue;
            }
            if (mustBeVisible && !child->isVisible()) {
                continue;
            }
            if (mustBeEnabled && !child->isEnabled()) {
                continue;
            }
            if (!child->getDimension().isContaining(childX, childY)) {
                continue;
            }

            Widget* const deeper =
                findWidgetAt(child, childX - child->getX(), childY - child->getY(), mustBeVisible, mustBeEnabled);
            if (deeper != nullptr) {
                return deeper;
            }
        }

        return root;
    }

    void DragHandler::distributeDragLeave()
    {
        if (m_activeTarget == nullptr || m_payload == nullptr) {
            return;
        }

        DragPoint const local = toLocal(m_activeTarget, m_lastMousePos.x, m_lastMousePos.y);
        DragEvent leaveEvt(
            m_activeTarget,
            m_payload.get(),
            DragEvent::Type::Leave,
            local.x,
            local.y,
            m_lastMousePos.x,
            m_lastMousePos.y);
        m_activeTarget->distributeDragLeave(leaveEvt);
    }

    bool DragHandler::distributeDragEnter(Widget* candidate)
    {
        if (candidate == nullptr || m_payload == nullptr) {
            return false;
        }

        DragPoint const local = toLocal(candidate, m_lastMousePos.x, m_lastMousePos.y);
        DragEvent enterEvt(
            candidate, m_payload.get(), DragEvent::Type::Enter, local.x, local.y, m_lastMousePos.x, m_lastMousePos.y);
        bool const accepted = candidate->distributeDragEnter(enterEvt);
        if (accepted) {
            m_activeTarget = candidate;
        }
        return accepted;
    }

    void DragHandler::updateActiveTarget(bool hoveredChanged)
    {
        // Leave the current target when the pointer moves outside it.
        if (m_activeTarget != nullptr) {
            bool const stillInside = m_hoveredWidget != nullptr && isWithin(m_hoveredWidget, m_activeTarget);
            if (!stillInside) {
                distributeDragLeave();
                m_activeTarget = nullptr;
            }
        }

        // Enter the nearest ancestor that accepts the drag. This lets a container
        // act as a drop target even when the pointer is over one of its children.
        // Enter is only attempted when the hovered widget changes, so listeners
        // that reject a drag are not re-notified on every pointer movement.
        if (m_activeTarget == nullptr && hoveredChanged && m_hoveredWidget != nullptr) {
            for (Widget* current = m_hoveredWidget; current != nullptr; current = current->getParent()) {
                if (m_modalWidget != nullptr && !isWithin(current, m_modalWidget)) {
                    break;
                }
                if (distributeDragEnter(current)) {
                    m_activeTarget = current;
                    break;
                }
            }
        }

        m_lastHoveredWidget = m_hoveredWidget;
    }

} // namespace fcn
