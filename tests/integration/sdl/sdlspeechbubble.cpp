// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

/**
 * @brief Test application demonstrating the SpeechBubble widget step by step.
 *
 * Keys 1-5 switch between demonstration stages:
 *   1 — Basic speech bubble with default settings
 *   2 — Speech bubble with custom background color
 *   3 — Speech bubble with tail pointing Right
 *   4 — Speech bubble with Label + Accept/Decline buttons
 *   5 — Speech bubble with icon+label option rows
 */

// Corresponding header include
#include "sdlspeechbubble.hpp"

// Standard library includes
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <format>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// Platform config include
#include "fifechan/platform.hpp"

#ifdef _WIN32
    #include <windows.h>
#endif

// Third-party library includes
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <fifechan/backends/sdl3/sdl.hpp>
#include <fifechan/fontloader.hpp>

#include <fifechan.hpp>

// Project headers (subdirs before local)
#include <fifechan/widgets/speechbubble.hpp>

#include <fifechan/gui.hpp>

using tests::integration::sdl::speechbubble::ActionDisplay;
using tests::integration::sdl::speechbubble::Application;
using tests::integration::sdl::speechbubble::OptionMouseListener;

void ActionDisplay::action(fcn::ActionEvent const & actionEvent)
{
    if (mTarget != nullptr) {
        mTarget->setCaption("Clicked: " + actionEvent.getSource()->getId());
        mTarget->adjustSize();
    }
}

void OptionMouseListener::mouseClicked(fcn::MouseEvent& /*mouseEvent*/)
{
    if (mTarget != nullptr) {
        mTarget->setCaption("Selected: " + mText);
        mTarget->adjustSize();
    }
}

namespace
{
    void centerChildInContent(fcn::Container* parent, fcn::Widget* child)
    {
        fcn::Rectangle const ca = parent->getChildrenArea();
        child->setPosition((ca.width - child->getWidth()) / 2, (ca.height - child->getHeight()) / 2);
    }
} // namespace

Application::Application(std::string const & title, int width, int height) :
    mWidth(width), mHeight(height), exePath(getExecutableDir().string())
{
    try {
        init_sdl(title, width, height);
        init_gui(width, height);
    } catch (...) {
        cleanup();
        throw;
    }
}

Application::~Application()
{
    cleanup();
}

std::shared_ptr<SDL_Window> Application::initWindow(std::string const & title, int width, int height, int flags)
{
    SDL_Window* window = SDL_CreateWindow(title.c_str(), width, height, flags);

    if (window == nullptr) {
        throw std::runtime_error(std::string("Failed to create SDL_Window: ") + SDL_GetError());
    }

    return fcn::sdl3::makeSDLSharedPtr(window);
}

std::shared_ptr<SDL_Renderer> Application::initRenderer(std::shared_ptr<SDL_Window> const & window)
{
    SDL_Renderer* renderer = SDL_CreateRenderer(window.get(), nullptr);

    if (renderer == nullptr) {
        throw std::runtime_error(std::string("Failed to create SDL_Renderer: ") + SDL_GetError());
    }

    return fcn::sdl3::makeSDLSharedPtr(renderer);
}

std::filesystem::path Application::getExecutableDir()
{
#ifdef __linux__
    return std::filesystem::read_symlink("/proc/self/exe").parent_path();
#elif defined(_WIN32)
    char buffer[MAX_PATH] = {0};
    GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    return std::filesystem::path(buffer).parent_path();
#else
    return std::filesystem::current_path();
#endif
}

void Application::init_sdl(std::string const & title, int width, int height)
{
    std::filesystem::current_path(Application::getExecutableDir());

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error(std::string("Failed to initialize SDL: ") + SDL_GetError());
    }

    window = initWindow(title, width, height, 0);

    renderer = initRenderer(window);

    imageLoader = std::make_shared<fcn::sdl3::ImageLoader>();
    imageLoader->setRenderer(renderer.get());

    fcn::Image::setImageLoader(imageLoader.get());

    graphics = std::make_unique<fcn::sdl3::Graphics>();
    graphics->setTarget(renderer.get(), width, height);

    input = std::make_unique<fcn::sdl3::Input>();

    gui = std::make_unique<fcn::Gui>();
    gui->setGraphics(std::move(graphics));
    gui->setInput(input.get());
}

