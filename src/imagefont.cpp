/*      _______   __   __   __   ______   __   __   _______   __   __                 
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\                
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /                 
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /                  
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /                   
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /                    
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/                      
 *
 * Copyright (c) 2004 darkbits                              Js_./
 * Per Larsson a.k.a finalman                          _RqZ{a<^_aa
 * Olof Naessén a.k.a jansem/yakslem                _asww7!uY`>  )\a//
 *                                                 _Qhm`] _f "'c  1!5m
 * Visit: http://guichan.darkbits.org             )Qk<P ` _: :+' .'  "{[
 *                                               .)j(] .d_/ '-(  P .   S
 * License: (BSD)                                <Td/Z <fP"5(\"??"\a.  .L
 * Redistribution and use in source and          _dV>ws?a-?'      ._/L  #'
 * binary forms, with or without                 )4d[#7r, .   '     )d`)[
 * modification, are permitted provided         _Q-5'5W..j/?'   -?!\)cam'
 * that the following conditions are met:       j<<WP+k/);.        _W=j f
 * 1. Redistributions of source code must       .$%w\/]Q  . ."'  .  mj$
 *    retain the above copyright notice,        ]E.pYY(Q]>.   a     J@\
 *    this list of conditions and the           j(]1u<sE"L,. .   ./^ ]{a
 *    following disclaimer.                     4'_uomm\.  )L);-4     (3=
 * 2. Redistributions in binary form must        )_]X{Z('a_"a7'<a"a,  ]"[
 *    reproduce the above copyright notice,       #}<]m7`Za??4,P-"'7. ).m
 *    this list of conditions and the            ]d2e)Q(<Q(  ?94   b-  LQ/
 *    following disclaimer in the                <B!</]C)d_, '(<' .f. =C+m
 *    documentation and/or other materials      .Z!=J ]e []('-4f _ ) -.)m]'
 *    provided with the distribution.          .w[5]' _[ /.)_-"+?   _/ <W"
 * 3. Neither the name of Guichan nor the      :$we` _! + _/ .        j?
 *    names of its contributors may be used     =3)= _f  (_yQmWW$#(    "
 *    to endorse or promote products derived     -   W,  sQQQQmZQ#Wwa]..
 *    from this software without specific        (js, \[QQW$QWW#?!V"".
 *    prior written permission.                    ]y:.<\..          .
 *                                                 -]n w/ '         [.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT       )/ )/           !
 * HOLDERS AND CONTRIBUTORS "AS IS" AND ANY         <  (; sac    ,    '
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING,               ]^ .-  %
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF            c <   r
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR            aga<  <La
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE          5%  )P'-3L
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR        _bQf` y`..)a
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          ,J?4P'.P"_(\?d'.,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES               _Pa,)!f/<[]/  ?"
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT      _2-..:. .r+_,.. .
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     ?a.<%"'  " -'.a_ _,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION)                     ^
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * For comments regarding functions please see the header file. 
 */

#include "guichan/imagefont.hpp"
#include "guichan/exception.hpp"
#include "guichan/image.hpp"

#include <iostream>

namespace gcn
{
  ImageFont::ImageFont(const std::string& filename, const std::string& glyphs)
  {
		if (Image::_getImageLoader() == NULL)
		{
			throw GCN_EXCEPTION("ImageFont::ImageFont. I have no ImageLoader!");
		}
		
		unsigned int i,x=0;

    for (i=0; i<256; i++)
    {
      mGlyphX[i] = mGlyphW[i] = 0;
    }
    
    Image::_getImageLoader()->prepare(filename);
    Color separator = Image::_getImageLoader()->getPixel(0, 0);
    
    unsigned char k;

    for (i=0; i<glyphs.size(); ++i) 
    {
      k = glyphs.at(i);
      x = addGlyph(k, x, separator);
    }
    
    int w = Image::_getImageLoader()->getWidth();
    int h = Image::_getImageLoader()->getHeight();
    void* data = Image::_getImageLoader()->finalize();
    
    mImage = new Image(data, w, h);
    mHeight = h;
		mSpacing = 0;
		
  } // end ImageFont

  ImageFont::ImageFont(const std::string& filename, unsigned char glyphsFrom, unsigned char glyphsTo)
  {
		if (Image::_getImageLoader() == NULL)
		{
			throw GCN_EXCEPTION("ImageFont::ImageFont. I have no ImageLoader!");
		}

    int i,x=0;

    for (i=0; i<256; i++)
    {
      mGlyphX[i] = mGlyphW[i] = 0;
    }

    Image::_getImageLoader()->prepare(filename);
    Color separator = Image::_getImageLoader()->getPixel(0, 0);
    
    for (i=glyphsFrom; i<glyphsTo+1; i++)
    {
      x = addGlyph(i, x, separator); 
    }

    int w = Image::_getImageLoader()->getWidth();
    int h = Image::_getImageLoader()->getHeight();
    void* data = Image::_getImageLoader()->finalize();
    
    mImage = new Image(data, w, h);
    mHeight = h;
		mSpacing = 0;
		
  } // end ImageFont

  ImageFont::~ImageFont()
  {
		Image::_getImageLoader()->free(mImage);
    delete mImage;
		
  } // end ~ImageFont
  
  int ImageFont::getWidth(unsigned char glyph) const
  {
    if (mGlyphW[glyph] == 0)
    {         
      return mGlyphW[(int)(' ')] + mSpacing;
    }
    
    return mGlyphW[glyph] + mSpacing;

  } // end getWidth

  int ImageFont::getHeight() const
  {
    return mHeight;

  } // end getHeight

  int ImageFont::drawGlyph(Graphics* graphics, unsigned char glyph, int x, int y)
  {    
    if (mGlyphW[glyph] == 0)
    {
      graphics->drawRectangle(Rectangle(x, y + 1, mGlyphW[(int)(' ')] - 1, mHeight - 2));
      
      return mGlyphW[(int)(' ')] + mSpacing;
    }
    
    graphics->drawImage(mImage, mGlyphX[glyph], 0, x, y, mGlyphW[glyph], mHeight);

    return mGlyphW[glyph] + mSpacing;

  } // end drawGlyph

	void ImageFont::setSpacing(int spacing)
	{
		mSpacing = spacing;
	}

	int ImageFont::getSpacing()
	{
		return mSpacing;
	}
	
  int ImageFont::addGlyph(unsigned char c, int x, const Color& separator)
  {
		ImageLoader* il = Image::_getImageLoader();
		
		Color color;
		do
		{
			++x;

			if (x >= il->getWidth())
			{
				throw GCN_EXCEPTION("ImageFont::addGlyph. Image with font is corrupt.");
			}			

			color = il->getPixel(x, 0);

		} while (color == separator);
		
		mGlyphX[c] = x;
		
		int w = 0;
		
		do
		{
			++w;

			if (x+w >= il->getWidth())
			{
				throw GCN_EXCEPTION("ImageFont::addGlyph. Image with font is corrupt.");
			}			
			
			color = il->getPixel(x + w, 0);
			
		} while (color != separator);
		
		mGlyphW[c] = w;
		
		return w+x;
		
  } // end addGlyph

} // end gcn
