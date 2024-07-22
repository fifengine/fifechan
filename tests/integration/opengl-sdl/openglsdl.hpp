
/*
 * Code that sets up an OpenGL application with Fifechan using the
 * Fifechan OpenGL back end and the SDL back end (as OpenGL cannot
 * load images nor check for user input an additional back end needs
 * to be used).
 */

#include <fifechan/gui.hpp>
#include <fifechan/opengl.hpp>

#include <fifechan.hpp>
// The openglsdlimageloader.hpp header file needs to be included
// in order to get the image loader that uses OpenGL and SDL.
#include <fifechan/opengl/openglsdlimageloader.hpp>
#include <fifechan/sdl.hpp>

namespace openglsdl
{
    bool running = true;

    SDL_Window* window;
    SDL_Surface* screen;

    // All back ends contain objects to make Fifechan work on a
    // specific target. They are a Graphics object to make Fifechan
    // able to draw itself using OpenGL, an input objec to make
    // Fifechan able to get user input using SDL and an ImageLoader
    // object to make Fifechan able to load images using OpenGL and SDL.
    fcn::OpenGLGraphics* graphics;
    fcn::SDLInput* input;
    fcn::OpenGLSDLImageLoader* imageLoader;

    fcn::Gui* gui;

    /**
     * Initialises the OpenGL and SDL application. This function creates the global
     * Gui object that can be populated by various examples.
     */
    void init()
    {
        // Initialize SDL with video and timer subsystems.
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
            printf("SDL_Init Error: %s\n", SDL_GetError());
            return;
        }

        // Create an SDL window with OpenGL context.
        window = SDL_CreateWindow(
            "My Game",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            640,
            480,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
        if (window == nullptr) {
            printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
            SDL_Quit();
            return;
        }

        // Create OpenGL context.
        SDL_GLContext glContext = SDL_GL_CreateContext(window);
        if (glContext == nullptr) {
            printf("SDL_GL_CreateContext Error: %s\n", SDL_GetError());
            SDL_DestroyWindow(window);
            SDL_Quit();
            return;
        }

        // Set the OpenGL viewport.
        glViewport(0, 0, 640, 480);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        // SDL_TEXTEDITING events are not sent,
        // and text that is being composed will be rendered in its own UI
        // SDL_SetHint(SDL_HINT_IME_INTERNAL_EDIT, 1);

        // Enable key repeat.
        SDL_StartTextInput();

        // Now it's time to initialise the Fifechan OpenGL back end
        // and the Fifechan SDL back end.

        imageLoader = new fcn::OpenGLSDLImageLoader();
        // The ImageLoader Fifechan should use needs to be passed to the Image object
        // using a static function.
        fcn::Image::setImageLoader(imageLoader);
        graphics = new fcn::OpenGLGraphics();
        // We need to tell the OpenGL Graphics object how big the screen is.
        graphics->setTargetPlane(640, 480);
        input = new fcn::SDLInput();

        // Now we create the Gui object to be used with this OpenGL
        // and SDL application.
        gui = new fcn::Gui();
        // The Gui object needs a Graphics to be able to draw itself and an Input
        // object to be able to check for user input. In this case we provide the
        // Gui object with OpenGL and SDL implementations of these objects hence
        // making Fifechan able to utilise OpenGL and SDL.
        gui->setGraphics(graphics);
        gui->setInput(input);
    }

    /**
     * Halts the OpenGL and SDL application.
     */
    void halt()
    {
        delete gui;

        delete imageLoader;
        delete input;
        delete graphics;

        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    /**
     * Runs the OpenGL and SDL application.
     */
    void run()
    {
        // The main loop
        while (running) {
            // Check user input
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

                // After we have manually checked user input with SDL for
                // any attempt by the user to halt the application we feed
                // the input to Fifechan by pushing the input to the Input
                // object.
                input->pushInput(event);
            }

            gui->logic();

            gui->draw();

            SDL_UpdateWindowSurface(window);
        }
    }
} // namespace openglsdl
