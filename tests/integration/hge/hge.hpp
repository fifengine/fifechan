// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef TESTS_INTEGRATION_HGE_HGE_HPP_
#define TESTS_INTEGRATION_HGE_HGE_HPP_

/**
 * @brief Contains the code that sets up an HGE application with FifeGUI.
 */

#include <fifechan/backends/hge/hge.hpp>

#include <fifechan.hpp>

#include <string>

namespace hge
{
    HGE* hge     = NULL;
    bool running = false;

    // Here the following backend objects are necessary for FifeGUI to work
    // with HGE. These objects include:
    // A Graphics object, allowing the GUI to render using HGE.
    // An Input object, enabling the GUI to receive user input via HGE.
    // An ImageLoader object, allowing the loading of images using SDL.
    fcn::HGEGraphics* graphics;
    fcn::HGEInput* input;
    fcn::HGEImageLoader* imageLoader;

    /**
     * The frame function for the HGE application.
     */
    bool frameFunc()
    {
        if (hge->Input_GetKeyState(HGEK_ESCAPE) || running) {
            return true;
        }

        // Now we let the Gui object perform its logic.
        globals::gui->logic();
        // Next we begin a scene.
        hge->Gfx_BeginScene();
        hge->Gfx_Clear(0);
        // Now we let the Gui object draw itself.
        globals::gui->draw();
        // Finally we end the scene causing the screen to be updated.
        hge->Gfx_EndScene();

        return false;
    }

    /**
     * Initialises the HGE application. This function creates the global
     * Gui object that can be populated by various examples.
     */
    void init()
    {
        // We simply initialise HGE as we would do with any HGE application.
        hge = hgeCreate(HGE_VERSION);

        hge->System_SetState(HGE_FRAMEFUNC, frameFunc);
        hge->System_SetState(HGE_SCREENWIDTH, 640);
        hge->System_SetState(HGE_SCREENHEIGHT, 480);
        hge->System_SetState(HGE_WINDOWED, true);
        hge->System_SetState(HGE_HIDEMOUSE, false);
        hge->System_SetState(HGE_USESOUND, false);
        hge->System_SetState(HGE_SHOWSPLASH, false);
        hge->System_SetState(HGE_LOGFILE, "hgelog.txt");

        if (!hge->System_Initiate()) {
            throw FCN_EXCEPTION("Unable to initialse HGE: " + std::string(hge->System_GetErrorMessage()));
        }

        // Now it's time to initialise the HGE backend.

        imageLoader = new fcn::HGEImageLoader();
        // Set the ImageLoader by calling a static function of the Image class.
        fcn::Image::setImageLoader(imageLoader);
        graphics = new fcn::HGEGraphics();
        input    = new fcn::HGEInput();

        // Finally, we create the Gui object and pass graphics and input to it.
        globals::gui = new fcn::Gui();
        globals::gui->setGraphics(graphics);
        globals::gui->setInput(input);
    }

    /**
     * Halts the HGE application.
     */
    void halt()
    {
        delete globals::gui;

        delete imageLoader;
        delete input;
        delete graphics;

        hge->System_Shutdown();
        hge->Release();
    }

    /**
     * Runs the HGE application.
     */
    void run()
    {
        hge->System_Start();
    }
} // namespace hge

#endif // TESTS_INTEGRATION_HGE_HGE_HPP_
