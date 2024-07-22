
/**
 * This is a simple Hello World example with Fifechan using the SDL backend.
 */

#include <SDL2/SDL.h>

#include <fifechan/gui.hpp>
#include <fifechan/sdl.hpp>

#include <fifechan.hpp>

#include <iostream>

bool running = true;

// SDL specific objects
SDL_Window* window;
SDL_Renderer* renderer;

// Fifechan SDL objects
fcn::SDL2Graphics* graphics;
fcn::SDLInput* input;
fcn::SDLImageLoader* imageLoader;

// Fifechan objects
fcn::Gui* gui;

// Fifechan widgets
fcn::Container* top;
fcn::ImageFont* font;
fcn::Label* label;

/**
 * Initialises the SDL application.
 * We create the SDL window and renderer and initialising the Fifechan SDL backend.
 */
void init_sdl()
{
    // We setup an SDL window and renderer.
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "FifeGUI - SDL Hello World");

    // SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    // Get the current display mode.
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    // Calculate the center position.
    int xPos = (current.w - 640) / 2;
    int yPos = (current.h - 480) / 2;
    // Set the window position to the center.
    SDL_SetWindowPosition(window, xPos, yPos);

    // Now it's time to initialise the Fifechan SDL back end.

    // The SDLImageLoader object is used to load images from the file system.
    imageLoader = new fcn::SDLImageLoader();
    imageLoader->setRenderer(renderer);
    fcn::Image::setImageLoader(imageLoader);

    // The SDLGraphics object is used to draw to the screen.
    graphics = new fcn::SDL2Graphics();
    graphics->setTarget(renderer, 640, 480);

    // The SDLInput object is used to get input from the user.
    input = new fcn::SDLInput();

    // Finally, we create the Gui object and pass graphics and input to it.
    gui = new fcn::Gui();
    gui->setGraphics(graphics);
    gui->setInput(input);
}

/**
 * Initialises the Hello World example by populating the Gui object.
 */
void init_gui()
{
    // We first create a container to be used as the top widget.
    // The top widget in Fifechan can be any kind of widget, but
    // in order to make the Gui contain more than one widget we
    // make the top widget a container.
    top = new fcn::Container();
    // We set the dimension of the top container to match the screen.
    top->setDimension(fcn::Rectangle(0, 0, 640, 480));
    // Finally we pass the top widget to the Gui object.
    gui->setTop(top);

    // Now we load the font used in this example.
    font = new fcn::ImageFont("fixedfont.bmp", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    // Widgets may have a global font so we don't need to pass the
    // font object to every created widget. The global font is static.
    fcn::Widget::setGlobalFont(font);

    // Now we create a label with the text "Hello World".
    label = new fcn::Label("Hello World");
    // We give the label a position.
    label->setPosition(280, 220);
    // And finally we add the label to the top container.
    top->add(label);
}

/**
 * Halts the Hello World example.
 */
void halt()
{
    // Cleanup Fifechan widgets used in the GUI
    delete label;
    delete font;
    delete top;

    // Cleanup Fifechan objects
    delete gui;

    // Cleanup Fifechan SDL objects
    delete imageLoader;
    delete input;
    delete graphics;

    // Cleanup SDL
    SDL_DestroyWindow(window);
    SDL_Quit();
}

/**
 * Runs the SDL application.
 */
void run()
{
    // This is the main loop of the application.
    // We will run this loop until the user closes the window.
    // The loop will update the GUI and draw it to the screen.
    // It will also check for user input and pass it to Fifechan.
    // Its two loops in one, one for SDL and one for Fifechan.
    // The SDL loop is used to check for user input that is not
    // handled by Fifechan, such as window close events.
    // The Fifechan loop is used to check for user input that is
    // handled by Fifechan, such as mouse clicks and key presses.
    // The Fifechan loop is also responsible for udpating the
    // GUI logic and drawing the GUI to the screen.
    while (running) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
                if (event.key.keysym.sym == SDLK_f) {
                    if (event.key.keysym.mod & KMOD_CTRL) {
                        // Works with X11 only
                        // SDL_WM_ToggleFullScreen(screen);
                    }
                }
            } else if (event.type == SDL_QUIT) {
                running = false;
            }

            // After we have checked for SDL events we pass the event to Fifechan.
            input->pushInput(event);
        }

        // Then we update the GUI logic.
        gui->logic();

        // And finally we draw the GUI to the screen.
        gui->draw();

        // We need to update the screen to make our changes visible.
        SDL_UpdateWindowSurface(window);
    }
}

int main(int argc, char** argv)
{
    try {
        init_sdl();
        init_gui();
        run();
        halt();
    } catch (fcn::Exception e) {
        std::cerr << e.getMessage() << std::endl;
        return 1;
    } catch (std::exception e) {
        std::cerr << "Std exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception" << std::endl;
        return 1;
    }

    return 0;
}
