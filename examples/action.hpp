/***************************************************************************
 *   Copyright (C) 2012 by the fifechan team                               *
 *   https://github.com/fifechan                                           *
 *   This file is part of fifechan.                                        *
 *                                                                         *
 *   fifechan is free software; you can redistribute it and/or             *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA          *
 ***************************************************************************/

/**
 * Code to populate a global Gui object with an example of how to
 * use actions in Guichan.
 */

#include <fifechan.hpp>
#include <iostream>
#include <sstream>

namespace action
{
    fcn::Container* top;
    fcn::ImageFont* font;
    fcn::Button* button1; 
    fcn::Button* button2;
    fcn::Label* label1;
    fcn::Label* label2;

    int clickCountButton1 = 0; // Holds clicks for button1
    int clickCountButton2 = 0; // Holds clicks for button2

    /*
     * To be able to recieve actions we must have a class inheriting from
     * ActionListener and implementing the action function.
     * When an action is generated in a Widget (i.e when a button is clicked)
     * that widget calls the action function for all its action listeners.
     * Below we make a class to be able to recieve our buttons actions.
     */
    class ButtonActionListener : public fcn::ActionListener
    {
    public:
        // Implement the action function in ActionListener to recieve actions
        // The eventId tells us which widget called the action function.
        void action(const fcn::ActionEvent& actionEvent)
        {
            std::string str;
            std::ostringstream os(str);

            // Here we use the widget pointer to check which widget the action
            // originated from.
            if (actionEvent.getSource() == button1)
            {
                clickCountButton1++;
                os << "Button1 clicks " << clickCountButton1;
                // Change the caption to show number of clicks
                label1->setCaption(os.str());
                // Adjust the label to fit the new caption
                label1->adjustSize();
            }
            // Here we use the event id in order to check what action occured.
            else if (actionEvent.getId() == "button2")
            {
                clickCountButton2++;
                os << "Button2 clicks " << clickCountButton2;
                // Change the caption to show number of clicks
                label2->setCaption(os.str());
                // Adjust the label to fit the new caption
                label2->adjustSize();
            }
        }
    };

    ButtonActionListener* buttonActionListener; // A pointer to the above class

    /**
     * Initialises the widgets example by populating the global Gui
     * object.
     */
    void init()
    {
         // We first create a container to be used as the top widget.
        // The top widget in Guichan can be any kind of widget, but
        // in order to make the Gui contain more than one widget we
        // make the top widget a container.
        top = new fcn::Container();
         // We set the dimension of the top container to match the screen.
        top->setDimension(fcn::Rectangle(0, 0, 640, 480));
        // Finally we pass the top widget to the Gui object.
        globals::gui->setTop(top);

        // Now we load the font used in this example.
        font = new fcn::ImageFont("fixedfont.bmp", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
        // Widgets may have a global font so we don't need to pass the
        // font object to every created widget. The global font is static.
        fcn::Widget::setGlobalFont(font);

        // Now we create the buttons.
        button1 = new fcn::Button("Button 1");
        button2 = new fcn::Button("Button 2");
        button1->setPosition(120, 230);
        button2->setPosition(420, 230);
        // We add the buttons to the top container.
        top->add(button1);
        top->add(button2);

        // Now we create the labels.
        label1 = new fcn::Label("Button1 clicks 0");
        label2 = new fcn::Label("Button2 clicks 0");
        label1->setPosition(100, 200);
        label2->setPosition(400, 200);
        // We add the labels to the top container.
        top->add(label1);
        top->add(label2);

        // Now we set the button's action event ids.
        button1->setActionEventId("button1");
        button2->setActionEventId("button2");

        // We make an instance of the ButtonActionListener.
        buttonActionListener = new ButtonActionListener();

        // And add the ButtonActionListener as an action listener of the buttons.
        button1->addActionListener(buttonActionListener);
        button2->addActionListener(buttonActionListener);
    }

    /**
     * Halts the action example.
     */
    void halt()
    {
        delete buttonActionListener;
        delete label1;
        delete label2;
        delete button1;
        delete button2;
        delete font;
        delete top;
    }
}
