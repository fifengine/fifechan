// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_IMAGEFONT_HPP
#define FCN_IMAGEFONT_HPP

#include <string>
#include <array>

#include "fifechan/font.hpp"
#include "fifechan/platform.hpp"
#include "fifechan/rectangle.hpp"

namespace fcn
{
    class Color;
    class Graphics;
    class Image;

    /**
     * A font using an image containing the font data. ImageFont can be used
     * with any image supported by the currently used ImageLoader.
     *
     * These are two examples of an image containing a font.
     *  \image html imagefontexample.bmp
     *  \image html imagefontexample2.bmp
     *
     * The first pixel at coordinate (0,0) tells which color the image font
     * looks for when seperating glyphs. The glyphs in the image is provided
     * to the image font's constructor in the order they appear in the image.
     *
     * To create an ImageFont from the first image example above the following
     * constructor call should be made:
     * @code fcn::ImageFont imageFont("fixedfont_big.bmp"," abcdefghijklmno\
pqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"); @endcode
     *
     * Noteworthy is that the first glyph actually gives the width of space.
     * Glyphs can, as seen in the second image example above, be seperated with
     * horizontal lines making it possible to draw glyphs on more then one
     * line in the image. However, these horizontal lines must have a height of
     * one pixel!
     */
    class FIFEGUI_API ImageFont : public Font
    {
    public:
        /**
         * Constructor. Takes an image file containing the font and
         * a string containing the glyphs. The glyphs in the string should
         * be in the same order as they appear in the font image.
         *
         * @param filename The filename of the image.
         * @param glyphs The glyphs found in the image.
         * @throws Exception when glyph list is incorrect or the font file is
         *                   corrupt or if no ImageLoader exists.
         */
        ImageFont(std::string const & filename, std::string const & glyphs);

        /**
         * Constructor. Takes an image containing the font and
         * a string containing the glyphs. The glyphs in the string should
         * be in the same order as they appear in the font image.
         * The image will be deleted in the destructor.
         *
         * @param image The image with font glyphs.
         * @param glyphs The glyphs found in the image.
         * @throws Exception when glyph list is incorrect or the font image is
         *                   is missing.
         */
        ImageFont(Image* image, std::string const & glyphs);

        /**
         * Constructor. Takes an image file containing the font and
         * two boundaries of ASCII values. The font image should include
         * all glyphs specified with the boundaries in increasing ASCII
         * order. The boundaries are inclusive.
         *
         * @param filename The filename of the image.
         * @param glyphsFrom The ASCII value of the first glyph found in the
         *                   image.
         * @param glyphsTo The ASCII value of the last glyph found in the
         *                 image.
         * @throws Exception when glyph bondaries are incorrect or the font
         *                   file is corrupt or if no ImageLoader exists.
         */
        explicit ImageFont(std::string const & filename, unsigned char glyphsFrom = 32, unsigned char glyphsTo = 126);

        /**
         * Destructor.
         */
        ~ImageFont() override;

        /**
         * Draws a glyph.
         *
         * NOTE: You normally won't use this function to draw text since
         *       the Graphics class contains better functions for drawing
         *       text.
         *
         * @param graphics A graphics object used for drawing.
         * @param glyph A glyph to draw.
         * @param x The x coordinate where to draw the glyph.
         * @param y The y coordinate where to draw the glyph.
         * @return The width of the glyph in pixels.
         */
        virtual int drawGlyph(Graphics* graphics, unsigned char glyph, int x, int y);

        /**
         * Sets the space between rows in pixels. Default is 0 pixels.
         * The space can be negative.
         *
         * @param spacing The space between rows in pixels.
         * @see getRowSpacing
         */
        virtual void setRowSpacing(int spacing);

        /**
         * Gets the space between rows in pixels.
         *
         * @return The space between rows in pixels.
         * @see setRowSpacing
         */
        virtual int getRowSpacing();

        /**
         * Sets the spacing between glyphs in pixels. Default is 0 pixels.
         * The space can be negative.
         *
         * @param spacing The glyph space in pixels.
         * @see getGlyphSpacing
         */
        virtual void setGlyphSpacing(int spacing);

        /**
         * Gets the spacing between letters in pixels.
         *
         * @return the spacing.
         * @see setGlyphSpacing
         */
        virtual int getGlyphSpacing();

        /**
         * Gets a width of a glyph in pixels.
         *
         * @param glyph The glyph which width will be returned.
         * @return The width of a glyph in pixels.
         */
        virtual int getWidth(unsigned char glyph) const;

        // Inherited from Font

        int getWidth(std::string const & text) const override;

        void drawString(Graphics* graphics, std::string const & text, int x, int y) override;

        int getHeight() const override;

        int getStringIndexAt(std::string const & text, int x) const override;

    protected:
        /**
         * Scans for a certain glyph.
         *
         * @param glyph The glyph to scan for. Used for exception messages.
         * @param x The x coordinate where to begin the scan. The coordinate
         *          will be updated with the end x coordinate of the glyph
         *          when the scan is complete.
         * @param y The y coordinate where to begin the scan. The coordinate
         *          will be updated with the end y coordinate of the glyph
         *          when the scan is complete.
         * @param separator The color separator to look for where the glyph ends.
         * @return A rectangle with the found glyph dimension in the image
         *         with the font.
         * @throws Exception when no glyph is found.
         */
        Rectangle scanForGlyph(unsigned char glyph, int x, int y, Color const & separator);

        /**
         * Holds the glyphs areas in the image.
         */
        Rectangle mGlyph[256];

        /**
         * Holds the height of the image font.
         */
        int mHeight;

        /**
         * Holds the glyph spacing of the image font.
         */
        int mGlyphSpacing;

        /**
         * Holds the row spacing of the image font.
         */
        int mRowSpacing;

        /**
         * Holds the image with the font data.
         */
        Image* mImage;

        /**
         * Holds the filename of the image with the font data.
         */
        std::string mFilename;
    };
} // namespace fcn

#endif // end FCN_IMAGEFONT_HPP
