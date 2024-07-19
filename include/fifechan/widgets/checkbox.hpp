// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

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
        enum MarkerStyle
        {
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
        CheckBox(std::string const & caption, bool selected = false);

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
        void setBackgroundImage(std::string const & filename);

        /**
         * Sets the background image to display, that includes the caption region.
         * Existing Image is freed automatically, if it was loaded internally.
         *
         * @param image The up image to display.
         */
        void setBackgroundImage(Image const * image);

        /**
         * Gets background image.
         *
         * @return The background image.
         */
        Image const * getBackgroundImage() const;

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
        virtual void drawBox(Graphics* graphics);

        /**
         * Draws the checkmark.
         *
         * @param graphics A Graphics object to draw with.
         * @param rec The rectangle that defines the position and size.
         */
        void drawCheckmark(Graphics* graphics, Rectangle const & rec);

        /**
         * Draws the cross.
         *
         * @param graphics A Graphics object to draw with.
         * @param rec The rectangle that defines the position and size.
         */
        void drawCross(Graphics* graphics, Rectangle const & rec);

        /**
         * Draws the dot.
         *
         * @param graphics A Graphics object to draw with.
         * @param rec The rectangle that defines the position and size.
         */
        void drawDot(Graphics* graphics, Rectangle const & rec);

        /**
         * Draws the marker image.
         *
         * @param graphics A Graphics object to draw with.
         * @param rec The rectangle that defines the position and size.
         */
        void drawMarkerImage(Graphics* graphics, Rectangle const & rec);

        /**
         * Draws the rhombus. Box, marker and selection.
         *
         * @param graphics A Graphics object to draw with.
         */
        void drawRhombus(Graphics* graphics);

        /**
         * Holds the background image, that includes the caption region.
         */
        Image const * mBackgroundImage;

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
} // namespace fcn

#endif // end FCN_CHECKBOX_HPP
