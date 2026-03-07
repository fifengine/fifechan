// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef SDLWIDGETS_HPP
#define SDLWIDGETS_HPP

#include <SDL2/SDL.h>
#include <SDL_render.h>
#include <SDL_ttf.h>

#include <fifechan/backends/sdl2/sdl.hpp>
#include <fifechan/gui.hpp>

#include <fifechan.hpp>

#include <fstream>
#include <iostream>
#include <memory>
#include <string>

class Application
{
public:
    explicit Application(std::string const & title, int width = 1280, int height = 1024);
    ~Application();

    void run();

private:
    void init_SDL(std::string const & title, int width, int height);
    void init_GUI(int width, int height);
    void cleanup();

    static std::string getExecutablePath();

    static std::shared_ptr<SDL_Window> initWindow(std::string const & title, int width, int height, int flags);
    static std::shared_ptr<SDL_Renderer> initRenderer(std::shared_ptr<SDL_Window> const & window, int flags);

    std::unique_ptr<fcn::Gui> gui;
    std::shared_ptr<SDL_Window> window{nullptr};
    std::shared_ptr<SDL_Renderer> renderer{nullptr};

    std::unique_ptr<fcn::sdl2::Graphics> graphics;
    std::unique_ptr<fcn::sdl2::Input> input;
    std::shared_ptr<fcn::sdl2::ImageLoader> imageLoader;

    std::string exePath;

    [[maybe_unused]] fcn::Font* font{nullptr};
    std::unique_ptr<fcn::Container> top;
    fcn::Label* label{nullptr};
    fcn::Icon* icon{nullptr};
    fcn::Button* button{nullptr};
    fcn::ImageButton* imageButton{nullptr};
    fcn::TextField* textField{nullptr};
    fcn::PasswordField* passwordField{nullptr};
    [[maybe_unused]] fcn::TextBox* textBox{nullptr};
    [[maybe_unused]] fcn::ScrollArea* textBoxScrollArea{nullptr};
    [[maybe_unused]] fcn::ListBox* listBox{nullptr};
    [[maybe_unused]] fcn::DropDown* dropDown{nullptr};
    [[maybe_unused]] fcn::CheckBox* checkBox1{nullptr};
    [[maybe_unused]] fcn::CheckBox* checkBox2{nullptr};
    fcn::ToggleButton* toggleButton{nullptr};
    [[maybe_unused]] fcn::RadioButton* radioButton1{nullptr};
    [[maybe_unused]] fcn::RadioButton* radioButton2{nullptr};
    [[maybe_unused]] fcn::RadioButton* radioButton3{nullptr};
    [[maybe_unused]] fcn::Slider* slider{nullptr};
    fcn::Window* fcnWindow{nullptr};
    [[maybe_unused]] fcn::Image* image{nullptr};
    [[maybe_unused]] fcn::Image* logoImage{nullptr};
    [[maybe_unused]] fcn::Icon* guisanLogoIcon{nullptr};
    [[maybe_unused]] fcn::TabbedArea* tabbedArea{nullptr};
    [[maybe_unused]] fcn::Button* tabOneButton{nullptr};
    [[maybe_unused]] fcn::Slider* nestedSlider{nullptr};
    [[maybe_unused]] fcn::ScrollArea* nestedScrollArea{nullptr};
    [[maybe_unused]] fcn::Container* nestedContainer{nullptr};
    [[maybe_unused]] fcn::AdjustingContainer* adjustingContainer{nullptr};
    [[maybe_unused]] fcn::BarGraph* barGraph{nullptr};
    [[maybe_unused]] fcn::CurveGraph* curveGraph{nullptr};
    [[maybe_unused]] fcn::FlowContainer* flowContainer{nullptr};
    [[maybe_unused]] fcn::IconProgressBar* iconProgressBar{nullptr};
    [[maybe_unused]] fcn::LineGraph* lineGraph{nullptr};
    [[maybe_unused]] fcn::PieGraph* pieGraph{nullptr};
    [[maybe_unused]] fcn::PointGraph* pointGraph{nullptr};
    [[maybe_unused]] fcn::Spacer* spacer{nullptr};
    [[maybe_unused]] fcn::Tab* tab{nullptr};
    [[maybe_unused]] fcn::TabbedArea* tabbedArea2{nullptr};
    [[maybe_unused]] fcn::CheckBox* tabTwoCheckBox{nullptr};

    bool running{false};

    std::unique_ptr<fcn::ListModel> demoListModel;
    std::unique_ptr<fcn::TextBox> ownedTextBox;
    std::unique_ptr<fcn::Container> ownedNestedContainer;
    std::unique_ptr<fcn::Tab> ownedTabOne;
    std::unique_ptr<fcn::Tab> ownedTabTwo;
    std::unique_ptr<fcn::Label> ownedTabOneContent;
    std::unique_ptr<fcn::Label> ownedTabTwoContent;
    std::unique_ptr<fcn::Image> ownedProgressFillImage;
};

namespace widgets
{
    class DemoListModel : public fcn::ListModel
    {
    public:
        int getNumberOfElements() override;
        std::string getElementAt(int i) override;
    };
} // namespace widgets

#endif // SDLWIDGETS_HPP
