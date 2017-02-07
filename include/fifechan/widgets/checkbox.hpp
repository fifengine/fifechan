/***************************************************************************
 *   Copyright (c) 2017 by the fifechan team                               *
 *   https://github.com/fifengine/fifechan                                 *
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

#ifndef FCN_CHECKBOX_HPP
#define FCN_CHECKBOX_HPP

#include <string>

#include "fifechan/platform.hpp"
#include "fifechan/widgets/imagebutton.hpp"

namespace fcn
{
    class Image;

    /**
     * An implementation of a check box where a user can select or deselect
     * the check box and where the status of the check box is displayed to the user.
     * A check box is capable of displaying a caption. 
     * 
     * If a check box's state changes an action event will be sent to all action 
     * listeners of the check box.
     */
    class FCN_CORE_DECLSPEC CheckBox : public fcn::ImageButton
    {
    public:

        /**
         * Marker style.
         */
        enum MarkerStyle {
            Marker_Checkmark = 0,
            Marker_Cross,
            Marker_Dot,
            Marker_Rhombus,
            Marker_Image
        };

        /**
         * Contructor.
         */
        CheckBox();

        /**
         * Constructor. The check box will be automatically resized
         * to fit the caption.
         *
         * @param caption The caption of the check box.
         * @param marked True if the check box is selected, false otherwise.
         */
        CheckBox(const std::string& caption, bool selected = false);

        /**
         * Destructor.
         */
        virtual ~CheckBox();

        /**
         * Checks if the check box is selected.
         *
         * @return True if the check box is selected, false otherwise.
         * @see setSelected
         */
        virtual bool isSelected() const;

        /**
         * Sets the check box to be selected or not.
         *
         * @param selected True if the check box should be set as selected.
         * @see isSelected
         */
        virtual void setSelected(bool selected);

        /**
         * Toggles the check box between being selected and
         * not being selected. It distribute a ActionEvent.
         */
        virtual void toggleSelected();

        /**
         * Sets the background image to display, that includes the caption region.
         * Existing Image is freed automatically, if it was loaded internally.
         *
         * @param filename The filename of the up image to display.
         */
        void setBackgroundImage(const std::string& filename);

        /**
         * Sets the background image to display, that includes the caption region.
         * Existing Image is freed automatically, if it was loaded internally.
         *
         * @param image The up image to display.
         */
        void setBackgroundImage(const Image* image);

        /**
         * Gets background image.
         *
         * @return The background image.
         */
        const Image* getBackgroundImage() const;

        /**
         * Gets the marker mode of the check box.
         * @return The mode of the check box.
         * @see setMarkerStyle, MarkerStyle
         */
        MarkerStyle getMarkerStyle() const;

        /**
         * Set the marker style of the check box.
         * @param mode The style of the check box.
         * @see getMarkerStyle, MarkerStyle
         */
        void setMarkerStyle(MarkerStyle mode);


        // Inherited from Widget

        virtual void adjustSize();
        virtual void draw(Graphics* graphics);


        // Inherited from KeyListener

        virtual void keyPressed(KeyEvent& keyEvent);
        virtual void keyReleased(KeyEvent& keyEvent);


        // Inherited from MouseListener

        virtual void mousePressed(MouseEvent& mouseEvent);
        virtual void mouseReleased(MouseEvent& mouseEvent);
        virtual void mouseClicked(MouseEvent& mouseEvent);

    protected:
        /**
         * Draws the box of the check box. 
         *
         * @param graphics A Graphics object to draw with.
         */
        virtual void drawBox(Graphics *graphics);

        /**
         * Draws the checkmark. 
         *
         * @param graphics A Graphics object to draw with.
         * @param rec The rectangle that defines the position and size.
         */
        void drawCheckmark(Graphics* graphics, const Rectangle& rec);

        /**
         * Draws the cross. 
         *
         * @param graphics A Graphics object to draw with.
         * @param rec The rectangle that defines the position and size.
         */
        void drawCross(Graphics* graphics, const Rectangle& rec);

        /**
         * Draws the dot. 
         *
         * @param graphics A Graphics object to draw with.
         * @param rec The rectangle that defines the position and size.
         */
        void drawDot(Graphics* graphics, const Rectangle& rec);

        /**
         * Draws the marker image.
         *
         * @param graphics A Graphics object to draw with.
         * @param rec The rectangle that defines the position and size.
         */
        void drawMarkerImage(Graphics* graphics, const Rectangle& rec);

        /**
         * Draws the rhombus. Box, marker and selection.
         *
         * @param graphics A Graphics object to draw with.
         */
        void drawRhombus(Graphics* graphics);

        /**
         * Holds the background image, that includes the caption region.
         */
        const Image* mBackgroundImage;

        /**
         * True if the image has been loaded internally.
         */
        bool mInternalBackgroundImage;

        /**
         * True if the check box is selected, false otherwise.
         */
        bool mSelected;

        /**
         * Holds the marker style of the check box.
         */
        MarkerStyle mMode;

    };
}

#endif // end FCN_CHECKBOX_HPP
