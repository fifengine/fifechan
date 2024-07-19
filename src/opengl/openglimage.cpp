// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/opengl/openglimage.hpp"

#include "fifechan/exception.hpp"

namespace fcn
{
    OpenGLImage::OpenGLImage(unsigned int const * pixels, int width, int height, bool convertToDisplayFormat)
    {
        mAutoFree = true;

        mWidth        = width;
        mHeight       = height;
        mTextureWidth = 1, mTextureHeight = 1;

        while (mTextureWidth < mWidth) {
            mTextureWidth *= 2;
        }

        while (mTextureHeight < mHeight) {
            mTextureHeight *= 2;
        }

        // Create a new pixel array and copy the pixels into it
        mPixels = new unsigned int[mTextureWidth * mTextureHeight];

#ifdef __BIG_ENDIAN__
        unsigned int const magicPink = 0xff00ffff;
#else
        unsigned int const magicPink = 0xffff00ff;
#endif
        int x, y;
        for (y = 0; y < mTextureHeight; y++) {
            for (x = 0; x < mTextureWidth; x++) {
                if (x < mWidth && y < mHeight) {
                    unsigned int c = pixels[x + y * mWidth];

                    // Magic pink to transparent
                    if (c == magicPink) {
                        c = 0x00000000;
                    }

                    mPixels[x + y * mTextureWidth] = c;
                } else {
                    mPixels[x + y * mTextureWidth] = 0x00000000;
                }
            }
        }

        if (convertToDisplayFormat) {
            OpenGLImage::convertToDisplayFormat();
        }
    }

    OpenGLImage::OpenGLImage(GLuint textureHandle, int width, int height, bool autoFree)
    {
        mTextureHandle = textureHandle;
        mAutoFree      = autoFree;
        mPixels        = NULL;

        mWidth        = width;
        mHeight       = height;
        mTextureWidth = 1, mTextureHeight = 1;

        while (mTextureWidth < mWidth) {
            mTextureWidth *= 2;
        }

        while (mTextureHeight < mHeight) {
            mTextureHeight *= 2;
        }
    }

    OpenGLImage::~OpenGLImage()
    {
        if (mAutoFree) {
            free();
        }
    }

    GLuint OpenGLImage::getTextureHandle() const
    {
        return mTextureHandle;
    }

    int OpenGLImage::getTextureWidth() const
    {
        return mTextureWidth;
    }

    int OpenGLImage::getTextureHeight() const
    {
        return mTextureHeight;
    }

    void OpenGLImage::free()
    {
        if (mPixels == NULL) {
            glDeleteTextures(1, &mTextureHandle);
        } else {
            delete[] mPixels;
            mPixels = NULL;
        }
    }

    int OpenGLImage::getWidth() const
    {
        return mWidth;
    }

    int OpenGLImage::getHeight() const
    {
        return mHeight;
    }

    Color OpenGLImage::getPixel(int x, int y)
    {
        if (mPixels == NULL) {
            fcn::throwException(
                ("Image has been converted to display format"),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        if (x < 0 || x >= mWidth || y < 0 || y >= mHeight) {
            fcn::throwException(
                ("Coordinates outside of the image"), static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        unsigned int c = mPixels[x + y * mTextureWidth];

#ifdef __BIG_ENDIAN__
        unsigned char r = (unsigned char)((c >> 24) & 0xff);
        unsigned char g = (unsigned char)((c >> 16) & 0xff);
        unsigned char b = (unsigned char)((c >> 8) & 0xff);
        unsigned char a = (unsigned char)(c & 0xff);
#else
        unsigned char a = (unsigned char)((c >> 24) & 0xff);
        unsigned char b = (unsigned char)((c >> 16) & 0xff);
        unsigned char g = (unsigned char)((c >> 8) & 0xff);
        unsigned char r = (unsigned char)(c & 0xff);
#endif

        return Color(r, g, b, a);
    }

    void OpenGLImage::putPixel(int x, int y, Color const & color)
    {
        if (mPixels == NULL) {
            fcn::throwException(
                ("Image has been converted to display format"),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        if (x < 0 || x >= mWidth || y < 0 || y >= mHeight) {
            fcn::throwException(
                ("Coordinates outside of the image"), static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

#ifdef __BIG_ENDIAN__
        unsigned int c = color.a | color.b << 8 | color.g << 16 | color.r << 24;
#else
        unsigned int c = color.r | color.g << 8 | color.b << 16 | color.a << 24;
#endif

        mPixels[x + y * mTextureWidth] = c;
    }

    void OpenGLImage::convertToDisplayFormat()
    {
        if (mPixels == NULL) {
            fcn::throwException(
                ("Image has already been converted to display format"),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        glGenTextures(1, &mTextureHandle);
        glBindTexture(GL_TEXTURE_2D, mTextureHandle);

        glTexImage2D(GL_TEXTURE_2D, 0, 4, mTextureWidth, mTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mPixels);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        delete[] mPixels;
        mPixels = NULL;

        GLenum error = glGetError();
        if (error) {
            std::string errmsg;
            switch (error) {
            case GL_INVALID_ENUM:
                errmsg = "GL_INVALID_ENUM";
                break;

            case GL_INVALID_VALUE:
                errmsg = "GL_INVALID_VALUE";
                break;

            case GL_INVALID_OPERATION:
                errmsg = "GL_INVALID_OPERATION";
                break;

            case GL_STACK_OVERFLOW:
                errmsg = "GL_STACK_OVERFLOW";
                break;

            case GL_STACK_UNDERFLOW:
                errmsg = "GL_STACK_UNDERFLOW";
                break;

            case GL_OUT_OF_MEMORY:
                errmsg = "GL_OUT_OF_MEMORY";
                break;
            }

            fcn::throwException(
                (std::string("Unable to convert to OpenGL display format, glGetError said: ") + errmsg),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }
    }
} // namespace fcn
