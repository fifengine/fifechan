#include "guichan/graphics.hpp"

namespace gcn
{
  
  /**
   *
   */
  bool Graphics::pushClipArea(Rectangle area)
  {
    if (mClipStack.empty())
    {
      ClipRectangle carea;
      carea.x = area.x;
      carea.y = area.y;
      carea.width = area.width;
      carea.height = area.height;
      mClipStack.push(carea);
      return true;
    }

    ClipRectangle top = mClipStack.top();
    ClipRectangle carea;
    carea = area;
    carea.xOffset = top.x + carea.x;
    carea.yOffset = top.y + carea.y;
    carea.x += top.x;
    carea.y += top.y;

    bool result = carea.intersect(top);

    mClipStack.push(carea);

    return result;
  }

  /**
   *
   */
  void Graphics::popClipArea()
  {
    if (mClipStack.empty())
    {
      //TODO: Throw exception
      return;
    }
    
    mClipStack.pop();
  }

  void Graphics::setColor(const Color& color)
  {
    mColor = color;    
  } // end setColor
  
} // end gcn
