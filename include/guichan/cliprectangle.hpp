#ifndef GCN_CLIPRECTANGLE_HPP
#define GCN_CLIPRECTANGLE_HPP

#include "guichan/rectangle.hpp"

namespace gcn
{
  /**
   *
   */
  class ClipRectangle : public Rectangle
  {
  public:
    /**
     *
     */
    ClipRectangle();

    /**
     *
     */
    ClipRectangle(int x, int y, int width, int height, int yOffset, int xOffset);

    /**
     *
     */
    const ClipRectangle& ClipRectangle::operator=(const Rectangle& other);
    
    int xOffset;
    int yOffset;

  }; // end ClipRectangle

} // end gcn

#endif // end GCN_CLIPRECTANGLE_HPP
