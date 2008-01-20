/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * For comments regarding functions please see the header file.
 */

#include "guichan/rectangle.hpp"

namespace gcn
{
    Rectangle::Rectangle() 
        : x(0), 
          y(0), 
          width(0), 
          height(0)
    {
    }

    Rectangle::Rectangle(int ax, int ay, int awidth, int aheight)
      : x(ax),
        y(ay),
        width(awidth),
        height(aheight)
    {
    }

    void Rectangle::setAll(int ax, int ay, int awidth, int aheight)
    {
        x = ax;
        y = ay;
        width = awidth;
        height = aheight;
    }

    bool Rectangle::isIntersecting(const Rectangle& rectangle)
    {
        x -= rectangle.x;
        y -= rectangle.y;

        if (x < 0)
        {
            width += x;
            x = 0;
        }
        else if (x + width > rectangle.width)
        {
            width = rectangle.width - x;
        }

        if (y < 0)
        {
            height += y;
            y = 0;
        }
        else if (y + height > rectangle.height)
        {
            height = rectangle.height - y;
        }

        if (width <= 0 || height <= 0)
        {
            height = 0;
            width = 0;
            x += rectangle.x;
            y += rectangle.y;

            return false;
        }

        x += rectangle.x;
        y += rectangle.y;

        return true;
    }

    bool Rectangle::isPointInRect(int ax, int ay) const
    {
        return (ax >= x) 
                && (ay >= y) 
                && ax < (x + width) 
                && ay < (y + height);
    }

    std::ostream& operator<<(std::ostream& out,
                             const Rectangle& rectangle)
    {
        out << "Rectangle [x = "
            << rectangle.x
            << ", y = "
            << rectangle.y
            << ", width = "
            << rectangle.width
            << ", height = "
            << rectangle.height
            << "]";

        return out;
    }
}
