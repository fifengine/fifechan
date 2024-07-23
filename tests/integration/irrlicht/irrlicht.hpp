// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors
#ifndef TESTS_INTEGRATION_IRRLICHT_IRRLICHT_HPP_
#define TESTS_INTEGRATION_IRRLICHT_IRRLICHT_HPP_

/*
 * Code that sets up an Irrlicht application with Fifechan using the
 * Fifechan Irrlicht back end.
 */

#include <fifechan/irrlicht.hpp>

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

    // All back ends contain objects to make Fifechan work on a
    // specific target - in this case Irrlicht - and they are a Graphics
    // object to make Fifechan able to draw itself using Irrlicht, an
    // input object to make Fifechan able to get user input using Irrlicht
    // and an ImageLoader object to make Fifechan able to load images
    // using Irrlicht.
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

        // Now it's time to initialise the Fifechan Irrlicht back end.

        imageLoader = new fcn::IrrlichtImageLoader(device->getVideoDriver());
        // The ImageLoader Fifechan should use needs to be passed to the Image object
        // using a static function.
        fcn::Image::setImageLoader(imageLoader);
        graphics = new fcn::IrrlichtGraphics(device->getVideoDriver());

        input    = new fcn::IrrlichtInput(device);
        receiver = new MyEventReceiver(input);
        device->setEventReceiver(receiver);

        // Now we create the Gui object to be used with this Irrlicht application.
        globals::gui = new fcn::Gui();
        // The Gui object needs a Graphics to be able to draw itself and an Input
        // object to be able to check for user input. In this case we provide the
        // Gui object with Irrlicht implementations of these objects hence making Fifechan
        // able to utilise Irrlicht.
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
