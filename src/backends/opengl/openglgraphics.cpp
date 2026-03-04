// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include <fifechan/backends/opengl/openglgraphics.hpp>
#include <fifechan/backends/opengl/openglimage.hpp>
#include <fifechan/exception.hpp>
#include <fifechan/image.hpp>

#if defined(_WIN32)
    #include <windows.h>
#endif

#if defined(__amigaos4__)
    #include <mgl/gl.h>
    #define glVertex3i glVertex3f
#elif defined(__APPLE__)
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
#endif

namespace fcn
{
    OpenGLGraphics::OpenGLGraphics()
    {
        setTargetPlane(640, 480);
        mAlpha = false;
    }

    OpenGLGraphics::OpenGLGraphics(int width, int height)
    {
        setTargetPlane(width, height);
    }

    OpenGLGraphics::~OpenGLGraphics() { }

    void OpenGLGraphics::_beginDraw()
    {
        glPushAttrib(
            GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT | GL_FOG_BIT | GL_LIGHTING_BIT |
            GL_LINE_BIT | GL_POINT_BIT | GL_POLYGON_BIT | GL_SCISSOR_BIT | GL_STENCIL_BUFFER_BIT | GL_TEXTURE_BIT |
            GL_TRANSFORM_BIT);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glMatrixMode(GL_TEXTURE);
        glPushMatrix();
        glLoadIdentity();

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();

        glOrtho(0.0, static_cast<double>(mWidth), static_cast<double>(mHeight), 0.0, -1.0, 1.0);

        glDisable(GL_LIGHTING);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_TEXTURE_2D);

        glEnable(GL_SCISSOR_TEST);
        glPointSize(1.0);
        glLineWidth(1.0);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

