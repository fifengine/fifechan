#include "guichan/rectangle.hpp"

namespace gcn
{
  Rectangle::Rectangle()
  {
    x = 0;
    y = 0;
    width = 0;
    height = 0;
  }
    
  Rectangle::Rectangle(int x, int y, int width, int height)
  {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
  }

  void Rectangle::setAll(int x, int y, int width, int height)
  {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
  }

  bool Rectangle::intersect(const Rectangle& rectangle)
  {
    x -= rectangle.x;
    y -= rectangle.y;
    
    if (x < 0)
    {        
      width += x;
      x = 0;
    }

    if (y < 0)
    {      
      height += y;
      y = 0;
    }

    if (x + width > rectangle.width)
    {
      width = rectangle.width - x;
    }

    if (y + height > rectangle.height)
    {
      height = rectangle.height - y;
    }

    if (width <= 0 || height <= 0)
    {
      height = 0;
      width = 0;
      x += rectangle.x;
      y += rectangle.y;
      return false;
    }

    x += rectangle.x;
    y += rectangle.y;
    return true;
  }

  bool Rectangle::isPointInRect(int x, int y)
  {
    return ((x >= this->x) && (y >= this->y)
            && (x <= this->x + this->width)
            && (y <= this->y + this->height));
  }
  
}
