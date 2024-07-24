// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef TESTS_INTEGRATION_OPENLAYER_OPENLAYER_HPP_
#define TESTS_INTEGRATION_OPENLAYER_OPENLAYER_HPP_

/**
 * @brief This example shows the widgets present in FifeGUI using the OpenLayer backend.
 */

#include <fifechan/openlayer.hpp>

#include <fifechan.hpp>

namespace openlayer
{
    // Here the following backend objects are necessary for FifeGUI to work
    // with OpenLayer. These objects include:
    // A Graphics object, allowing the GUI to render using OpenLayer.
    // An Input object, enabling the GUI to receive user input via OpenLayer.
    // An ImageLoader object, allowing the loading of images using OpenLayer.
    fcn::OpenLayerGraphics* graphics;
    fcn::OpenLayerInput* input;
    fcn::OpenLayerImageLoader* imageLoader;

    /**
     * Initialises the OpenLayer application. This function creates the global
     * Gui object that can be populated by various examples.
     */
    void init()
    {
        // We simply initialise OpenLayer as we would do with any OpenLayer application.
        ol::Setup::SetupProgram(true, true, true);
        ol::Setup::SetupScreen(640, 480, WINDOWED);

        // Now it's time to initialise the OpenLayer backend.

        imageLoader = new fcn::OpenLayerImageLoader();
        // Set the ImageLoader by calling a static function of the Image class.
        fcn::Image::setImageLoader(imageLoader);
        graphics = new fcn::OpenLayerGraphics();
        // We need to tell the OpenLayer Graphics object how big the screen is.
        graphics->setTargetPlane(640, 480);
        input = new fcn::OpenLayerInput();

        // Finally, we create the Gui object and pass graphics and input to it.
        globals::gui = new fcn::Gui();
        globals::gui->setGraphics(graphics);
        globals::gui->setInput(input);
    }

    /**
     * Halts the OpenLayer application.
     */
    void halt()
    {
        delete globals::gui;

        delete imageLoader;
        delete input;
        delete graphics;
    }

    /**
     * Runs the OpenLayer application.
     */
    void run()
    {
        ol::Bitmap mouse_bmp;
        mouse_bmp.Load(mouse_sprite, ol::CONVERT_MAGIC_PINK);

        if (!mouse_bmp) {
            ol::OlError("Pointer not loaded.");
        }

        while (!key[KEY_ESC]) {
            // Now we let the Gui object perform its logic.
            globals::gui->logic();
            // Now we let the Gui object draw itself.
            globals::gui->draw();

            // We draw the mouse pointer manually.
            mouse_bmp.Blit(mouse_x, mouse_y);
            // Finally we update the screen.
            ol::GfxRend::RefreshScreen();
        }
    }
} // namespace openlayer

#endif // TESTS_INTEGRATION_OPENLAYER_OPENLAYER_HPP_
