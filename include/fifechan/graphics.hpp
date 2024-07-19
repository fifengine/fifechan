// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_GRAPHICS_HPP
#define FCN_GRAPHICS_HPP

#include <iosfwd>
#include <stack>

#include "fifechan/cliprectangle.hpp"
#include "fifechan/platform.hpp"
#include "fifechan/point.hpp"

namespace fcn
{
    class Color;
    class Font;
    class Image;

    /**
     * Abstract class for providing drawing primitve functions.
     * It contains all vital functions for drawing.
     *
     * Guichan contains implementations of Graphics for common
     * libraries like the Allegro library, the HGE library,
     * the OpenGL library, the OpenLayer library, and the SDL library.
     * To make Guichan usable with other libraries, a Graphics class
     * must be implemented.
     *
     * In Graphics you can set clip areas to limit drawing to certain
     * areas of the screen. Clip areas are put on a stack, which
     * means that you can push smaller and smaller clip areas onto the
     * stack. All coordinates will be relative to the top most clip area.
     * In most cases you won't have to worry about the clip areas,
     * unless you want to implement some really complex widget.
     * Pushing and poping of clip areas are handled automatically by
     * container widgets when their child widgets are drawn.
     *
     * IMPORTANT: Remember to pop each clip area that you pushed on the stack
     * after you are done with it.
     *
     * If you feel that Graphics is to restrictive for your needs,
     * there is no one stopping you from using your own code for drawing
     * in widgets. You could for instance use pure SDL in the drawing of
     * widgets bypassing Graphics. This might however hurt portability of
     * your application.
     *
     * If you implement a Graphics class not present in Guichan we would
     * be very happy to add it to Guichan.
     *
     * @see AllegroGraphics, HGEGraphics, OpenLayerGraphics, OpenGLGraphics,
     *      SDLGraphics, Image
     */
    class FIFEGUI_API Graphics
    {
    public:
        /**
         * Alignments for text drawing.
         */
        enum Alignment
        {
            Left = 0,
            Center,
            Right
        };

        /**
         * Constructor.
         */
        Graphics();

        /**
         * Destructor.
         */
        virtual ~Graphics() = default;

        /**
         * Initializes drawing. Called by the Gui when Gui::draw() is called.
         * It is needed by some implementations of Graphics to perform
         * preparations before drawing. An example of such an implementation
         * is the OpenGLGraphics.
         *
         * NOTE: You will never need to call this function yourself, unless
         *       you use a Graphics object outside of Guichan.
         *
         * @see _endDraw, Gui::draw
         */
        virtual void _beginDraw() { }

        /**
         * Deinitializes drawing. Called by the Gui when a Gui::draw() is done.
         * done. It should reset any state changes made by _beginDraw().
         *
         * NOTE: You will never need to call this function yourself, unless
         *       you use a Graphics object outside of Guichan.
         *
         * @see _beginDraw, Gui::draw
         */
        virtual void _endDraw() { }

        /**
         * Pushes a clip area onto the stack. The x and y coordinates in the
         * rectangle is  relative to the last pushed clip area.
         * If the new area falls outside the current clip area, it will be
         * clipped as necessary.
         *
         * If a clip area is outside of the top clip area a clip area with
         * zero width and height will be pushed.
         *
         * @param area The clip area to be pushed onto the stack.
         * @return False if the the new area lays outside the current clip
         *         area.
         */
        virtual bool pushClipArea(Rectangle area);

        /**
         * Removes the top most clip area from the stack.
         *
         * @throws Exception if the stack is empty.
         */
        virtual void popClipArea();

        /**
         * Gets the current clip area. Usefull if you want to do drawing
         * bypassing Graphics.
         *
         * @return The current clip area.
         */
        virtual ClipRectangle const & getCurrentClipArea();

        /**
         * Draws a part of an image.
         *
         * NOTE: Width and height arguments will not scale the image but
         *       specifies the size of the part to be drawn. If you want
         *       to draw the whole image there is a simplified version of
         *       this function.
         *
         * EXAMPLE: @code drawImage(myImage, 10, 10, 20, 20, 40, 40); @endcode
         *          Will draw a rectangular piece of myImage starting at
         *          coordinate (10, 10) in myImage, with width and height 40.
         *          The piece will be drawn with it's top left corner at
         *          coordinate (20, 20).
         *
         * @param image The image to draw.
         * @param srcX The source image x coordinate.
         * @param srcY The source image y coordinate.
         * @param dstX The destination x coordinate.
         * @param dstY The destination y coordinate.
         * @param width The width of the piece.
         * @param height The height of the piece.
         */
        virtual void drawImage(Image const * image, int srcX, int srcY, int dstX, int dstY, int width, int height) = 0;
        /**
         * Draws an image. A simplified version of the other drawImage.
         * It will draw a whole image at the coordinate you specify.
         * It is equivalent to calling:
         * @code drawImage(myImage, 0, 0, dstX, dstY, image->getWidth(), \
         image->getHeight()); @endcode
         */
        virtual void drawImage(Image const * image, int dstX, int dstY);

