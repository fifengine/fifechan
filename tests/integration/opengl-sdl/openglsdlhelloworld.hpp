// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef TESTS_INTEGRATION_OPENGL_SDL_OPENGLSDLHELLOWORLD_HPP_
#define TESTS_INTEGRATION_OPENGL_SDL_OPENGLSDLHELLOWORLD_HPP_

#include <fifechan/exception.hpp>

#include <fifechan.hpp>

#include "openglsdl.hpp"

namespace helloworld
{
    inline fcn::Container* top  = nullptr;
    inline fcn::ImageFont* font = nullptr;
    inline fcn::Label* label    = nullptr;

    void init()
    {
        if (openglsdl::gui == nullptr) {
            fcn::throwException("openglsdl::gui is null. Initialize GUI backend before helloworld::init().");
        }

        top = new fcn::Container();
        top->setDimension(fcn::Rectangle(0, 0, 640, 480));
        openglsdl::gui->setTop(top);

        font = new fcn::ImageFont("fixedfont.bmp", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
        fcn::Widget::setGlobalFont(font);

        label = new fcn::Label("Hello World");
        label->setPosition(280, 220);
        top->add(label);
    }

    void halt()
    {
        delete label;
        label = nullptr;

        delete font;
        font = nullptr;

        delete top;
        top = nullptr;
    }
} // namespace helloworld

#endif // TESTS_INTEGRATION_OPENGL_SDL_OPENGLSDLHELLOWORLD_HPP_
