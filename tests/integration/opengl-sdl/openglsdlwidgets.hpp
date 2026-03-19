// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef TESTS_INTEGRATION_OPENGL_SDL_OPENGLSDLWIDGETS_HPP_
#define TESTS_INTEGRATION_OPENGL_SDL_OPENGLSDLWIDGETS_HPP_

#include <fifechan/backends/opengl/imageloader.hpp>
#include <fifechan/backends/opengl/opengl.hpp>
#include <fifechan/backends/sdl2/sdl.hpp>
#include <fifechan/exception.hpp>
#include <fifechan/gui.hpp>

#include <fifechan.hpp>

#include <cstdio>
#include <memory>
#include <string>

namespace openglsdl
{
    inline bool running = true;

    inline SDL_Window* window = nullptr;

    inline std::unique_ptr<fcn::opengl::Graphics> graphics;
    inline std::unique_ptr<fcn::sdl2::Input> input;
    inline std::unique_ptr<fcn::opengl::ImageLoader> imageLoader;

    inline std::unique_ptr<fcn::Gui> gui;

    inline void init()
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
            printf("SDL_Init Error: %s\n", SDL_GetError());
            return;
        }

        window = SDL_CreateWindow(
            "Widgets",
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

        SDL_GLContext glContext = SDL_GL_CreateContext(window);
        if (glContext == nullptr) {
            printf("SDL_GL_CreateContext Error: %s\n", SDL_GetError());
            SDL_DestroyWindow(window);
            SDL_Quit();
            return;
        }

        glViewport(0, 0, 640, 480);
        glClearColor(0.0F, 0.0F, 0.0F, 0.0F);

        SDL_StartTextInput();

        imageLoader = std::make_unique<fcn::opengl::ImageLoader>();
        fcn::Image::setImageLoader(imageLoader.get());
        graphics = std::make_unique<fcn::opengl::Graphics>();
        graphics->setTargetPlane(640, 480);
        input = std::make_unique<fcn::sdl2::Input>();

        gui = std::make_unique<fcn::Gui>();
        gui->setGraphics(graphics.get());
        gui->setInput(input.get());
    }

    inline void halt()
    {
        gui.reset();
        imageLoader.reset();
        input.reset();
        graphics.reset();
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    inline void run()
    {
        while (running) {
            SDL_Event event;
            while (SDL_PollEvent(&event) != 0) {
                if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        running = false;
                    }
                } else if (event.type == SDL_QUIT) {
                    running = false;
                }
                input->pushInput(event);
            }

            gui->logic();
            gui->draw();
            SDL_UpdateWindowSurface(window);
        }
    }
} // namespace openglsdl

namespace widgets
{
    inline fcn::ImageFont* font               = nullptr;
    inline fcn::Container* top                = nullptr;
    inline fcn::Label* label                  = nullptr;
    inline fcn::Icon* icon                    = nullptr;
    inline fcn::Button* button                = nullptr;
    inline fcn::TextField* textField          = nullptr;
    inline fcn::TextBox* textBox              = nullptr;
    inline fcn::ScrollArea* textBoxScrollArea = nullptr;
    inline fcn::ListBox* listBox              = nullptr;
    inline fcn::DropDown* dropDown            = nullptr;
    inline fcn::CheckBox* checkBox1           = nullptr;
    inline fcn::CheckBox* checkBox2           = nullptr;
    inline fcn::RadioButton* radioButton1     = nullptr;
    inline fcn::RadioButton* radioButton2     = nullptr;
    inline fcn::RadioButton* radioButton3     = nullptr;
    inline fcn::Slider* slider                = nullptr;
    inline fcn::Image* image                  = nullptr;
    inline fcn::Window* window                = nullptr;
    inline fcn::Image* logoImage              = nullptr;
    inline fcn::Icon* logoIcon                = nullptr;
    inline fcn::TabbedArea* tabbedArea        = nullptr;
    inline fcn::Button* tabOneButton          = nullptr;
    inline fcn::CheckBox* tabTwoCheckBox      = nullptr;

    class DemoListModel : public fcn::ListModel
    {
    public:
        int getNumberOfElements() override
        {
            return 5;
        }

