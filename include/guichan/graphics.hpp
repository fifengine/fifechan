#ifndef GCN_GRAPHICS_HPP
#define GCN_GRAPHICS_HPP

#include <stack>
#include <string>
#include "guichan/image.hpp"
#include "guichan/cliprectangle.hpp"
#include "guichan/color.hpp"

namespace gcn
{
  /**
   * 
   */
  class Graphics
  {
  public:
//     /**
//      * @return the current Graphics width
//      */    
//     int getWidth(){ return mWidth; }

//     /**
//      * @return the current Graphics height
//      */
//     int getHeight(){ return mHeight; }

    /**
     * 
     */
    virtual bool pushClipArea(Rectangle area);
    /**
     * 
     */
    virtual void popClipArea();
    
     /**
      * 
      */
    virtual void drawImage(const Image* image, int srcY, int srcX,
                           int dstX, int dstY, int width,
                           int height) = 0;
//     /**
//      * 
//      */
//     virtual void drawImage(const Image& image, int dstX, int dstY) = 0;
    /**
     * 
     */
    virtual Image* loadImage(const std::string& filename) = 0;
    
    /**
     * 
     */
    virtual void freeImage(const std::string& filename) = 0;
    
    /**
     * 
     */
    virtual void freeImage(Image* image) = 0;

    /**
     * 
     */
    virtual void drawPoint(int x, int y) = 0;
    /**
     * 
     */
    virtual void drawLine(int x1, int y1, int x2, int y2) = 0;

    /**
     * 
     */
    virtual void drawHLine(int x1, int y, int x2) = 0;

    /**
     * 
     */
    virtual void drawVLine(int x, int y1, int y2) = 0;

    /**
     * 
     */
    virtual void drawRectangle(const Rectangle& rectangle) = 0;

    /**
     * 
     */
    virtual void fillRectangle(const Rectangle& rectangle) = 0;

    /**
     * 
     */
    virtual void setColor(const Color& color);
//     /**
//      * 
//      */
//     void setHorizontalGradient(const Color& color1, const Color& color2){}
//     /**
//      * 
//      */
//     void setVerticalGradient(const Color& color1, const Color& color2){}

//     /**
//      * 
//      */
//     virtual Font* loadFont(const std:string& filename) = 0;
//     /**
//      * 
//      */
//     virtual bool freeFont(const std::string& filename) = 0;
//     /**
//      * 
//      */
//     void setFont(const Font& font){}

//     /**
//      * 
//      */
//     virtual void drawText(const std::string& text, const int x, const int y) = 0;
//     /**
//      * 
//      */
//     virtual void drawTextCenter(const std::string& text, const int x, const int y) = 0;

//     /**
//      * 
//      */
//     void setBlender(const std::string blenderMode){}

  protected:
    std::stack<ClipRectangle> mClipStack;
    Color mColor;
    
  }; // end graphics
  
} // end gcn

#endif // end GCN_GRAPHICS_HPP
