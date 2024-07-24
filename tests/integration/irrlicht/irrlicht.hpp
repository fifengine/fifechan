// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors
#ifndef TESTS_INTEGRATION_IRRLICHT_IRRLICHT_HPP_
#define TESTS_INTEGRATION_IRRLICHT_IRRLICHT_HPP_

/**
 * @brief This example shows the widgets present in FifeGUI using the Irrlicht backend.
 */

#include <fifechan/backends/irrlicht/irrlicht.hpp>

#include <fifechan.hpp>

namespace irrlicht
{
    // Irrlicht requires you implement an event receiver
    // which will automatically receive events. We can pass
    // those events off to the input handler then.
    class MyEventReceiver : public irr::IEventReceiver
    {
    public:
        explicit MyEventReceiver(fcn::IrrlichtInput* input) : mInput(input) { }

        bool OnEvent(irr::SEvent const & event)
        {
            mInput->pushInput(event);
            return false;
        }

    private:
        fcn::IrrlichtInput* mInput;
    };

    irr::IrrlichtDevice* device;
    MyEventReceiver* receiver;

    // Here the following backend objects are necessary for FifeGUI to work
    // with Irrlicht. These objects include:
    // A Graphics object, allowing the GUI to render using Irrlicht.
    // An Input object, enabling the GUI to receive user input via Irrlicht.
    // An ImageLoader object, allowing the loading of images using Irrlicht.
    fcn::IrrlichtGraphics* graphics;
    fcn::IrrlichtInput* input;
    fcn::IrrlichtImageLoader* imageLoader;

    /**
     * Initialises the Irrlicht application. This function creates the global
     * Gui object that can be populated by various examples.
     */
    void init()
    {
        // We simply initialise Irrlicht as we would do with any Irrlicht application.
        device = irr::createDevice(
            irr::video::EDT_OPENGL, irr::core::dimension2d<irr::s32>(640, 480), 16, false, false, false, 0);

        // Now it's time to initialise the Irrlicht backend.

        imageLoader = new fcn::IrrlichtImageLoader(device->getVideoDriver());
        // Set the ImageLoader by calling a static function of the Image class.
        fcn::Image::setImageLoader(imageLoader);
        graphics = new fcn::IrrlichtGraphics(device->getVideoDriver());

        input    = new fcn::IrrlichtInput(device);
        receiver = new MyEventReceiver(input);
        device->setEventReceiver(receiver);

        // Finally, we create the Gui object and pass graphics and input to it.
        globals::gui = new fcn::Gui();
        globals::gui->setGraphics(graphics);
        globals::gui->setInput(input);
    }

    /**
     * Halts the Irrlicht application.
     */
    void halt()
    {
        delete globals::gui;

        delete imageLoader;
        delete receiver;
        delete input;
        delete graphics;

        device->drop();
    }

    /**
     * Runs the Irrlicht application.
     */
    void run()
    {
        // The main loop
        while (device->run()) {
            // Begin the rendering
            device->getVideoDriver()->beginScene(true, true, irr::video::SColor(255, 100, 101, 140));
            // Now we let the Gui object perform its logic.
            globals::gui->logic();
            // Now we let the Gui object draw itself.
            globals::gui->draw();
            // End the rendering
            device->getVideoDriver()->endScene();
        }
    }
} // namespace irrlicht

#endif // TESTS_INTEGRATION_IRRLICHT_IRRLICHT_HPP_
