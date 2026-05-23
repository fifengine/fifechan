// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_IMAGEFONT_HPP_
#define INCLUDE_FIFECHAN_IMAGEFONT_HPP_

// Standard library includes
#include <array>
#include <memory>
#include <string>

// Platform config include
#include "fifechan/platform.hpp"

// Project headers (subdirs before local)
#include "fifechan/color.hpp"
#include "fifechan/font.hpp"
#include "fifechan/rectangle.hpp"

namespace fcn
{
    class Color;
    class Graphics;
    class Image;

    /**
     * Defines a font implementation using an image atlas containing glyph data.
     *
     * Strategies:
     * - PixelAtOrigin: Use the color of the pixel at (0,0) as the separator.
     *   This is the legacy behavior and works for most font sheets that follow
     *   the convention of using the top-left pixel as a separator color.
     * - BorderDominant: Scan the edges of the image to find the most frequent RGB color,
     *   which is likely the separator. This is highly reliable for font sheets that
     *   use a solid background color as a separator, even if that color isn't at (0,0).
     * - ExplicitColor: Use a user-specified RGB color as the separator. This is useful for font sheets
     *   that don't follow conventions or have noisy edges, but requires manual configuration.
     * - Auto: Try PixelAtOrigin first; if the number of detected glyphs is significantly lower
     *   than expected, automatically fall back to BorderDominant.
     */
    enum class SeparatorStrategy : std::uint8_t
    {
        PixelAtOrigin,
        BorderDominant,
        ExplicitColor,
        Auto
    };

    /**
     * Configuration struct for ImageFont constructors.
     *
     * Strategy: The method used to determine the separator color in the font image.
     * ExplicitSeparator: The RGB color to use as a separator when using the ExplicitColor strategy (default magenta).
     * GlyphPadding: The number of pixels to ignore around detected glyphs when scanning (useful for anti-aliased
     * fonts). Verbose: If true, the constructor will print debug information about the scanning process to standard
     * error.
     */
    struct ImageFontConfig
    {
            /**
             * Strategy used to detect separator color in the image.
             */
            SeparatorStrategy strategy = SeparatorStrategy::Auto;

            /**
             * Explicit separator color used when `ExplicitColor` strategy is selected.
             */
            Color explicitSeparator = Color{255, 0, 255, 255};

            /**
             * Number of pixels to pad/ignore around detected glyphs.
             */
            int glyphPadding = 0;

            /**
             * If true, enable verbose debug output while scanning fonts.
             */
            bool verbose = false;
    };

    /**
     * A font implementation using an image atlas containing glyph data.
     *
     * ImageFont can be used with any image supported by the currently
     * used ImageLoader.
     *
     * These are two examples of an image containing a font.
     *  \image html fixedfont.bmp
     *  \image html rpgfont.png
     *
     * The first pixel at coordinate (0,0) tells which color the image font
     * looks for when separating glyphs. The glyphs in the image is provided
     * to the image font's constructor in the order they appear in the image.
     *
     * To create an ImageFont from the first image example above the following
     * constructor call should be made:
     * @code
     * fcn::ImageFont imageFont("fixedfont_big.bmp",
     *     " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
     * @endcode
     *
     * The first glyph defines the width of a space.
     *
     * Glyphs can span multiple lines using horizontal separators,
     * but these must be exactly 1 pixel high.
     *
     * Space glyph:
     * If included (e.g. as the first glyph), its width defines
     * the logical space width. Otherwise, ImageFont uses a default.
     *
     * Vertical separators:
     * Glyphs are divided by columns of a single color.
     * These may span multiple adjacent columns; any column where all pixels
     * match the separator color is treated as part of a separator.
     * This allows flexible spacing (e.g. wider gaps before punctuation).
     *
     * Horizontal separators:
     * Used to split glyph rows and must be exactly
     * 1 pixel high. Multi-pixel rows are not recognized and may break row
     * detection. If your atlas uses thicker dividers, convert them to 1-pixel
     * rows or use an explicit separator strategy.
     * Support for multi-pixel separators can be added if needed.
     *
     * @ingroup fonts
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
             * Constructor with configuration.
             *
             * @param filename The filename of the image.
             * @param glyphs The glyphs found in the image.
             * @param config Configuration for parsing the image font.
             */
            ImageFont(std::string const & filename, std::string const & glyphs, ImageFontConfig const & config);

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
             * Constructor from an image with configuration.
             * The image will be owned by the ImageFont instance.
             *
             * @param image The image with font glyphs.
             * @param glyphs The glyphs found in the image.
             * @param config Configuration for parsing the image font.
             */
            ImageFont(Image* image, std::string const & glyphs, ImageFontConfig const & config);

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
            explicit ImageFont(
                std::string const & filename, unsigned char glyphsFrom = 32, unsigned char glyphsTo = 126);
            // New constructor with configuration
            /**
             * Constructor defining glyph range with configuration.
             *
             * @param filename The filename of the image.
             * @param glyphsFrom ASCII of the first glyph (inclusive).
             * @param glyphsTo ASCII of the last glyph (inclusive).
             * @param config Configuration for parsing the image font.
             */
            ImageFont(
                std::string const & filename,
                unsigned char glyphsFrom,
                unsigned char glyphsTo,
                ImageFontConfig const & config);

            ~ImageFont() override;

            ImageFont(ImageFont const &)            = delete;
            ImageFont& operator=(ImageFont const &) = delete;
            ImageFont(ImageFont&&)                  = delete;
            ImageFont& operator=(ImageFont&&)       = delete;

            /**
             * Draws a glyph.
             *
             * @note You normally won't use this function to draw text since
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

            int getWidth(std::string_view text) const override;

            int getHeight() const override;

            int getStringIndexAt(std::string_view text, int x) const override;

            auto renderToSurface(std::string_view text) const
                -> std::unique_ptr<SDL_Surface, SDL_SurfaceDeleter> override;

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
            std::array<Rectangle, 256> mGlyph;

            /**
             * Holds the height of the image font.
             */
            int mHeight{0};

            /**
             * Holds the glyph spacing of the image font.
             */
            int mGlyphSpacing{0};

            /**
             * Holds the row spacing of the image font.
             */
            int mRowSpacing{0};

            /**
             * Holds the image with the font data.
             */
            Image* mImage{nullptr};

            /**
             * Holds the filename of the image with the font data.
             */
            std::string mFilename;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_IMAGEFONT_HPP_
