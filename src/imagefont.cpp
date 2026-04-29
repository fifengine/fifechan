// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/imagefont.hpp"

// Standard library includes
#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// Platform config include
#include "fifechan/platform.hpp"

// Project headers (subdirs before local)
#include "fifechan/color.hpp"
#include "fifechan/exception.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/image.hpp"
#include "fifechan/rectangle.hpp"

namespace fcn
{
    /**
     * Find most frequent RGB color along image borders
     */
    static Color getBorderDominantColor(Image* img)
    {
        int const w = img->getWidth();
        int const h = img->getHeight();
        if (w <= 0 || h <= 0) {
            return Color{0, 0, 0, 255};
        }

        struct RGB
        {
            uint8_t r, g, b;
            bool operator<(RGB const & o) const
            {
                return r != o.r ? r < o.r : g != o.g ? g < o.g : b < o.b;
            }
        };
        std::map<RGB, int> freq;

        auto count = [&](int x, int y) {
            Color c = img->getPixel(x, y);
            ++freq[{c.r, c.g, c.b}];
        };

        for (int x = 0; x < w; ++x) {
            count(x, 0);
            count(x, h - 1);
        }
        for (int y = 1; y < h - 1; ++y) {
            count(0, y);
            count(w - 1, y);
        }

        RGB best     = {0, 0, 0};
        int maxCount = 0;
        for (auto const & p : freq) {
            if (p.second > maxCount) {
                maxCount = p.second;
                best     = p.first;
            }
        }
        return Color{best.r, best.g, best.b, static_cast<uint8_t>(255)};
    }

    static Color resolveSeparator(Image* img, ImageFontConfig const & cfg)
    {
        switch (cfg.strategy) {
        case SeparatorStrategy::ExplicitColor:
            return cfg.explicitSeparator;
        case SeparatorStrategy::BorderDominant:
            return getBorderDominantColor(img);
        case SeparatorStrategy::PixelAtOrigin:
            return img->getPixel(0, 0);
        case SeparatorStrategy::Auto:
        default:
            return img->getPixel(0, 0);
        }
    }

    static inline bool isSeparator(Color const & p, Color const & sep)
    {
        return p.r == sep.r && p.g == sep.g && p.b == sep.b;
    }

    static std::vector<Rectangle> scanGlyphs(
        Image* img, int expectedCount, Color const & sep, int padding, bool verbose)
    {
        int const w = img->getWidth();
        int const h = img->getHeight();
        std::vector<Rectangle> found;

        int startY      = 0;
        bool foundStart = false;
        for (int y = 0; y < h && !foundStart; ++y) {
            for (int x = 0; x < w; ++x) {
                if (!isSeparator(img->getPixel(x, y), sep)) {
                    startY     = y;
                    foundStart = true;
                    break;
                }
            }
        }
        if (!foundStart) {
            throwException("Image contains no glyph content");
        }

        int ycur         = startY;
        int maxRowHeight = 0;

        while (ycur < h && static_cast<int>(found.size()) < expectedCount) {
            int rowEnd = ycur;
            for (; rowEnd < h; ++rowEnd) {
                bool hasContent = false;
                for (int x = 0; x < w; ++x) {
                    if (!isSeparator(img->getPixel(x, rowEnd), sep)) {
                        hasContent = true;
                        break;
                    }
                }
                if (!hasContent) {
                    break;
                }
            }
            if (rowEnd == ycur) {
                break;
            }

            int const rowH = rowEnd - ycur;
            maxRowHeight   = std::max(maxRowHeight, rowH);

            auto isSepCol = [&](int col) {
                // Check top and bottom of column (matches fixedfont.bmp and rpgfont.png)
                return isSeparator(img->getPixel(col, ycur), sep) && isSeparator(img->getPixel(col, rowEnd - 1), sep);
            };

            int xcur = 0;
            while (xcur < w && static_cast<int>(found.size()) < expectedCount) {
                while (xcur < w && isSepCol(xcur)) {
                    ++xcur;
                }
                if (xcur >= w) {
                    break;
                }

                int const sx = xcur;
                while (xcur < w && !isSepCol(xcur)) {
                    ++xcur;
                }

                int width  = std::max(1, xcur - sx - (padding * 2));
                int height = std::max(1, rowH - (padding * 2));
                found.push_back({sx + padding, ycur + padding, width, height});
            }
            ycur = rowEnd + 1;
        }
        return found;
    }