        pushClipArea(Rectangle(0, 0, mWidth, mHeight));
    }

    void OpenGLGraphics::_endDraw()
    {
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        glMatrixMode(GL_TEXTURE);
        glPopMatrix();

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();

        glPopAttrib();

        popClipArea();
    }

    bool OpenGLGraphics::pushClipArea(Rectangle area)
    {
        bool const result = Graphics::pushClipArea(area);

        glScissor(
            mClipStack.top().x,
            mHeight - mClipStack.top().y - mClipStack.top().height,
            mClipStack.top().width,
            mClipStack.top().height);

        return result;
    }

    void OpenGLGraphics::popClipArea()
    {
        Graphics::popClipArea();

        if (mClipStack.empty()) {
            return;
        }

        glScissor(
            mClipStack.top().x,
            mHeight - mClipStack.top().y - mClipStack.top().height,
            mClipStack.top().width,
            mClipStack.top().height);
    }

    void OpenGLGraphics::setTargetPlane(int width, int height)
    {
        mWidth  = width;
        mHeight = height;
    }

    void OpenGLGraphics::drawImage(Image const * image, int srcX, int srcY, int dstX, int dstY, int width, int height)
    {
        OpenGLImage const * srcImage = dynamic_cast<OpenGLImage const *>(image);

        if (srcImage == nullptr) {
            throwException("Trying to draw an image of unknown format, must be an OpenGLImage.");
        }

        if (mClipStack.empty()) {
            throwException(
                "The clip stack is empty, perhaps you called a draw function outside of _beginDraw() and _endDraw()?");
        }

        ClipRectangle const & top = mClipStack.top();

        dstX += top.xOffset;
        dstY += top.yOffset;

        // Find OpenGL texture coordinates
        float const texX1 = srcX / static_cast<float>(srcImage->getTextureWidth());
        float const texY1 = srcY / static_cast<float>(srcImage->getTextureHeight());
        float const texX2 = (srcX + width) / static_cast<float>(srcImage->getTextureWidth());
        float const texY2 = (srcY + height) / static_cast<float>(srcImage->getTextureHeight());

        glBindTexture(GL_TEXTURE_2D, srcImage->getTextureHandle());

        glEnable(GL_TEXTURE_2D);

        // Check if blending already is enabled
        if (!mAlpha) {
            glEnable(GL_BLEND);
        }

        // Draw a textured quad -- the image
        glBegin(GL_QUADS);
        glTexCoord2f(texX1, texY1);
        glVertex3i(dstX, dstY, 0);

        glTexCoord2f(texX1, texY2);
        glVertex3i(dstX, dstY + height, 0);

        glTexCoord2f(texX2, texY2);
        glVertex3i(dstX + width, dstY + height, 0);

        glTexCoord2f(texX2, texY1);
        glVertex3i(dstX + width, dstY, 0);
        glEnd();
        glDisable(GL_TEXTURE_2D);

        // Don't disable blending if the color has alpha
        if (!mAlpha) {
            glDisable(GL_BLEND);
        }
    }

    void OpenGLGraphics::drawPoint(int x, int y)
    {
        if (mClipStack.empty()) {
            throwException(
                "The clip stack is empty, perhaps you called a draw function outside of _beginDraw() and _endDraw()?");
        }

        ClipRectangle const & top = mClipStack.top();

        x += top.xOffset;
        y += top.yOffset;

        glBegin(GL_POINTS);
        glVertex2i(x, y);
        glEnd();
    }

    void OpenGLGraphics::drawLine(int x1, int y1, int x2, int y2)
    {
        if (mClipStack.empty()) {
            throwException(
                "The clip stack is empty, perhaps you called a draw function outside of _beginDraw() and _endDraw()?");
        }

        ClipRectangle const & top = mClipStack.top();

        x1 += top.xOffset;
        y1 += top.yOffset;
        x2 += top.xOffset;
        y2 += top.yOffset;

        glBegin(GL_LINES);
        glVertex2f(x1 + 0.375F, y1 + 0.375F);
        glVertex2f(x2 + 1.0F - 0.375F, y2 + 1.0F - 0.375F);
        glEnd();

        glBegin(GL_POINTS);
        glVertex2f(x2 + 1.0F - 0.375F, y2 + 1.0F - 0.375F);
        glEnd();

        glBegin(GL_POINTS);
        glVertex2f(x1 + 0.375F, y1 + 0.375F);
        glEnd();
    }

    void OpenGLGraphics::drawLine(int x1, int y1, int x2, int y2, unsigned int width)
    {
        // TODO(jakoch): Implement this function
    }

    void OpenGLGraphics::drawPolyLine(PointVector const & points, unsigned int width)
    {
        // TODO(jakoch): Implement this function
    }

    void OpenGLGraphics::drawBezier(PointVector const & points, int steps, unsigned int width)
    {
        // TODO(jakoch): Implement this function
    }

    void OpenGLGraphics::drawRectangle(Rectangle const & rectangle)
    {
        if (mClipStack.empty()) {
            throwException(
                "The clip stack is empty, perhaps you called a draw function outside of _beginDraw() and _endDraw()?");
        }

        ClipRectangle const & top = mClipStack.top();

        glBegin(GL_LINE_LOOP);
        glVertex2f(rectangle.x + top.xOffset, rectangle.y + top.yOffset);
        glVertex2f(rectangle.x + rectangle.width + top.xOffset - 1.0F, rectangle.y + top.yOffset + 0.375F);
        glVertex2f(rectangle.x + rectangle.width + top.xOffset - 1.0F, rectangle.y + rectangle.height + top.yOffset);
        glVertex2f(rectangle.x + top.xOffset, rectangle.y + rectangle.height + top.yOffset);
        glEnd();
    }

    void OpenGLGraphics::fillRectangle(Rectangle const & rectangle)
    {
        if (mClipStack.empty()) {
            throwException(
                    "The clip stack is empty, perhaps you called a draw function outside of _beginDraw() and _endDraw()?",
        }

        ClipRectangle const & top = mClipStack.top();

        glBegin(GL_QUADS);
        glVertex2i(rectangle.x + top.xOffset, rectangle.y + top.yOffset);
        glVertex2i(rectangle.x + rectangle.width + top.xOffset, rectangle.y + top.yOffset);
        glVertex2i(rectangle.x + rectangle.width + top.xOffset, rectangle.y + rectangle.height + top.yOffset);
        glVertex2i(rectangle.x + top.xOffset, rectangle.y + rectangle.height + top.yOffset);
        glEnd();
    }

    void OpenGLGraphics::drawCircle(Point const & p, unsigned int radius)
    {
        // TODO(jakoch): Implement this function
    }

    void OpenGLGraphics::drawFillCircle(Point const & p, unsigned int radius)
    {
        // TODO(jakoch): Implement this function
    }

    void OpenGLGraphics::drawCircleSegment(Point const & p, unsigned int radius, int sangle, int eangle)
    {
        // TODO(jakoch): Implement this function
    }

    void OpenGLGraphics::drawFillCircleSegment(Point const & p, unsigned int radius, int sangle, int eangle)
    {
        // TODO(jakoch): Implement this method.
    }

    void OpenGLGraphics::setColor(Color const & color)
    {
        mColor = color;
        glColor4ub((GLubyte)color.r, (GLubyte)color.g, (GLubyte)color.b, (GLubyte)color.a);

        mAlpha = color.a != 255;

        if (mAlpha) {
            glEnable(GL_BLEND);
        }
    }

    Color const & OpenGLGraphics::getColor() const
    {
        return mColor;
    }

    int OpenGLGraphics::getTargetPlaneWidth() const
    {
        return mWidth;
    }

    int OpenGLGraphics::getTargetPlaneHeight() const
    {
        return mHeight;
    }
} // namespace fcn