void Application::init_gui(int width, int height)
{
    if (!TTF_Init()) {
        std::cerr << "[ERROR] Failed to initialize SDL3_ttf: " << SDL_GetError() << '\n';
        exit(2);
    }

    std::filesystem::path const fontPath = fcn::font::FontLoader::findFontFile("ArchitectsDaughter.ttf");
    if (fontPath.empty()) {
        std::cerr << "[ERROR] Could not find ArchitectsDaughter.ttf in search paths\n";
        exit(5);
    }

    int const fontSize = std::max(12, std::min(width, height) / 48);

    top = std::make_unique<fcn::Container>();
    top->setLayout(fcn::Container::LayoutPolicy::Absolute);
    top->setDimension(fcn::Rectangle(0, 0, width, height));
    top->setOpaque(true);
    top->setBaseColor(fcn::Color(40, 44, 52, 255));
    gui->setTop(top.get());

    try {
        gui->setGlobalFont(fontPath.string(), fontSize);
    } catch (std::exception const & e) {
        std::cerr << "[ERROR] Exception loading font: " << e.what() << '\n';
        exit(5);
    }

    font = gui->getTop()->getFont();

    showInstructions();
}

void Application::showInstructions()
{
    instructionLabel =
        std::make_unique<fcn::Label>("[1] Comic styles  [2] Colored  [3] Right  [4] Quest  [5] Class  [ESC] Quit");
    instructionLabel->setForegroundColor(fcn::Color(200, 200, 200, 255));
    instructionLabel->setPosition(20, 12);
    instructionLabel->adjustSize();
    top->add(instructionLabel.get());

    feedbackLabel = std::make_unique<fcn::Label>("");
    feedbackLabel->setForegroundColor(fcn::Color(255, 220, 100, 255));
    feedbackLabel->setPosition(20, mHeight - 30);
    feedbackLabel->adjustSize();
    top->add(feedbackLabel.get());

    showPage(1);
}

void Application::clearPage()
{
    for (auto& b : mBubbles) {
        if (b)
            top->remove(b.get());
    }
    for (auto& w : mChildren) {
        if (w) {
            if (auto* container = dynamic_cast<fcn::Container*>(w.get())) {
                container->removeAllChildren();
            }
        }
    }
    mChildren.clear();
    mOptionListeners.clear();
    mActionDisplay.reset();
    mBubbles.clear();
}

