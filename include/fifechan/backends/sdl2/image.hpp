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

namespace fcn::sdl2
{
    /**
     * SDL implementation of Image.
     */
    class FIFEGUI_EXT_API Image : public fcn::Image
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
        Image(SDL_Surface* surface, bool autoFree, SDL_Renderer* renderer = nullptr);

        ~Image() override;

        /**
         * Gets the SDL surface for the image.
         *
         * @return the SDL surface for the image (transient, for pixel operations only).
         * @deprecated Use getTexture() for rendering. Returns nullptr if only texture is available.
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
        /** SDL texture for accelerated rendering (primary storage). */
        SDL_Texture* mTexture = nullptr;

        /** SDL renderer associated with the texture. */
        SDL_Renderer* mRenderer = nullptr;

        /** Transient surface for pixel operations (getPixel/putPixel). */
        SDL_Surface* mTransientSurface = nullptr;

        /** Whether the transient surface should be freed on destruction. */
        bool mAutoFree;
    };
} // namespace fcn::sdl2

#endif // INCLUDE_FIFECHAN_BACKENDS_SDL_SDLIMAGE_HPP_
