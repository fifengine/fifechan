// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_IMAGEBUTTON_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_IMAGEBUTTON_HPP_

#include <memory>
#include <string>
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
    class FIFEGUI_API ImageButton : public fcn::Button
    {
    public:
        // enum for easy image access
        enum class ImageType : uint8_t
        {
            Up             = 0,
            Down           = 1,
            Hover          = 2,
            Up_Inactive    = 3,
            Down_Inactive  = 4,
            Hover_Inactive = 5
        };

        ImageButton();

        /**
         * Constructor.
         *
         * @param filename The filename of the up image to display.
         */
        explicit ImageButton(std::string const & filename);

        /**
         * Constructor.
         * Existing Images are freed automatically, if they were loaded internally.
         *
         * @param image The up image to display.
         */
        explicit ImageButton(Image const * image);

        virtual ~ImageButton();

        /**
         * Sets the up image to display. That is the basic image that is also used if other images are not set.
         * Existing Image is freed automatically, if it was loaded internally.
         *
         * @param filename The filename of the up image to display.
         */
        void setUpImage(std::string const & filename);

        /**
         * Sets the up image to display. That is the basic image that is also used if other images are not set.
         * Existing Image is freed automatically, if it was loaded internally.
         *
         * @param image The up image to display.
         */
        void setUpImage(Image const * image);

        /**
         * Gets current up image.
         *
         * @return The current up image.
         */
        Image const * getUpImage() const;

        /**
         * Sets the down image to display. That is the image that is used if the button is pressed.
         * Existing Image is freed automatically, if it was loaded internally.
         *
         * @param filename The filename of the down image to display.
         */
        void setDownImage(std::string const & filename);

        /**
         * Sets the down image to display. That is the image that is used if the button is pressed.
         * Existing Image is freed automatically, if it was loaded internally.
         *
         * @param image The down image to display.
         */
        void setDownImage(Image const * image);

        /**
         * Gets current down image.
         *
         * @return The current down image.
         */
        Image const * getDownImage() const;

        /**
         * Sets the hover image to display. That is the image that is used if the mouse is over the button.
         * Existing Image is freed automatically, if it was loaded internally.
         *
         * @param filename The filename of the hover image to display.
         */
        void setHoverImage(std::string const & filename);

        /**
         * Sets the hover image to display. That is the image that is used if the mouse is over the button.
         * Existing Image is freed automatically, if it was loaded internally.
         *
         * @param image The hover image to display.
         */
        void setHoverImage(Image const * image);

        /**
         * Gets current hover image.
         *
         * @return The current hover image.
         */
        Image const * getHoverImage() const;

        /**
         * Sets the up image to display if the button is inactive. If not provided the active up image is used.
         * Existing Image is freed automatically, if it was loaded internally.
         *
         * @param filename The filename of the inactive up image to display.
         */
        void setInactiveUpImage(std::string const & filename);

        /**
         * Sets the up image to display if the button is inactive. If not provided the active up image is used.
         * Existing Image is freed automatically, if it was loaded internally.
         *
         * @param image The inactive up image to display.
         */
        void setInactiveUpImage(Image const * image);

        /**
         * Gets inactive up image.
         *
         * @return The inactive up image.
         */
        Image const * getInactiveUpImage() const;

        /**
         * Sets the down image to display if the button is inactive.
         * Existing Image is freed automatically, if it was loaded internally.
         *
         * @param filename The filename of the inactive down image to display.
         */
        void setInactiveDownImage(std::string const & filename);

        /**
         * Sets the down image to display if the button is inactive.
         * Existing Image is freed automatically, if it was loaded internally.
         *
         * @param image The inactive down image to display.
         */
        void setInactiveDownImage(Image const * image);

        /**
         * Gets inactive down image.
         *
         * @return The inactive down image.
         */
        Image const * getInactiveDownImage() const;

        /**
         * Sets the hover image to display if the button is inactive.
         * Existing Image is freed automatically, if it was loaded internally.
         *
         * @param filename The filename of the inactive hover image to display.
         */
        void setInactiveHoverImage(std::string const & filename);

        /**
         * Sets the hover image to display if the button is inactive.
         * Existing Image is freed automatically, if it was loaded internally.
         *
         * @param image The inactive hover image to display.
         */
        void setInactiveHoverImage(Image const * image);

        /**
         * Gets inactive hover image.
         *
         * @return The inactive hover image.
         */
        Image const * getInactiveHoverImage() const;

        // Inherited from Widget

        virtual void resizeToContent(/*bool recursion = true*/);
        virtual void adjustSize();
        virtual void draw(fcn::Graphics* graphics);

    protected:
        void setImage(std::string const & filename, ImageType type);
        void setImage(Image const * image, ImageType type);

        /**
         * The images to display.
         */
        std::vector<Image const *> mImages;

        /**
         * Owned images loaded by filename.
         */
        std::vector<std::unique_ptr<Image const>> mOwnedImages;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_IMAGEBUTTON_HPP_
