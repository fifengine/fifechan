#include <cmath>
#include "guichan/sdl/sdlgraphics.hpp"
#include "config.hpp"
#ifdef HAVE_SDL_SDL_IMAGE_H
#include <SDL/SDL_image.h>
#endif

namespace gcn
{
  /**
   *
   */
  void SDLGraphics::setTarget(SDL_Surface* target)
  {
    mTarget = target;
    Rectangle area;
    area.x = 0;
    area.y = 0;
    area.width = target->w;
    area.height = target->h;
    pushClipArea(area);

  } // end setTarget

  /**
   *
   */
  bool SDLGraphics::pushClipArea(Rectangle area)
  {
    SDL_Rect rect;
    bool result = Graphics::pushClipArea(area);

    ClipRectangle carea = mClipStack.top();
    rect.x = carea.x;
    rect.y = carea.y;
    rect.w = carea.width;
    rect.h = carea.height;
    
    SDL_SetClipRect(mTarget, &rect);

    return result;
    
  } // end pushClipArea

  /**
   *
   */
  void SDLGraphics::popClipArea()
  {
    SDL_Rect rect;
    Graphics::popClipArea();

    ClipRectangle carea = mClipStack.top();
    rect.x = carea.x;
    rect.y = carea.y;
    rect.w = carea.width;
    rect.h = carea.height;
    
    SDL_SetClipRect(mTarget, &rect);    
  } // end 
  
  /**
   *
   */
  SDL_Surface* SDLGraphics::getTarget() const
  {
    return mTarget;
  }
  
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
  Image* SDLGraphics::loadImage(const std::string& filename)
  {
    if (mImages.find(filename) == mImages.end())
    {
      SDL_Surface* temp;
      
#ifdef HAVE_SDL_SDL_IMAGE_H      
      temp = SDL_DisplayFormat(IMG_Load(filename.c_str()));
#else
      temp = SDL_DisplayFormat(SDL_LoadBMP(filename.c_str()));
#endif
      
      if (temp == NULL)
      {
        //TODO
        //Add error
        return NULL;
      }
      
      SDLImage* image = new SDLImage(temp, filename);
      mImages[filename] = imageRefCount(image,1);
    }

    mImages[filename].second++;

    return mImages[filename].first;
    
  } // end loadImage

