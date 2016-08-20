/***************************************************************************
 *   Copyright (C) 2012 by the fifechan team                               *
 *   http://fifechan.github.com/fifechan                                   *
 *   This file is part of fifechan.                                        *
 *                                                                         *
 *   fifechan is free software; you can redistribute it and/or             *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA          *
 ***************************************************************************/

/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naess�n and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naess�n a.k.a jansem/yakslem
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

#ifndef FCN_ICON_HPP
#define FCN_ICON_HPP

#include "fifechan/image.hpp"
#include "fifechan/platform.hpp"
#include "fifechan/widget.hpp"

namespace fcn
{
    /**
     * Implements an icon capable of displaying an image.
     */
    class FCN_CORE_DECLSPEC Icon: public Widget
    {
    public:
        /**
         * Default constructor.
         */
        Icon();

        /**
         * Constructor.
         *
         * @param filename The filename of the image to display.
         */
        Icon(const std::string& filename);

        /**
         * Constructor.
         *
         * @param image The image to display.
         */
        Icon(const Image* image);

        /**
         * Descructor.
         */
        virtual ~Icon();

        /**
         * Sets the image to display. Existing image is freed automatically
         * if it was loaded internally.
         *
         * @param image The image to display. 
         */
        void setImage(const std::string& filename);

        /**
         * Sets the image to display. Existing image is freed automatically
         * if it was loaded internally.
         *
         * @param image The image to display. 
         */
        void setImage(const Image* image);

        /**
         * Gets the current image.
         *
         * @return The current image.
         */
        const Image* getImage() const;

        /**
         * Sets if the image should be scaled to widget size.
         *
         * @param scale True if the image should be scaled.
         */
        void setScaling(bool scale);

        /**
         * Gets if the image is scaled to widget size.
         *
         * @return True if the image is scaled to widget size, otherwise false.
         */
        bool isScaling() const;

        /**
         * Sets if the image should be tiled to widget size.
         *
         * @param tile True if the image should be tiled.
         */
        void setTiling(bool tile);

        /**
         * Gets if the image is tiled to widget size.
         *
         * @return True if the image is tiled to widget size, otherwise false.
         */
        bool isTiling() const;

        /**
         * Sets the opacity of the background.
         * 
         * @param opaque True if opaque, false otherwise.
         */
        void setOpaque(bool opaque);

        /**
         * @return Whether this icons background is opaque or not.
         */
        bool isOpaque() const;


        // Inherited from Widget

        virtual void resizeToContent(bool recursiv=true);
        virtual void adjustSize();
        virtual void draw(Graphics* graphics);

    protected:
        /**
         * The image to display.
         */
        const Image* mImage;

        /**
         * True if the image has been loaded internally, false otherwise.
         * An image not loaded internally should not be deleted in the
         * destructor.
         */
        bool mInternalImage;

        // True if scaling is enabled, otherwise false.
        bool mScale;

        // True if tiling is enabled, otherwise false.
        bool mTile;

        // True if opaque, otherwise false.
        bool mOpaque;
    };
}

#endif // end FCN_ICON_HPP
