/*
 * Code that sets up an OpenLayer application with Guichan using the 
 * Guichan OpenLayer back end.
 */

#include <fifechan.hpp>
#include <fifechan/openlayer.hpp>

namespace openlayer
{
    // All back ends contain objects to make Guichan work on a
    // specific target - in this case OpenLayer - and they are a Graphics
    // object to make Guichan able to draw itself using OpenLayer, an
    // input objec to make Guichan able to get user input using OpenLayer
    // and an ImageLoader object to make Guichan able to load images
    // using OpenLayer.
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
    
        // Now it's time to initialise the Guichan OpenLayer back end.

        imageLoader = new fcn::OpenLayerImageLoader();
        // The ImageLoader Guichan should use needs to be passed to the Image object
        // using a static function.
        fcn::Image::setImageLoader(imageLoader);
        graphics = new fcn::OpenLayerGraphics();
        // We need to tell the OpenLayer Graphics object how big the screen is.
        graphics->setTargetPlane(640, 480);
        input = new fcn::OpenLayerInput();

        // Now we create the Gui object to be used with this OpenLayer application.
        globals::gui = new fcn::Gui();
        // The Gui object needs a Graphics to be able to draw itself and an Input
        // object to be able to check for user input. In this case we provide the
        // Gui object with OpenLayer implementations of these objects hence making Guichan
        // able to utilise OpenLayer.
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
    
        if (!mouse_bmp )
        {
            ol::OlError("Pointer not loaded.");
        }
    
        while(!key[KEY_ESC])
        {
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
}
