// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include <SDL2/SDL.h>

#include <fifechan/backends/sdl2/sdl2graphics.hpp>
#include <fifechan/backends/sdl2/sdlimageloader.hpp>
#include <fifechan/backends/sdl2/sdlinput.hpp>

#include <fifechan.hpp>

#include <iostream>
#include <memory>
#include <sstream>
#include <utility>
#include <vector>

class MyActionListener : public fcn::ActionListener
{
    fcn::Container* mParent;

public:
    explicit MyActionListener(fcn::Container* parent) : mParent(parent) { }

    void action(fcn::ActionEvent const & e) override
    {
        fcn::Widget* w = e.getSource();

        if (w->getParent() == mParent) {
            mParent->remove(w);
        }
    }
};

int main(int /*argc*/, char** /*argv*/)
{
    SDL_Window* sdlWindow  = nullptr;
    SDL_Renderer* renderer = nullptr;
    auto input             = std::unique_ptr<fcn::SDLInput>();
    auto graphics          = std::unique_ptr<fcn::SDL2Graphics>();
    auto imageLoader       = std::unique_ptr<fcn::SDLImageLoader>();
    auto font              = std::unique_ptr<fcn::ImageFont>();
    auto gui               = std::unique_ptr<fcn::Gui>();
    auto top               = std::unique_ptr<fcn::Container>();
    int exitCode           = 0;

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
        imageLoader = std::make_unique<fcn::SDLImageLoader>();
        imageLoader->setRenderer(renderer);
        fcn::Image::setImageLoader(imageLoader.get());

        graphics = std::make_unique<fcn::SDL2Graphics>();
        graphics->setTarget(renderer, 800, 600);
        input = std::make_unique<fcn::SDLInput>();

        font = std::make_unique<fcn::ImageFont>(
            "rpgfont.png", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
        fcn::Widget::setGlobalFont(font.get());

        gui = std::make_unique<fcn::Gui>();
        gui->setGraphics(graphics.get());
        gui->setInput(input.get());

        top = std::make_unique<fcn::Container>();
        top->setDimension(fcn::Rectangle(0, 0, 800, 600));
        top->setOpaque(false);
        gui->setTop(top.get());

        auto testVBox = std::make_unique<fcn::Container>();
        testVBox->setLayout(fcn::Container::LayoutPolicy::Vertical);
        testVBox->setVerticalSpacing(0);
        testVBox->setOpaque(false);

        auto testHBox = std::make_unique<fcn::Container>();
        testHBox->setLayout(fcn::Container::LayoutPolicy::Horizontal);
        testHBox->setHorizontalSpacing(0);
        testHBox->setOpaque(false);
        auto* testHBoxRaw = testHBox.get();

        std::vector<std::unique_ptr<MyActionListener>> actionListeners;

        for (int i = 0; i < 4; ++i) {
            std::stringstream s;
            s << "Button " << i;

            auto btn      = std::make_unique<fcn::Button>(s.str());
            auto listener = std::make_unique<MyActionListener>(testHBoxRaw);
            btn->adjustSize();
            btn->addActionListener(listener.get());
            btn->setSize(100, 100);
            testHBoxRaw->addWidget(std::move(btn));
            actionListeners.push_back(std::move(listener));

            auto spacer = std::make_unique<fcn::Spacer>();
            spacer->setSize(5, 1);
            testHBoxRaw->addWidget(std::move(spacer));
        }

        testHBoxRaw->resizeToContent();

        testVBox->addWidget(std::move(testHBox));

        for (int i = 0; i < 5; ++i) {
            std::stringstream s;
            s << "Label " << i;

            auto lbl = std::make_unique<fcn::Label>(s.str());
            lbl->adjustSize();
            testVBox->addWidget(std::move(lbl));

            auto spacer = std::make_unique<fcn::Spacer>();
            spacer->setSize(1, 5);
            testVBox->addWidget(std::move(spacer));
        }

        auto btn = std::make_unique<fcn::Button>("Another Button");
        btn->adjustSize();
        testVBox->addWidget(std::move(btn));

        testVBox->resizeToContent();

        auto demoWindow = std::make_unique<fcn::Window>("Window");
        demoWindow->addWidget(std::move(testVBox));
        demoWindow->adjustSize();

        top->addWidget(std::move(demoWindow));

        bool running = true;
        SDL_Event evt;
        while (running) {
            while (SDL_PollEvent(&evt) != 0) {
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

    fcn::Widget::setGlobalFont(nullptr);

    fcn::Image::setImageLoader(nullptr);

    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
    }
    if (sdlWindow != nullptr) {
        SDL_DestroyWindow(sdlWindow);
    }
    SDL_Quit();

    return exitCode;
}
