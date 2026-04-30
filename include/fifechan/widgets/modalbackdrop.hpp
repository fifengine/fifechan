// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_MODALBACKDROP_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_MODALBACKDROP_HPP_

// Platform config include
#include "fifechan/platform.hpp"

// Standard library includes
#include <functional>

#include "fifechan/listeners/mouselistener.hpp"
#include "fifechan/widget.hpp"
#include "fifechan/widgets/container.hpp"

namespace fcn
{
    class MenuPopup;

    /**
     * A Transparent fullscreen backdrop.
     *
     * It is used to detect clicks outside a `MenuPopup` and close it.
     * It's added to the GUI top container.
     */
    class FIFEGUI_API ModalBackdrop : public Container, public MouseListener
    {
        public:
            explicit ModalBackdrop(MenuPopup* owner);
            ~ModalBackdrop() override;

            ModalBackdrop(ModalBackdrop const &)            = delete;
            ModalBackdrop& operator=(ModalBackdrop const &) = delete;
            ModalBackdrop(ModalBackdrop&&)                  = delete;
            ModalBackdrop& operator=(ModalBackdrop&&)       = delete;

            // Widget
            void draw(Graphics* graphics) override;

            // MouseListener
            void mousePressed(MouseEvent& event) override;
            void mouseReleased(MouseEvent& event) override;
            void mouseEntered(MouseEvent& event) override;
            void mouseExited(MouseEvent& event) override;

        private:
            MenuPopup* mOwner{nullptr};
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_MODALBACKDROP_HPP_
