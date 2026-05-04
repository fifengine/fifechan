// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_LISTENERS_DROPTARGETLISTENER_HPP_
#define INCLUDE_FIFECHAN_LISTENERS_DROPTARGETLISTENER_HPP_

#include "fifechan/events/dragevent.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * Listener interface for widgets that accept dragged payloads.
     *
     * Implement this interface to be informed about drag enter/leave
     * hover and drop events for a potential drop target.
     */
    class FIFEGUI_API DropTargetListener
    {
        public:
            /**
             * Listener interface for drop target events.
             */
            virtual ~DropTargetListener() = default;

            DropTargetListener(DropTargetListener const &)            = delete;
            DropTargetListener& operator=(DropTargetListener const &) = delete;
            DropTargetListener(DropTargetListener&&)                  = delete;
            DropTargetListener& operator=(DropTargetListener&&)       = delete;

            /**
             * Called when a drag enters the target.
             *
             * @param dragEvent The drag event describing the source and payload.
             * @return True to accept the drag, false to indicate rejection.
             */
            virtual bool dragEntered(DragEvent& dragEvent)
            {
                return false;
            }
            /**
             * Called when a drag exits the target.
             *
             * @param dragEvent The drag event describing the exit.
             */
            virtual void dragExited(DragEvent& dragEvent)
            {
            }
            /**
             * Called when a drag is hovering over the target.
             *
             * @param dragEvent The drag event describing current hover position.
             */
            virtual void dragHovered(DragEvent& dragEvent)
            {
            }
            /**
             * Called when a payload is dropped on the target.
             *
             * @param dragEvent The drag event describing the drop action.
             */
            virtual void dragDropped(DragEvent& dragEvent)
            {
            }

        protected:
            DropTargetListener() = default;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_LISTENERS_DROPTARGETLISTENER_HPP_
