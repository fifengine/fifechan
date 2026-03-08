// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_BACKENDS_SDL_SDLIMAGE_HPP_
#define INCLUDE_FIFECHAN_BACKENDS_SDL_SDLIMAGE_HPP_

#include <SDL2/SDL.h>

#include <string>

#include "fifechan/color.hpp"
#include "fifechan/image.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * SDL implementation of Image.
     */
    class FIFEGUI_EXT_API SDLImage : public Image
    {
    public:
        /**
         * Constructor. Load an image from an SDL surface.
         *
         * NOTE: The functions getPixel and putPixel are only guaranteed to work
         *       before an image has been converted to display format.
         *
         * @param surface the surface from which to load.
         * @param autoFree true if the surface should automatically be deleted.
         * @param renderer the renderer to use for creating the texture.
         */
        SDLImage(SDL_Surface* surface, bool autoFree, SDL_Renderer* renderer = nullptr);

        ~SDLImage() override;

        /**
         * Gets the SDL surface for the image.
         *
         * @return the SDL surface for the image.
         */
        virtual SDL_Surface* getSurface() const;

        /**
         * Gets the SDL texture for the image.
         *
         * @return the SDL texture for the image.
         */
        virtual SDL_Texture* getTexture() const;

        // Inherited from Image

        void free() override;

        int getWidth() const override;

        int getHeight() const override;

        Color getPixel(int x, int y) override;

        void putPixel(int x, int y, Color const & color) override;

        void convertToDisplayFormat() override;

    protected:
        /** Underlying SDL surface for this image (may be nullptr). */
        SDL_Surface* mSurface;

        /** Optional SDL texture created for accelerated rendering. */
        SDL_Texture* mTexture = nullptr;

        /** SDL renderer associated with the texture, if any. */
        SDL_Renderer* mRenderer = nullptr;

        /** Whether the surface should be freed automatically on destruction. */
        bool mAutoFree;
    };
} // namespace fcn

namespace fcn::sdl2
{
    using Image = fcn::SDLImage;
}

#endif // INCLUDE_FIFECHAN_BACKENDS_SDL_SDLIMAGE_HPP_
