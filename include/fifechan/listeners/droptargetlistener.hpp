// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_LISTENERS_DROPTARGETLISTENER_HPP_
#define INCLUDE_FIFECHAN_LISTENERS_DROPTARGETLISTENER_HPP_

#include "fifechan/events/dragevent.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    class FIFEGUI_API DropTargetListener
    {
    public:
        virtual ~DropTargetListener() = default;

        DropTargetListener(DropTargetListener const &)            = delete;
        DropTargetListener& operator=(DropTargetListener const &) = delete;
        DropTargetListener(DropTargetListener&&)                  = delete;
        DropTargetListener& operator=(DropTargetListener&&)       = delete;

        virtual bool dragEntered(DragEvent& dragEvent)
        {
            return false;
        }
        virtual void dragExited(DragEvent& dragEvent) { }
        virtual void dragHovered(DragEvent& dragEvent) { }
        virtual void dragDropped(DragEvent& dragEvent) { }

    protected:
        DropTargetListener() = default;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_LISTENERS_DROPTARGETLISTENER_HPP_
