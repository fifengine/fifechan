#ifndef GCN_XGRAPHICS_HPP
#define GCN_XGRAPHICS_HPP

#include <string>
#include <map>
#include <X11/Xlib.h>
#include "guichan/graphics.hpp"
#include "guichan/x/ximage.hpp"
#include "guichan/rectangle.hpp"

namespace gcn
{
  class XGraphics : public Graphics
  {
  public:

    /**
     *
     */
    virtual void setTarget(Display* display, Window window, GC graphicsContext);

    /**
     *
     */
    //virtual SDL_Surface* getTarget() const;
    
    /**
     * 
     */
    virtual Image* loadImage(const std::string& filename);
    
    /**
     * 
     */
    virtual void freeImage(const std::string& filename);

    /**
     * 
     */
    virtual void freeImage(Image* image){}
    
    virtual bool pushClipArea(Rectangle area);

    virtual void popClipArea();

    /**
     * 
     */
    virtual void drawImage(const Image* image, int srcX, int srcY,
                           int dstX, int dstY, int width,
                           int height);
    /**
     * 
     */
    virtual void drawImage(const Image* image, int dstX, int dstY){}
    
    /**
     * 
     */
    virtual void drawPoint(int x, int y){}
    
    /**
     * 
     */
    virtual void drawLine(int x1, int y1, int x2, int y2);

    /**
     * 
     */
    virtual void drawHLine(int x1, int y, int x2);

    /**
     * 
     */
    virtual void drawVLine(int x, int y1, int y2);

    /**
     * 
     */
    virtual void drawRectangle(const Rectangle& rectangle);

    virtual void setColor(const Color& color);
    
    /**
     * 
     */
    void fillRectangle(const Rectangle& rectangle){}
//     /**
//      * 
//      */
//     Font* loadFont(const std:string& filename);
//     /**
//      * 
//      */
//     bool freeFont(const std::string& filename);
//     /**
//      * 
//      */
//     void drawText(const std::string& text, int x, int y);
//     /**
//      * 
//      */
//     void drawTextCenter(const std::string& text, int x, int y);
    
  protected:
    typedef std::pair<XImage*, int> imageRefCount; 
    typedef std::map<std::string, imageRefCount> imageMap;  
    imageMap mImages;
    Display* mDisplay;
    Window mWindow;
    GC mGraphicsContext;
  }; // end XGraphics
  
} // end gcn

#endif // end GCN_XGRAPHICS_HPP