        /**
         * Draws a single point/pixel.
         *
         * @param x The x coordinate.
         * @param y The y coordinate.
         */
        virtual void drawPoint(int x, int y) = 0;

        /**
         * Draws a line.
         *
         * @param x1 The first x coordinate.
         * @param y1 The first y coordinate.
         * @param x2 The second x coordinate.
         * @param y2 The second y coordinate.
         */
        virtual void drawLine(int x1, int y1, int x2, int y2) = 0;

        /**
         * Draws a thick line.
         *
         * @param x1    The first x coordinate.
         * @param y1    The first y coordinate.
         * @param x2    The second x coordinate.
         * @param y2    The second y coordinate.
         * @param width	The line width.
         */
        virtual void drawLine(int x1, int y1, int x2, int y2, unsigned int width) = 0;

        /**
         * Draws lines between points with given width.
         *
         * @param points Contains the points that are used for drawing.
         * @param width  The line width.
         */
        virtual void drawPolyLine(PointVector const & points, unsigned int width) = 0;

        /**
         * Draws a bezier curve.
         *
         * @param points The coordinates as points in a vector.
         * @param steps  The steps for each line between two points.
         * @param width  The line width.
         */
        virtual void drawBezier(PointVector const & points, int steps, unsigned int width) = 0;

        /**
         * Draws a simple, non-filled rectangle with a one pixel width.
         *
         * @param rectangle The rectangle to draw.
         */
        virtual void drawRectangle(Rectangle const & rectangle) = 0;

        /**
         * Draws a simple, non-filled rectangle with a one pixel width.
         *
         * This is an overload provided for convenience.
         *
         * @param x      The x coordinate of the rectangle
         * @param y      The y coordinate of the rectangle
         * @param width  The width of the rectangle
         * @param height The height of the rectangle
         *
         */
        inline void drawRectangle(int x, int y, int width, int height)
        {
            drawRectangle(Rectangle(x, y, width, height));
        }

        /**
         * Draws a filled rectangle.
         *
         * @param rectangle The filled rectangle to draw.
         */
        virtual void fillRectangle(Rectangle const & rectangle) = 0;

        /**
         * Draws a filled rectangle.
         *
         * This is an overload provided for convenience.
         *
         * @param x      The x coordinate of the rectangle
         * @param y      The y coordinate of the rectangle
         * @param width  The width of the rectangle
         * @param height The height of the rectangle
         *
         */
        inline void fillRectangle(int x, int y, int width, int height)
        {
            fillRectangle(Rectangle(x, y, width, height));
        }

        /**
         * Draws a simple, non-filled circle with a one pixel width.
         *
         * @param p      The circle center coordinate as point.
         * @param radius The circle radius.
         *
         */
        virtual void drawCircle(Point const & p, unsigned int radius) = 0;

        /**
         * Draws a filled circle.
         *
         * @param p      The circle center coordinate as point.
         * @param radius The circle radius.
         *
         */
        virtual void drawFillCircle(Point const & p, unsigned int radius) = 0;

        /**
         * Draws a simple, non-filled circle segment with a one pixel width.
         * Note: The start angle must be less than the end angle. 0 angle is right side.
         *
         * @param p      The circle center coordinate as point.
         * @param radius The circle radius.
         * @param sangle The start angle of the segment.
         * @param eangle The end angle of the segment.
         *
         */
        virtual void drawCircleSegment(Point const & p, unsigned int radius, int sangle, int eangle) = 0;

        /**
         * Draws a filled circle segment.
         * Note: The start angle must be less than the end angle. 0 angle is right side.
         *
         * @param p      The circle center coordinate as point.
         * @param radius The circle radius.
         * @param sangle The start angle of the segment.
         * @param eangle The end angle of the segment.
         *
         */
        virtual void drawFillCircleSegment(Point const & p, unsigned int radius, int sangle, int eangle) = 0;

        /**
         * Sets the color to use when drawing.
         *
         * @param color A color.
         * @see getColor
         */
        virtual void setColor(Color const & color) = 0;

        /**
         * Gets the color to use when drawing.
         *
         * @return The color used when drawing.
         * @see setColor
         */
        virtual Color const & getColor() const = 0;

        /**
         * Sets the font to use when drawing text.
         *
         * @param font The font to use when drawing.
         */
        virtual void setFont(Font* font);

        /**
         * Draws text.
         *
         * @param text The text to draw.
         * @param x The x coordinate where to draw the text.
         * @param y The y coordinate where to draw the text.
         * @param alignment The alignemnt to use when drawing.
         * @throws Exception when no font has been set.
         */
        virtual void drawText(std::string const & text, int x, int y, Alignment alignment = Left);

    protected:
        /**
         * Holds the clip area stack.
         */
        std::stack<ClipRectangle> mClipStack;

        /**
         * Holds the current font.
         */
        Font* mFont{nullptr};
    };
} // namespace fcn

#endif // end FCN_GRAPHICS_HPP