void Application::showPage(int index)
{
    clearPage();
    feedbackLabel->setCaption("");
    feedbackLabel->adjustSize();

    int const bubbleX = mWidth / 2 - 150;
    int const bubbleY = mHeight / 2 - 80;

    auto const makeComicBubble = [&](fcn::SpeechBubble::TailDirection dir,
                                     fcn::SpeechBubble::TailProfile const & profile,
                                     std::string text,
                                     int x,
                                     int y,
                                     int bw,
                                     int bh,
                                     int cr = 10) -> fcn::SpeechBubble* {
        auto bubble = std::make_unique<fcn::SpeechBubble>();
        bubble->setDimension(fcn::Rectangle(x, y, bw, bh));
        bubble->setBaseColor(fcn::Color(255, 255, 255, 255));
        bubble->setBorderSize(1);
        bubble->setBorderColor(fcn::Color(0, 0, 0, 255));
        bubble->setTailDirection(dir);
        bubble->setTailProfile(profile);
        bubble->setCornerRadius(cr);

        auto label = std::make_unique<fcn::Label>(std::move(text));
        label->setForegroundColor(fcn::Color(80, 80, 80, 255));
        label->setPosition(cr + 4, cr + 4);
        label->adjustSize();
        auto* labelPtr = label.get();
        bubble->add(labelPtr);
        mChildren.push_back(std::move(label));

        fcn::SpeechBubble* raw = bubble.get();
        top->add(raw);
        mBubbles.push_back(std::move(bubble));
        return raw;
    };

    switch (index) {
    case 1: {
        int const bw  = 224;
        int const bh  = 232;
        int const gap = 24;
        int const x0  = (mWidth - bw * 5 - gap * 4) / 2;
        int const y0  = (mHeight - bh) / 2;

        auto makeStyle = [&](fcn::SpeechBubble::BubbleStyle style,
                             fcn::SpeechBubble::TailDirection dir,
                             std::string text,
                             int x,
                             int cr = 22) -> fcn::SpeechBubble* {
            auto bubble = std::make_unique<fcn::SpeechBubble>();
            bubble->setDimension(fcn::Rectangle(x, y0, bw, bh));
            bubble->setBaseColor(fcn::Color(255, 255, 255, 255));
            bubble->setBorderSize(1);
            bubble->setBorderColor(fcn::Color(0, 0, 0, 255));
            bubble->setBubbleStyle(style);
            bubble->setTailDirection(dir);
            bubble->setCornerRadius(cr);
            bubble->setTailWidth(38);
            bubble->setTailHeight(30);

            auto label = std::make_unique<fcn::Label>(std::move(text));
            label->setForegroundColor(fcn::Color(80, 80, 80, 255));
            label->adjustSize();
            auto* labelPtr = label.get();
            centerChildInContent(bubble.get(), labelPtr);
            bubble->add(labelPtr);
            mChildren.push_back(std::move(label));

            fcn::SpeechBubble* raw = bubble.get();
            top->add(raw);
            mBubbles.push_back(std::move(bubble));
            return raw;
        };

        makeStyle(
            fcn::SpeechBubble::BubbleStyle::Classic,
            fcn::SpeechBubble::TailDirection::BottomLeft,
            "Classic\nDialogue",
            x0);

        makeStyle(
            fcn::SpeechBubble::BubbleStyle::Round,
            fcn::SpeechBubble::TailDirection::BottomLeft,
            "Round\nSoft",
            x0 + bw + gap);

        makeStyle(
            fcn::SpeechBubble::BubbleStyle::Thought,
            fcn::SpeechBubble::TailDirection::BottomLeft,
            "Thought\nInner voice",
            x0 + (bw + gap) * 2);

        makeStyle(
            fcn::SpeechBubble::BubbleStyle::Shout,
            fcn::SpeechBubble::TailDirection::None,
            "Shout\nImpact",
            x0 + (bw + gap) * 3);

        makeStyle(
            fcn::SpeechBubble::BubbleStyle::Whisper,
            fcn::SpeechBubble::TailDirection::BottomLeft,
            "Whisper\nQuiet",
            x0 + (bw + gap) * 4);

        // Second row: the four corner tail directions on classic bubbles
        int const bw2  = 196;
        int const bh2  = 110;
        int const gap2 = 24;
        int const x02  = (mWidth - bw2 * 4 - gap2 * 3) / 2;
        int const y02  = y0 + bh + 20;

        auto makeCorner = [&](fcn::SpeechBubble::TailDirection dir, std::string text, int x) {
            auto bubble = std::make_unique<fcn::SpeechBubble>();
            bubble->setDimension(fcn::Rectangle(x, y02, bw2, bh2));
            bubble->setBaseColor(fcn::Color(255, 255, 255, 255));
            bubble->setBorderSize(1);
            bubble->setBorderColor(fcn::Color(0, 0, 0, 255));
            bubble->setTailDirection(dir);
            bubble->setCornerRadius(16);
            bubble->setTailWidth(34);
            bubble->setTailHeight(26);

            auto label = std::make_unique<fcn::Label>(std::move(text));
            label->setForegroundColor(fcn::Color(80, 80, 80, 255));
            label->adjustSize();
            auto* labelPtr = label.get();
            centerChildInContent(bubble.get(), labelPtr);
            bubble->add(labelPtr);
            mChildren.push_back(std::move(label));

            fcn::SpeechBubble* raw = bubble.get();
            top->add(raw);
            mBubbles.push_back(std::move(bubble));
            return raw;
        };

        makeCorner(fcn::SpeechBubble::TailDirection::BottomRight, "Bottom-Right", x02);
        makeCorner(fcn::SpeechBubble::TailDirection::TopRight, "Top-Right", x02 + bw2 + gap2);
        makeCorner(fcn::SpeechBubble::TailDirection::TopLeft, "Top-Left", x02 + (bw2 + gap2) * 2);
        makeCorner(fcn::SpeechBubble::TailDirection::BottomLeft, "Bottom-Left", x02 + (bw2 + gap2) * 3);

        feedbackLabel->setCaption("Classic | Round | Thought | Shout | Whisper  +  corner tail directions");
        break;
    }
    case 2: {
        fcn::SpeechBubble* b = makeComicBubble(
            fcn::SpeechBubble::TailDirection::Down,
            fcn::SpeechBubble::TailProfile::sharp(),
            "A colored SpeechBubble",
            bubbleX,
            bubbleY,
            300,
            140);
        b->setBaseColor(fcn::Color(100, 180, 230, 255));
        b->setTailColor(fcn::Color(100, 180, 230, 255));
        b->setBorderSize(0);
        feedbackLabel->setCaption("Colored bubble");
        break;
    }
    case 3: {
        fcn::SpeechBubble* b = makeComicBubble(
            fcn::SpeechBubble::TailDirection::Right,
            fcn::SpeechBubble::TailProfile::sharp(),
            "Tail pointing Right",
            bubbleX - 40,
            bubbleY,
            320,
            140);
        b->setBaseColor(fcn::Color(220, 190, 160, 255));
        b->setBorderSize(0);
        b->setTailColor(fcn::Color(220, 190, 160, 255));
        feedbackLabel->setCaption("Right tail, warm colors");
        break;
    }
    case 4: {
        auto bubble = std::make_unique<fcn::SpeechBubble>();
        bubble->setDimension(fcn::Rectangle(bubbleX, bubbleY, 300, 160));
        bubble->setBaseColor(fcn::Color(160, 210, 175, 255));
        bubble->setTailColor(fcn::Color(160, 210, 175, 255));

        auto questLabel = std::make_unique<fcn::Label>("Would you like to accept\nthe quest 'Save the Princess'?");
        questLabel->setPosition(10, 10);
        questLabel->adjustSize();

        auto acceptBtn = std::make_unique<fcn::Button>("Accept");
        acceptBtn->setId("Accept");
        acceptBtn->setPosition(30, 80);
        acceptBtn->adjustSize();

        auto declineBtn = std::make_unique<fcn::Button>("Decline");
        declineBtn->setId("Decline");
        declineBtn->setPosition(150, 80);
        declineBtn->adjustSize();

        mActionDisplay = std::make_unique<ActionDisplay>(feedbackLabel.get());
        acceptBtn->addActionListener(mActionDisplay.get());
        declineBtn->addActionListener(mActionDisplay.get());

        auto* questLabelPtr = questLabel.get();
        auto* acceptPtr     = acceptBtn.get();
        auto* declinePtr    = declineBtn.get();

        bubble->add(questLabelPtr);
        bubble->add(acceptPtr);
        bubble->add(declinePtr);

        mChildren.push_back(std::move(questLabel));
        mChildren.push_back(std::move(acceptBtn));
        mChildren.push_back(std::move(declineBtn));
        top->add(bubble.get());
        mBubbles.push_back(std::move(bubble));
        feedbackLabel->setCaption("Quest dialog");
        break;
    }
    case 5: {
        auto bubble = std::make_unique<fcn::SpeechBubble>();
        bubble->setDimension(fcn::Rectangle(bubbleX - 30, bubbleY - 20, 360, 220));
        bubble->setBaseColor(fcn::Color(195, 185, 220, 255));
        bubble->setTailColor(fcn::Color(195, 185, 220, 255));
        bubble->setTailHeight(16);
        bubble->setTailWidth(20);

        auto promptLabel = std::make_unique<fcn::Label>("Choose your class:");
        promptLabel->setPosition(10, 8);
        promptLabel->adjustSize();

        int const optionY  = 32;
        int const optionH  = 44;
        int const iconSize = 32;

        struct OptionDef
        {
                std::string label;
        };
        OptionDef const options[3] = {
            {"Warrior"},
            {"Mage"},
            {"Rogue"},
        };

        for (int i = 0; i < 3; ++i) {
            auto optionBox = std::make_unique<fcn::Container>();
            optionBox->setDimension(fcn::Rectangle(8, optionY + i * optionH, 330, 40));
            optionBox->setBorderSize(1);
            optionBox->setBaseColor(fcn::Color(220, 215, 235, 255));

            fcn::Color const iconColors[3] = {
                fcn::Color(200, 80, 80, 255),
                fcn::Color(80, 140, 210, 255),
                fcn::Color(80, 190, 130, 255),
            };

            auto iconBox = std::make_unique<fcn::Container>();
            iconBox->setDimension(fcn::Rectangle(4, 4, iconSize, iconSize));
            iconBox->setOpaque(true);
            iconBox->setBaseColor(iconColors[i]);

            auto optionLabel = std::make_unique<fcn::Label>(options[i].label);
            optionLabel->setPosition(iconSize + 12, 10);
            optionLabel->adjustSize();

            auto* iconBoxPtr     = iconBox.get();
            auto* optionLabelPtr = optionLabel.get();

            optionBox->add(iconBoxPtr);
            optionBox->add(optionLabelPtr);

            auto listener     = std::make_unique<OptionMouseListener>(feedbackLabel.get(), options[i].label);
            auto* listenerPtr = listener.get();
            optionBox->addMouseListener(listenerPtr);

            auto* optionBoxPtr = optionBox.get();
            bubble->add(optionBoxPtr);

            mChildren.push_back(std::move(optionBox));
            mChildren.push_back(std::move(iconBox));
            mChildren.push_back(std::move(optionLabel));
            mOptionListeners.push_back(std::move(listener));
        }

        auto* promptLabelPtr = promptLabel.get();
        bubble->add(promptLabelPtr);
        mChildren.push_back(std::move(promptLabel));
        top->add(bubble.get());
        mBubbles.push_back(std::move(bubble));
        feedbackLabel->setCaption("Class selection");
        break;
    }
    default:
        break;
    }
    feedbackLabel->adjustSize();
}

