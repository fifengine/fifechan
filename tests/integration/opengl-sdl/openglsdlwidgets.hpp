// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef TESTS_INTEGRATION_OPENGL_SDL_OPENGLSDLWIDGETS_HPP_
#define TESTS_INTEGRATION_OPENGL_SDL_OPENGLSDLWIDGETS_HPP_

#include <SDL2/SDL.h>

#include <fifechan/backends/opengl/imageloader.hpp>
#include <fifechan/backends/opengl/opengl.hpp>
#include <fifechan/backends/sdl2/sdl.hpp>
#include <fifechan/gui.hpp>

#include <fifechan.hpp>

#include <memory>
#include <string>

namespace tests::integration::opengl_sdl::widgets
{

    class Application
    {
    public:
        explicit Application(std::string const & title, int width = 1280, int height = 1024);
        ~Application();

        void run();

    private:
        static std::string getExecutablePath();
        void init_SDL(std::string const & title, int width, int height);
        void init_GUI(int width, int height);
        void cleanup();

        static SDL_Window* initWindow(std::string const & title, int width, int height, int flags);
        static SDL_GLContext initGLContext(SDL_Window* window);

        bool running{false};

        SDL_Window* window{nullptr};
        SDL_GLContext glContext{nullptr};

        std::unique_ptr<fcn::opengl::Graphics> graphics;
        std::unique_ptr<fcn::sdl2::Input> input;
        std::shared_ptr<fcn::opengl::ImageLoader> imageLoader;

        std::unique_ptr<fcn::Gui> gui;

        std::string exePath;

        fcn::ImageFont* font{nullptr};
        std::unique_ptr<fcn::Container> top;
        fcn::Label* label{nullptr};
        fcn::Icon* icon{nullptr};
        fcn::Button* button{nullptr};
        fcn::ImageButton* imageButton{nullptr};
        fcn::TextField* textField{nullptr};
        fcn::PasswordField* passwordField{nullptr};
        fcn::TextBox* textBox{nullptr};
        fcn::ScrollArea* textBoxScrollArea{nullptr};
        fcn::ListBox* listBox{nullptr};
        fcn::DropDown* dropDown{nullptr};
        fcn::CheckBox* checkBox1{nullptr};
        fcn::CheckBox* checkBox2{nullptr};
        fcn::ToggleButton* toggleButton{nullptr};
        fcn::RadioButton* radioButton1{nullptr};
        fcn::RadioButton* radioButton2{nullptr};
        fcn::RadioButton* radioButton3{nullptr};
        fcn::Slider* slider{nullptr};
        fcn::Window* fcnWindow{nullptr};
        fcn::Image* image{nullptr};
        fcn::Image* logoImage{nullptr};
        fcn::Icon* guisanLogoIcon{nullptr};
        fcn::TabbedArea* tabbedArea{nullptr};
        fcn::Button* tabOneButton{nullptr};
        fcn::Slider* nestedSlider{nullptr};
        fcn::ScrollArea* nestedScrollArea{nullptr};
        fcn::Container* nestedContainer{nullptr};
        fcn::AdjustingContainer* adjustingContainer{nullptr};
        fcn::BarGraph* barGraph{nullptr};
        fcn::CurveGraph* curveGraph{nullptr};
        fcn::FlowContainer* flowContainer{nullptr};
        fcn::IconProgressBar* iconProgressBar{nullptr};
        fcn::LineGraph* lineGraph{nullptr};
        fcn::PieGraph* pieGraph{nullptr};
        fcn::PointGraph* pointGraph{nullptr};
        fcn::Spacer* spacer{nullptr};
        fcn::Tab* tab{nullptr};
        fcn::TabbedArea* tabbedArea2{nullptr};
        fcn::CheckBox* tabTwoCheckBox{nullptr};

        std::unique_ptr<fcn::ListModel> demoListModel;
        std::unique_ptr<fcn::TextBox> ownedTextBox;
        std::unique_ptr<fcn::Container> ownedNestedContainer;
        std::unique_ptr<fcn::Tab> ownedTabOne;
        std::unique_ptr<fcn::Tab> ownedTabTwo;
        std::unique_ptr<fcn::Label> ownedTabOneContent;
        std::unique_ptr<fcn::Label> ownedTabTwoContent;
        std::unique_ptr<fcn::Image> ownedProgressFillImage;
    };

    class DemoListModel : public fcn::ListModel
    {
    public:
        int getNumberOfElements() override;
        std::string getElementAt(int i) override;
    };

} // namespace tests::integration::opengl_sdl::widgets

#endif // TESTS_INTEGRATION_OPENGL_SDL_OPENGLSDLWIDGETS_HPP_
