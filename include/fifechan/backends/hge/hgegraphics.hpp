// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_BACKENDS_HGE_HGEGRAPHICS_HPP_
#define INCLUDE_FIFECHAN_BACKENDS_HGE_HGEGRAPHICS_HPP_

#include "fifechan/color.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/platform.hpp"
#include <hge.h>

namespace fcn
{
    class Color;

    /**
     * HGE implementation of Graphics.
     */
    class FIFEGUI_EXT_API HGEGraphics : public Graphics
    {
    public:
        HGEGraphics();

        ~HGEGraphics();

        // Inherited from Graphics

        void _beginDraw();

        void _endDraw();

        void drawImage(Image const * image, int srcX, int srcY, int dstX, int dstY, int width, int height);

        void drawImage(Image const * image, int dstX, int dstY);

        void drawPoint(int x, int y);

        void drawLine(int x1, int y1, int x2, int y2);

        void drawRectangle(Rectangle const & rectangle);

        void fillRectangle(Rectangle const & rectangle);

        void setColor(Color const & color);

        Color const & getColor() const;

        bool pushClipArea(Rectangle area);

        void popClipArea();

    protected:
        static HGE* mHGE;
        Color mColor;
        DWORD mHardwareColor;
        bool mClipNull;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_BACKENDS_HGE_HGEGRAPHICS_HPP_
