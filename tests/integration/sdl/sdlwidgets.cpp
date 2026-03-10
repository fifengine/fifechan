// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

/**
 * @brief This example shows the widgets present in FifeGUI using the SDL backend.
 */

#include "sdlwidgets.hpp"

#include <fifechan/backends/sdl2/sdlimage.hpp>
#include <fifechan/gui.hpp>
#include <fifechan/widget.hpp>

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#ifdef _WIN32
    #include <windows.h>
#endif

namespace tests::integration::sdl::widgets
{
    int DemoListModel::getNumberOfElements()
    {
        return 5;
    }

    std::string DemoListModel::getElementAt(int i)
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
} // namespace tests::integration::sdl::widgets

using namespace tests::integration::sdl::widgets;

Application::Application(std::string const & title, int width, int height) :
    exePath(getExecutablePath()), demoListModel(std::make_unique<DemoListModel>())
{
    init_SDL(title, width, height);
    init_GUI(width, height);
}

std::string Application::getExecutablePath()
{
#ifdef __linux__
    return std::filesystem::read_symlink("/proc/self/exe").parent_path().string();
#elif defined(_WIN32)
    char buffer[MAX_PATH];
    GetModuleFileName(nullptr, buffer, MAX_PATH);
    return std::filesystem::path(buffer).parent_path().string();
#else
    throw std::runtime_error("Unsupported platform");
#endif
}

Application::~Application()
{
    cleanup();
}

std::shared_ptr<SDL_Window> Application::initWindow(std::string const & title, int width, int height, int flags)
{
    SDL_Window* createdWindow =
        SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    if (createdWindow == nullptr) {
        throw std::runtime_error("Failed to create SDL_Window");
    }

    return fcn::sdl2::makeSDLSharedPtr(createdWindow);
}

std::shared_ptr<SDL_Renderer> Application::initRenderer(std::shared_ptr<SDL_Window> const & window, int flags)
{
    SDL_Renderer* createdRenderer = SDL_CreateRenderer(window.get(), -1, flags);
    if (createdRenderer == nullptr) {
        std::string const err = SDL_GetError();

        createdRenderer = SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_SOFTWARE);
        if (createdRenderer == nullptr) {
            std::string const err2 = SDL_GetError();
            throw std::runtime_error(std::string("Failed to create SDL_Renderer: ") + err + " -> " + err2);
        }
    }

    return fcn::sdl2::makeSDLSharedPtr(createdRenderer);
}

void Application::init_SDL(std::string const & title, int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << '\n';
        exit(1);
    }

    auto const windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI;

    window = initWindow(title, width, height, windowFlags);

    SDL_RaiseWindow(window.get());

    auto const rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

    renderer = initRenderer(window, rendererFlags);
}

void Application::cleanup()
{
    fcn::Image::setImageLoader(nullptr);
    gui.reset();
    top.reset();
    ownedTabTwoContent.reset();
    ownedTabOneContent.reset();
    ownedTabTwo.reset();
    ownedTabOne.reset();
    ownedNestedContainer.reset();
    ownedTextBox.reset();
    imageLoader.reset();
    graphics.reset();
    input.reset();
    renderer.reset();
    window.reset();
    TTF_Quit();
    SDL_Quit();
}

