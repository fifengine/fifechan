// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include <SDL2/SDL.h>

#include <fifechan/backends/sdl2/sdl2graphics.hpp>
#include <fifechan/backends/sdl2/sdlimageloader.hpp>
#include <fifechan/backends/sdl2/sdlinput.hpp>

#include <fifechan.hpp>

#include <iostream>
#include <sstream>

/**
 * Test doesn't do any memory management in sake of
 * usability. (and my sanity :) )
 */

class MyActionListener : public fcn::ActionListener
{
    fcn::Container* mParent;

public:
    explicit MyActionListener(fcn::Container* parent)
    {
        mParent = parent;
    }

    ~MyActionListener() { }

    virtual void action(fcn::ActionEvent const & e)
    {
        fcn::Widget* w = e.getSource();

        if (w->getParent() == mParent) {
            mParent->remove(w);
        }
    }
};

int main(int argc, char** argv)
{
    SDL_Window* sdlWindow            = NULL;
    SDL_Renderer* renderer           = nullptr;
    fcn::SDLInput* input             = nullptr;
    fcn::SDL2Graphics* graphics      = nullptr;
    fcn::SDLImageLoader* imageLoader = nullptr;
    fcn::ImageFont* font             = nullptr;
    fcn::Gui* gui                    = nullptr;
    fcn::Container* top              = nullptr;
    int exitCode                     = 0;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return 1;
    }

    if (SDL_CreateWindowAndRenderer(800, 600, 0, &sdlWindow, &renderer) != 0) {
        std::cerr << "SDL_CreateWindowAndRenderer Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    SDL_SetWindowTitle(sdlWindow, "Spacers In VBox/HBox");

    try {
        imageLoader = new fcn::SDLImageLoader;
        imageLoader->setRenderer(renderer);
        fcn::Image::setImageLoader(imageLoader);

        graphics = new fcn::SDL2Graphics();
        graphics->setTarget(renderer, 800, 600);
        input = new fcn::SDLInput;

        font = new fcn::ImageFont("rpgfont.png", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
        fcn::Widget::setGlobalFont(font);

        gui = new fcn::Gui;
        gui->setGraphics(graphics);
        gui->setInput(input);

        top = new fcn::Container;
        top->setDimension(fcn::Rectangle(0, 0, 800, 600));
        top->setOpaque(false);
        gui->setTop(top);

        fcn::Container* testVBox = new fcn::Container;
        testVBox->setLayout(fcn::Container::LayoutPolicy::Vertical);
        testVBox->setVerticalSpacing(0);
        testVBox->setOpaque(false);

        fcn::Container* testHBox = new fcn::Container;
        testHBox->setLayout(fcn::Container::LayoutPolicy::Horizontal);
        testHBox->setHorizontalSpacing(0);
        testHBox->setOpaque(false);

        fcn::Spacer* spacer;

        fcn::Button* btn;
        for (int i = 0; i < 4; ++i) {
            std::stringstream s;
            s << "Button " << i;

            btn = new fcn::Button(s.str());
            btn->adjustSize();
            btn->addActionListener(new MyActionListener(testHBox));
            btn->setSize(100, 100);
            testHBox->add(btn);

            spacer = new fcn::Spacer;
            spacer->setSize(5, 1);
            testHBox->add(spacer);
        }

        testHBox->resizeToContent();

        testVBox->add(testHBox);

        fcn::Label* lbl;
        for (int i = 0; i < 5; ++i) {
            std::stringstream s;
            s << "Label " << i;

            lbl = new fcn::Label(s.str());
            lbl->adjustSize();
            testVBox->add(lbl);

            spacer = new fcn::Spacer;
            spacer->setSize(1, 5);
            testVBox->add(spacer);
        }

        btn = new fcn::Button("Another Button");
        btn->adjustSize();
        testVBox->add(btn);

        testVBox->resizeToContent();

        fcn::Window* demoWindow = new fcn::Window("Window");
        demoWindow->add(testVBox);
        demoWindow->adjustSize();

        top->add(demoWindow);

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
            SDL_SetRenderDrawColor(renderer, 40, 40, 48, 255);
            SDL_RenderClear(renderer);
            gui->draw();
            SDL_RenderPresent(renderer);
        }
    } catch (fcn::Exception const & e) {
        std::cerr << e.getMessage() << "\n";
        exitCode = 1;
    }

    if (gui != nullptr) {
        gui->setTop(nullptr);
    }

    delete top;
    delete gui;

    fcn::Widget::setGlobalFont(nullptr);
    delete font;

    delete input;
    delete graphics;

    fcn::Image::setImageLoader(nullptr);
    delete imageLoader;

    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
    }
    if (sdlWindow != nullptr) {
        SDL_DestroyWindow(sdlWindow);
    }
    SDL_Quit();

    return exitCode;
}