void Application::cleanup()
{
    clearPage();
    fcn::Widget::resetGlobalFont();
    fcn::Image::resetImageLoader();

    instructionLabel.reset();
    feedbackLabel.reset();
    gui.reset();
    top.reset();
    imageLoader.reset();
    input.reset();
    graphics.reset();

    renderer.reset();
    window.reset();
    SDL_StopTextInput(window.get());
    TTF_Quit();
    SDL_Quit();
}

void Application::screenshotPage(int page, char const * filename)
{
    showPage(page);
    gui->logic();
    gui->draw();
    SDL_RenderPresent(renderer.get());

    SDL_Surface* surf = SDL_RenderReadPixels(renderer.get(), nullptr);
    if (surf) {
        SDL_SaveBMP(surf, filename);
        SDL_DestroySurface(surf);
    }
}

void Application::run()
{
    while (this->running) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_ESCAPE) {
                    this->running = false;
                }
                if (event.key.key >= SDLK_1 && event.key.key <= SDLK_5) {
                    showPage(static_cast<int>(event.key.key - SDLK_1 + 1));
                }
                if (event.key.key == SDLK_F && (event.key.mod & SDL_KMOD_CTRL) != 0) {
                    uint32_t const fullscreen = SDL_GetWindowFlags(window.get()) & SDL_WINDOW_FULLSCREEN;
                    SDL_SetWindowFullscreen(window.get(), fullscreen == 0);
                }
            } else if (event.type == SDL_EVENT_QUIT) {
                this->running = false;
            }

            input->pushInput(event);
        }

        gui->logic();
        gui->draw();
        SDL_RenderPresent(renderer.get());
    }
}

int main(int argc, char** argv)
{
    int screenshotPage          = 0;
    char const * screenshotFile = nullptr;
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--screenshot") == 0 && i + 2 < argc) {
            screenshotPage = std::atoi(argv[++i]);
            screenshotFile = argv[++i];
        }
    }

    try {
        std::string const fifeguiVersion = fcn::fifechanVersion();

        int const sdlVersion            = SDL_GetVersion();
        std::string const sdlVersionStr = std::format(
            "{}.{}.{}",
            SDL_VERSIONNUM_MAJOR(sdlVersion),
            SDL_VERSIONNUM_MINOR(sdlVersion),
            SDL_VERSIONNUM_MICRO(sdlVersion));

        std::string const title =
            std::format("FifeGUI v{} using SDL {}: SpeechBubble Demo", fifeguiVersion, sdlVersionStr);

        Application app(title, 1280, 768);
        if (screenshotPage > 0 && screenshotFile) {
            app.screenshotPage(screenshotPage, screenshotFile);
        } else {
            app.run();
        }
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
