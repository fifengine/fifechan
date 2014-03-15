/***************************************************************************
 *   Copyright (C) 2012 by the fifechan team                               *
 *   http://fifechan.github.com/fifechan                                   *
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
 * Code to populate a global Gui object with all the widgets
 * of Guichan.
 */

namespace widgets
{
    fcn::ImageFont* font;
    fcn::Container* top;
    fcn::Label* label;
    fcn::Icon* icon;
    fcn::Button* button;
    fcn::TextField* textField;
    fcn::TextBox* textBox;
    fcn::ScrollArea* textBoxScrollArea;
    fcn::ListBox* listBox;
    fcn::DropDown* dropDown;
    fcn::CheckBox* checkBox1;
    fcn::CheckBox* checkBox2;
    fcn::RadioButton* radioButton1;
    fcn::RadioButton* radioButton2;
    fcn::RadioButton* radioButton3;
    fcn::Slider* slider;
    fcn::Image *image;
    fcn::Window *window;
    fcn::Image *darkbitsImage;
    fcn::Icon* darkbitsIcon;
    fcn::TabbedArea* tabbedArea;
    fcn::Button* tabOneButton;
    fcn::CheckBox* tabTwoCheckBox;

    /*
     * List boxes and drop downs need an instance of a list model
     * in order to display a list.
     */
    class DemoListModel : public fcn::ListModel
    {
    public:
        int getNumberOfElements()
        {
            return 5;
        }

        std::string getElementAt(int i)
        {
            switch(i)
            {
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
              default: // Just to keep warnings away
                  return std::string("");
            }
        }
    };

    DemoListModel demoListModel;

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

        // Now we create the widgets

        label = new fcn::Label("Label");

        image = fcn::Image::load("gui-chan.bmp");
        icon = new fcn::Icon(image);

        button = new fcn::Button("Button");

        textField = new fcn::TextField("Text field");

        textBox = new fcn::TextBox("Multiline\nText box");
        textBoxScrollArea = new fcn::ScrollArea(textBox);
        textBoxScrollArea->setWidth(200);
        textBoxScrollArea->setHeight(100);
        textBoxScrollArea->setFrameSize(1);

        listBox = new fcn::ListBox(&demoListModel);
        listBox->setFrameSize(1);
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

        darkbitsImage = fcn::Image::load("darkbitslogo_by_haiko.bmp");
        darkbitsIcon = new fcn::Icon(darkbitsImage);
        window->add(darkbitsIcon);
        window->resizeToContent();

        tabbedArea = new fcn::TabbedArea();
        tabbedArea->setSize(200, 100);
        tabOneButton = new fcn::Button("A button in tab 1");
        tabbedArea->addTab("Tab 1", tabOneButton);
        tabTwoCheckBox = new fcn::CheckBox("A check box in tab 2");
        tabbedArea->addTab("Tab 2", tabTwoCheckBox);

        // Now it's time to add the widgets to the top container
        // so they will be conected to the GUI.
        
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
        top->add(tabbedArea, 400, 350);
    }
    
    /**
     * Halts the widgets example.
     */
    void halt()
    {
        delete font;
        delete top;
        delete label;
        delete icon;
        delete button;
        delete textField;
        delete textBox;
        delete textBoxScrollArea;
        delete listBox;
        delete dropDown;
        delete checkBox1;
        delete checkBox2;
        delete radioButton1;
        delete radioButton2;
        delete radioButton3;
        delete slider;
        delete window;
        delete darkbitsIcon;
        delete darkbitsImage;
        delete tabbedArea;
        delete tabOneButton;
        delete tabTwoCheckBox;
    }
}
