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

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#ifdef _WIN32
    #include <windows.h>
#endif
using tests::integration::sdl::helloworld::Application;

Application::Application(std::string const & title, int width, int height)
{
    init_sdl(title, width, height);
    init_gui(width, height);
}

Application::~Application()
{
    cleanup();
}

std::shared_ptr<SDL_Window> Application::initWindow(std::string const & title, int width, int height, int flags)
{
    SDL_Window* rawWindow =
        SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    if (rawWindow == nullptr) {
        throw std::runtime_error(std::string("Failed to create SDL_Window: ") + SDL_GetError());
    }

    return fcn::sdl2::makeSDLSharedPtr(rawWindow);
}

std::shared_ptr<SDL_Renderer> Application::initRenderer(std::shared_ptr<SDL_Window> const & window, int flags)
{
    SDL_Renderer* rawRenderer = SDL_CreateRenderer(window.get(), -1, flags);
    if (rawRenderer == nullptr) {
        std::string const rendererError = SDL_GetError();
        rawRenderer                     = SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_SOFTWARE);
        if (rawRenderer == nullptr) {
            throw std::runtime_error(
                std::string("Failed to create SDL_Renderer: ") + rendererError + " -> " + SDL_GetError());
        }
    }

    return fcn::sdl2::makeSDLSharedPtr(rawRenderer);
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
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error(std::string("Failed to initialize SDL: ") + SDL_GetError());
    }

    auto const windowFlags = static_cast<int>(SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    window                 = initWindow(title, width, height, windowFlags);

    auto const rendererFlags = static_cast<int>(SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    renderer                 = initRenderer(window, rendererFlags);

    // Now it's time to initialise the SDL backend.

    // The SDLImageLoader object is used to load images from the file system.
    imageLoader = std::make_shared<fcn::sdl2::ImageLoader>();
    imageLoader->setRenderer(renderer.get());

    // Set the ImageLoader by calling a static function of the Image class.
    fcn::Image::setImageLoader(imageLoader.get());

    // The SDLGraphics object is used to draw to the screen.
    graphics = std::make_unique<fcn::sdl2::Graphics>();
    graphics->setTarget(renderer.get(), width, height);

    // The SDLInput object is used to get input from the user.
    input = std::make_unique<fcn::sdl2::Input>();

    // Finally, we create the Gui object and pass graphics and input to it.
    gui = std::make_unique<fcn::Gui>();
    gui->setGraphics(std::move(graphics));
    gui->setInput(input.get());
}

/**
 * Initialises the Hello World example by populating the Gui object.
 */
void Application::init_gui(int width, int height)
{
    // We first create a container to be used as the top widget.
    // The top widget can be any kind of widget, but in order to make the
    // Gui contain more than one widget we make the top widget a container.
    top = std::make_unique<fcn::Container>();
    top->setLayout(fcn::Container::LayoutPolicy::Absolute);
    top->setMinSize(fcn::Size(width, height));
    top->setMaxSize(fcn::Size(width, height));
    top->setFixedSize(fcn::Size(width, height));
    top->setDimension(fcn::Rectangle(0, 0, width, height));
    gui->setTop(top.get());

    // Load and set the classic fixed bitmap font globally.
    font = std::make_unique<fcn::ImageFont>(
        "fixedfont.bmp", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    fcn::Widget::setGlobalFont(font.get());

    // Load RPG bitmap font for the second label.
    rpgFont = std::make_unique<fcn::ImageFont>(
        "rpgfont.png", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#[]\"");

    // Now we create a label with the text "Hello World".
    label = std::make_unique<fcn::Label>("Hello World");

    // Create a second label with the RPG font.
    label2 = std::make_unique<fcn::Label>("Hello from FifeGui");
    label2->setFont(rpgFont.get());

    int const labelX  = std::max(0, (width - label->getWidth()) / 2);
    int const labelY  = std::max(0, (height / 2) - label->getHeight());
    int const label2X = std::max(0, (width - label2->getWidth()) / 2);
    int const label2Y = std::max(0, (height / 2) + 10);

    // And finally we add the label to the top container.
    top->add(label.get(), labelX, labelY);
    top->add(label2.get(), label2X, label2Y);
}

/**
 * Halts the Hello World example.
 */
void Application::cleanup()
{
    // Reset global GUI hooks before releasing the GUI-owned widget tree.
    fcn::Widget::setGlobalFont(nullptr);
    fcn::Image::setImageLoader(nullptr);

    label2.reset();
    label.reset();
    rpgFont.reset();
    font.reset();
    gui.reset();
    top.reset();
    imageLoader.reset();
    input.reset();
    graphics.reset();

    // Cleanup SDL
    renderer.reset();
    window.reset();
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
                        uint32_t const fullscreen = SDL_GetWindowFlags(window.get()) & SDL_WINDOW_FULLSCREEN_DESKTOP;
                        SDL_SetWindowFullscreen(window.get(), fullscreen ^ SDL_WINDOW_FULLSCREEN_DESKTOP);
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
        SDL_RenderPresent(renderer.get());
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
