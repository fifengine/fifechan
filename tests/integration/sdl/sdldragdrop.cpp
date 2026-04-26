// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors
//

/**
 * SDL Drag-and-Drop Inventory Demo with Tooltip Widget
 * -------------------------------------------------
 *
 * This is an integration test for a drag-and-drop system.
 * The player has a 3x3 inventory grid and can drag items to a 4x3 stash box.
 *
 * It also demonstrates the tooltip widget with:
 *   - a hover delay of 300ms
 *   - an ALT-key modifier for extended stats view
 */

// Standard library includes
#include <algorithm>
#include <filesystem>
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

// Platform config include
#include "fifechan/platform.hpp"

#ifdef _WIN32
    #include <windows.h>
#endif

// Third-party library includes
#include <SDL2/SDL.h>

#include <fifechan/widgets/tooltip.hpp>

#include <fifechan/backends/sdl2/sdl.hpp>
#include <fifechan/dragdrop.hpp>

#include <fifechan.hpp>

namespace
{

    // Item database entry (hardcoded for demo)
    struct ItemData
    {
        int id;
        std::string name;
        int damage; // Stat 1
        int armor;  // Stat 2
        int weight; // Stat 3
        std::string description;

        ItemData(int i, std::string const & n, int dmg, int arm, int wgt, std::string const & desc) :
            id(i), name(n), damage(dmg), armor(arm), weight(wgt), description(desc)
        {
        }
    };

    // Hardcoded item database (internal to demo)
    std::vector<ItemData> const itemDatabase = {
        {1, "Sword", 15, 0, 3, "A sharp blade for close combat."},
        {2, "Shield", 0, 10, 5, "A sturdy shield for protection."},
        {3, "Potion", 0, 0, 1, "Restores health when consumed."},
        {4, "Bow", 12, 0, 2, "A ranged weapon for hunters."},
        {5, "Helmet", 0, 5, 2, "Protects the head from damage."},
    };

    // Find item in database by ID
    ItemData const * findItemData(int id)
    {
        auto it = std::find_if(itemDatabase.begin(), itemDatabase.end(), [id](ItemData const & item) {
            return item.id == id;
        });

        if (it != itemDatabase.end())
            return &*it;

        return nullptr;
    }

    // Inventory item structure (references database by ID)
    struct InventoryItem
    {
        int itemId; // References itemDatabase
        int quantity;

        explicit InventoryItem(int id = 0, int qty = 1) : itemId(id), quantity(qty) { }

        bool isValid() const
        {
            return itemId > 0 && quantity > 0;
        }
    };

    // Inventory slot structure
    struct InventorySlot
    {
        int x, y;          // Position in pixels
        int width, height; // Pixel dimensions
        std::optional<InventoryItem> item;
        bool isPlayerInventory; // true = player, false = stash

        InventorySlot(int px, int py, int w, int h, bool player = true) :
            x(px), y(py), width(w), height(h), item(std::nullopt), isPlayerInventory(player)
        {
        }

        bool contains(int mx, int my) const
        {
            return mx >= x && mx < x + width && my >= y && my < y + height;
        }
    };

    // Drag state structure
    struct DragState
    {
        bool isDragging;
        InventoryItem item;
        int startSlotIndex;
        int offsetX, offsetY;

        DragState() : isDragging(false), startSlotIndex(-1), offsetX(0), offsetY(0) { }
    };

    class InventorySystem
    {
    private:
        std::vector<InventorySlot> slots;
        DragState dragState;

        int const PLAYER_INV_START_X = 50;
        int const PLAYER_INV_START_Y = 50;
        int const WINDOW_WIDTH       = 640;
        int const STASH_INV_START_X  = WINDOW_WIDTH - PLAYER_INV_START_X - (4 * 60); // Same right margin as left
        int const STASH_INV_START_Y  = 50;
        int const SLOT_SIZE          = 60;
        int const PLAYER_COLS        = 3;
        int const PLAYER_ROWS        = 3;
        int const STASH_COLS         = 4;
        int const STASH_ROWS         = 3;

