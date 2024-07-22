// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/imagefont.hpp"

#include <sstream>
#include <string>

#include "fifechan/color.hpp"
#include "fifechan/exception.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/image.hpp"
#include "fifechan/rectangle.hpp"

namespace fcn
{
    ImageFont::ImageFont(std::string const & filename, std::string const & glyphs) :
        mFilename(filename), mImage(Image::load(filename, false))
    {

        Color const separator = mImage->getPixel(0, 0);

        // Find the starting point for glyphs in the image
        int startColumn = 0;
        for (; startColumn < mImage->getWidth(); ++startColumn) {
            if (separator != mImage->getPixel(startColumn, 0)) {
                break;
            }
        }

        // Check for corrupt image (all pixels are separator color)
        if (startColumn >= mImage->getWidth()) {
            fcn::throwException("Corrupt image.", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        // Find the height of glyphs
        int height = 0;
        for (int j = 0; j < mImage->getHeight(); ++j) {
            if (separator == mImage->getPixel(startColumn, j)) {
                break;
            }
            ++height;
        }

        mHeight = height;

        int x = 0;
        int y = 0;

        // Scan for all glyphs
        for (char const glyph : glyphs) {
            unsigned char const k = static_cast<unsigned char>(glyph);
            mGlyph[k]             = scanForGlyph(k, x, y, separator);
            // Update x and y with new coordinates.
            x = mGlyph[k].x + mGlyph[k].width;
            y = mGlyph[k].y;
        }

        mImage->convertToDisplayFormat();

        mRowSpacing   = 0;
        mGlyphSpacing = 0;
    }

    ImageFont::ImageFont(Image* image, std::string const & glyphs) : mFilename("Image*")
    {

        if (image == nullptr) {
            fcn::throwException("Font image is NULL", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }
        mImage = image;

        Color const separator = mImage->getPixel(0, 0);

        int i = 0;
        for (i = 0; i < mImage->getWidth() && separator == mImage->getPixel(i, 0); ++i) { }

        if (i >= mImage->getWidth()) {
            fcn::throwException("Corrupt image.", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        int j = 0;
        for (j = 0; j < mImage->getHeight(); ++j) {
            if (separator == mImage->getPixel(i, j)) {
                break;
            }
        }

        mHeight             = j;
        int x               = 0;
        int y               = 0;
        unsigned char glyph = 0;

        for (i = 0; i < (int)glyphs.size(); ++i) {
            glyph = glyphs.at(i);

            mGlyph[glyph] = scanForGlyph(glyph, x, y, separator);
            // Update x and y with new coordinates.
            x = mGlyph[glyph].x + mGlyph[glyph].width;
            y = mGlyph[glyph].y;
        }

        mImage->convertToDisplayFormat();

        mRowSpacing   = 0;
        mGlyphSpacing = 0;
    }

    ImageFont::ImageFont(std::string const & filename, unsigned char glyphsFrom, unsigned char glyphsTo) :
        mFilename(filename), mImage(Image::load(filename, false))
    {

        Color const separator = mImage->getPixel(0, 0);

        int i = 0;
        for (i = 0; separator == mImage->getPixel(i, 0) && i < mImage->getWidth(); ++i) { }

        if (i >= mImage->getWidth()) {
            fcn::throwException("Corrupt image.", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        int j = 0;
        for (j = 0; j < mImage->getHeight(); ++j) {
            if (separator == mImage->getPixel(i, j)) {
                break;
            }
        }

        mHeight = j;
        int x   = 0;
        int y   = 0;

        for (i = glyphsFrom; i < glyphsTo + 1; i++) {
            mGlyph[i] = scanForGlyph(i, x, y, separator);
            // Update x och y with new coordinates.
            x = mGlyph[i].x + mGlyph[i].width;
            y = mGlyph[i].y;
        }

        mImage->convertToDisplayFormat();

        mRowSpacing   = 0;
        mGlyphSpacing = 0;
    }

    ImageFont::~ImageFont()
    {
        delete mImage;
    }

    int ImageFont::getWidth(unsigned char glyph) const
    {
        if (mGlyph[glyph].width == 0) {
            return mGlyph[(int)(' ')].width + mGlyphSpacing;
        }

        return mGlyph[glyph].width + mGlyphSpacing;
    }

    int ImageFont::getHeight() const
    {
        return mHeight + mRowSpacing;
    }

    int ImageFont::drawGlyph(Graphics* graphics, unsigned char glyph, int x, int y)
    {
        // This is needed for drawing the glyph in the middle
        // if we have spacing.
        int const yoffset = getRowSpacing() / 2;

        if (mGlyph[glyph].width == 0) {
            graphics->drawRectangle(x, y + 1 + yoffset, mGlyph[(int)(' ')].width - 1, mGlyph[(int)(' ')].height - 2);

            return mGlyph[(int)(' ')].width + mGlyphSpacing;
        }

        graphics->drawImage(
            mImage, mGlyph[glyph].x, mGlyph[glyph].y, x, y + yoffset, mGlyph[glyph].width, mGlyph[glyph].height);

        return mGlyph[glyph].width + mGlyphSpacing;
    }

    void ImageFont::drawString(Graphics* graphics, std::string const & text, int x, int y)
    {
        for (char const c : text) {
            drawGlyph(graphics, c, x, y);
            x += getWidth(c);
        }
    }

    void ImageFont::setRowSpacing(int spacing)
    {
        mRowSpacing = spacing;
    }

    int ImageFont::getRowSpacing()
    {
        return mRowSpacing;
    }

    void ImageFont::setGlyphSpacing(int spacing)
    {
        mGlyphSpacing = spacing;
    }

    int ImageFont::getGlyphSpacing()
    {
        return mGlyphSpacing;
    }

    Rectangle ImageFont::scanForGlyph(unsigned char glyph, int x, int y, Color const & separator)
    {
        Color color;
        bool foundGlyphStart = false;

        // Finding the start of the glyph
        for (; !foundGlyphStart; ++x) {
            if (x >= mImage->getWidth()) {
                y += mHeight + 1;
                x = 0;
                if (y >= mImage->getHeight()) {
                    std::ostringstream os;
                    os << "Image " << mFilename << " with font is corrupt near character '" << glyph << "'";
                    fcn::throwException(os.str(), static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
                }
            }
            color = mImage->getPixel(x, y);
            if (color != separator) {
                foundGlyphStart = true;
            }
        }

        int width          = 0;
        bool foundGlyphEnd = false;

        // Finding the width of the glyph
        for (; !foundGlyphEnd; ++width) {
            if (x + width >= mImage->getWidth()) {
                std::ostringstream os;
                os << "Image " << mFilename << " with font is corrupt near character '" << glyph << "'";
                fcn::throwException(os.str(), static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
            }
            color = mImage->getPixel(x + width, y);
            if (color == separator) {
                foundGlyphEnd = true;
            }
        }

        return {x, y, width, mHeight};
    }

    int ImageFont::getWidth(std::string const & text) const
    {
        unsigned int i = 0;
        int size       = 0;

        for (i = 0; i < text.size(); ++i) {
            size += getWidth(text.at(i));
        }

        return size - mGlyphSpacing;
    }

    int ImageFont::getStringIndexAt(std::string const & text, int x) const
    {
        unsigned int i = 0;
        int size       = 0;

        for (i = 0; i < text.size(); ++i) {
            size += getWidth(text.at(i));

            if (size > x) {
                return i;
            }
        }

        return text.size();
    }
} // namespace fcn
