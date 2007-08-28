#ifndef GCN_OPENGLALLEGROIMAGELOADER_HPP
#define GCN_OPENGLALLEGROIMAGELOADER_HPP

#include <allegro.h>
#include <alleggl.h>

#include <guichan/imageloader.hpp>
#include <guichan/exception.hpp>
#include <guichan/opengl/openglimage.hpp>

namespace gcn
{
    /**
     * OpenGL ImageLoader that loads images with Allegro.
     */
    class OpenGLAllegroImageLoader : public ImageLoader
    {
    public:
        
        // Inherited from ImageLoader

        virtual Image* load(const std::string& filename,
                            bool convertToDisplayFormat = true)
       
        {
            BITMAP *bmp = load_bitmap(filename.c_str(), NULL);
            
            if (bmp == NULL)
            {
                throw GCN_EXCEPTION(
                        std::string("Unable to load image file: ") + filename);
            }

            int bitmapBpp = bitmap_color_depth(bmp);
            
            // As a BITMAP without an alpha channel in Allegro has
            // all it's alpha values set to zero we have to check
            // if the BITMAP has an alpha channel. If no alpha channel
            // exists then we need to change the alpha values to 255
            // before creating the OpenGL image.
            bool hasAlphaChannel = false;
            int x, y;
            for (y = 0; y < bmp->h; y++)
            {
                for (x = 0; x < bmp->w; x++)
                {
                    int pixel = getpixel(bmp, x, y);
                    if (geta_depth(bitmapBpp, pixel) != 0)
                    {                        
                        hasAlphaChannel = true;                
                    }
                }
            }

            unsigned int pixels[bmp->w * bmp->h];
            
            for (y = 0; y < bmp->h; y++)
            {
                for (x = 0; x < bmp->w; x++)
                {
                    int pixel = getpixel(bmp,x, y);

                    if (!hasAlphaChannel)
                    {                        
                        pixels[x + y * bmp->w] = pixel | 0xff000000;
                    }
                    else
                    {
                        pixels[x + y * bmp->w] = pixel;
                    }
                }
            }

            OpenGLImage *image = new OpenGLImage(pixels,
                                                 bmp->w,
                                                 bmp->h,
                                                 convertToDisplayFormat);

            destroy_bitmap(bmp);

            return image;
        }
    };
}

#endif // end GCN_OPENGLALLEGROIMAGELOADER_HPP
