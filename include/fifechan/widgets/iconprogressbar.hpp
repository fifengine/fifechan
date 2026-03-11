// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_ICONPROGRESSBAR_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_ICONPROGRESSBAR_HPP_

#include <fifechan/widget.hpp>

#include <string>
#include <vector>

namespace fcn
{
    class Graphics;
    class Image;

    /**
     * A widget that displays a progress bar using icons.
     */
    class FIFEGUI_API IconProgressBar : public Widget
    {
    public:
        /** Orientation of the IconProgressBar (horizontal or vertical). */
        enum class Orientation : uint8_t
        {
            Horizontal = 0,
            Vertical
        };

        /**
         * Default constructor.
         */
        IconProgressBar();

        /**
         * Constructor. The image passed is not owned by this object.
         *
         * @param image Image used by the progress bar's icons.
         * @param maxIcons Count of icons when the progress bar is full.
         */
        IconProgressBar(Image* image, int maxIcons);

        /**
         * Constructor. The image indicated by filename is opened and it's
         * owned by this object.
         *
         * @param filename Filename of the image to be used by the progress bar's icons.
         * @param maxIcons Count of icons when the progress bar is full.
         */
        IconProgressBar(std::string const & filename, int maxIcons);

        ~IconProgressBar() override;

        /**
         * Draws this IconProgressBar.
         */
        void draw(Graphics* graphics) override;

        /**
         * Sets the opacity of the IconProgressBar.
         *
         * @param opaque True if opaque, false otherwise.
         */
        void setOpaque(bool opaque);

        /**
         * @return Whether this IconProgressBar is opaque or not.
         */
        bool isOpaque() const;

        /**
         * Sets the IconProgressBar's image. The image passed is not owned by
         * this object.
         *
         * @param image Image used by the progress bar's icons.
         * @see setMaxIcons
         */
        void setImage(Image* image);

        /**
         * @return The image used by this IconProgressBar.
         */
        Image const * getImage() const;

        /**
         * Sets count of icons when the progress bar is full.
         * Also creates enough icons to represent that.
         *
         * @param maxIcons desired maximum count of icons.
         */
        void setMaxIcons(int maxIcons);

        /**
         * @return Maximum count of icons for this IconProgressBar.
         */
        int getMaxIcons() const;

        /**
         * Sets the IconProgressBar's orientation.
         *
         * @param orientation Desired orientation.
         */
        void setOrientation(Orientation orientation);

        /**
         * @return Orientation of this IconProgressBar.
         */
        Orientation getOrientation() const;

        /**
         * Advances the progress bar to use one more icon. If there can be no advancement
         * (maxIcons is reached), then the counter is reset to 0, useful if client wishes
         * to make the IconProgressBar looping.
         */
        void advance();

        /**
         * Resets the progress bar.
         */
        void reset();

        /**
         * Sets count of icons.
         * @param icons The count of the active icons.
         */
        void setIconCount(int icons);

        /**
         * @return Current count of icons for this IconProgressBar.
         */
        int getIconCount() const;

        // Inherited from Widget

        using Widget::resizeToContent;

        void resizeToContent(bool recursion) override;
        void adjustSize() override;

    protected:
        /**
         * Adjusts the size of the IconProgressBar to fit the icons.
         *
         * The public `adjustSize()` method serves as a virtual entry point
         * for polymorphism, while this `adjustSizeImpl()` method contains
         * the concrete implementation of the resizing logic.
         *
         * @see adjustSize (virtual entry point for polymorphism)
         */
        void adjustSizeImpl();

        /**
         * Image used by the progress bar.
         */
        Image const * mImage;

        /**
         * True if the image has been loaded internally, false otherwise.
         * An image not loaded internally should not be deleted in the
         * destructor.
         */
        bool mInternalImage{false};

        /**
         * Count of icons when progress bar is full.
         */
        int mMaxIcons{0};

        /**
         * Holds how many icons are currently displayed.
         */
        int mIconCounter{0};

        /**
         * IconProgressBar's orientation.
         */
        Orientation mOrientation{Orientation::Horizontal};

        /**
         * True if the widget is opaque, false otherwise.
         */
        bool mOpaque{true};
    };
}; // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_ICONPROGRESSBAR_HPP_
