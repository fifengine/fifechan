#include "guichan/x/xgraphics.hpp"
#include <X11/Xutil.h>

namespace gcn
{
  /**
   *
   */
  void XGraphics::setTarget(Display* display, Window window, GC graphicsContext)
  {
    mDisplay = display;
    mWindow = window;

    XWindowAttributes windowAttributes;
    XGetWindowAttributes(display, window, &windowAttributes);

    Rectangle area;
    area.x = 0;
    area.y = 0;
    area.width = windowAttributes.width;
    area.height = windowAttributes.height;

    Graphics::pushClipArea(area);
    
    mGraphicsContext = graphicsContext;
    
  } // end setTarget


  /**
   *
   */
   bool XGraphics::pushClipArea(Rectangle area)
   {
     XRectangle rec;
     bool result = Graphics::pushClipArea(area);

     ClipRectangle carea = mClipStack.top();
     rec.x = area.x;
     rec.y = area.y;
     rec.width = area.width;
     rec.height = area.height;

     XRectangle rectangles[1];
     rectangles[0] = rec;

     XSetClipRectangles(mDisplay, mGraphicsContext, 0, 0, rectangles, 1, Unsorted);
     
     return result;
     
   } // end pushClipArea

  /**
   *
   */
   void XGraphics::popClipArea()
   {
     XRectangle rec;
     
     Graphics::popClipArea();

     ClipRectangle carea = mClipStack.top();
     rec.x = carea.x;
     rec.y = carea.y;
     rec.width = carea.width;
     rec.height = carea.height;

     XRectangle rectangles[1];
     rectangles[0] = rec;
     
     XSetClipRectangles(mDisplay, mGraphicsContext, 0, 0, rectangles, 1, Unsorted);

   } // end popClipArea 
  
  /**
   *
   */
//   SDL_Surface* SDLGraphics::getTarget() const
//   {
//     return mTarget;
//   }
  
  /**
   * loadImage
   * This function loads an image. If it's the first time the
   * image is loaded memory will be allocated and the image
   * will be added to a list of images.
   * If it has been loaded before memory will not be allocated,
   * the function will simply return a pointer to the already
   * loaded image. The function does this to save memory.
   * Normaly you do not want to allocate memory more the once for
   * the same image.
   * 
   * @param filname the filename of the image
   * @return a pointer to an image
   */
   Image* XGraphics::loadImage(const std::string& filename)
   {
     if (mImages.find(filename) == mImages.end())
     {
       unsigned int width;
       unsigned int height;
       int hotspotx;
       int hotspoty;
       Pixmap temp;
       int rc = XReadBitmapFile(mDisplay, mWindow,
                                filename.c_str(),
                                &width, &height,
                                &temp,
                                &hotspotx, &hotspoty);
       
       XImage* image = new XImage(temp, filename, width, height);
       mImages[filename] = imageRefCount(image,0);

       switch (rc)
       {
         case BitmapOpenFailed:
           // TODO
           // Add error
           break;
         case BitmapFileInvalid:
           // TODO
           // Add error
           break;
         case BitmapNoMemory:
           // TODO
           // Add error
           break;
         case BitmapSuccess:
           break;
       }
     }
     
     mImages[filename].second++;
     
     return mImages[filename].first;
    
   } // end loadImage

  /**
   *
   */
  void XGraphics::freeImage(const std::string& filename)
  {
    if (mImages.find(filename) == mImages.end())
    {
      //TODO
      //Add error
      return;
    }

    mImages[filename].second--;

     if (mImages[filename].second == 0)
     {
       delete mImages[filename].first;
       mImages.erase(filename);
     }   
  } // end freeImage

