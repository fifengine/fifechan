// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef TESTS_INTEGRATION_OPENGL_SDL_OPENGLSDLACTION_HPP_
#define TESTS_INTEGRATION_OPENGL_SDL_OPENGLSDLACTION_HPP_

#include <fifechan/exception.hpp>

#include <fifechan.hpp>

#include <sstream>
#include <string>

#include "openglsdl.hpp"

namespace action
{
    inline fcn::Container* top  = nullptr;
    inline fcn::ImageFont* font = nullptr;
    inline fcn::Button* button1 = nullptr;
    inline fcn::Button* button2 = nullptr;
    inline fcn::Label* label1   = nullptr;
    inline fcn::Label* label2   = nullptr;

    inline int clickCountButton1 = 0;
    inline int clickCountButton2 = 0;

    class ButtonActionListener : public fcn::ActionListener
    {
    public:
        void action(fcn::ActionEvent const & actionEvent)
        {
            std::string const str;
            std::ostringstream os(str);

            if (actionEvent.getSource() == button1) {
                clickCountButton1++;
                os << "Button1 clicks " << clickCountButton1;
                label1->setCaption(os.str());
                label1->adjustSize();
            } else if (actionEvent.getId() == "button2") {
                clickCountButton2++;
                os << "Button2 clicks " << clickCountButton2;
                label2->setCaption(os.str());
                label2->adjustSize();
            }
        }
    };

    inline ButtonActionListener* buttonActionListener = nullptr;

    void init()
    {
        if (openglsdl::gui == nullptr) {
            fcn::throwException("openglsdl::gui is null. Initialize GUI backend before action::init().");
        }

        clickCountButton1 = 0;
        clickCountButton2 = 0;

        top = new fcn::Container();
        top->setDimension(fcn::Rectangle(0, 0, 640, 480));
        openglsdl::gui->setTop(top);

        font = new fcn::ImageFont("fixedfont.bmp", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
        fcn::Widget::setGlobalFont(font);

        button1 = new fcn::Button("Button 1");
        button2 = new fcn::Button("Button 2");
        button1->setPosition(120, 230);
        button2->setPosition(420, 230);
        top->add(button1);
        top->add(button2);

        label1 = new fcn::Label("Button1 clicks 0");
        label2 = new fcn::Label("Button2 clicks 0");
        label1->setPosition(100, 200);
        label2->setPosition(400, 200);
        top->add(label1);
        top->add(label2);

        button1->setActionEventId("button1");
        button2->setActionEventId("button2");

        buttonActionListener = new ButtonActionListener();
        button1->addActionListener(buttonActionListener);
        button2->addActionListener(buttonActionListener);
    }

    void halt()
    {
        delete buttonActionListener;
        buttonActionListener = nullptr;

        delete label1;
        label1 = nullptr;

        delete label2;
        label2 = nullptr;

        delete button1;
        button1 = nullptr;

        delete button2;
        button2 = nullptr;

        delete font;
        font = nullptr;

        delete top;
        top = nullptr;
    }
} // namespace action

#endif // TESTS_INTEGRATION_OPENGL_SDL_OPENGLSDLACTION_HPP_