    public:
        InventorySystem()
        {
            initializeSlots();
        }

        void initializeSlots()
        {
            // Player inventory slots (3x3 grid)
            for (int row = 0; row < PLAYER_ROWS; ++row) {
                for (int col = 0; col < PLAYER_COLS; ++col) {
                    slots.emplace_back(
                        PLAYER_INV_START_X + col * SLOT_SIZE,
                        PLAYER_INV_START_Y + row * SLOT_SIZE,
                        SLOT_SIZE,
                        SLOT_SIZE,
                        true);
                }
            }

            // Stash inventory slots (4x3 grid for more space)
            for (int row = 0; row < STASH_ROWS; ++row) {
                for (int col = 0; col < STASH_COLS; ++col) {
                    slots.emplace_back(
                        STASH_INV_START_X + col * SLOT_SIZE,
                        STASH_INV_START_Y + row * SLOT_SIZE,
                        SLOT_SIZE,
                        SLOT_SIZE,
                        false);
                }
            }

            // Add some test items to player inventory (slots 0, 1, 2)
            slots[0].item = InventoryItem(1, 1); // Sword
            slots[1].item = InventoryItem(2, 1); // Shield
            slots[2].item = InventoryItem(3, 5); // Potion (qty 5)
        }

        int getSlotIndexAtPosition(int mouseX, int mouseY) const
        {
            for (size_t i = 0; i < slots.size(); ++i) {
                if (slots[i].contains(mouseX, mouseY)) {
                    return static_cast<int>(i);
                }
            }
            return -1;
        }

        bool isPlayerSlot(int slotIndex) const
        {
            if (slotIndex < 0 || slotIndex >= static_cast<int>(slots.size()))
                return false;
            return slots[slotIndex].isPlayerInventory;
        }

        bool startDrag(int slotIndex, SDL_Point const & mousePos)
        {
            if (slotIndex < 0 || slotIndex >= static_cast<int>(slots.size()))
                return false;

            InventorySlot& slot = slots[slotIndex];
            if (!slot.item.has_value())
                return false;

            dragState.isDragging     = true;
            dragState.item           = slot.item.value();
            dragState.startSlotIndex = slotIndex;
            dragState.offsetX        = mousePos.x - slot.x;
            dragState.offsetY        = mousePos.y - slot.y;

            return true;
        }

        void endDrag()
        {
            dragState.isDragging     = false;
            dragState.item           = InventoryItem();
            dragState.startSlotIndex = -1;
        }

        bool isDragging() const
        {
            return dragState.isDragging;
        }

        bool tryDrop(int slotIndex)
        {
            if (!dragState.isDragging || slotIndex < 0 || slotIndex >= static_cast<int>(slots.size()))
                return false;

            // Can't drop on player inventory in this demo (stash only)
            if (isPlayerSlot(slotIndex))
                return false;

            InventorySlot& targetSlot = slots[slotIndex];

            // Can't drop on occupied slot
            if (targetSlot.item.has_value())
                return false;

            // Transfer item
            targetSlot.item = dragState.item;

            // Clear source slot (player inventory)
            if (dragState.startSlotIndex >= 0 && dragState.startSlotIndex < static_cast<int>(slots.size())) {
                slots[dragState.startSlotIndex].item = std::nullopt;
            }

            endDrag();
            return true;
        }

        bool isValidDrop(int slotIndex) const
        {
            if (!dragState.isDragging || slotIndex < 0 || slotIndex >= static_cast<int>(slots.size()))
                return false;
            if (isPlayerSlot(slotIndex))
                return false;
            if (slots[slotIndex].item.has_value())
                return false;
            return true;
        }