        std::string getElementAt(int i) override
        {
            switch (i) {
            case 0:
                return "zero";
            case 1:
                return "one";
            case 2:
                return "two";
            case 3:
                return "three";
            case 4:
                return "four";
            default:
                return "";
            }
        }
    };

    inline DemoListModel demoListModel;

    inline void init()
    {
        if (openglsdl::gui == nullptr) {
            fcn::throwException("openglsdl::gui is null. Initialize GUI backend before widgets::init().");
        }

        top = new fcn::Container();
        top->setDimension(fcn::Rectangle(0, 0, 640, 480));
        openglsdl::gui->setTop(top);

        font = new fcn::ImageFont("fixedfont.bmp", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
        fcn::Widget::setGlobalFont(font);

        label = new fcn::Label("Label");

        image = fcn::Image::load("gui-chan.bmp");
        icon  = new fcn::Icon(image);

        button = new fcn::Button("Button");

        textField = new fcn::TextField("Text field");

        textBox           = new fcn::TextBox("Multiline\nText box");
        textBoxScrollArea = new fcn::ScrollArea(textBox);
        textBoxScrollArea->setWidth(200);
        textBoxScrollArea->setHeight(100);
        textBoxScrollArea->setBorderSize(1);

        listBox = new fcn::ListBox(&demoListModel);
        listBox->setBorderSize(1);
        dropDown = new fcn::DropDown(&demoListModel);

        checkBox1 = new fcn::CheckBox("Checkbox 1");
        checkBox2 = new fcn::CheckBox("Checkbox 2");

        radioButton1 = new fcn::RadioButton("RadioButton 1", "radiogroup", true);
        radioButton2 = new fcn::RadioButton("RadioButton 2", "radiogroup");
        radioButton3 = new fcn::RadioButton("RadioButton 3", "radiogroup");

        slider = new fcn::Slider(0, 10);
        slider->setSize(100, 10);

        window = new fcn::Window("I am a window  Drag me");
        window->setBaseColor(fcn::Color(255, 150, 200, 190));

        logoImage = fcn::Image::load("darkbitslogo_by_haiko.bmp");
        logoIcon  = new fcn::Icon(logoImage);
        window->add(logoIcon);
        window->resizeToContent();

        top->add(label, 10, 10);
        top->add(icon, 10, 30);
        top->add(button, 200, 10);
        top->add(textField, 250, 10);
        top->add(textBoxScrollArea, 200, 50);
        top->add(listBox, 200, 200);
        top->add(dropDown, 500, 10);
        top->add(checkBox1, 500, 130);
        top->add(checkBox2, 500, 150);
        top->add(radioButton1, 500, 200);
        top->add(radioButton2, 500, 220);
        top->add(radioButton3, 500, 240);
        top->add(slider, 500, 300);
        top->add(window, 50, 350);
    }

    inline void halt()
    {
        delete tabTwoCheckBox;
        tabTwoCheckBox = nullptr;

        delete tabOneButton;
        tabOneButton = nullptr;

        delete tabbedArea;
        tabbedArea = nullptr;

        delete logoIcon;
        logoIcon = nullptr;

        delete logoImage;
        logoImage = nullptr;

        delete window;
        window = nullptr;

        delete slider;
        slider = nullptr;

        delete radioButton3;
        radioButton3 = nullptr;

        delete radioButton2;
        radioButton2 = nullptr;

        delete radioButton1;
        radioButton1 = nullptr;

        delete checkBox2;
        checkBox2 = nullptr;

        delete checkBox1;
        checkBox1 = nullptr;

        delete dropDown;
        dropDown = nullptr;

        delete listBox;
        listBox = nullptr;

        delete textBoxScrollArea;
        textBoxScrollArea = nullptr;

        delete textBox;
        textBox = nullptr;

        delete textField;
        textField = nullptr;

        delete button;
        button = nullptr;

        delete icon;
        icon = nullptr;

        delete image;
        image = nullptr;

        delete label;
        label = nullptr;

        delete top;
        top = nullptr;

        delete font;
        font = nullptr;
    }
} // namespace widgets

#endif // TESTS_INTEGRATION_OPENGL_SDL_OPENGLSDLWIDGETS_HPP_
