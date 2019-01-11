/***************************************************************************
 *   Copyright (C) 2012-2019 by the fifechan team                               *
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

#ifndef FCN_IMAGEPROGRESSBAR_HPP
#define FCN_IMAGEPROGRESSBAR_HPP

#include <fifechan/widget.hpp>

#include <vector>

namespace fcn
{
    class Graphics;
    class Image;
    
    class FCN_CORE_DECLSPEC ImageProgressBar : public Widget
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
        ImageProgressBar(const std::string& filename, int maxValue);
        
        /**
         * Destructor.
         */
        virtual ~ImageProgressBar();

        /**
         * Draws this ImageProgressBar.
         */
        virtual void draw(Graphics* graphics);
        
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
        const Image* getBarImage() const;

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
        const Image* getForegroundImage() const;
         
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

        virtual void resizeToContent(bool recursiv=true);
        virtual void adjustSize();

    protected:
        
        /**
         * Bar image.
         */
        const Image* mBarImage;

        /**
         * Foreground image.
         */
        const Image* mForegroundImage;
        
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
};

#endif //FCN_IMAGEPROGRESSBAR_HPP
