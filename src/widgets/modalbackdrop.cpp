// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/widgets/modalbackdrop.hpp"

// Project headers
#include "fifechan/graphics.hpp"
#include "fifechan/widgets/menubar.hpp"
#include "fifechan/widgets/menuitem.hpp"
#include "fifechan/widgets/menupopup.hpp"

namespace fcn
{
    ModalBackdrop::ModalBackdrop(MenuPopup* owner) : mOwner(owner)
    {
        setOpaque(false);
        setFocusable(false);
        setEnabled(true);
    }

    ModalBackdrop::~ModalBackdrop() = default;

    void ModalBackdrop::draw([[maybe_unused]] Graphics*)
    {
        // Intentionally transparent; no drawing required.
    }

    void ModalBackdrop::mousePressed(MouseEvent& event)
    {
        // Click outside the menu should close the owning menu tree.
        // If the click targets the top MenuBar or a MenuItem,
        // we should allow the event to propagate,
        // so the MenuBar can toggle the popup.
        int x = event.getX();
        int y = event.getY();

        bool clickHitsMenuBar          = false;
        Container const * topContainer = dynamic_cast<Container*>(getTop());
        if (topContainer) {
            for (unsigned i = 0; i < topContainer->getChildrenCount(); ++i) {
                Widget* child = topContainer->getChild(i);
                if (!child || child == this)
                    continue;
                int cx = 0, cy = 0;
                child->getAbsolutePosition(cx, cy);
                if (x >= cx && x < cx + child->getWidth() && y >= cy && y < cy + child->getHeight()) {
                    if (dynamic_cast<MenuBar*>(child) || dynamic_cast<MenuItem*>(child)) {
                        clickHitsMenuBar = true;
                        break;
                    }
                }
            }
        }

        // If the click hits the top MenuBar/MenuItem, allow the event to
        // propagate so the MenuBar can handle toggling the popup.
        if (clickHitsMenuBar) {
            return;
        }

        // Otherwise close the owning popup and consume the event
        // so other widgets don't react to the click.
        if (mOwner) {
            mOwner->hide();
        }

        event.consume();
    }

    void ModalBackdrop::mouseReleased([[maybe_unused]] MouseEvent&) { }
    void ModalBackdrop::mouseEntered([[maybe_unused]] MouseEvent&) { }
    void ModalBackdrop::mouseExited([[maybe_unused]] MouseEvent&) { }
} // namespace fcn
