// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef TESTS_INTEGRATION_OPENGL_ALLEGRO_OPENGLALLEGRO_HPP_
#define TESTS_INTEGRATION_OPENGL_ALLEGRO_OPENGLALLEGRO_HPP_

/**
 * @brief Example that shows all widgets using the OpenGL and Allegro backends.
 *
 * The example is a simple application that creates a window and populates
 * it with all the widgets present in FifeGUI. The example is a good starting
 * point for anyone who wants to start using FifeGUI with OpenGL and Allegro.
 */

// The openglallegroimageloader.hpp header file needs to be included
// in order to get the image loader that uses OpenGL and Allegro.
#include <fifechan/backends/allegro/allegro.hpp>
#include <fifechan/backends/opengl/opengl.hpp>
#include <fifechan/backends/opengl/openglallegroimageloader.hpp>

#include <fifechan.hpp>

namespace openglallegro
{
    // Here the following backend objects are necessary for FifeGUI to work
    // with Irrlicht. These objects include:
    // A Graphics object, allowing the GUI to render using OpenGL,
    // An Input object, enabling the GUI to receive user input via Allegro,
    // An ImageLoader object, allowing the loading of images using Allegro for OpenGL.
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
        allegro_gl_set(AGL_COLOR_DEPTH, 32);
        allegro_gl_set(AGL_Z_DEPTH, 24);
        allegro_gl_set(AGL_FULLSCREEN, TRUE);
        allegro_gl_set(AGL_DOUBLEBUFFER, 1);
        allegro_gl_set(AGL_SUGGEST, AGL_COLOR_DEPTH | AGL_Z_DEPTH | AGL_DOUBLEBUFFER);

        set_gfx_mode(GFX_OPENGL_WINDOWED, 640, 480, 0, 0);

        install_keyboard();
        install_mouse();
        install_timer();

        glViewport(0, 0, 640, 480);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        // Now it's time to initialise the OpenGL backend and Allegro backend.

        imageLoader = new fcn::OpenGLAllegroImageLoader();
        // Set the ImageLoader by calling a static function of the Image class.
        fcn::Image::setImageLoader(imageLoader);
        graphics = new fcn::OpenGLGraphics();
        // We need to tell the OpenGL Graphics object how big the screen is.
        graphics->setTargetPlane(640, 480);
        input = new fcn::AllegroInput();

        // Finally, we create the Gui object and pass graphics and input to it.
        globals::gui = new fcn::Gui();
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
        while (!key[KEY_ESC]) {
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
} // namespace openglallegro

#endif // TESTS_INTEGRATION_OPENGL_ALLEGRO_OPENGLALLEGRO_HPP_