void Application::init_GUI(int width, int height)
{
    graphics = std::make_unique<fcn::sdl2::Graphics>();
    graphics->setTarget(renderer.get(), width, height);
    input       = std::make_unique<fcn::sdl2::Input>();
    imageLoader = std::make_shared<fcn::sdl2::ImageLoader>();
    imageLoader->setRenderer(renderer.get());
    fcn::Image::setImageLoader(imageLoader.get());

    gui = std::make_unique<fcn::Gui>();
    gui->setGraphics(std::move(graphics));
    gui->setInput(input.get());

    top = std::make_unique<fcn::Container>();
    top->setLayout(fcn::Container::LayoutPolicy::Absolute);
    top->setMinSize(fcn::Size(width, height));
    top->setMaxSize(fcn::Size(width, height));
    top->setFixedSize(fcn::Size(width, height));
    top->setDimension(fcn::Rectangle(0, 0, width, height));
    top->setSize(width, height);
    top->adjustSize();
    top->setWidth(width);
    top->setHeight(height);
    top->setOpaque(true);
    top->setBaseColor(fcn::Color(220, 220, 220, 255));
    gui->setTop(top.get());

    if (TTF_Init() == -1) {
        std::cerr << "[ERROR] Failed to initialize SDL2_ttf: " << TTF_GetError() << '\n';
        exit(2);
    }

    std::string const fontPath = exePath + "/ArchitectsDaughter.ttf";
    int const fontSize         = std::max(12, std::min(width, height) / 64);
    try {
        gui->setGlobalFont(fontPath, fontSize);
    } catch (std::exception const & e) {
        std::cerr << "[ERROR] Exception loading font: " << e.what() << '\n';
        exit(5);
    }

    font = gui->getTop()->getFont();
    if (font == nullptr) {
        std::cerr << "[ERROR] Global font is nullptr after setGlobalFont\n";
        exit(6);
    }

    fcn::Color const descriptionLabelColor(64, 64, 64, 255);

    int constexpr marginX         = 24;
    int constexpr topMarginY      = 20;
    int constexpr columnWidth     = 282;
    int constexpr columnGap       = 24;
    int constexpr labelGap        = 30;
    int constexpr bottomMargin    = 20;
    int constexpr graphWidth      = 220;
    int constexpr graphHeight     = 108;
    int constexpr graphGap        = 30;
    int constexpr pieGraphWidth   = 280;
    int constexpr pieGraphHeight  = 140;
    int const graphRow2TopY       = height - bottomMargin - labelGap - pieGraphHeight;
    int const graphRow2Y          = graphRow2TopY + labelGap;
    int const graphRow1TopY       = graphRow2TopY - 144;
    int const graphRow1Y          = graphRow1TopY + labelGap;
    int const col1X               = marginX;
    int const col2X               = col1X + columnWidth + columnGap;
    int const col3X               = col2X + columnWidth + columnGap;
    int const col4X               = col3X + columnWidth + columnGap;
    int const leftDetailX         = col1X + 138;
    int constexpr imageButtonTopY = 344;
    int constexpr windowTopY      = 502;
    int const graphRow1StartX     = (width - ((3 * graphWidth) + (2 * graphGap))) / 2;
    int const graphRow1Col2X      = graphRow1StartX + graphWidth + graphGap;
    int const graphRow1Col3X      = graphRow1Col2X + graphWidth + graphGap;
    int const graphRow2StartX     = (width - (graphWidth + graphGap + pieGraphWidth)) / 2;
    int const pieGraphX           = graphRow2StartX + graphWidth + graphGap;

    auto showLastWidget = [this]() {
        top->getChild(top->getChildrenCount() - 1)->setVisible(true);
    };

    auto addCaption = [&](std::string const & caption, int x, int y) {
        auto captionPtr = std::make_unique<fcn::Label>(caption);
        captionPtr->adjustSize();
        captionPtr->setForegroundColor(descriptionLabelColor);
        top->addWidget(std::move(captionPtr), x, y);
        showLastWidget();
    };

    std::string const iconPath = exePath + "/gui-chan.bmp";
    std::ifstream const iconFileCheck(iconPath);
    bool const hasIcon = iconFileCheck.good();

    if (!hasIcon) {
        std::cerr << "[ERROR] Icon file gui-chan.bmp not found. Icon-based widgets will not be displayed." << '\n';
    }

    addCaption("Icon", col1X, topMarginY);
    if (hasIcon) {
        auto iconPtr = std::make_unique<fcn::Icon>(iconPath);
        iconPtr->adjustSize();
        top->addWidget(std::move(iconPtr), col1X, topMarginY + labelGap);
        showLastWidget();
        icon = dynamic_cast<fcn::Icon*>(top->getChild(top->getChildrenCount() - 1));
    }

    addCaption("Label", leftDetailX, topMarginY);
    auto labelPtr = std::make_unique<fcn::Label>("Label");
    labelPtr->adjustSize();
    top->addWidget(std::move(labelPtr), leftDetailX, topMarginY + labelGap);
    showLastWidget();
    label = dynamic_cast<fcn::Label*>(top->getChild(top->getChildrenCount() - 1));

    addCaption("Button", leftDetailX, 96);
    auto buttonPtr = std::make_unique<fcn::Button>("Button");
    buttonPtr->adjustSize();
    top->addWidget(std::move(buttonPtr), leftDetailX, 96 + labelGap);
    showLastWidget();
    button = dynamic_cast<fcn::Button*>(top->getChild(top->getChildrenCount() - 1));

    addCaption("ImageButton", col1X, imageButtonTopY);
    if (hasIcon) {
        auto imageButtonPtr = std::make_unique<fcn::ImageButton>(iconPath);
        imageButtonPtr->setSize(260, 96);
        top->addWidget(std::move(imageButtonPtr), col1X, imageButtonTopY + labelGap);
        showLastWidget();
        imageButton = dynamic_cast<fcn::ImageButton*>(top->getChild(top->getChildrenCount() - 1));
    }

    addCaption("Window", col1X, windowTopY);
    auto windowPtr = std::make_unique<fcn::Window>("I am a window  Drag me");
    windowPtr->setBaseColor(fcn::Color(212, 255, 150, 190));
    if (hasIcon) {
        auto guisanLogoImage   = fcn::Image::load(iconPath);
        auto guisanLogoIconPtr = std::make_unique<fcn::Icon>(guisanLogoImage);
        windowPtr->addWidget(std::move(guisanLogoIconPtr));
    }
    windowPtr->resizeToContent();
    top->addWidget(std::move(windowPtr), col1X, windowTopY + labelGap);
    showLastWidget();
    fcnWindow = dynamic_cast<fcn::Window*>(top->getChild(top->getChildrenCount() - 1));

    addCaption("TextField", col2X, topMarginY);
    auto textFieldPtr = std::make_unique<fcn::TextField>("Text field");
    textFieldPtr->setEditable(true);
    textFieldPtr->setWidth(240);
    top->addWidget(std::move(textFieldPtr), col2X, topMarginY + labelGap);
    showLastWidget();
    textField = dynamic_cast<fcn::TextField*>(top->getChild(top->getChildrenCount() - 1));
    gui->getFocusHandler()->requestFocus(textField);

    addCaption("PasswordField", col2X, 110);
    auto passwordFieldPtr = std::make_unique<fcn::PasswordField>("password");
    passwordFieldPtr->setWidth(240);
    top->addWidget(std::move(passwordFieldPtr), col2X, 110 + labelGap);
    showLastWidget();
    passwordField = dynamic_cast<fcn::PasswordField*>(top->getChild(top->getChildrenCount() - 1));

    addCaption("ToggleButton", col2X, 200);
    auto toggleButtonPtr = std::make_unique<fcn::ToggleButton>("Toggle font", "toggle_group", false);
    toggleButtonPtr->adjustSize();
    top->addWidget(std::move(toggleButtonPtr), col2X, 200 + labelGap);
    showLastWidget();
    toggleButton = dynamic_cast<fcn::ToggleButton*>(top->getChild(top->getChildrenCount() - 1));

    addCaption("Slider", col2X, 290);
    auto sliderPtr = std::make_unique<fcn::Slider>(0, 10);
    sliderPtr->setSize(180, 18);
    top->addWidget(std::move(sliderPtr), col2X, 290 + labelGap);
    showLastWidget();
    slider = dynamic_cast<fcn::Slider*>(top->getChild(top->getChildrenCount() - 1));

    addCaption("Spacer", col2X, 380);
    auto spacerPtr = std::make_unique<fcn::Spacer>();
    spacerPtr->setSize(72, 20);
    top->addWidget(std::move(spacerPtr), col2X, 380 + labelGap);
    showLastWidget();
    spacer = dynamic_cast<fcn::Spacer*>(top->getChild(top->getChildrenCount() - 1));

    addCaption("AdjustingContainer", col2X, 446);
    auto adjustingContainerPtr = std::make_unique<fcn::AdjustingContainer>();
    auto adjLabelPtr           = std::make_unique<fcn::Label>("Adjusting");
    adjLabelPtr->adjustSize();
    adjustingContainerPtr->addWidget(std::move(adjLabelPtr));
    adjustingContainerPtr->adjustSize();
    top->addWidget(std::move(adjustingContainerPtr), col2X, 446 + labelGap);
    showLastWidget();
    adjustingContainer = dynamic_cast<fcn::AdjustingContainer*>(top->getChild(top->getChildrenCount() - 1));

    addCaption("FlowContainer", col2X, 526);
    auto flowContainerPtr = std::make_unique<fcn::FlowContainer>();
    flowContainerPtr->setSize(200, 56);
    auto flowButton1Ptr = std::make_unique<fcn::Button>("Flow1");
    flowButton1Ptr->adjustSize();
    flowContainerPtr->addWidget(std::move(flowButton1Ptr));
    auto flowButton2Ptr = std::make_unique<fcn::Button>("Flow2");
    flowButton2Ptr->adjustSize();
    flowContainerPtr->addWidget(std::move(flowButton2Ptr));
    top->addWidget(std::move(flowContainerPtr), col2X, 526 + labelGap);
    showLastWidget();
    flowContainer = dynamic_cast<fcn::FlowContainer*>(top->getChild(top->getChildrenCount() - 1));

    addCaption("TextBox", col3X, topMarginY);
    ownedTextBox = std::make_unique<fcn::TextBox>(
        "Lorem ipsum dolor sit amet\n"
        "consectetur adipiscing elit.\n"
        "Vivamus facilisis sem ut massa.\n"
        "Curabitur malesuada dolor.\n"
        "Praesent consequat urna leo.\n"
        "Mauris hendrerit velit nulla.");
    ownedTextBox->setForegroundColor(fcn::Color(52, 52, 52, 255));
    ownedTextBox->setBackgroundColor(fcn::Color(247, 244, 236, 255));
    ownedTextBox->setSize(282, 120);
    top->add(ownedTextBox.get(), col3X, topMarginY + labelGap);
    textBoxScrollArea = nullptr;
    textBox           = ownedTextBox.get();

    addCaption("ListBox", col3X, 248);
    auto listBoxPtr = std::make_unique<fcn::ListBox>();
    listBoxPtr->setListModel(demoListModel.get());
    listBoxPtr->setSize(200, 120);
    top->addWidget(std::move(listBoxPtr), col3X, 248 + labelGap);
    showLastWidget();
    listBox = dynamic_cast<fcn::ListBox*>(top->getChild(top->getChildrenCount() - 1));

    addCaption("ScrollArea", col3X, 444);
    auto nestedLabelPtr = std::make_unique<fcn::Label>("Scrollable content");
    nestedLabelPtr->setForegroundColor(fcn::Color(60, 60, 60, 255));
    nestedLabelPtr->adjustSize();

    auto nestedSliderPtr = std::make_unique<fcn::Slider>(0, 10);
    nestedSliderPtr->setSize(160, 18);
    nestedSliderPtr->setValue(6);

    auto nestedButtonPtr = std::make_unique<fcn::Button>("Bottom Right");
    nestedButtonPtr->adjustSize();

    auto nestedHintPtr = std::make_unique<fcn::Label>("Drag the bars to reveal more.");
    nestedHintPtr->setForegroundColor(fcn::Color(86, 92, 98, 255));
    nestedHintPtr->adjustSize();

    ownedNestedContainer = std::make_unique<fcn::Container>();
    ownedNestedContainer->setSize(420, 220);
    ownedNestedContainer->setOpaque(true);
    ownedNestedContainer->setBaseColor(fcn::Color(239, 241, 233, 255));
    ownedNestedContainer->setBorderSize(1);
    ownedNestedContainer->addWidget(std::move(nestedLabelPtr), 16, 14);
    ownedNestedContainer->addWidget(std::move(nestedSliderPtr), 24, 54);
    ownedNestedContainer->addWidget(std::move(nestedHintPtr), 24, 132);
    ownedNestedContainer->addWidget(std::move(nestedButtonPtr), 286, 172);
    auto nestedScrollAreaPtr = std::make_unique<fcn::ScrollArea>(ownedNestedContainer.get());
    nestedScrollAreaPtr->setSize(250, 110);
    nestedScrollAreaPtr->setBaseColor(fcn::Color(214, 219, 205, 255));
    nestedScrollAreaPtr->setBackgroundColor(fcn::Color(248, 248, 244, 255));
    nestedScrollAreaPtr->setHorizontalScrollPolicy(fcn::ScrollArea::ScrollPolicy::ShowAlways);
    nestedScrollAreaPtr->setVerticalScrollPolicy(fcn::ScrollArea::ScrollPolicy::ShowAlways);
    top->addWidget(std::move(nestedScrollAreaPtr), col3X, 444 + labelGap);
    showLastWidget();
    nestedScrollArea = dynamic_cast<fcn::ScrollArea*>(top->getChild(top->getChildrenCount() - 1));
    nestedContainer  = ownedNestedContainer.get();
    nestedSlider     = dynamic_cast<fcn::Slider*>(ownedNestedContainer->getChild(1));

    addCaption("IconProgressBar", col3X, 596);
    if (hasIcon) {
        SDL_Surface* progressSurface =
            SDL_CreateRGBSurface(0, 10, 18, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
        if (progressSurface == nullptr) {
            std::cerr << "[ERROR] Failed to create IconProgressBar fill surface: " << SDL_GetError() << '\n';
            exit(7);
        }

        SDL_FillRect(progressSurface, nullptr, SDL_MapRGBA(progressSurface->format, 0, 0, 0, 0));
        SDL_Rect const outlineRect{0, 2, 10, 14};
        SDL_FillRect(progressSurface, &outlineRect, SDL_MapRGBA(progressSurface->format, 132, 152, 89, 255));
        SDL_Rect const fillRect{1, 3, 8, 12};
        SDL_FillRect(progressSurface, &fillRect, SDL_MapRGBA(progressSurface->format, 177, 198, 120, 255));

        ownedProgressFillImage = std::make_unique<fcn::sdl2::Image>(progressSurface, true, renderer.get());

        auto iconProgressBarPtr = std::make_unique<fcn::IconProgressBar>(ownedProgressFillImage.get(), 20);
        iconProgressBarPtr->setIconCount(13);
        iconProgressBarPtr->setBackgroundColor(fcn::Color(214, 214, 214, 255));
        iconProgressBarPtr->setBaseColor(fcn::Color(214, 214, 214, 255));
        iconProgressBarPtr->setSize(220, 24);
        top->addWidget(std::move(iconProgressBarPtr), col3X, 596 + labelGap);
        showLastWidget();
        iconProgressBar = dynamic_cast<fcn::IconProgressBar*>(top->getChild(top->getChildrenCount() - 1));
    }

    addCaption("DropDown", col4X, topMarginY);
    auto dropDownPtr = std::make_unique<fcn::DropDown>(static_cast<fcn::ListModel*>(demoListModel.get()));
    dropDownPtr->setWidth(220);
    top->addWidget(std::move(dropDownPtr), col4X, topMarginY + labelGap);
    showLastWidget();
    dropDown = dynamic_cast<fcn::DropDown*>(top->getChild(top->getChildrenCount() - 1));

    addCaption("Checkbox", col4X, 110);
    auto checkBox1Ptr = std::make_unique<fcn::CheckBox>("Checkbox 1");
    checkBox1Ptr->adjustSize();
    top->addWidget(std::move(checkBox1Ptr), col4X, 110 + labelGap);
    showLastWidget();
    checkBox1 = dynamic_cast<fcn::CheckBox*>(top->getChild(top->getChildrenCount() - 1));

    auto checkBox2Ptr = std::make_unique<fcn::CheckBox>("Checkbox 2");
    checkBox2Ptr->adjustSize();
    top->addWidget(std::move(checkBox2Ptr), col4X, 110 + labelGap + 34);
    showLastWidget();
    checkBox2 = dynamic_cast<fcn::CheckBox*>(top->getChild(top->getChildrenCount() - 1));

    addCaption("RadioButton", col4X, 246);
    auto radioButton1Ptr = std::make_unique<fcn::RadioButton>("Radio Button 1", "radiogroup", true);
    radioButton1Ptr->adjustSize();
    top->addWidget(std::move(radioButton1Ptr), col4X, 246 + labelGap);
    showLastWidget();
    radioButton1 = dynamic_cast<fcn::RadioButton*>(top->getChild(top->getChildrenCount() - 1));

    auto radioButton2Ptr = std::make_unique<fcn::RadioButton>("Radio Button 2", "radiogroup");
    radioButton2Ptr->adjustSize();
    top->addWidget(std::move(radioButton2Ptr), col4X, 246 + labelGap + 34);
    showLastWidget();
    radioButton2 = dynamic_cast<fcn::RadioButton*>(top->getChild(top->getChildrenCount() - 1));

    auto radioButton3Ptr = std::make_unique<fcn::RadioButton>("Radio Button 3", "radiogroup");
    radioButton3Ptr->adjustSize();
    top->addWidget(std::move(radioButton3Ptr), col4X, 246 + labelGap + 68);
    showLastWidget();
    radioButton3 = dynamic_cast<fcn::RadioButton*>(top->getChild(top->getChildrenCount() - 1));

    addCaption("TabbedArea", col4X, 468);
    auto tabbedAreaPtr = std::make_unique<fcn::TabbedArea>();
    tabbedAreaPtr->setSize(224, 108);
    ownedTabOne    = std::make_unique<fcn::Tab>();
    auto tab1Label = std::make_unique<fcn::Label>("Tab 1");
    tab1Label->adjustSize();
    ownedTabOne->addWidget(std::move(tab1Label));
    ownedTabOne->adjustSize();
    ownedTabOneContent = std::make_unique<fcn::Label>("Tab 1 content");
    ownedTabOneContent->adjustSize();
    tabbedAreaPtr->addTab(ownedTabOne.get(), ownedTabOneContent.get());
    ownedTabTwo    = std::make_unique<fcn::Tab>();
    auto tab2Label = std::make_unique<fcn::Label>("Tab 2");
    tab2Label->adjustSize();
    ownedTabTwo->addWidget(std::move(tab2Label));
    ownedTabTwo->adjustSize();
    ownedTabTwoContent = std::make_unique<fcn::Label>("Tab 2 content");
    ownedTabTwoContent->adjustSize();
    tabbedAreaPtr->addTab(ownedTabTwo.get(), ownedTabTwoContent.get());
    top->addWidget(std::move(tabbedAreaPtr), col4X, 468 + labelGap);
    showLastWidget();
    tabbedArea = dynamic_cast<fcn::TabbedArea*>(top->getChild(top->getChildrenCount() - 1));

    addCaption("BarGraph", graphRow1StartX, graphRow1TopY);
    auto barGraphPtr = std::make_unique<fcn::BarGraph>();
    barGraphPtr->setOpaque(true);
    barGraphPtr->setBackgroundColor(fcn::Color(231, 235, 239, 255));
    barGraphPtr->setBaseColor(fcn::Color(110, 123, 158, 255));
    barGraphPtr->setBorderSize(1);
    barGraphPtr->setBarPosition(18, 18);
    barGraphPtr->setBarSize(136, 68);
    barGraphPtr->setSize(graphWidth, graphHeight);
    top->addWidget(std::move(barGraphPtr), graphRow1StartX, graphRow1Y);
    showLastWidget();
    barGraph = dynamic_cast<fcn::BarGraph*>(top->getChild(top->getChildrenCount() - 1));

    addCaption("LineGraph", graphRow1Col2X, graphRow1TopY);
    auto lineGraphPtr                      = std::make_unique<fcn::LineGraph>();
    std::vector<fcn::Point> const lineData = {
        fcn::Point{18, 78},
        fcn::Point{50, 34},
        fcn::Point{88, 58},
        fcn::Point{126, 20},
        fcn::Point{164, 72},
        fcn::Point{198, 40}};
    lineGraphPtr->setPointVector(lineData);
    lineGraphPtr->setThickness(2);
    lineGraphPtr->setOpaque(true);
    lineGraphPtr->setBackgroundColor(fcn::Color(236, 238, 242, 255));
    lineGraphPtr->setBaseColor(fcn::Color(88, 102, 138, 255));
    lineGraphPtr->setBorderSize(1);
    lineGraphPtr->setSize(graphWidth, graphHeight);
    top->addWidget(std::move(lineGraphPtr), graphRow1Col2X, graphRow1Y);
    showLastWidget();
    lineGraph = dynamic_cast<fcn::LineGraph*>(top->getChild(top->getChildrenCount() - 1));

    addCaption("PointGraph", graphRow1Col3X, graphRow1TopY);
    auto pointGraphPtr                      = std::make_unique<fcn::PointGraph>();
    std::vector<fcn::Point> const pointData = {
        fcn::Point{18, 84},
        fcn::Point{42, 58},
        fcn::Point{64, 30},
        fcn::Point{92, 68},
        fcn::Point{116, 44},
        fcn::Point{144, 22},
        fcn::Point{168, 62},
        fcn::Point{196, 36}};
    pointGraphPtr->setPointVector(pointData);
    pointGraphPtr->setThickness(2);
    pointGraphPtr->setOpaque(true);
    pointGraphPtr->setBackgroundColor(fcn::Color(228, 234, 238, 255));
    pointGraphPtr->setBaseColor(fcn::Color(69, 108, 153, 255));
    pointGraphPtr->setBorderSize(1);
    pointGraphPtr->setSize(graphWidth, graphHeight);
    top->addWidget(std::move(pointGraphPtr), graphRow1Col3X, graphRow1Y);
    showLastWidget();
    pointGraph = dynamic_cast<fcn::PointGraph*>(top->getChild(top->getChildrenCount() - 1));

    addCaption("CurveGraph", graphRow2StartX, graphRow2TopY);
    auto curveGraphPtr                      = std::make_unique<fcn::CurveGraph>();
    std::vector<fcn::Point> const curveData = {
        fcn::Point{16, 86},
        fcn::Point{42, 44},
        fcn::Point{72, 70},
        fcn::Point{108, 18},
        fcn::Point{146, 82},
        fcn::Point{178, 30},
        fcn::Point{206, 58}};
    curveGraphPtr->setPointVector(curveData);
    curveGraphPtr->setThickness(3);
    curveGraphPtr->setOpaque(true);
    curveGraphPtr->setBackgroundColor(fcn::Color(233, 239, 235, 255));
    curveGraphPtr->setBaseColor(fcn::Color(78, 148, 122, 255));
    curveGraphPtr->setBorderSize(1);
    curveGraphPtr->setSize(graphWidth, graphHeight);
    top->addWidget(std::move(curveGraphPtr), graphRow2StartX, graphRow2Y);
    showLastWidget();
    curveGraph = dynamic_cast<fcn::CurveGraph*>(top->getChild(top->getChildrenCount() - 1));

    addCaption("PieGraph", pieGraphX, graphRow2TopY);
    auto pieGraphPtr = std::make_unique<fcn::PieGraph>();
    pieGraphPtr->addSegment(0, 42, fcn::Color(216, 126, 103));
    pieGraphPtr->addSegment(42, 96, fcn::Color(229, 181, 98));
    pieGraphPtr->addSegment(96, 152, fcn::Color(177, 198, 120));
    pieGraphPtr->addSegment(152, 214, fcn::Color(104, 174, 149));
    pieGraphPtr->addSegment(214, 286, fcn::Color(90, 132, 176));
    pieGraphPtr->addSegment(286, 360, fcn::Color(148, 124, 179));
    pieGraphPtr->setSize(pieGraphWidth, pieGraphHeight);
    pieGraphPtr->setCenter(pieGraphWidth / 2, pieGraphHeight / 2);
    pieGraphPtr->setRadius((std::min(pieGraphWidth, pieGraphHeight) / 2) - 8);
    top->addWidget(std::move(pieGraphPtr), pieGraphX, graphRow2Y);
    showLastWidget();
    pieGraph = dynamic_cast<fcn::PieGraph*>(top->getChild(top->getChildrenCount() - 1));
}

void Application::run()
{
    running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
                if (event.key.keysym.sym == SDLK_f && ((event.key.keysym.mod & KMOD_CTRL) != 0)) {
                    uint32_t const flags = SDL_GetWindowFlags(window.get()) & SDL_WINDOW_FULLSCREEN_DESKTOP;
                    SDL_SetWindowFullscreen(window.get(), flags ^ SDL_WINDOW_FULLSCREEN_DESKTOP);
                }
            }
            if (event.type == SDL_QUIT) {
                running = false;
            }
            input->pushInput(event);
        }

        gui->logic();
        gui->draw();
        SDL_RenderPresent(renderer.get());

        SDL_Delay(500);
    }
}

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    try {
        Application app("FifeGUI using SDL2 Backend: Widgets Example", 1280, 1024);
        app.run();
    } catch (fcn::Exception const & e) {
        std::cerr << e.getMessage() << '\n';
        return 1;
    } catch (std::exception const & e) {
        std::cerr << "Std exception: " << e.what() << '\n';
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception" << '\n';
        return 1;
    }

    return 0;
}
