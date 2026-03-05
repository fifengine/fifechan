// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

/**
 * @brief This is a HelloWorld example with FifeGUI using the SDL backend.
 */

#include "sdlhelloworld.hpp"

#include <fifechan/backends/sdl2/sdl.hpp>
#include <fifechan/gui.hpp>

#include <fifechan.hpp>

#include <iostream>

#ifdef _WIN32
    #include <windows.h>
#endif

Application::Application(std::string const & title, int width, int height)
{
    init_sdl(title, width, height);
    init_gui(width, height);
}

Application::~Application()
{
    cleanup();
}

std::filesystem::path Application::getExecutableDir()
{
#ifdef __linux__
    return std::filesystem::read_symlink("/proc/self/exe").parent_path();
#elif defined(_WIN32)
    char buffer[MAX_PATH] = {0};
    GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    return std::filesystem::path(buffer).parent_path();
#else
    return std::filesystem::current_path();
#endif
}

/**
 * Initialises the SDL application.
 * We create the SDL window and renderer and initialising the SDL backend.
 */
void Application::init_sdl(std::string const & title, int width, int height)
{
    std::filesystem::current_path(Application::getExecutableDir());

    // We setup an SDL window and renderer.
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
    SDL_SetWindowTitle(window, title.c_str());

    // SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    // Get the current display mode.
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    // Calculate the center position.
    int const xPos = (current.w - width) / 2;
    int const yPos = (current.h - height) / 2;
    // Set the window position to the center.
    SDL_SetWindowPosition(window, xPos, yPos);

    // Now it's time to initialise the SDL backend.

    // The SDLImageLoader object is used to load images from the file system.
    imageLoader = new fcn::SDLImageLoader();
    imageLoader->setRenderer(renderer);

    // Set the ImageLoader by calling a static function of the Image class.
    fcn::Image::setImageLoader(imageLoader);

    // The SDLGraphics object is used to draw to the screen.
    graphics = new fcn::SDL2Graphics();
    graphics->setTarget(renderer, width, height);

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
void Application::init_gui(int width, int height)
{
    // We first create a container to be used as the top widget.
    // The top widget can be any kind of widget, but in order to make the
    // Gui contain more than one widget we make the top widget a container.
    top = new fcn::Container();
    // We set the dimension of the top container to match the screen.
    top->setDimension(fcn::Rectangle(0, 0, width, height));
    // Finally we pass the top widget to the Gui object.
    gui->setTop(top);

    // Load and set the classic fixed bitmap font globally.
    font = new fcn::ImageFont("fixedfont.bmp", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    fcn::Widget::setGlobalFont(font);

    // Load RPG bitmap font for the second label.
    rpgFont = new fcn::ImageFont(
        "rpgfont.png", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#[]\"");

    // Now we create a label with the text "Hello World".
    label = new fcn::Label("Hello World");
    // We give the first label a position.
    label->setPosition(280, 200);

    // Create a second label with the RPG font.
    label2 = new fcn::Label("Hello from FifeGui");
    label2->setFont(rpgFont);
    label2->setPosition(180, 240);

    // And finally we add the label to the top container.
    top->add(label);
    top->add(label2);
}

/**
 * Halts the Hello World example.
 */
void Application::cleanup()
{
    // Cleanup FifeGUI widgets used in the GUI
    delete label2;
    delete label;
    delete rpgFont;
    delete font;
    delete top;

    // Cleanup FifeGUI objects
    delete gui;

    // Cleanup FifeGUI SDL objects
    delete imageLoader;
    delete input;
    delete graphics;

    // Cleanup SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

/**
 * Runs the SDL application.
 */
void Application::run()
{
    // This is the main loop of the application.
    // We will run this loop until the user closes the window.
    // The loop will update the GUI and draw it to the screen.
    // It will also check for user input and pass it to FifeGUI.
    //
    // It is two loops in one, one for SDL and one for FifeGUI.
    // The SDL loop is used to check for user input that is not
    // handled by FifeGUI, such as window close events.
    // The FifeGUI loop is used to check for user input that is
    // handled by FifeGUI, such as mouse clicks and key presses.
    // The FifeGUI loop is also responsible for updating the
    // GUI logic and drawing the GUI to the screen.
    while (this->running) {

        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    this->running = false;
                }
                if (event.key.keysym.sym == SDLK_f) {
                    if ((event.key.keysym.mod & KMOD_CTRL) != 0) {
                        // Works with X11 only
                        // SDL_WM_ToggleFullScreen(screen);
                    }
                }
            } else if (event.type == SDL_QUIT) {
                this->running = false;
            }

            // After checking SDL events we forward the events to the GUI.
            input->pushInput(event);
        }

        // Then we update the GUI logic.
        gui->logic();

        // And finally we draw the GUI to the screen.
        gui->draw();

        // We need to update the screen to make our changes visible.
        SDL_RenderPresent(renderer);
    }
}

int main(int argc, char** argv)
{
    (void)argc; // Unused variable.
    (void)argv; // Unused variable.

    try {
        Application app("FifeGUI - SDL Hello World", 640, 480);
        app.run();
    } catch (fcn::Exception const & e) {
        // catch Fifegui exceptions
        std::cerr << e.getMessage() << '\n';
        return 1;
    } catch (std::exception const & e) {
        // catch std exceptions
        std::cerr << "Std exception: " << e.what() << '\n';
        return 1;
    } catch (...) {
        // catch all other exceptions
        std::cerr << "Unknown exception" << '\n';
        return 1;
    }

    // std::cerr << "[DEBUG] Application exited successfully" << '\n';

    return 0;
}
