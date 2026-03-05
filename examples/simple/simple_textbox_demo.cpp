#include <SDL2/SDL.h>

#include <fifechan/backends/opengl/openglgraphics.hpp>
#include <fifechan/backends/opengl/openglsdlimageloader.hpp>
#include <fifechan/backends/sdl2/sdlinput.hpp>

#include <fifechan.hpp>

#include <sstream>

/**
 * Demonstrates the minimal SDL2 + OpenGL integration path for Fifechan.
 *
 * The example creates an SDL window, wires SDL input into a FifeGUI GUI,
 * uses the OpenGL graphics backend and SDL-based image loader, loads a bitmap
 * font, and renders a single TextBox while processing the SDL event loop.
 */
int main(int argc, char** argv)
{
    SDL_Window* window                     = NULL;
    SDL_GLContext glContext                = nullptr;
    fcn::SDLInput* input                   = nullptr;
    fcn::OpenGLGraphics* graphics          = nullptr;
    fcn::OpenGLSDLImageLoader* imageLoader = nullptr;
    fcn::ImageFont* font                   = nullptr;
    fcn::Gui* gui                          = nullptr;
    fcn::Container* top                    = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return 1;
    }

    window = SDL_CreateWindow("Simple TextBox Demo", 0, 0, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (window == NULL) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    glContext = SDL_GL_CreateContext(window);
    if (glContext == nullptr) {
        std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    try {
        imageLoader = new fcn::OpenGLSDLImageLoader;
        fcn::Image::setImageLoader(imageLoader);

        graphics = new fcn::OpenGLGraphics(800, 600);
        input    = new fcn::SDLInput;

        font = new fcn::ImageFont(
            "rpgfont.png", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#[]\"");
        fcn::Widget::setGlobalFont(font);

        gui = new fcn::Gui;
        gui->setGraphics(graphics);
        gui->setInput(input);

        top = new fcn::Container;
        top->setDimension(fcn::Rectangle(0, 0, 800, 600));
        top->setOpaque(false);
        gui->setTop(top);

        fcn::TextBox* txtBox = new fcn::TextBox;
        txtBox->setDimension(fcn::Rectangle(100, 50, 200, 200));
        txtBox->setText("Hello from Fifechan!\nThis is a simple TextBox demo.");

        top->add(txtBox);

        bool running = true;
        SDL_Event evt;
        while (running) {
            while (SDL_PollEvent(&evt)) {
                if (evt.type == SDL_QUIT) {
                    running = false;
                }

                input->pushInput(evt);
            }

            gui->logic();
            gui->draw();
            SDL_GL_SwapWindow(window);
        }
    } catch (fcn::Exception const & exc) {
        std::cerr << exc.getMessage() << " " << exc.getLine() << "\n";
    }

    if (gui != nullptr) {
        gui->setTop(nullptr);
    }

    delete top;
    delete gui;
    delete font;
    delete input;
    delete graphics;

    fcn::Image::setImageLoader(nullptr);
    delete imageLoader;

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