        std::string getTooltipContent(int slotIndex) const
        {
            if (slotIndex < 0 || slotIndex >= static_cast<int>(slots.size()))
                return "";
            auto const & slot = slots[slotIndex];
            if (!slot.item.has_value())
                return "";

            auto const * itemData = findItemData(slot.item->itemId);
            if (!itemData)
                return "";

            std::string content = itemData->name + "\n";
            content += "Damage: " + std::to_string(itemData->damage) + "\n";
            content += "Armor: " + std::to_string(itemData->armor) + "\n";
            content += "Weight: " + std::to_string(itemData->weight);
            return content;
        }

        std::string getExtendedTooltipContent(int slotIndex) const
        {
            if (slotIndex < 0 || slotIndex >= static_cast<int>(slots.size()))
                return "";
            auto const & slot = slots[slotIndex];
            if (!slot.item.has_value())
                return "";

            auto const * itemData = findItemData(slot.item->itemId);
            if (!itemData)
                return "";

            std::string content = getTooltipContent(slotIndex) + "\n\n";
            content += "Description:\n" + itemData->description;
            return content;
        }

        int getItemIdAtSlot(int slotIndex) const
        {
            if (slotIndex < 0 || slotIndex >= static_cast<int>(slots.size()))
                return 0;
            auto const & slot = slots[slotIndex];
            if (!slot.item.has_value())
                return 0;
            return slot.item->itemId;
        }

        int getPlayerInvStartX() const
        {
            return PLAYER_INV_START_X;
        }
        int getStashInvStartX() const
        {
            return STASH_INV_START_X;
        }
        int getSlotSize() const
        {
            return SLOT_SIZE;
        }

        void render(SDL_Renderer* renderer, int mouseX, int mouseY)
        {
            // Clear screen
            SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
            SDL_RenderClear(renderer);

            // Draw player inventory background
            SDL_Rect playerBg = {
                PLAYER_INV_START_X - 10,
                PLAYER_INV_START_Y - 10,
                PLAYER_COLS * SLOT_SIZE + 20,
                PLAYER_ROWS * SLOT_SIZE + 20};
            SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
            SDL_RenderFillRect(renderer, &playerBg);

            // Draw "Player Inventory" label background
            SDL_Rect playerLabelBg = {PLAYER_INV_START_X, PLAYER_INV_START_Y - 25, PLAYER_COLS * SLOT_SIZE, 20};
            SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
            SDL_RenderFillRect(renderer, &playerLabelBg);

            // Draw stash background
            SDL_Rect stashBg = {
                STASH_INV_START_X - 10,
                STASH_INV_START_Y - 10,
                STASH_COLS * SLOT_SIZE + 20,
                STASH_ROWS * SLOT_SIZE + 20};
            SDL_SetRenderDrawColor(renderer, 30, 30, 50, 255);
            SDL_RenderFillRect(renderer, &stashBg);

            // Draw "Stash Box" label background
            SDL_Rect stashLabelBg = {STASH_INV_START_X, STASH_INV_START_Y - 25, STASH_COLS * SLOT_SIZE, 20};
            SDL_SetRenderDrawColor(renderer, 50, 50, 80, 255);
            SDL_RenderFillRect(renderer, &stashLabelBg);

            // Draw slots and items
            for (size_t i = 0; i < slots.size(); ++i) {
                auto& slot = slots[i];

                // Draw slot border
                SDL_Rect rect = {slot.x, slot.y, slot.width, slot.height};
                SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
                SDL_RenderDrawRect(renderer, &rect);

                // Draw item if present
                if (slot.item.has_value()) {
                    SDL_Rect itemRect = {slot.x + 2, slot.y + 2, slot.width - 4, slot.height - 4};

                    // Get item color from database
                    auto const * itemData = findItemData(slot.item->itemId);
                    if (itemData) {
                        if (itemData->name == "Sword") {
                            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
                        } else if (itemData->name == "Shield") {
                            SDL_SetRenderDrawColor(renderer, 150, 150, 255, 255);
                        } else if (itemData->name == "Potion") {
                            SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
                        } else {
                            SDL_SetRenderDrawColor(renderer, 100, 200, 100, 255);
                        }
                    } else {
                        SDL_SetRenderDrawColor(renderer, 100, 200, 100, 255);
                    }

                    SDL_RenderFillRect(renderer, &itemRect);
                }
            }

            // Draw drag ghost and drop target highlight
            if (dragState.isDragging) {
                // Draw drag ghost
                SDL_Rect ghostRect = {mouseX - dragState.offsetX, mouseY - dragState.offsetY, SLOT_SIZE, SLOT_SIZE};

                // Enable alpha blending for ghost
                SDL_BlendMode oldBlendMode;
                SDL_GetRenderDrawBlendMode(renderer, &oldBlendMode);
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 180);
                SDL_RenderFillRect(renderer, &ghostRect);

                // Restore blend mode
                SDL_SetRenderDrawBlendMode(renderer, oldBlendMode);

                // Draw drop target highlight
                int hoveredSlot = getSlotIndexAtPosition(mouseX, mouseY);
                if (hoveredSlot >= 0) {
                    bool valid        = isValidDrop(hoveredSlot);
                    SDL_Rect slotRect = {
                        slots[hoveredSlot].x,
                        slots[hoveredSlot].y,
                        slots[hoveredSlot].width,
                        slots[hoveredSlot].height};

                    // Draw highlight border (green for valid, red for invalid)
                    if (valid) {
                        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green
                    } else {
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
                    }
                    SDL_RenderDrawRect(renderer, &slotRect);
                }
            }
        }
    };

} // namespace

