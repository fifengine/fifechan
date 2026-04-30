// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

/**
 * SDL Markdown Editor Demo
 *
 * This demo application validates the new widget elements:
 * - MenuBar
 *   - Menu, Menuitem, MenuPopup
 * - StatusBar
 *   - BarSection
 * - ActivityBar
 *   - ActivityBarItem
 * - Panel
 *   - PrimaryPanel
 *   - SecondaryPanel
 *
 * The demo also uses a Glyph font to demonstrate the usage of
 * glyph icons in the menu and activity bar.
 */

// Corresponding header include
#include "sdlmdedit.hpp"

// Standard library includes
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// Platform-specific includes
#if defined(_WIN32)
    #include <windows.h>
#endif // _WIN32

// Third-party library includes
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

using tests::integration::sdl::mdedit::Application;

Application::Application(std::string const & title, int width, int height)
{
    try {
        init_SDL(title, width, height);
        init_GUI(width, height);
    } catch (...) {
        cleanup();
        throw;
    }
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

Application::~Application()
{
    cleanup();
}

std::shared_ptr<SDL_Window> Application::initWindow(std::string const & title, int width, int height, int flags)
{
    SDL_Window* createdWindow = SDL_CreateWindow(title.c_str(), width, height, flags);
    if (createdWindow == nullptr) {
        throw std::runtime_error("Failed to create SDL_Window");
    }

    return fcn::sdl3::makeSDLSharedPtr(createdWindow);
}

std::shared_ptr<SDL_Renderer> Application::initRenderer(std::shared_ptr<SDL_Window> const & window)
{
    SDL_Renderer* createdRenderer = SDL_CreateRenderer(window.get(), nullptr);
    if (createdRenderer == nullptr) {
        throw std::runtime_error(std::string("Failed to create SDL_Renderer: ") + SDL_GetError());
    }

    return fcn::sdl3::makeSDLSharedPtr(createdRenderer);
}

void Application::init_SDL(std::string const & title, int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << '\n';
        exit(1);
    }

    window = initWindow(title, width, height, 0);

    SDL_RaiseWindow(window.get());

    SDL_SetRenderVSync(renderer.get(), 1);
}

void Application::cleanup()
{
    // Reset GUI (clears non-owning pointers) then free top container
    gui.reset();
    top.reset();
    graphics.reset();
    input.reset();
    renderer.reset();
    window.reset();
    // Release any fonts held by the Application before shutting down SDL_ttf
    // to ensure TTF_CloseFont is called while the library is still initialized.
    activityFont.reset();
    TTF_Quit();
    SDL_Quit();
}

