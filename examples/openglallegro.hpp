/***************************************************************************
 *   Copyright (C) 2012 by the fifechan team                               *
 *   http://fifechan.github.com/fifechan                                   *
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
 * Code that sets up an OpenGL application with Guichan using the 
 * Guichan OpenGL back end and the Allegro back end (as OpenGL cannot
 * load images nor check for user input an additional back end needs
 * to be used).
 */

// The openglallegroimageloader.hpp header file needs to be included
// in order to get the image loader that uses OpenGL and Allegro.
#include <fifechan/opengl/openglallegroimageloader.hpp>
#include <fifechan.hpp>
#include <fifechan/opengl.hpp>
#include <fifechan/allegro.hpp>

namespace openglallegro
{
    // All back ends contain objects to make Guichan work on a
    // specific target. They are a Graphics object to make Guichan 
    // able to draw itself using OpenGL, an input objec to make 
    // Guichan able to get user input using Allegro and an ImageLoader 
    // object to make Guichan able to load images using OpenGL and Allegro.
    fcn::OpenGLGraphics* graphics;
    fcn::AllegroInput* input;
    fcn::OpenGLAllegroImageLoader* imageLoader;

    /**
     * Initialises the OpenGL and Allegro application. This function creates the global
     * Gui object that can be populated by various examples.
     */
    void init()
    {
        // We simply initialise OpenGL and Allegro as we would do with any OpenGL
        // and Allegro application.
        allegro_init();
      
        install_allegro_gl();

        allegro_gl_clear_settings();
	    allegro_gl_set (AGL_COLOR_DEPTH, 32);
	    allegro_gl_set (AGL_Z_DEPTH, 24);
	    allegro_gl_set (AGL_FULLSCREEN, TRUE);
	    allegro_gl_set (AGL_DOUBLEBUFFER, 1);
	    allegro_gl_set (AGL_SUGGEST, AGL_COLOR_DEPTH | AGL_Z_DEPTH | AGL_DOUBLEBUFFER);

        set_gfx_mode(GFX_OPENGL_WINDOWED, 640, 480, 0, 0);

        install_keyboard();
        install_mouse();
        install_timer();

        glViewport(0, 0, 640, 480);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        // Now it's time to initialise the Guichan OpenGL back end
        // and the Guichan Allegro back end.

        imageLoader = new fcn::OpenGLAllegroImageLoader();
        // The ImageLoader Guichan should use needs to be passed to the Image object
        // using a static function.
        fcn::Image::setImageLoader(imageLoader);
        graphics = new fcn::OpenGLGraphics();
        // We need to tell the OpenGL Graphics object how big the screen is.
        graphics->setTargetPlane(640, 480);
        input = new fcn::AllegroInput();

        // Now we create the Gui object to be used with this OpenGL
        // and Allegro application.
        globals::gui = new fcn::Gui();
        // The Gui object needs a Graphics to be able to draw itself and an Input
        // object to be able to check for user input. In this case we provide the
        // Gui object with OpenGL and Allegro implementations of these objects hence 
        // making Guichan able to utilise OpenGL and Allegro.
        globals::gui->setGraphics(graphics);
        globals::gui->setInput(input);
    }

    /**
     * Halts the OpenGL and Allegro application.
     */
    void halt()
    {
        delete globals::gui;

        delete imageLoader;
        delete input;
        delete graphics;
    }

    /**
     * Runs the OpenGL and Allegro application.
     */
    void run()
    {
        while(!key[KEY_ESC])
        {
            // Now we let the Gui object perform its logic.
            globals::gui->logic();
            // Now we let the Gui object draw itself.
            globals::gui->draw();
            
            // Now we draw the mouse cursor.
            allegro_gl_set_allegro_mode();
            show_mouse(screen);
            algl_draw_mouse();
            allegro_gl_unset_allegro_mode();
        
            // Finally we update the screen.
            allegro_gl_flip();
        }
    }
}