  /**
   *
   */
  void SDLGraphics::freeImage(const std::string& filename)
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
      SDL_FreeSurface(mImages[filename].first->getImageData());
      delete mImages[filename].first;
      mImages.erase(filename);
    }
    
  } // end freeImage

  /**
   * 
   */
  void SDLGraphics::freeImage(Image* image)
  {
    freeImage(image->getFilename());

  } // end SDLGraphics

  void SDLGraphics::drawImage(const Image* image, int srcX,
                              int srcY, int dstX, int dstY,
                              int width, int height)
  {
    ClipRectangle top = mClipStack.top();
    SDL_Rect src;
    SDL_Rect dst;
    src.x = srcX;
    src.y = srcY;
    src.w = width;
    src.h = height;
    dst.x = dstX + top.xOffset;
    dst.y = dstY + top.yOffset;

    SDL_Surface* srcImage;
    srcImage = ((SDLImage*)image)->getImageData();
    SDL_BlitSurface(srcImage, &src, mTarget, &dst);
    
  } // end drawImage

  /**
   *
   */
  void SDLGraphics::drawImage(const Image* image, int dstX, int dstY)
  {
    drawImage(image, 0, 0, dstX, dstY, image->getWidth(), image->getHeight());
    
  } // end drawImage

  /**
   *
   */
  void SDLGraphics::fillRectangle(const Rectangle& rectangle)
  {
    
    Rectangle area = rectangle;
    ClipRectangle top = mClipStack.top(); 
    area.x += top.xOffset;
    area.y += top.yOffset;

    if(!area.intersect(top))
    {
      return;
    }
    
    SDL_Rect rect;
    rect.x = area.x;
    rect.y = area.y;
    rect.w = area.width;
    rect.h = area.height;
    
    Uint32 color = SDL_MapRGB(mTarget->format, mColor.r, mColor.g, mColor.b);
    SDL_FillRect(mTarget, &rect, color);

  } // end fillRectangle

  /**
   * 
   */
  void SDLGraphics::drawPoint(int x, int y)
  {
    ClipRectangle top = mClipStack.top();
    x += top.xOffset;
    y += top.yOffset;

    if(!top.isPointInRect(x,y))
      return;
    
    int bpp = mTarget->format->BytesPerPixel;

    SDL_LockSurface(mTarget);

    Uint8 *p = (Uint8 *)mTarget->pixels + y * mTarget->pitch + x * bpp;
    
    Uint32 pixel = SDL_MapRGB(mTarget->format, mColor.r, mColor.g, mColor.b);
    
    switch(bpp) {
      case 1:
        *p = pixel;
        break;
        
      case 2:
        *(Uint16 *)p = pixel;
        break;
        
      case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
          p[0] = (pixel >> 16) & 0xff;
          p[1] = (pixel >> 8) & 0xff;
          p[2] = pixel & 0xff;
        } else {
          p[0] = pixel & 0xff;
          p[1] = (pixel >> 8) & 0xff;
          p[2] = (pixel >> 16) & 0xff;
        }
        break;
        
      case 4:
        *(Uint32 *)p = pixel;
        break;
    }

    SDL_UnlockSurface(mTarget);
  } // end drawPoint

  void SDLGraphics::drawHLine(int x1, int y, int x2)
  {
    ClipRectangle top = mClipStack.top();
    x1 += top.xOffset;
    y += top.yOffset;
    x2 += top.xOffset;

    if (y < top.y || y > top.y + top.height)
      return;
    
    if (x1 > x2)
    {
      x1 ^= x2;
      x2 ^= x1;
      x1 ^= x2;
    }

    if (top.x > x1)
    {
      if (top.x > x2)
      {
        return;
      }
      x1 = top.x;
    }

    if (top.x + top.width < x2)
    {
      if (top.x + top.width < x1)
      {
        return;
      }      
      x2 = top.x + top.width;
    }
    
    int bpp = mTarget->format->BytesPerPixel;
    
    SDL_LockSurface(mTarget);
    
    Uint8 *p = (Uint8 *)mTarget->pixels + y * mTarget->pitch + x1 * bpp;
    
    Uint32 pixel = SDL_MapRGB(mTarget->format, mColor.r, mColor.g, mColor.b);
    
    switch(bpp) {
      case 1:
      {
        for (;x1 <= x2; ++x1)
        { 
          *(p++) = pixel;
        }
      } break;
      
      case 2:
      {
        Uint16* q = (Uint16*)p;
        for (;x1 <= x2; ++x1)
        {
          *(q++) = pixel;
        }
      } break;
        
      case 3:  
      {
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
          for (;x1 <= x2; ++x1)
          {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
            p += 3;
          }
        }
        else
        {
          for (;x1 <= x2; ++x1)
          {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
            p += 3;
          }
        } 
      } break;
  
      case 4:
      {
        Uint32* q = (Uint32*)p;
        for (;x1 <= x2; ++x1)
        {
          *(q++) = pixel;
        }
      } break;
    } // end switch
    
    SDL_UnlockSurface(mTarget);
    
  } // end drawHLine

  void SDLGraphics::drawVLine(int x, int y1, int y2)
  {
    ClipRectangle top = mClipStack.top();
    x += top.xOffset;
    y1 += top.yOffset;
    y2 += top.yOffset;

    if (x < top.x || x > top.x + top.width)
      return;
    
    if (y1 > y2)
    {
      y1 ^= y2;
      y2 ^= y1;
      y1 ^= y2;
    }

    if (top.y > y1)
    {
      if (top.y > y2)
      {
        return;
      }
      y1 = top.y;
    }

    if (top.y + top.height < y2)
    {
      if (top.y + top.height < y1)
      {
        return;
      }      
      y2 = top.y + top.height;
    }
    
    int bpp = mTarget->format->BytesPerPixel;
    
    SDL_LockSurface(mTarget);
    
    Uint8 *p = (Uint8 *)mTarget->pixels + y1 * mTarget->pitch + x * bpp;
    
    Uint32 pixel = SDL_MapRGB(mTarget->format, mColor.r, mColor.g, mColor.b);
    
    switch(bpp) {
      case 1:
      {
        for (;y1 <= y2; ++y1)
        { 
          *p = pixel;
          p += mTarget->pitch;
        }
      } break;
      
      case 2:
      {
        for (;y1 <= y2; ++y1)
        {
          *(Uint16*)p = pixel;
          p += mTarget->pitch;
        }
      } break;
        
      case 3:  
      {
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
          for (;y1 <= y2; ++y1)
          {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
            p += mTarget->pitch;
          }
        }
        else
        {
          for (;y1 <= y2; ++y1)
          {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
            p += mTarget->pitch;
          }
        } 
      } break;
  
      case 4:
      {
        for (;y1 <= y2; ++y1)
        {
          *(Uint32*)p = pixel;
          p += mTarget->pitch;
        }
      } break;
    } // end switch
    
    SDL_UnlockSurface(mTarget);

  } // end drawVLine

  void SDLGraphics::drawRectangle(const Rectangle& rectangle)
  {
    int x1 = rectangle.x;
    int x2 = rectangle.x + rectangle.width;
    int y1 = rectangle.y;
    int y2 = rectangle.y + rectangle.height;

    drawHLine(x1, y1, x2);
    drawHLine(x1, y2, x2);

    drawVLine(x1, y1, y2);
    drawVLine(x2, y1, y2);
    
  } // end drawRectangle


  void SDLGraphics::drawLine(int x1, int y1, int x2, int y2)
  {
    bool yLonger = false;
    int incrementVal;
    int endVal;
    
    int shortLen = y2 - y1;
    int longLen = x2 - x1;

    if (std::abs(shortLen) > std::abs(longLen))
    {
      int swap = shortLen;
      shortLen = longLen;
      longLen = swap;
      yLonger = true;
    }
	
    endVal = longLen;

    if (longLen< 0)
    {
      incrementVal = -1;
      longLen = - longLen;
    }
    else
    {
      incrementVal = 1;
    }
    
    double decInc;

    if (longLen == 0)
    {
      decInc = (double)shortLen;
    }
    else
    {
      decInc = (double)shortLen / (double)longLen;
    }
    
    double j = 0.0;

    if (yLonger)
    {
      for (int i = 0; i != endVal; i += incrementVal)
      {
        drawPoint(x1 + (int)j, y1 + i);
        j += decInc;
      }
    }
    else
    {
      for (int i = 0; i != endVal; i += incrementVal)
      {
        drawPoint(x1 + i, y1 + (int)j);
        j += decInc;
      }
    } 
  } // end drawLine
  
} // end gcn
