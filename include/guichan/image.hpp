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

#ifndef GCN_IMAGE_HPP
#define GCN_IMAGE_HPP

#include <string>

#include "guichan/imageloader.hpp"
#include "guichan/platform.hpp"

namespace gcn
{
  /**
	 * This is a class holding images in Guichan. To be able to use
	 * this class you must first set the ImageLoader in Image by
	 * calling Image::setImageLoader(myImageLoader)
	 * (it is static). If this is not done, the constructor taking
	 * a filename will throw an exception.
	 * The ImageLoader you use must be compatible with your Graphis
	 * obejct.
	 * Ex: If you use SDLGraphics you should use SDLImageLoader
	 *     otherwise your program will crash in a most bizarre way.
	 */
  class GCN_CORE_DECLSPEC Image
  {
  public:

    /**
     * Constructor.
     *
     * @param data the data of the image
     * @param width the width of the image     
     * @param height the height of the image
     */
     Image(void* data, int width, int height);

    /**
     * Constructor.
     *
     * @param filename the filename of the image.
     * @throws Exception when no ImageLoader exists.
     */
    Image(const std::string& filename);

		/**
		 * Destructor.
		 * Unloads the image with the ImageLoader, if it was
		 * loaded with it.
		 */
    virtual ~Image();
    
    /**
     * @return the filename of the image (if existing)
     */
    virtual const std::string& getFilename() const;

    /**
     * @return the image width
     */
    virtual int getWidth() const;

    /**
     * @return the image height
     */
    virtual int getHeight() const;

		/**
		 * @return a void pointer to some image data. Image data can
		 *         be different things depending on what ImageLoader
		 *         you use. If you for instance use the SDLImageLoader
		 *         then an SDL_Surface will be returned.
		 * @see SDLImageLoader, AllegroImageLoader
		 */
    virtual void* _getData() const;

		/**
		 * @return a pointer to the currently used ImageLoader
		 * @see SDLImageLoader, AllegroImageLoader
		 */
		static ImageLoader* _getImageLoader();
		
		/**
		 * Sets the ImageLoader to be used be the Image class.
		 *
		 * IMPORTANT: The ImageLoader is static and MUST be set before
		 *            loading images!
		 *
		 * @param imageLoader the ImageLoader to be used.
		 * @see SDLImageLoader, AllegroImageLoader
		 */
    static void setImageLoader(ImageLoader* imageLoader);
		
	protected:
		/**
		 * Default constructor. It is protected, but it's here so
		 * that you can overload Image it.
		 */
		Image();
		
  private:
    void* mData;
    int mWidth;
    int mHeight;
    std::string mFilename;
		bool mLoadedWithImageLoader;
    static ImageLoader* mImageLoader;
		
  }; // end Image
  
} // end gcn

#endif // end GCN_IMAGE_HPP
