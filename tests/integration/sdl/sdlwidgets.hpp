// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef TESTS_INTEGRATION_SDL_SDLWIDGETS_HPP_
#define TESTS_INTEGRATION_SDL_SDLWIDGETS_HPP_

#include <fifechan/exception.hpp>
#include <fifechan/gui.hpp>

#include <fifechan.hpp>

#include <memory>
#include <string>

namespace widgets
{
    // FifeGUI objects
    inline fcn::Gui* gui = nullptr;

    // FifeGUI Widgets
    inline std::unique_ptr<fcn::ImageFont> font;
    inline std::unique_ptr<fcn::Container> top;
    inline std::unique_ptr<fcn::Label> label;
    inline std::unique_ptr<fcn::Icon> icon;
    inline std::unique_ptr<fcn::Button> button;
    inline std::unique_ptr<fcn::TextField> textField;
    inline std::unique_ptr<fcn::TextBox> textBox;
    inline std::unique_ptr<fcn::ScrollArea> textBoxScrollArea;
    inline std::unique_ptr<fcn::ListBox> listBox;
    inline std::unique_ptr<fcn::DropDown> dropDown;
    inline std::unique_ptr<fcn::CheckBox> checkBox1;
    inline std::unique_ptr<fcn::CheckBox> checkBox2;
    inline std::unique_ptr<fcn::RadioButton> radioButton1;
    inline std::unique_ptr<fcn::RadioButton> radioButton2;
    inline std::unique_ptr<fcn::RadioButton> radioButton3;
    inline std::unique_ptr<fcn::Slider> slider;
    inline std::unique_ptr<fcn::Image> image;
    inline std::unique_ptr<fcn::Window> window;
    inline std::unique_ptr<fcn::Image> logoImage;
    inline std::unique_ptr<fcn::Icon> logoIcon;
    inline std::unique_ptr<fcn::TabbedArea> tabbedArea;
    inline std::unique_ptr<fcn::Button> tabOneButton;
    inline std::unique_ptr<fcn::CheckBox> tabTwoCheckBox;

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

        top = std::make_unique<fcn::Container>();
        top->setDimension(fcn::Rectangle(0, 0, 640, 480));
        gui->setTop(top.get());

        font = std::make_unique<fcn::ImageFont>(
            "fixedfont.bmp", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
        fcn::Widget::setGlobalFont(font.get());

        label = std::make_unique<fcn::Label>("Label");

        image = std::unique_ptr<fcn::Image>(fcn::Image::load("gui-chan.bmp"));
        icon  = std::make_unique<fcn::Icon>(image.get());

        button = std::make_unique<fcn::Button>("Button");

        textField = std::make_unique<fcn::TextField>("Text field");

        textBox           = std::make_unique<fcn::TextBox>("Multiline\nText box");
        textBoxScrollArea = std::make_unique<fcn::ScrollArea>(textBox.get());
        textBoxScrollArea->setWidth(200);
        textBoxScrollArea->setHeight(100);
        textBoxScrollArea->setBorderSize(1);

        listBox = std::make_unique<fcn::ListBox>(&demoListModel);
        listBox->setBorderSize(1);
        dropDown = std::make_unique<fcn::DropDown>(&demoListModel);

        checkBox1 = std::make_unique<fcn::CheckBox>("Checkbox 1");
        checkBox2 = std::make_unique<fcn::CheckBox>("Checkbox 2");

        radioButton1 = std::make_unique<fcn::RadioButton>("RadioButton 1", "radiogroup", true);
        radioButton2 = std::make_unique<fcn::RadioButton>("RadioButton 2", "radiogroup");
        radioButton3 = std::make_unique<fcn::RadioButton>("RadioButton 3", "radiogroup");

        slider = std::make_unique<fcn::Slider>(0, 10);
        slider->setSize(100, 10);

        window = std::make_unique<fcn::Window>("I am a window  Drag me");
        window->setBaseColor(fcn::Color(255, 150, 200, 190));

        logoImage = std::unique_ptr<fcn::Image>(fcn::Image::load("darkbitslogo_by_haiko.bmp"));
        logoIcon  = std::make_unique<fcn::Icon>(logoImage.get());
        window->add(logoIcon.get());
        window->resizeToContent();

        top->add(label.get(), 10, 10);
        top->add(icon.get(), 10, 30);
        top->add(button.get(), 200, 10);
        top->add(textField.get(), 250, 10);
        top->add(textBoxScrollArea.get(), 200, 50);
        top->add(listBox.get(), 200, 200);
        top->add(dropDown.get(), 500, 10);
        top->add(checkBox1.get(), 500, 130);
        top->add(checkBox2.get(), 500, 150);
        top->add(radioButton1.get(), 500, 200);
        top->add(radioButton2.get(), 500, 220);
        top->add(radioButton3.get(), 500, 240);
        top->add(slider.get(), 500, 300);
        top->add(window.get(), 50, 350);
    }

    void halt()
    {
        if (gui != nullptr) {
            gui->setTop(nullptr);
        }
        fcn::Widget::setGlobalFont(nullptr);

        tabTwoCheckBox.reset();
        tabOneButton.reset();
        tabbedArea.reset();
        logoIcon.reset();
        logoImage.reset();
        window.reset();
        slider.reset();
        radioButton3.reset();
        radioButton2.reset();
        radioButton1.reset();
        checkBox2.reset();
        checkBox1.reset();
        dropDown.reset();
        listBox.reset();
        textBoxScrollArea.reset();
        textBox.reset();
        textField.reset();
        button.reset();
        icon.reset();
        image.reset();
        label.reset();
        top.reset();
        font.reset();

        gui = nullptr;
    }
} // namespace widgets

#endif // TESTS_INTEGRATION_SDL_SDLWIDGETS_HPP_
