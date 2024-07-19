// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

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
    class FCN_CORE_DECLSPEC Icon : public Widget
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
        Icon(std::string const & filename);

        /**
         * Constructor.
         *
         * @param image The image to display.
         */
        Icon(Image const * image);

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
        void setImage(std::string const & filename);

        /**
         * Sets the image to display. Existing image is freed automatically
         * if it was loaded internally.
         *
         * @param image The image to display.
         */
        void setImage(Image const * image);

        /**
         * Gets the current image.
         *
         * @return The current image.
         */
        Image const * getImage() const;

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

        virtual void resizeToContent(bool recursiv = true);
        virtual void adjustSize();
        virtual void draw(Graphics* graphics);

    protected:
        /**
         * The image to display.
         */
        Image const * mImage;

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
} // namespace fcn

#endif // end FCN_ICON_HPP
