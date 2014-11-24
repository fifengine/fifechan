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

#ifndef FCN_IMAGEBUTTON_HPP
#define FCN_IMAGEBUTTON_HPP

#include <vector>

#include "fifechan/platform.hpp"
#include "fifechan/widgets/button.hpp"

namespace fcn
{
    class Image;

    /**
     * An implementation of a regular clickable button. Unlike a normal button an image 
     * button is capable of displaying images instead of only a simple text caption.
     * It is possible to define images for button up, down and hover.
     * Also the same 3 images for a inactive button so that it's possible to visualize the inactivity.
     * As minimum the button up image should be set. All other 5 images are optional.
     * Whenever an image button is clicked an action event will be sent to the action 
     * listener's of the image button.
     *
     * @see Button
     */
    class FCN_CORE_DECLSPEC ImageButton : public fcn::Button
    {
    public:
        /**
         * Default constructor.
         */
        ImageButton();

        /**
         * Constructor.
         *
         * @param filename The filename of the up image to display.
         */
        ImageButton(const std::string& filename);

        /**
         * Constructor.
         *
         * @param image The up image to display.
         */
        ImageButton(const Image* image);

        /**
         * Destructor.
         * Existing Images are freed automatically, if they were loaded internally.
         */
        virtual ~ImageButton();

        /**
         * Sets the up image to display. That is the basic image that is also used if other images are not set.
         * Existing Image is freed automatically, if it was loaded internally.
         *
         * @param filename The filename of the up image to display.
         */
        void setUpImage(const std::string& filename);

        /**
         * Sets the up image to display. That is the basic image that is also used if other images are not set.
         * Existing Image is freed automatically, if it was loaded internally.
         *
         * @param image The up image to display.
         */
        void setUpImage(const Image* image);

        /**
         * Gets current up image.
         *
         * @return The current up image.
         */
        const Image* getUpImage() const;

        /**
         * Sets the down image to display. That is the image that is used if the button is pressed.
         * Existing Image is freed automatically, if it was loaded internally.
         *
         * @param filename The filename of the down image to display.
         */
        void setDownImage(const std::string& filename);

        /**
         * Sets the down image to display. That is the image that is used if the button is pressed.
         * Existing Image is freed automatically, if it was loaded internally.
         *
         * @param image The down image to display.
         */
        void setDownImage(const Image* image);

        /**
         * Gets current down image.
         *
         * @return The current down image.
         */
        const Image* getDownImage() const;

        /**
         * Sets the hover image to display. That is the image that is used if the mouse is over the button.
         * Existing Image is freed automatically, if it was loaded internally.
         *
         * @param filename The filename of the hover image to display.
         */
        void setHoverImage(const std::string& filename);

        /**
         * Sets the hover image to display. That is the image that is used if the mouse is over the button.
         * Existing Image is freed automatically, if it was loaded internally.
         *
         * @param image The hover image to display.
         */
        void setHoverImage(const Image* image);

        /**
         * Gets current hover image.
         *
         * @return The current hover image.
         */
        const Image* getHoverImage() const;

        /**
         * Sets the up image to display if the button is inactive. If not provided the active up image is used.
         * Existing Image is freed automatically, if it was loaded internally.
         *
         * @param filename The filename of the inactive up image to display.
         */
        void setInactiveUpImage(const std::string& filename);

        /**
         * Sets the up image to display if the button is inactive. If not provided the active up image is used.
         * Existing Image is freed automatically, if it was loaded internally.
         *
         * @param image The inactive up image to display.
         */
        void setInactiveUpImage(const Image* image);

        /**
         * Gets inactive up image.
         *
         * @return The inactive up image.
         */
        const Image* getInactiveUpImage() const;

        /**
         * Sets the down image to display if the button is inactive.
         * Existing Image is freed automatically, if it was loaded internally.
         *
         * @param filename The filename of the inactive down image to display.
         */
        void setInactiveDownImage(const std::string& filename);

        /**
         * Sets the down image to display if the button is inactive.
         * Existing Image is freed automatically, if it was loaded internally.
         *
         * @param image The inactive down image to display.
         */
        void setInactiveDownImage(const Image* image);

        /**
         * Gets inactive down image.
         *
         * @return The inactive down image.
         */
        const Image* getInactiveDownImage() const;

        /**
         * Sets the hover image to display if the button is inactive.
         * Existing Image is freed automatically, if it was loaded internally.
         *
         * @param filename The filename of the inactive hover image to display.
         */
        void setInactiveHoverImage(const std::string& filename);

        /**
         * Sets the hover image to display if the button is inactive.
         * Existing Image is freed automatically, if it was loaded internally.
         *
         * @param image The inactive hover image to display.
         */
        void setInactiveHoverImage(const Image* image);

        /**
         * Gets inactive hover image.
         *
         * @return The inactive hover image.
         */
        const Image* getInactiveHoverImage() const;

        // Inherited from Widget

        virtual void resizeToContent(bool recursiv=true);
        virtual void adjustSize();
        virtual void draw(fcn::Graphics* graphics);

    protected:
        // enum for easy image access
        enum ImageType {
            Image_Up = 0,
            Image_Down = 1,
            Image_Hover = 2,
            Image_Up_De = 3,
            Image_Down_De = 4,
            Image_Hover_De = 5
        };

        void setImage(const std::string& filename, ImageType type);
        void setImage(const Image* image, ImageType type);

        /**
         * The images to display.
         */
        std::vector<const Image*> mImages;

        /**
         * True if the image has been loaded internally, false otherwise.
         * An image not loaded internally should not be deleted in the
         * destructor.
         */
        std::vector<bool> mInternalImages;
    };
}
#endif
