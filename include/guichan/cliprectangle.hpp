#ifndef GCN_CLIPRECTANGLE_HPP
#define GCN_CLIPRECTANGLE_HPP

#include "guichan/rectangle.hpp"

namespace gcn
{
  /**
   * ClipRectangle is used internally by the graphics
   * object. It is a regular Rectangle, extended with
   * variables xOffset and yOffset.
   */
  class ClipRectangle : public Rectangle
  {
  public:
    
    /**
     * Constructor
     */
    ClipRectangle();

    /**
     * Constructor
     *
     * @param x the rectangle x coordinate
     * @param y the rectangle y coordinate
     * @param width the rectangle width
     * @param height the rectangle height
     * @param xOffset origin of drawing (used by the Graphics object)
     * @param yOffset origin of drawing (used by the Graphics object)     
     */
    ClipRectangle(int x, int y, int width, int height, int xOffset, int yOffset);

    /**
     * Copies x, y, width and height field from a Rectangle.
     *
     * @param other the Rectangle to copy from
     * @returns a reference to this ClipRectangle
     */
    const ClipRectangle& operator=(const Rectangle& other);

    /**
     * x-origin of drawing (used by the Graphics object)
     */
    int xOffset;

    /**
     * y-origin of drawing (used by the Graphics object)
     */
    int yOffset;
  }; // end ClipRectangle

} // end gcn

#endif // end GCN_CLIPRECTANGLE_HPP
