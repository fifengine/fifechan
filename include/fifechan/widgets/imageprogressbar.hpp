// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_IMAGEPROGRESSBAR_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_IMAGEPROGRESSBAR_HPP_

#include <fifechan/widget.hpp>

#include <string>
#include <vector>

namespace fcn
{
    class Graphics;
    class Image;

    /**
     * A widget that displays a progress bar using images.
     */
    class FIFEGUI_API ImageProgressBar : public Widget
    {
    public:
        /** Orientation of the progress bar (horizontal or vertical). */
        enum class Orientation : uint8_t
        {
            Horizontal = 0,
            Vertical
        };

        ImageProgressBar();

        /**
         * Constructor. The image passed is not owned by this object.
         *
         * @param image Image used by the progress bar.
         * @param maxValue Value when the progress bar is full.
         */
        ImageProgressBar(Image* image, int maxValue);

        /**
         * Constructor. The image indicated by filename is opened and it's
         * owned by this object.
         *
         * @param filename Filename of the image to be used by the progress bar.
         * @param maxValue Value when the progress bar is full.
         */
        ImageProgressBar(std::string const & filename, int maxValue);

        ~ImageProgressBar() override;

        /**
         * Draws this ImageProgressBar.
         */
        void draw(Graphics* graphics) override;

        /**
         * Sets the opacity of the ImageProgressBar.
         *
         * @param opaque True if opaque, false otherwise.
         */
        void setOpaque(bool opaque);

        /**
         * @return Whether this ImageProgressBar is opaque or not.
         */
        bool isOpaque() const;

        /**
         * Sets the Bar image. The image passed is not owned by
         * this object.
         *
         * @param image Image used by the progress bar.
         */
        void setBarImage(Image* image);

        /**
         * @return The image used by this Bar.
         */
        Image const * getBarImage() const;

        /**
         * Sets the foreground image. The image passed is not owned by
         * this object.
         *
         * @param image Image used as overlay by the progress bar.
         */
        void setForegroundImage(Image* image);

        /**
         * @return The image used as overlay by this Bar.
         */
        Image const * getForegroundImage() const;

        /**
         * Sets the ImageProgressBar's orientation.
         *
         * @param orientation Desired orientation.
         */
        void setOrientation(Orientation orientation);

        /**
         * @return Orientation of this ImageProgressBar.
         */
        Orientation getOrientation() const;

        /**
         * Gets the max value of the bar
         *
         * @return The max value of the bar.
         */
        int getMaxValue() const;

        /**
         * Sets the max value of the bar
         *
         * @param value The max value of the bar.
         */
        void setMaxValue(int value);

        /**
         * Gets the value of the bar
         *
         * @return The value of the bar.
         */
        int getValue() const;

        /**
         * Sets the value of the bar
         *
         * @param value In the range of 0-max.
         */
        void setValue(int value);

        // Inherited from Widget

        using Widget::resizeToContent;

        void resizeToContent(bool recursion) override;
        void adjustSize() override;

    protected:
        /**
         * Adjusts the size of the progress bar to fit the content.
         *
         * The public `adjustSize()` method serves as a virtual entry point
         * for polymorphism, while this `adjustSizeImpl()` method contains
         * the concrete implementation of the resizing logic.
         *
         * @see adjustSize (virtual entry point for polymorphism)
         */
        void adjustSizeImpl();

        /**
         * Bar image.
         */
        Image const * mBarImage;

        /**
         * Foreground image.
         */
        Image const * mForegroundImage;

        /**
         * True if the image has been loaded internally, false otherwise.
         * An image not loaded internally should not be deleted in the
         * destructor.
         */
        bool mInternalImage;

        /**
         * Value when progress bar is full.
         */
        int mMaxValue;

        /**
         * Holds the current progress bar value.
         */
        int mValue;

        /**
         * ImageProgressBar's orientation.
         */
        Orientation mOrientation;

        /**
         * True if the widget is opaque, false otherwise.
         */
        bool mOpaque;
    };
}; // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_IMAGEPROGRESSBAR_HPP_
