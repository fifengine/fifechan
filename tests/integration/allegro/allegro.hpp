// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef TESTS_INTEGRATION_ALLEGRO_ALLEGRO_HPP_
#define TESTS_INTEGRATION_ALLEGRO_ALLEGRO_HPP_

/**
 * @brief Contains the Allegro application setup code.
 *
 * The code uses FifeGUI and FifeGUI's Allegro backend to create a GUI.
 */

#include <fifechan/backends/allegro/allegro.hpp>

#include <fifechan.hpp>

namespace allegro
{
    BITMAP* screenBuffer;

    // Here the following backend objects are necessary for FifeGUI to work
    // with Allegro. These objects include:
    // A Graphics object, allowing the GUI to render using Allegro.
    // An Input object, enabling the GUI to receive user input via Allegro.
    // An ImageLoader object, allowing the loading of images using Allegro.
    fcn::AllegroGraphics* graphics;
    fcn::AllegroInput* input;
    fcn::AllegroImageLoader* imageLoader;

    /**
     * Initialises the Allegro application. This function creates the global
     * Gui object that can be populated by various examples.
     */
    void init()
    {
        // We simply initialise Allegro as we would do with any Allegro application.
        allegro_init();

        int bpp = desktop_color_depth();
        if (bpp == 0) {
            bpp = 16;
        }

        set_color_depth(bpp);

        if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0)) {
            if (set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0)) {
                throw FCN_EXCEPTION("Unable to set graphics mode");
            }
        }

        screenBuffer = create_bitmap(SCREEN_W, SCREEN_H);

        if (screenBuffer == NULL) {
            throw FCN_EXCEPTION("Unable to create a screen buffer");
        }

        install_keyboard();
        install_mouse();
        install_timer();

        // Now it's time to initialise the Allegro backend.

        imageLoader = new fcn::AllegroImageLoader();
        // Set the ImageLoader by calling a static function of the Image class.
        fcn::Image::setImageLoader(imageLoader);
        graphics = new fcn::AllegroGraphics();
        // Set the target for the graphics object to be the doublebuffer
        // for the screen. Drawing to the screen directly is not a good
        // idea, as it will produce flicker, unless you use page flipping.
        graphics->setTarget(screenBuffer);
        input = new fcn::AllegroInput();

        // Finally, we create the Gui object and pass graphics and input to it.
        globals::gui = new fcn::Gui();
        globals::gui->setGraphics(graphics);
        globals::gui->setInput(input);
    }

    /**
     * Halts the Allegro application.
     */
    void halt()
    {
        delete globals::gui;

        delete imageLoader;
        delete input;
        delete graphics;

        destroy_bitmap(screenBuffer);
    }

    /**
     * Runs the Allegro application.
     */
    void run()
    {
        while (!key[KEY_ESC]) {
            // Now we let the Gui object perform its logic.
            globals::gui->logic();
            // Now we let the Gui object draw itself.
            globals::gui->draw();

            // We draw the mouse pointer manually, as Allegro's mouse
            // drawing code is so weird.
            draw_sprite(screenBuffer, mouse_sprite, mouse_x, mouse_y);

            // Finally we update the screen.
            blit(screenBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        }
    }
} // namespace allegro

#endif // TESTS_INTEGRATION_ALLEGRO_ALLEGRO_HPP_