void Application::init_GUI(int width, int height)
{
    graphics = std::make_unique<fcn::sdl3::Graphics>();
    graphics->setTarget(renderer.get(), width, height);

    input = std::make_unique<fcn::sdl3::Input>();

    gui = std::make_unique<fcn::Gui>();
    gui->setGraphics(graphics.get());
    gui->setInput(input.get());

    // Create top container
    top = std::make_unique<fcn::Container>();
    top->setLayout(fcn::Container::LayoutPolicy::Vertical);
    // Make it fixed size matching the window, so layout operations
    // do not shrink it to fit child content.
    top->setFixedSize(fcn::Size(width, height));
    top->setDimension(fcn::Rectangle(0, 0, width, height));
    top->setOpaque(true);
    top->setBaseColor(fcn::Color(220, 220, 220, 255));

    // Set top widget early so widgets can access the global font
    gui->setTop(top.get());

    // Initialize SDL_ttf and try to set a global font from executable dir
    try {
        std::filesystem::current_path(getExecutableDir());
    } catch (...) {
        // ignore; continue with current working directory
    }

    // TODO we need a better FONT finding and loading API
    if (!TTF_Init()) {
        // SDL_ttf initialization failed - continue without text rendering
    } else {
        std::filesystem::path fontPath = getExecutableDir() / "ArchitectsDaughter.ttf";
        // Try fallbacks if the font isn't next to the executable (CMake may not copy it for this test)
        std::vector<std::filesystem::path> candidates = {
            fontPath,
            std::filesystem::current_path() / "tests" / "resources" / "ArchitectsDaughter.ttf",
            std::filesystem::path("tests") / "resources" / "ArchitectsDaughter.ttf",
        };

        std::filesystem::path chosen;
        auto it = std::find_if(candidates.begin(), candidates.end(), [](auto const & p) {
            return std::filesystem::exists(p);
        });
        if (it != candidates.end()) {
            chosen = *it;
        }

        if (chosen.empty()) {
            // Try source tree relative to this source file (tests/resources)
            try {
                auto src = std::filesystem::path(__FILE__).parent_path().parent_path().parent_path().parent_path() /
                           "tests" / "resources" / "ArchitectsDaughter.ttf";
                if (std::filesystem::exists(src)) {
                    chosen = src;
                }
            } catch (...) {
                // ignore
            }
        }

        if (!chosen.empty()) {
            try {
                int const fontSize = std::max(12, std::min(width, height) / 64);
                gui->setGlobalFont(chosen.string(), fontSize);
                this->font = gui->getTop()->getFont();
            } catch (std::exception const &) {
                // Failed to load font - continue without global font
            }
        }
    }

    // Try to preload an OpenMoji font for menu/activity icons so menu items
    // can use glyph icons (e.g. "📁" for Open).
    try {
        std::vector<std::filesystem::path> candidates = {
            std::filesystem::path("/workspaces/fifechan_new/tests/resources/OpenMoji-color-colr0_svg.ttf"),
            std::filesystem::current_path() / "tests" / "resources" / "OpenMoji-color-colr0_svg.ttf",
            std::filesystem::path("tests") / "resources" / "OpenMoji-color-colr0_svg.ttf",
        };

        std::filesystem::path openmoji;
        auto it = std::find_if(candidates.begin(), candidates.end(), [](auto const & p) {
            return std::filesystem::exists(p);
        });
        if (it != candidates.end()) {
            openmoji = *it;
        }

        if (!openmoji.empty()) {
            // Use a size suitable for menu icons (slightly larger than default font)
            this->activityFont = gui->getGraphics()->createFont(openmoji.string(), 18);
        }
    } catch (std::exception const &) {
        // ignore
    }

    // Create MenuBar (owned by `top` container)
    {
        auto mb = std::make_unique<fcn::MenuBar>();
        menuBar = mb.get();
        menuBar->setPadding(6);
        menuBar->setSpacing(8);

        // Ensure menu items size to their content only, do not expand equally
        menuBar->setUniformSize(false);

        // Request focus for menu bar so it receives keyboard events
        // Note: FocusHandler may not be initialized yet, so we set up the focus manually later
        menuBar->setFocusable(true);

        top->addWidget(std::move(mb));

        // Create basic menus
        {
            // File menu
            auto* filePopup = new fcn::MenuPopup();
            filePopup->addItem(new fcn::MenuItem("New"));
            // Create Open item with glyph icon and shortcut
            {
                auto* openItem = new fcn::MenuItem("Open");
                openItem->setShortcut("Ctrl+O");
                if (this->activityFont) {
                    openItem->setIconGlyph(std::string("📁"), this->activityFont.get());
                } else {
                    // set glyph without font will fall back to main font if available
                    openItem->setIconGlyph(std::string("📁"));
                }
                filePopup->addItem(openItem);
            }
            filePopup->addSeparator();
            filePopup->addItem(new fcn::MenuItem("Exit"));

            auto* fileItem = dynamic_cast<fcn::MenuItem*>(menuBar->addMenu("File", filePopup));

            if (fileItem) {
                fileItem->setPaddingLeft(8);
                fileItem->setPaddingRight(8);
                fileItem->setPaddingTop(6);
                fileItem->setPaddingBottom(6);
                fileItem->setHeight(fileItem->getFont()->getHeight());
                fileItem->adjustSize();
                // Do not allow the top-level menu items to expand
                fileItem->setHorizontalExpand(false);
            }

            // Help menu
            auto* helpPopup = new fcn::MenuPopup();
            helpPopup->addItem(new fcn::MenuItem("About"));

            auto* helpItem = dynamic_cast<fcn::MenuItem*>(menuBar->addMenu("Help", helpPopup));

            if (helpItem) {
                helpItem->setPaddingLeft(8);
                helpItem->setPaddingRight(8);
                helpItem->setPaddingTop(6);
                helpItem->setPaddingBottom(6);
                helpItem->setHeight(helpItem->getFont()->getHeight() + 12);
                helpItem->adjustSize();
                helpItem->setHorizontalExpand(false);
            }
        }
    }

    // Create content container (horizontal layout) and add to top
    std::unique_ptr<fcn::Container> content = std::make_unique<fcn::Container>();
    content->setLayout(fcn::Container::LayoutPolicy::Horizontal);
    // Let content expand to fill remaining vertical space in `top`.
    content->setHorizontalExpand(true);
    content->setVerticalExpand(true);
    auto* contentPtr = content.get();

    // Create ActivityBar (owned by `content` container)
    {
        auto ab     = std::make_unique<fcn::ActivityBar>(48);
        activityBar = ab.get();
        contentPtr->addWidget(std::move(ab));

        // Try to load an OpenMoji TTF for activity bar icons and add emoji items
        try {
            // Prefer the color COLR OpenMoji variant first to test colored emoji rendering
            std::vector<std::filesystem::path> candidates = {
                std::filesystem::path("/workspaces/fifechan_new/tests/resources/OpenMoji-color-colr0_svg.ttf"),
                std::filesystem::path("/workspaces/fifechan_new/tests/resources/OpenMoji-black-glyf.ttf"),
                std::filesystem::current_path() / "tests" / "resources" / "OpenMoji-color-colr0_svg.ttf",
                std::filesystem::current_path() / "tests" / "resources" / "OpenMoji-black-glyf.ttf",
                std::filesystem::path("tests") / "resources" / "OpenMoji-color-colr0_svg.ttf",
                std::filesystem::path("tests") / "resources" / "OpenMoji-black-glyf.ttf",
            };

            std::filesystem::path openmoji;
            auto it = std::find_if(candidates.begin(), candidates.end(), [](auto const & p) {
                return std::filesystem::exists(p);
            });
            if (it != candidates.end()) {
                openmoji = *it;
            }

            if (!openmoji.empty()) {
                // keep the font alive on the Application instance
                // Do not overwrite if already created earlier (avoid dangling pointers)
                if (!this->activityFont) {
                    this->activityFont = gui->getGraphics()->createFont(openmoji.string(), 22);
                }
            }
        } catch (std::exception const &) {
            // Failed to load OpenMoji font - continue without activity icons
        }

        // Add some emoji activity items (icons are UTF-8 emoji strings)
        if (activityBar != nullptr) {
            std::vector<std::string> icons = {"📁", "🔍", "⭐", "⚙️"};
            for (auto const & ico : icons) {
                auto item = std::make_unique<fcn::ActivityBarItem>(ico);
                if (this->activityFont) {
                    item->setFont(this->activityFont.get());
                }
                item->setSize(40);
                activityBar->addWidget(std::move(item));
            }
        }
    }

    // Create PrimaryPanel (owned by `content` container)
    {
        auto pp      = std::make_unique<fcn::PrimaryPanel>("Explorer");
        primaryPanel = pp.get();
        primaryPanel->setDefaultWidth(250);
        primaryPanel->setOpaque(true);
        contentPtr->addWidget(std::move(pp));
    }

    // Create editor area (owned by `content` container)
    {
        auto editorArea = std::make_unique<fcn::Container>();
        editorArea->setLayout(fcn::Container::LayoutPolicy::Vertical);
        // Editor area should expand to fill content area.
        editorArea->setHorizontalExpand(true);
        editorArea->setVerticalExpand(true);
        auto* editorPtr = editorArea.get();

        // Create TextBox for editing (owned by editor area)
        auto tb = std::make_unique<fcn::TextBox>();
        tb->setText("# Markdown Editor\n\nStart typing here...");
        tb->setOpaque(true);
        // Text box should expand to fill editor area
        tb->setHorizontalExpand(true);
        tb->setVerticalExpand(true);
        textBox = tb.get();
        editorPtr->addWidget(std::move(tb));

        // move editor area into content
        contentPtr->addWidget(std::move(editorArea));
    }

    // Create SecondaryPanel (owned by `content` container)
    {
        auto sp        = std::make_unique<fcn::SecondaryPanel>("Outline");
        secondaryPanel = sp.get();
        secondaryPanel->setDefaultWidth(250);
        secondaryPanel->setOpaque(true);
        contentPtr->addWidget(std::move(sp));
    }

    // Wire ActivityBar items to panels (first item = primary panel, second = secondary panel)
    if (activityBar != nullptr) {
        // First ActivityBarItem (📁) toggles PrimaryPanel
        if (contentPtr->getChildrenCount() >= 2) {
            fcn::ActivityBarItem* firstItem = dynamic_cast<fcn::ActivityBarItem*>(activityBar->getChild(0));
            if (firstItem != nullptr && primaryPanel != nullptr) {
                firstItem->setPanel(primaryPanel);
                firstItem->setPanelVisible(true);
            }
        }
        // Second ActivityBarItem (🔍) toggles SecondaryPanel
        if (contentPtr->getChildrenCount() >= 3) {
            fcn::ActivityBarItem* secondItem = dynamic_cast<fcn::ActivityBarItem*>(activityBar->getChild(1));
            if (secondItem != nullptr && secondaryPanel != nullptr) {
                secondItem->setPanel(secondaryPanel);
                secondItem->setPanelVisible(false);
            }
        }
    }

    // move content into top
    top->addWidget(std::move(content));

    // Create StatusBar (owned by `top` container)
    {
        auto sb = std::make_unique<fcn::StatusBar>();
        // Initial status bar text - will be updated by updateStatusBar()
        sb->setText("Ready");
        statusBar = sb.get();
        // Fix status bar height and make it span full width
        int const statusBarHeight = 24;
        statusBar->setFixedSize(fcn::Size(width, statusBarHeight));
        statusBar->setDimension(fcn::Rectangle(0, 0, width, statusBarHeight));
        top->addWidget(std::move(sb));
    }

    // TODO why do i have to call this to make things show up?
    top->adaptLayout();

    // Only set menu bar height - let width shrink to fit menu items
    int const menuBarHeight   = 36;
    int const statusBarHeight = 24;
    if (menuBar != nullptr) {
        // Manually reposition menu items for horizontal layout since
        // automatic layout doesn't seem to work properly
        auto const * mbc = dynamic_cast<fcn::Container*>(menuBar);
        int maxX         = 0;
        if (mbc) {
            int xPos = 0;
            for (unsigned j = 0; j < mbc->getChildrenCount(); ++j) {
                fcn::Widget* item = mbc->getChild(j);
                if (!item)
                    continue;
                fcn::Rectangle dim = item->getDimension();
                dim.x              = xPos;
                item->setDimension(dim);
                xPos += dim.width + 8; // spacing
            }

            // Calculate menu bar width from children positions
            for (unsigned j = 0; j < mbc->getChildrenCount(); ++j) {
                fcn::Widget const * item = mbc->getChild(j);
                if (!item)
                    continue;
                int itemRight = item->getX() + item->getWidth();
                if (itemRight > maxX)
                    maxX = itemRight;
            }
        }
        int menuBarWidth = maxX + 4; // padding

        // Then set fixed height
        menuBar->setFixedHeight(menuBarHeight);
        // Ensure width is at least the content width
        if (menuBar->getWidth() < menuBarWidth) {
            menuBar->setWidth(menuBarWidth);
        }
        // Set the menu bar's position and dimensions in the top container
        menuBar->setDimension(fcn::Rectangle(0, 0, menuBar->getWidth(), menuBarHeight));
    }
    // Ensure content and editor area fill the area between menu and status bar.
    int const contentY   = menuBarHeight;
    int const contentH   = height - menuBarHeight - statusBarHeight;
    int const panelWidth = 250;
    contentPtr->setFixedSize(fcn::Size(width, contentH));
    contentPtr->setDimension(fcn::Rectangle(0, contentY, width, contentH));
    // ActivityBar (index 0) - first child
    if (contentPtr->getChildrenCount() >= 1) {
        fcn::Widget* activityWidget = contentPtr->getChild(0);
        if (activityWidget != nullptr) {
            activityWidget->setFixedSize(fcn::Size(48, contentH));
            activityWidget->setDimension(fcn::Rectangle(0, contentY, 48, contentH));
        }
    }
    // PrimaryPanel (index 1) - left panel
    if (contentPtr->getChildrenCount() >= 2) {
        fcn::Widget* primaryWidget = contentPtr->getChild(1);
        if (primaryWidget != nullptr) {
            primaryWidget->setFixedSize(fcn::Size(panelWidth, contentH));
            primaryWidget->setDimension(fcn::Rectangle(48, contentY, panelWidth, contentH));
        }
    }
    // Editor area (index 2) - middle, flexible width
    if (contentPtr->getChildrenCount() >= 3) {
        fcn::Widget* editorAreaWidget = contentPtr->getChild(2);
        if (editorAreaWidget != nullptr) {
            int const editorX = 48 + panelWidth;
            int const editorW = width - 48 - panelWidth - panelWidth;
            editorAreaWidget->setFixedSize(fcn::Size(editorW, contentH));
            editorAreaWidget->setDimension(fcn::Rectangle(editorX, contentY, editorW, contentH));
        }
    }
    // SecondaryPanel (index 3) - right panel
    if (contentPtr->getChildrenCount() >= 4) {
        fcn::Widget* secondaryWidget = contentPtr->getChild(3);
        if (secondaryWidget != nullptr) {
            int const secondaryX = width - panelWidth;
            secondaryWidget->setFixedSize(fcn::Size(panelWidth, contentH));
            secondaryWidget->setDimension(fcn::Rectangle(secondaryX, contentY, panelWidth, contentH));
        }
    }
    if (statusBar != nullptr) {
        statusBar->setFixedSize(fcn::Size(width, statusBarHeight));
        statusBar->setDimension(fcn::Rectangle(0, height - statusBarHeight, width, statusBarHeight));
    }

    // Initialize cursor tracking for status bar updates
    mLastCaretRow    = 0;
    mLastCaretColumn = 0;
    mLastText        = textBox ? textBox->getText() : "";
    updateStatusBar();
}

