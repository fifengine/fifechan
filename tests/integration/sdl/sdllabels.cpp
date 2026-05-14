// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

/**
 * @brief Test application demonstrating all 9 combinations of
 *        horizontal and vertical alignment for Label widgets.
 */

// Corresponding header include
#include "sdllabels.hpp"

// Standard library includes
#include <algorithm>
#include <format>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

// Platform config include
#include "fifechan/platform.hpp"

#ifdef _WIN32
    #include <windows.h>
#endif

// Third-party library includes
#include <SDL3/SDL_main.h>

#include <fifechan/backends/sdl3/sdl.hpp>

#include <fifechan.hpp>

// Project headers (subdirs before local)
#include <fifechan/gui.hpp>

using tests::integration::sdl::labels::Application;

Application::Application(std::string const & title, int width, int height)
{
    try {
        init_sdl(title, width, height);
        init_gui(width, height);
    } catch (...) {
        cleanup();
        throw;
    }
}

Application::~Application()
{
    cleanup();
}

std::shared_ptr<SDL_Window> Application::initWindow(std::string const & title, int width, int height, int flags)
{
    SDL_Window* window = SDL_CreateWindow(title.c_str(), width, height, flags);

    if (window == nullptr) {
        throw std::runtime_error(std::string("Failed to create SDL_Window: ") + SDL_GetError());
    }

    return fcn::sdl3::makeSDLSharedPtr(window);
}

std::shared_ptr<SDL_Renderer> Application::initRenderer(std::shared_ptr<SDL_Window> const & window)
{
    SDL_Renderer* renderer = SDL_CreateRenderer(window.get(), nullptr);

    if (renderer == nullptr) {
        throw std::runtime_error(std::string("Failed to create SDL_Renderer: ") + SDL_GetError());
    }

    return fcn::sdl3::makeSDLSharedPtr(renderer);
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
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error(std::string("Failed to initialize SDL: ") + SDL_GetError());
    }

    window = initWindow(title, width, height, 0);

    renderer = initRenderer(window);

    // Now it's time to initialise the SDL backend.

    // The SDLImageLoader object is used to load images from the file system.
    imageLoader = std::make_shared<fcn::sdl3::ImageLoader>();
    imageLoader->setRenderer(renderer.get());

    // Set the ImageLoader by calling a static function of the Image class.
    fcn::Image::setImageLoader(imageLoader.get());

    // The SDLGraphics object is used to draw to the screen.
    graphics = std::make_unique<fcn::sdl3::Graphics>();
    graphics->setTarget(renderer.get(), width, height);

    // The SDLInput object is used to get input from the user.
    input = std::make_unique<fcn::sdl3::Input>();

    // Finally, we create the GUI object and pass graphics and input to it.
    gui = std::make_unique<fcn::Gui>();
    gui->setGraphics(std::move(graphics));
    gui->setInput(input.get());
}

/**
 * Initialises the Label Alignment test by creating a 3x3 grid of labels.
 * Each cell demonstrates a unique combination of horizontal and vertical alignment.
 */
void Application::init_gui(int width, int height)
{
    // We first create a container to be used as the top widget.
    top = std::make_unique<fcn::Container>();
    top->setLayout(fcn::Container::LayoutPolicy::Absolute);
    top->setMinSize(fcn::Size(width, height));
    top->setMaxSize(fcn::Size(width, height));
    top->setFixedSize(fcn::Size(width, height));
    top->setDimension(fcn::Rectangle(0, 0, width, height));
    gui->setTop(top.get());

    // Load the classic fixed bitmap font globally.
    font = std::make_unique<fcn::ImageFont>(
        "fixedfont.bmp", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    fcn::Widget::setGlobalFont(font.get());

    // Define alignment combinations
    std::array<fcn::Graphics::Alignment, 3> hAlignments = {
        fcn::Graphics::Alignment::Left, fcn::Graphics::Alignment::Center, fcn::Graphics::Alignment::Right};

    std::array<fcn::Graphics::VerticalAlignment, 3> vAlignments = {
        fcn::Graphics::VerticalAlignment::Top,
        fcn::Graphics::VerticalAlignment::Center,
        fcn::Graphics::VerticalAlignment::Bottom};

    char const const * hLabels[] = {"Left", "Center", "Right"};
    char const const * vLabels[] = {"Top", "Center", "Bottom"};

    // Calculate grid cell dimensions
    int const cellWidth  = width / 3;
    int const cellHeight = height / 3;

    // Create 3x3 grid of labels
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            // Create a container for this cell with border to visualize bounds
            auto cell = std::make_unique<fcn::Container>();
            cell->setBorderSize(2);
            cell->setDimension(fcn::Rectangle(col * cellWidth, row * cellHeight, cellWidth, cellHeight));

            // Create label with descriptive text
            auto label = std::make_unique<fcn::Label>(std::format("{} {}", vLabels[row], hLabels[col]));

            // Set horizontal alignment
            label->setAlignment(hAlignments[col]);

            // Set vertical alignment
            label->setVerticalAlignment(vAlignments[row]);

            // Make label fill the entire cell to demonstrate alignment
            label->setBorderSize(1);
            label->setDimension(fcn::Rectangle(0, 0, cellWidth, cellHeight));

            // Add label to cell, add cell to top container
            auto* labelPtr = label.get();
            cell->add(label.release(), 0, 0);
            top->add(cell.get(), col * cellWidth, row * cellHeight);

            // Store pointers for cleanup
            labels.push_back(std::unique_ptr<fcn::Label>(labelPtr));
            cells.push_back(std::move(cell));
        }
    }
}

/**
 * Cleans up the SDL application.
 */
void Application::cleanup()
{
    // Reset global GUI hooks before releasing the GUI-owned widget tree.
    fcn::Widget::resetGlobalFont();
    fcn::Image::resetImageLoader();

    labels.clear();
    cells.clear();
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
    while (this->running) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_ESCAPE) {
                    this->running = false;
                }
                if (event.key.key == SDLK_F) {
                    if ((event.key.mod & SDL_KMOD_CTRL) != 0) {
                        uint32_t const fullscreen = SDL_GetWindowFlags(window.get()) & SDL_WINDOW_FULLSCREEN;
                        SDL_SetWindowFullscreen(window.get(), fullscreen != 0 ? false : SDL_WINDOW_FULLSCREEN);
                    }
                }
            } else if (event.type == SDL_EVENT_QUIT) {
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
        // Append library version to window title
        std::string const fifeguiVersion = fcn::fifechanVersion();

        int const sdlVersion            = SDL_GetVersion();
        std::string const sdlVersionStr = std::format(
            "{}.{}.{}",
            SDL_VERSIONNUM_MAJOR(sdlVersion),
            SDL_VERSIONNUM_MINOR(sdlVersion),
            SDL_VERSIONNUM_MICRO(sdlVersion));

        std::string const title =
            std::format("FifeGUI v{} using SDL {}: Label Alignment Test", fifeguiVersion, sdlVersionStr);

        Application app(title, 1024, 768);
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

    return 0;
}
