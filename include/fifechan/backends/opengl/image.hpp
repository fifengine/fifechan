// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_BACKENDS_OPENGL_IMAGE_HPP_
#define INCLUDE_FIFECHAN_BACKENDS_OPENGL_IMAGE_HPP_

#if defined(_WIN32)
    #include <windows.h>
#endif

#if defined(__APPLE__)
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
#endif

#include <span>
#include <string>
#include <vector>

#include "fifechan/color.hpp"
#include "fifechan/image.hpp"
#include "fifechan/platform.hpp"

namespace fcn::opengl
{
    /**
     * OpenGL implementation of Image.
     */
    class FIFEGUI_EXT_API Image : public fcn::Image
    {
    public:
        /**
         * Constructor. Loads an image from an array of pixels. The pixel array is
         * is copied in the constructor and should thus be freed after the constructor
         * has been called.
         *
         * NOTE: The functions getPixel and putPixel are only guaranteed to work
         *       before an image has been converted to display format.
         *
         * @param pixels pixel buffer to load from.
         * @param width the width of the image.
         * @param height the height of the image.
         * @param convertToDisplayFormat true if the image should be converted
         *                               to display, false otherwise.
         */
        Image(std::span<unsigned int const> pixels, int width, int height, bool convertToDisplayFormat = true);

        /**
         * Constructor. Load an image from an OpenGL texture handle. The width
         * and height specifies the size of the "interesting" part of the
         * texture, the real width and height of the texture are assumed to
         * be the closest higher power of two.
         *
         * @param textureHandle the texture handle from which to load.
         * @param width the width of the image.
         * @param height the height of the image.
         * @param autoFree true if the surface should automatically be deleted.
         */
        Image(GLuint textureHandle, int width, int height, bool autoFree);

        ~Image() override;

        /**
         * Gets the OpenGL texture handle for the image.
         *
         * @return the OpenGL texture handle for the image.
         */

        virtual GLuint getTextureHandle() const;

        /**
         * Gets the width of texture.
         *
         * @return the width of the texture.
         */
        virtual int getTextureWidth() const;

        /**
         * Gets the height of the texture.
         *
         * @return the height of the texture.
         */
        virtual int getTextureHeight() const;

        // Inherited from Image

        void free() override;

        int getWidth() const override;

        int getHeight() const override;

        Color getPixel(int x, int y) override;

        void putPixel(int x, int y, Color const & color) override;

        void convertToDisplayFormat() override;

    protected:
        /** OpenGL texture handle backing this image (if any). */
        GLuint mTextureHandle;

        /** Pixel buffer copy for the image (RGBA packed). */
        std::vector<unsigned int> mPixels;

        /** Whether the texture handle should be freed on destruction. */
        bool mAutoFree;

        /** Logical image width in pixels. */
        int mWidth;

        /** Logical image height in pixels. */
        int mHeight;

        /** Actual texture width (power-of-two) used by GL. */
        int mTextureWidth;

        /** Actual texture height (power-of-two) used by GL. */
        int mTextureHeight;
    };
} // namespace fcn::opengl

namespace fcn
{
    using OpenGLImage = fcn::opengl::Image;
}

#endif // INCLUDE_FIFECHAN_BACKENDS_OPENGL_IMAGE_HPP_