void Application::updateStatusBar()
{
    if (textBox == nullptr || statusBar == nullptr) {
        return;
    }

    // Get current cursor position (add 1 for 1-based line/column display)
    int const currentRow    = static_cast<int>(textBox->getCaretRow()) + 1;
    int const currentColumn = static_cast<int>(textBox->getCaretColumn()) + 1;

    // Get current text
    std::string const & currentText = textBox->getText();

    // Check if anything changed
    bool const cursorChanged = (currentRow != mLastCaretRow) || (currentColumn != mLastCaretColumn);
    bool const textChanged   = (currentText != mLastText);

    if (cursorChanged || textChanged) {
        // Update stored values
        mLastCaretRow    = currentRow;
        mLastCaretColumn = currentColumn;
        mLastText        = currentText;

        // Calculate character count
        int const charCount = static_cast<int>(currentText.size());

        // Calculate word count (count non-whitespace sequences)
        int wordCount = 0;
        bool inWord   = false;
        for (char c : currentText) {
            if (std::isspace(static_cast<unsigned char>(c))) {
                inWord = false;
            } else if (!inWord) {
                inWord = true;
                ++wordCount;
            }
        }

        // Format status bar text: "Ln X, Col Y | N chars | M words"
        std::string const statusText =
            std::format("Ln {}, Col {} | {} chars | {} words", currentRow, currentColumn, charCount, wordCount);

        statusBar->setText(statusText);
    }
}

void Application::run()
{
    running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            } else if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_ESCAPE) {
                    running = false;
                }
            }
            input->pushInput(event);
        }

        SDL_SetRenderDrawColor(renderer.get(), 30, 30, 30, 255);
        SDL_RenderClear(renderer.get());

        gui->logic();

        // Update status bar with cursor position and text stats
        updateStatusBar();

        gui->draw();

        SDL_RenderPresent(renderer.get());

        SDL_Delay(16); // ~60 FPS
        // SDL_Delay(160);
    }
}

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    try {
        // Append library version to window title
        std::string const fifeguiVersion = fcn::fifechanVersion();
        std::string const title = std::format("FifeGUI v{} using SDL2 Backend: Markdown Editor", fifeguiVersion);
        Application app(title, 1024, 768);
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