    ImageFont::ImageFont(std::string const & filename, std::string const & glyphs, ImageFontConfig const & config) :
        mFilename(filename), mImage(Image::load(filename, false))
    {
        if (mImage == nullptr) {
            throwException(std::string("Failed to load image: ") + filename);
        }

        int expected = static_cast<int>(glyphs.size());
        Color sep    = resolveSeparator(mImage, config);

        auto found = scanGlyphs(mImage, expected, sep, config.glyphPadding, config.verbose);

        if (config.strategy == SeparatorStrategy::Auto && static_cast<int>(found.size()) < expected * 0.8) {
            sep   = getBorderDominantColor(mImage);
            found = scanGlyphs(mImage, expected, sep, config.glyphPadding, config.verbose);
        }

        if (static_cast<int>(found.size()) < expected) {
            std::ostringstream os;
            os << "Image " << mFilename << " is corrupt or uses wrong separator.\n"
               << "Expected: " << expected << " glyphs, Found: " << found.size() << "\n"
               << "Detected separator: R:" << static_cast<int>(sep.r) << " G:" << static_cast<int>(sep.g)
               << " B:" << static_cast<int>(sep.b) << "\n"
               << "Suggestion: Use ExplicitColor strategy with magenta (255,0,255)";
            throwException(os.str());
        }

        for (size_t i = 0; i < glyphs.size(); ++i) {
            mGlyph[static_cast<unsigned char>(glyphs[i])] = found[i];
        }

        if (config.verbose) {
            std::cerr << "[ImageFont] Loaded '" << mFilename << "' ExpectedGlyphs=" << expected
                      << " Found=" << found.size() << " Separator=R:" << static_cast<int>(sep.r)
                      << " G:" << static_cast<int>(sep.g) << " B:" << static_cast<int>(sep.b) << "\n";
            for (size_t i = 0; i < glyphs.size(); ++i) {
                unsigned char const c = static_cast<unsigned char>(glyphs[i]);
                Rectangle const & r   = mGlyph[c];
                std::cerr << "  glyph '" << glyphs[i] << "' (" << static_cast<int>(c) << ") -> x=" << r.x
                          << " y=" << r.y << " w=" << r.width << " h=" << r.height << "\n";
            }
        }

        mHeight = std::accumulate(found.begin(), found.end(), 0, [](int maxHeight, auto const & r) {
            return std::max(maxHeight, r.height);
        });

        mImage->convertToDisplayFormat();
        mRowSpacing   = 0;
        mGlyphSpacing = 0;
    }

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
            throwException("Corrupt image.");
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
            auto const k = static_cast<unsigned char>(glyph);
            mGlyph[k]    = scanForGlyph(k, x, y, separator);
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
            throwException("Font image is nullptr.");
        }
        mImage = image;

        Color const separator = mImage->getPixel(0, 0);

        int i = 0;
        for (i = 0; i < mImage->getWidth() && separator == mImage->getPixel(i, 0); ++i) { }

        if (i >= mImage->getWidth()) {
            throwException("Corrupt image.");
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

        for (i = 0; std::cmp_less(i, glyphs.size()); ++i) {
            unsigned char const glyph = glyphs.at(i);

            mGlyph[glyph] = scanForGlyph(glyph, x, y, separator);
            // Update x and y with new coordinates.
            x = mGlyph[glyph].x + mGlyph[glyph].width;
            y = mGlyph[glyph].y;
        }

        mImage->convertToDisplayFormat();

        mRowSpacing   = 0;
        mGlyphSpacing = 0;
    }

    ImageFont::ImageFont(Image* image, std::string const & glyphs, ImageFontConfig const & config) : mFilename("Image*")
    {
        if (image == nullptr) {
            throwException("Font image is nullptr.");
        }
        mImage = image;

        int expected = static_cast<int>(glyphs.size());
        Color sep    = resolveSeparator(mImage, config);

        auto found = scanGlyphs(mImage, expected, sep, config.glyphPadding, config.verbose);

        if (config.strategy == SeparatorStrategy::Auto && static_cast<int>(found.size()) < expected * 0.8) {
            sep   = getBorderDominantColor(mImage);
            found = scanGlyphs(mImage, expected, sep, config.glyphPadding, config.verbose);
        }

        if (static_cast<int>(found.size()) < expected) {
            std::ostringstream os;
            os << "Image " << mFilename << " is corrupt or uses wrong separator.\n"
               << "Expected: " << expected << " glyphs, Found: " << found.size() << "\n"
               << "Detected separator: R:" << static_cast<int>(sep.r) << " G:" << static_cast<int>(sep.g)
               << " B:" << static_cast<int>(sep.b) << "\n"
               << "Suggestion: Use ExplicitColor strategy with magenta (255,0,255)";
            throwException(os.str());
        }

        for (size_t i = 0; i < glyphs.size(); ++i) {
            mGlyph[static_cast<unsigned char>(glyphs[i])] = found[i];
        }

        mHeight = std::accumulate(found.begin(), found.end(), 0, [](int maxHeight, auto const & r) {
            return std::max(maxHeight, r.height);
        });

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
            throwException("Corrupt image.");
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

    ImageFont::ImageFont(
        std::string const & filename,
        unsigned char glyphsFrom,
        unsigned char glyphsTo,
        ImageFontConfig const & config) :
        mFilename(filename), mImage(Image::load(filename, false))
    {
        if (mImage == nullptr) {
            throwException(std::string("Failed to load image: ") + filename);
        }

        int expected = static_cast<int>(glyphsTo) - static_cast<int>(glyphsFrom) + 1;
        Color sep    = resolveSeparator(mImage, config);

        auto found = scanGlyphs(mImage, expected, sep, config.glyphPadding, config.verbose);

        if (config.strategy == SeparatorStrategy::Auto && static_cast<int>(found.size()) < expected * 0.8) {
            sep   = getBorderDominantColor(mImage);
            found = scanGlyphs(mImage, expected, sep, config.glyphPadding, config.verbose);
        }

        if (static_cast<int>(found.size()) < expected) {
            std::ostringstream os;
            os << "Image " << mFilename << " is corrupt or uses wrong separator.\n"
               << "Expected: " << expected << " glyphs, Found: " << found.size() << "\n"
               << "Detected separator: R:" << static_cast<int>(sep.r) << " G:" << static_cast<int>(sep.g)
               << " B:" << static_cast<int>(sep.b) << "\n"
               << "Suggestion: Use ExplicitColor strategy with magenta (255,0,255)";
            throwException(os.str());
        }

        for (int i = 0; i < expected; ++i) {
            unsigned char const glyph = static_cast<unsigned char>(static_cast<int>(glyphsFrom) + i);
            mGlyph[glyph]             = found[i];
        }

        mHeight = std::accumulate(found.begin(), found.end(), 0, [](int maxH, auto const & r) {
            return std::max(maxH, r.height);
        });

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
            return mGlyph[static_cast<int>((' '))].width + mGlyphSpacing;
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
            graphics->drawRectangle(
                x,
                y + 1 + yoffset,
                mGlyph[static_cast<int>((' '))].width - 1,
                mGlyph[static_cast<int>((' '))].height - 2);

            return mGlyph[static_cast<int>((' '))].width + mGlyphSpacing;
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

        // Find glyph start
        bool foundGlyphStart = false;

        while (!foundGlyphStart) {
            if (x >= mImage->getWidth()) {
                x = 0;
                y += mHeight + 1;

                if (y >= mImage->getHeight()) {
                    std::ostringstream os;
                    os << "Image " << mFilename << " with font is corrupt near character '" << glyph << "'";
                    throwException(os.str());
                }
            }

            color = mImage->getPixel(x, y);

            foundGlyphStart = (color != separator);

            if (!foundGlyphStart) {
                ++x;
            }
        }

        // Find glyph width
        int width          = 0;
        bool foundGlyphEnd = false;

        while (!foundGlyphEnd) {
            if (x + width >= mImage->getWidth()) {
                std::ostringstream os;
                os << "Image " << mFilename << " with font is corrupt near character '" << glyph << "'";
                throwException(os.str());
            }

            color = mImage->getPixel(x + width, y);

            foundGlyphEnd = (color == separator);

            if (!foundGlyphEnd) {
                ++width;
            }
        }

        // width now points to the separator pixel; glyph width is the measured width
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