  /**
   *
   */
  void XGraphics::drawImage(const Image* image, int srcX,
                            int srcY, int dstX, int dstY,
                            int width, int height)
  {
    ClipRectangle top = mClipStack.top();

    Pixmap* temp = ((XImage*)image)->getImageData();

    XCopyArea(mDisplay, *temp, mWindow, mGraphicsContext,
              srcX, srcY,
              image->getWidth(), image->getHeight(),
              dstX, dstY);

  } // end drawImage

//   /**
//    *
//    */
//   void SDLGraphics::drawImage(const Image* image, int dstX, int dstY)
//   {
//     drawImage(image, 0, 0, dstX, dstY, image->getWidth(), image->getHeight());
    
//   } // end drawImage

//   /**
//    *
//    */
//   void SDLGraphics::fillRectangle(const Rectangle& rectangle)
//   {
    
//     Rectangle area = rectangle;
//     ClipRectangle top = mClipStack.top(); 
//     area.x += top.xOffset;
//     area.y += top.yOffset;

//     if(!area.intersect(top))
//     {
//       return;
//     }
    
//     SDL_Rect rect;
//     rect.x = area.x;
//     rect.y = area.y;
//     rect.w = area.width;
//     rect.h = area.height;
    
//     Uint32 color = SDL_MapRGB(mTarget->format, mColor.r, mColor.g, mColor.b);
//     SDL_FillRect(mTarget, &rect, color);

//   } // end fillRectangle

//   /**
//    * 
//    */
//   void SDLGraphics::drawPoint(int x, int y)
//   {
//     ClipRectangle top = mClipStack.top();
//     x += top.xOffset;
//     y += top.yOffset;

//     if(!top.isPointInRect(x,y))
//       return;
    
//     int bpp = mTarget->format->BytesPerPixel;

//     SDL_LockSurface(mTarget);

//     Uint8 *p = (Uint8 *)mTarget->pixels + y * mTarget->pitch + x * bpp;
    
//     Uint32 pixel = SDL_MapRGB(mTarget->format, mColor.r, mColor.g, mColor.b);
    
//     switch(bpp) {
//       case 1:
//         *p = pixel;
//         break;
        
//       case 2:
//         *(Uint16 *)p = pixel;
//         break;
        
//       case 3:
//         if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
//           p[0] = (pixel >> 16) & 0xff;
//           p[1] = (pixel >> 8) & 0xff;
//           p[2] = pixel & 0xff;
//         } else {
//           p[0] = pixel & 0xff;
//           p[1] = (pixel >> 8) & 0xff;
//           p[2] = (pixel >> 16) & 0xff;
//         }
//         break;
        
//       case 4:
//         *(Uint32 *)p = pixel;
//         break;
//     }

//     SDL_UnlockSurface(mTarget);
//   } // end drawPoint

  void XGraphics::drawLine(int x1, int y1, int x2, int y2)
  {
    ClipRectangle top = mClipStack.top(); 
    
    XDrawLine(mDisplay, mWindow, mGraphicsContext,
              x1,// + top.xOffset,
              y1, // + top.yOffset,
              x2, //+ top.xOffset,
              y2);//+ top.yOffset);

  } // end drawLine

  void XGraphics::drawHLine(int x1, int y, int x2)
  {
    drawLine(x1, y, x2, y);
  } // end drawHLine

  void XGraphics::drawVLine(int x, int y1, int y2)
  {
    drawLine(x, y1, x, y2);
  } // end drawVLine

  void XGraphics::drawRectangle(const Rectangle& rectangle)
  {
    ClipRectangle top = mClipStack.top(); 
    XDrawRectangle(mDisplay, mWindow, mGraphicsContext,
                   rectangle.x,// + top.xOffset,
                   rectangle.x  + rectangle.width,// + top.xOffset,
                   rectangle.y,// + top.yOffset,
                   rectangle.y + rectangle.height);// + top.yOffset);
  } // end drawRectangle
  
  void XGraphics::setColor(const Color& color)
  {
    Graphics::setColor(color);
    Colormap colormap = DefaultColormap(mDisplay, 0);
    XColor xcolor;

    /*char hexColor[3];
    hexColor[0] = '#';
    hexColor[1] = (color.g)&0;
    hexColor[2] = color.b;

    hexColor[0] = color.r;
    hexColor[1] = color.g;
    hexColor[2] = color.b;
    XParseColor(mDisplay, colormap, hexColor, &xcolor);
    */

    xcolor.red = (color.r / 100)*65535;
    xcolor.green = (color.g / 100)*65535;
    xcolor.blue = (color.b / 100)*65535;
    
    XAllocColor(mDisplay, colormap, &xcolor);
    XSetForeground(mDisplay, mGraphicsContext, xcolor.pixel);
  } // end setColor

} // end gcn
