// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef TESTS_INTEGRATION_SDL_SDLWIDGETS_HPP_
#define TESTS_INTEGRATION_SDL_SDLWIDGETS_HPP_

#include <fifechan/exception.hpp>
#include <fifechan/gui.hpp>

#include <fifechan.hpp>

#include <string>

namespace widgets
{
    // FifeGUI objects
    inline fcn::Gui* gui = nullptr;

    // FifeGUI Widgets
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
        int getNumberOfElements()
        {
            return 5;
        }

        std::string getElementAt(int i)
        {
            switch (i) {
            case 0:
                return std::string("zero");
            case 1:
                return std::string("one");
            case 2:
                return std::string("two");
            case 3:
                return std::string("three");
            case 4:
                return std::string("four");
            default:
                return std::string("");
            }
        }
    };

    inline DemoListModel demoListModel;

    void init()
    {
        if (gui == nullptr) {
            fcn::throwException("widgets::gui is null. Initialize GUI backend before widgets::init().");
        }

        top = new fcn::Container();
        top->setDimension(fcn::Rectangle(0, 0, 640, 480));
        gui->setTop(top);

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

    void halt()
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

        gui = nullptr;
    }
} // namespace widgets

#endif // TESTS_INTEGRATION_SDL_SDLWIDGETS_HPP_
