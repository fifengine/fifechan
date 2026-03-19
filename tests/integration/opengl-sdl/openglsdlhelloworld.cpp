// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

/**
 * @brief This is a HelloWorld example with FifeGUI using the OpenGL SDL backend.
 */

#include "openglsdlhelloworld.hpp"

#include <fifechan/backends/opengl/graphics.hpp>
#include <fifechan/gui.hpp>

#include <fifechan.hpp>

#include <algorithm>
#include <format>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#ifdef _WIN32
    #include <windows.h>
#endif

using tests::integration::opengl_sdl::helloworld::Application;

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

SDL_Window* Application::initWindow(std::string const & title, int width, int height, int flags)
{
    SDL_Window* rawWindow =
        SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    if (rawWindow == nullptr) {
        throw std::runtime_error(std::string("Failed to create SDL_Window: ") + SDL_GetError());
    }

    return rawWindow;
}

SDL_GLContext Application::initGLContext(SDL_Window* window)
{
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context == nullptr) {
        throw std::runtime_error(std::string("Failed to create SDL_GLContext: ") + SDL_GetError());
    }

    return context;
}

void Application::init_sdl(std::string const & title, int width, int height)
{
    std::filesystem::current_path(Application::getExecutableDir());

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        throw std::runtime_error(std::string("Failed to initialize SDL: ") + SDL_GetError());
    }

    auto const windowFlags = static_cast<int>(SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    window                 = initWindow(title, width, height, windowFlags);

    glContext = initGLContext(window);

    glViewport(0, 0, width, height);
    glClearColor(0.0F, 0.0F, 0.0F, 0.0F);

    SDL_StartTextInput();

    imageLoader = std::make_shared<fcn::opengl::ImageLoader>();
    fcn::Image::setImageLoader(imageLoader.get());

    graphics = std::make_unique<fcn::opengl::Graphics>();
    graphics->setTargetPlane(width, height);

    input = std::make_unique<fcn::sdl2::Input>();

    gui = std::make_unique<fcn::Gui>();
    gui->setGraphics(std::move(graphics));
    gui->setInput(input.get());
}

void Application::init_gui(int width, int height)
{
    top = std::make_unique<fcn::Container>();
    top->setLayout(fcn::Container::LayoutPolicy::Absolute);
    top->setMinSize(fcn::Size(width, height));
    top->setMaxSize(fcn::Size(width, height));
    top->setFixedSize(fcn::Size(width, height));
    top->setDimension(fcn::Rectangle(0, 0, width, height));
    gui->setTop(top.get());

    font = std::make_unique<fcn::ImageFont>(
        "fixedfont.bmp", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    fcn::Widget::setGlobalFont(font.get());

    label = std::make_unique<fcn::Label>("Hello World");

    int const labelX = std::max(0, (width - label->getWidth()) / 2);
    int const labelY = std::max(0, (height - label->getHeight()) / 2);

    top->add(label.get(), labelX, labelY);
}

void Application::cleanup()
{
    fcn::Widget::setGlobalFont(nullptr);
    fcn::Image::setImageLoader(nullptr);

    label.reset();
    font.reset();
    gui.reset();
    top.reset();
    imageLoader.reset();
    input.reset();
    graphics.reset();

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Application::run()
{
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
                if (event.key.keysym.sym == SDLK_f) {
                    if ((event.key.keysym.mod & KMOD_CTRL) != 0) {
                        uint32_t const fullscreen = SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP;
                        SDL_SetWindowFullscreen(window, fullscreen ^ SDL_WINDOW_FULLSCREEN_DESKTOP);
                    }
                }
            } else if (event.type == SDL_QUIT) {
                running = false;
            }

            input->pushInput(event);
        }

        glClear(GL_COLOR_BUFFER_BIT);
        gui->logic();
        gui->draw();
        SDL_GL_SwapWindow(window);
    }
}

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    try {
        std::string const fifeguiVersion = fcn::fifechanVersion();
        std::string const title =
            std::format("FifeGUI v{} using OpenGL SDL2 Backend: Hello World Example", fifeguiVersion);
        Application app(title, 640, 480);
        app.run();
    } catch (fcn::Exception const & e) {
        std::cerr << e.getMessage() << '\n';
        return 1;
    } catch (std::exception const & e) {
        std::cerr << "Std exception: " << e.what() << '\n';
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception" << '\n';
        return 1;
    }

    return 0;
}