int main(int argc, char* argv[])
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Set current path to executable directory
    auto const execDir = []() {
#ifdef __linux__
        return std::filesystem::read_symlink("/proc/self/exe").parent_path();
#elif defined(_WIN32)
        char buffer[MAX_PATH] = {0};
        GetModuleFileNameA(nullptr, buffer, MAX_PATH);
        return std::filesystem::path(buffer).parent_path();
#else
        return std::filesystem::current_path();
#endif
    }();
    std::filesystem::current_path(execDir);

    // Append library version to window title
    std::string const fifeguiVersion = fcn::fifechanVersion();
    std::string const title = std::format("FifeGUI v{} using SDL2 Backend: Drag-and-Drop Example", fifeguiVersion);

    // Create window
    SDL_Window* window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        360,
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

    if (window == nullptr) {
        std::cerr << "Failed to create SDL_Window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == nullptr) {
        std::string const rendererError = SDL_GetError();
        renderer                        = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
        if (renderer == nullptr) {
            std::cerr << "Failed to create SDL_Renderer: " << rendererError << " -> " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }
    }

    // Initialize fifechan GUI system
    auto graphics = std::make_unique<fcn::sdl2::Graphics>();
    graphics->setTarget(renderer, 640, 360);

    auto input = std::make_unique<fcn::sdl2::Input>();

    // Set up image loader for fifechan
    auto imageLoader = std::make_unique<fcn::sdl2::ImageLoader>();
    imageLoader->setRenderer(renderer);
    fcn::Image::setImageLoader(imageLoader.get());

    auto gui = std::make_unique<fcn::Gui>();
    gui->setGraphics(std::move(graphics));
    gui->setInput(input.get());

    // Prefer a TrueType font so text color can be tinted by the backend.
    if (TTF_Init() == -1) {
        std::cerr << "[ERROR] Failed to initialize SDL2_ttf: " << TTF_GetError() << '\n';
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 2;
    }

    std::string const fontPath = execDir.string() + "/ArchitectsDaughter.ttf";
    int const fontSize         = std::max(12, std::min(640, 360) / 64);
    try {
        gui->setGlobalFont(fontPath, fontSize);
    } catch (std::exception const & e) {
        std::cerr << "[ERROR] Exception loading font: " << e.what() << '\n';
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 5;
    }

    // Initialize inventory system
    InventorySystem inventory;

    fcn::DragHandler dragHandler(gui.get());

    // Create a Tooltip widget and spec to show item info with ALT-extended view
    auto tooltip = std::make_unique<fcn::Tooltip>();
    fcn::TooltipSpec spec;
    spec.delayMs   = 300;
    spec.placement = fcn::TooltipPlacement::Cursor;
    spec.content   = [&inventory](int widgetId) {
        return inventory.getTooltipContent(widgetId);
    };
    spec.modifierBehavior.enabled         = true;
    spec.modifierBehavior.modifier        = 0x100; // KMOD_ALT
    spec.modifierBehavior.modifiedContent = [&inventory](int widgetId) {
        return inventory.getExtendedTooltipContent(widgetId);
    };
    tooltip->setSpec(spec);
    tooltip->setPosition(0, 0);
    tooltip->setWidth(200);
    tooltip->setHeight(100);
    // Create a Label to render tooltip text and add it as a child of the tooltip.
    auto ttLabel           = std::make_unique<fcn::Label>(std::string());
    fcn::Label* ttLabelPtr = ttLabel.get();
    ttLabelPtr->setAlignment(fcn::Graphics::Alignment::Left);
    ttLabelPtr->setForegroundColor(fcn::Color(255, 255, 255, 255));
    // Add the label inside the tooltip with padding (ownership transferred)
    tooltip->addWidget(std::move(ttLabel), 8, 8);
    // Tooltip will be added to the top container after it's created

    // Create labels using fifechan Label widget
    auto playerLabel = std::make_unique<fcn::Label>("Player Inventory");
    playerLabel->setPosition(inventory.getPlayerInvStartX(), 25);
    playerLabel->setWidth(inventory.getSlotSize() * 3);
    playerLabel->setHeight(20);
    playerLabel->setForegroundColor(fcn::Color(255, 255, 255)); // white text
    playerLabel->setAlignment(fcn::Graphics::Alignment::Center);

    auto stashLabel = std::make_unique<fcn::Label>("Stash Box");
    stashLabel->setPosition(inventory.getStashInvStartX(), 25);
    stashLabel->setWidth(inventory.getSlotSize() * 4);
    stashLabel->setHeight(20);
    stashLabel->setForegroundColor(fcn::Color(255, 255, 255)); // white text
    stashLabel->setAlignment(fcn::Graphics::Alignment::Center);

    // Create top container and add labels
    auto topContainer = std::make_unique<fcn::Container>();
    topContainer->setLayout(fcn::Container::LayoutPolicy::Absolute);
    topContainer->setDimension(fcn::Rectangle(0, 0, 640, 360));
    topContainer->add(playerLabel.get(), playerLabel->getX(), playerLabel->getY());
    topContainer->add(stashLabel.get(), stashLabel->getX(), stashLabel->getY());

    // Add tooltip widget to the top container so it's drawn by gui->draw()
    topContainer->add(tooltip.get(), tooltip->getX(), tooltip->getY());

    // Make container non-opaque so it doesn't draw over the inventory
    topContainer->setOpaque(false);

    // Set top widget for GUI
    gui->setTop(topContainer.get());

    // Main loop
    bool running = true;
    SDL_Event event;
    int const frameDelay = 16; // ~60 FPS
    Uint32 lastTime      = SDL_GetTicks();

    while (running) {
        // Track ALT key state (scope reduced to main loop)
        // ALT state will be sampled each frame via SDL_GetModState()

        Uint32 currentTime = SDL_GetTicks();
        lastTime           = currentTime;

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int slotIdx = inventory.getSlotIndexAtPosition(event.button.x, event.button.y);
                    if (slotIdx >= 0) {
                        SDL_Point pt = {event.button.x, event.button.y};
                        inventory.startDrag(slotIdx, pt);

                        // Create a simple DragPayload carrying the item id (application data)
                        int itemId = inventory.getItemIdAtSlot(slotIdx);
                        if (itemId != 0) {
                            auto data    = std::make_shared<int>(itemId);
                            auto payload = std::make_unique<fcn::DragPayload>(data);
                            // Optional: set a minimal ghost renderer (no-op here)
                            payload->setGhostRenderer([](fcn::Graphics const & g, fcn::DragPoint const & pos) {
                                (void)g;
                                (void)pos;
                            });
                            dragHandler.beginDrag(nullptr, std::move(payload), event.button.x, event.button.y);
                        }
                    }
                }
            } else if (event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_LEFT && inventory.isDragging()) {
                    int slotIdx = inventory.getSlotIndexAtPosition(event.button.x, event.button.y);
                    if (slotIdx >= 0) {
                        inventory.tryDrop(slotIdx);
                        dragHandler.drop(event.button.x, event.button.y);
                    } else {
                        inventory.endDrag();
                        dragHandler.cancel();
                    }
                }
            }
        }

        // Update drag manager with current mouse position
        dragHandler.update(mouseX, mouseY);

        // Tooltip handling: map mouse->slot and update tooltip widget
        static int prevHoverSlot = -1;
        int hoverSlot            = inventory.getSlotIndexAtPosition(mouseX, mouseY);
        if (hoverSlot >= 0) {
            tooltip->setWidgetId(hoverSlot);
            if (hoverSlot != prevHoverSlot) {
                tooltip->startHover();
            }
        } else {
            if (prevHoverSlot >= 0) {
                tooltip->endHover();
            }
        }

        // Update tooltip (pass modifier state: ALT key)
        // Update tooltip (read modifier state fresh each frame via SDL_GetModState())
        int modifierState = (SDL_GetModState() & KMOD_ALT) ? 0x100 : 0;
        tooltip->update(frameDelay, modifierState);

        // Position the tooltip near the mouse cursor and size it to content
        std::string const & content = tooltip->getCurrentContent();
        int lines                   = static_cast<int>(std::count(content.begin(), content.end(), '\n')) + 1;
        int lineHeight              = 18;
        int padding                 = 8;
        int width                   = 220;
        int height                  = padding * 2 + lines * lineHeight;
        // Place tooltip slightly offset from cursor to avoid occluding
        // Use a tighter offset so the tooltip appears closer to hover position
        tooltip->setPosition(mouseX + 8, mouseY + 8);
        tooltip->setWidth(width);
        tooltip->setHeight(height);

        prevHoverSlot = hoverSlot;

        // Render custom inventory UI with SDL2
        inventory.render(renderer, mouseX, mouseY);

        // Tooltip and its label will be drawn by gui->draw() as part of the top container.
        // Update the label caption and resize tooltip to fit content before drawing the GUI.
        std::string const & ttContent = tooltip->getCurrentContent();
        if (!ttContent.empty()) {
            // `ttLabel` is created above via make_unique and owned by the tooltip,
            // so `ttLabelPtr` is guaranteed to be valid here. Update caption
            // and adjust tooltip size to fit the label.
            ttLabelPtr->setCaption(ttContent);
            ttLabelPtr->adjustSize();
            padding = 8;
            width   = ttLabelPtr->getWidth() + padding * 2;
            height  = ttLabelPtr->getHeight() + padding * 2;
            tooltip->setWidth(width);
            tooltip->setHeight(height);
        }

        // Render fifechan GUI (for labels and tooltip)
        gui->draw();

        SDL_RenderPresent(renderer);
    }

    // Cleanup
    fcn::Widget::setGlobalFont(nullptr);
    fcn::Image::setImageLoader(nullptr);

    gui.reset();
    input.reset();
    graphics.reset();
    imageLoader.reset();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
