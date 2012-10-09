/***************************************************************************
 *   Copyright (C) 2012 by the fifechan team                               *
 *   https://github.com/fifechan                                           *
 *   This file is part of fifechan.                                        *
 *                                                                         *
 *   fifechan is free software; you can redistribute it and/or             *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA          *
 ***************************************************************************/

/*
 * Code that sets up an SDL application with Guichan using the 
 * Guichan SDL back end.
 */

#include <fifechan.hpp>
#include <fifechan/sdl.hpp>

namespace sdl
{
    bool running = true;
    SDL_Surface* screen;

    // All back ends contain objects to make Guichan work on a
    // specific target - in this case SDL - and they are a Graphics
    // object to make Guichan able to draw itself using SDL, an
    // input objec to make Guichan able to get user input using SDL
    // and an ImageLoader object to make Guichan able to load images
    // using SDL.
    fcn::SDLGraphics* graphics;
    fcn::SDLInput* input;
    fcn::SDLImageLoader* imageLoader;

    /**
     * Initialises the SDL application. This function creates the global
     * Gui object that can be populated by various examples.
     */
    void init()
    {
        // We simply initialise SDL as we would do with any SDL application.
        SDL_Init(SDL_INIT_VIDEO);
        screen = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE);
        // We want unicode for the SDLInput object to function properly.
        SDL_EnableUNICODE(1);
        // We also want to enable key repeat.
        SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

        // Now it's time to initialise the Guichan SDL back end.

        imageLoader = new fcn::SDLImageLoader();
        // The ImageLoader Guichan should use needs to be passed to the Image object
        // using a static function.
        fcn::Image::setImageLoader(imageLoader);
        graphics = new fcn::SDLGraphics();
        // The Graphics object needs a target to draw to, in this case it's the
        // screen surface, but any surface will do, it doesn't have to be the screen.
        graphics->setTarget(screen);
        input = new fcn::SDLInput();

        // Now we create the Gui object to be used with this SDL application.
        globals::gui = new fcn::Gui();
        // The Gui object needs a Graphics to be able to draw itself and an Input
        // object to be able to check for user input. In this case we provide the
        // Gui object with SDL implementations of these objects hence making Guichan
        // able to utilise SDL.
        globals::gui->setGraphics(graphics);
        globals::gui->setInput(input);
    }

    /**
     * Halts the SDL application.
     */
    void halt()
    {
        delete globals::gui;

        delete imageLoader;
        delete input;
        delete graphics;

        SDL_Quit();
    }

    /**
     * Runs the SDL application.
     */
    void run()
    {
        // The main loop
        while(running)
        {
            // Check user input
            SDL_Event event;
            while(SDL_PollEvent(&event))
            {
                if (event.type == SDL_KEYDOWN)
                {
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        running = false;
                    }
                    if (event.key.keysym.sym == SDLK_f)
                    {
                        if (event.key.keysym.mod & KMOD_CTRL)
                        {
                            // Works with X11 only
                            SDL_WM_ToggleFullScreen(screen);
                        }
                    }
                }
                else if(event.type == SDL_QUIT)
                {
                    running = false;
                }
                
                // After we have manually checked user input with SDL for
                // any attempt by the user to halt the application we feed
                // the input to Guichan by pushing the input to the Input
                // object.
                input->pushInput(event);
            }
            // Now we let the Gui object perform its logic.
            globals::gui->logic();
            // Now we let the Gui object draw itself.
            globals::gui->draw();
            // Finally we update the screen.
            SDL_Flip(screen);
        }
    }
}
