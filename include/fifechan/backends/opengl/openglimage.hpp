// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_BACKENDS_OPENGL_OPENGLIMAGE_HPP_
#define INCLUDE_FIFECHAN_BACKENDS_OPENGL_OPENGLIMAGE_HPP_

#if defined(_WIN32)
    #include <windows.h>
#endif

#if defined(__APPLE__)
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
#endif

#include <memory>
#include <string>

#include "fifechan/color.hpp"
#include "fifechan/image.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * OpenGL implementation of Image.
     */
    class FIFEGUI_EXT_API OpenGLImage : public Image
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
         * @param pixels to load from.
         * @param width the width of the image.
         * @param height the height of the image.
         * @param convertToDisplayFormat true if the image should be converted
         *                               to display, false otherwise.
         */
        OpenGLImage(unsigned int const * pixels, int width, int height, bool convertToDisplayFormat = true);

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
        OpenGLImage(GLuint textureHandle, int width, int height, bool autoFree);

        virtual ~OpenGLImage();

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

        virtual void free();

        virtual int getWidth() const;

        virtual int getHeight() const;

        virtual Color getPixel(int x, int y);

        virtual void putPixel(int x, int y, Color const & color);

        virtual void convertToDisplayFormat();

    protected:
        GLuint mTextureHandle;
        std::unique_ptr<unsigned int[]> mPixels;
        bool mAutoFree;
        int mWidth;
        int mHeight;
        int mTextureWidth;
        int mTextureHeight;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_BACKENDS_OPENGL_OPENGLIMAGE_HPP_
