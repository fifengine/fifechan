// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_ICONPROGRESSBAR_HPP
#define FCN_ICONPROGRESSBAR_HPP

#include <fifechan/widget.hpp>

#include <vector>

namespace fcn
{
    class Graphics;
    class Image;

    class FCN_CORE_DECLSPEC IconProgressBar : public Widget
    {
    public:
        enum Orientation
        {
            HORIZONTAL = 0,
            VERTICAL
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

        /**
         * Destructor.
         */
        virtual ~IconProgressBar();

        /**
         * Draws this IconProgressBar.
         */
        virtual void draw(Graphics* graphics);

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
         * @param counter The count of the active icons.
         */
        void setIconCount(int icons);

        /**
         * @return Current count of icons for this IconProgressBar.
         */
        int getIconCount() const;

        // Inherited from Widget

        virtual void resizeToContent(bool recursiv = true);
        virtual void adjustSize();

    protected:
        /**
         * Image used by the progress bar.
         */
        Image const * mImage;

        /**
         * True if the image has been loaded internally, false otherwise.
         * An image not loaded internally should not be deleted in the
         * destructor.
         */
        bool mInternalImage;

        /**
         * Count of icons when progress bar is full.
         */
        int mMaxIcons;

        /**
         * Holds how many icons are currently displayed.
         */
        int mIconCounter;

        /**
         * IconProgressBar's orientation.
         */
        Orientation mOrientation;

        /**
         * True if the widget is opaque, false otherwise.
         */
        bool mOpaque;
    };
}; // namespace fcn

#endif // FCN_ICONPROGRESSBAR_HPP
