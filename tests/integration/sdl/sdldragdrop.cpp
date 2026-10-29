// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

/**
 * SDL Drag-and-Drop Inventory Demo with Tooltip Widget
 * -------------------------------------------------
 *
 * This is an integration test for a drag-and-drop system.
 * The player has a 3x3 inventory grid and can drag items to a 4x3 stash box.
 *
 * The slot grid is drawn with raw SDL, but the drag operation itself goes
 * through fifechan: DragHandler performs hit testing and drives DragEvents,
 * while a DropTargetListener registered on non-drawing per-slot widgets accepts
 * or rejects the drop. The ghost and target highlight are rendered by
 * DragHandler::render inside the GUI draw pass.
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
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <fifechan/widgets/textbox.hpp>
#include <fifechan/widgets/tooltip.hpp>

#include <fifechan/backends/sdl3/sdl.hpp>
#include <fifechan/dragdrop.hpp>
#include <fifechan/events/dragevent.hpp>
#include <fifechan/fontloader.hpp>
#include <fifechan/listeners/droptargetlistener.hpp>

#include <fifechan.hpp>

#include <cmath>

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

            ItemData(int i, std::string n, int dmg, int arm, int wgt, std::string desc) :
                id(i), name(std::move(n)), damage(dmg), armor(arm), weight(wgt), description(std::move(desc))
            {
            }
    };

    // Hardcoded item database (internal to demo)
    std::vector<ItemData> const & getItemDatabase()
    {
        static std::vector<ItemData> const itemDatabase = {
            {1, "Sword", 15, 0, 3, "A sharp blade for close combat."},
            {2, "Shield", 0, 10, 5, "A sturdy shield for protection."},
            {3, "Potion", 0, 0, 1, "Restores health when consumed."},
            {4, "Bow", 12, 0, 2, "A ranged weapon for hunters."},
            {5, "Helmet", 0, 5, 2, "Protects the head from damage."},
        };
        return itemDatabase;
    }

    // Find item in database by ID
    ItemData const * findItemData(int id)
    {
        auto const & itemDatabase = getItemDatabase();
        auto it                   = std::ranges::find_if(itemDatabase, [id](ItemData const & item) {
            return item.id == id;
        });

        if (it != itemDatabase.end()) {
            return &*it;
        }

        return nullptr;
    }

    // Inventory item structure (references database by ID)
    struct InventoryItem
    {
            int itemId; // References itemDatabase
            int quantity;

            explicit InventoryItem(int id = 0, int qty = 1) : itemId(id), quantity(qty)
            {
            }

            bool isValid() const
            {
                return itemId > 0 && quantity > 0;
            }
    };

    // Inventory slot structure
    struct InventorySlot
    {
            int x; // Position in pixels
            int y;
            int width; // Pixel dimensions
            int height;
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

    // Data carried by the DragPayload while an item is dragged.
    struct DragData
    {
            int itemId;
            int sourceSlot;
            int offsetX; // Grab offset inside the source slot
            int offsetY;
    };

    // Colour used to draw an item, shared by the slot grid and the drag ghost.
    fcn::Color getItemColor(int itemId)
    {
        auto const * itemData = findItemData(itemId);
        if (itemData == nullptr) {
            return fcn::Color(100, 200, 100);
        }
        if (itemData->name == "Sword") {
            return fcn::Color(200, 200, 200);
        }
        if (itemData->name == "Shield") {
            return fcn::Color(150, 150, 255);
        }
        if (itemData->name == "Potion") {
            return fcn::Color(255, 100, 100);
        }
        return fcn::Color(100, 200, 100);
    }

    class InventorySystem
    {
        private:
            static constexpr int PLAYER_INV_START_X = 50;
            static constexpr int PLAYER_INV_START_Y = 50;
            static constexpr int WINDOW_WIDTH       = 640;
            static constexpr int SLOT_SIZE          = 60;
            static constexpr int PLAYER_COLS        = 3;
            static constexpr int PLAYER_ROWS        = 3;
            static constexpr int STASH_COLS         = 4;
            static constexpr int STASH_ROWS         = 3;
            static constexpr int STASH_INV_START_X  = WINDOW_WIDTH - PLAYER_INV_START_X - (STASH_COLS * SLOT_SIZE);
            static constexpr int STASH_INV_START_Y  = 50;

            std::vector<InventorySlot> slots;

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
                            PLAYER_INV_START_X + (col * SLOT_SIZE),
                            PLAYER_INV_START_Y + (row * SLOT_SIZE),
                            SLOT_SIZE,
                            SLOT_SIZE,
                            true);
                    }
                }

                // Stash inventory slots (4x3 grid for more space)
                for (int row = 0; row < STASH_ROWS; ++row) {
                    for (int col = 0; col < STASH_COLS; ++col) {
                        slots.emplace_back(
                            STASH_INV_START_X + (col * SLOT_SIZE),
                            STASH_INV_START_Y + (row * SLOT_SIZE),
                            SLOT_SIZE,
                            SLOT_SIZE,
                            false);
                    }
                }

                // Add some test items to player inventory (slots 0, 1, 2)
                slots.at(0).item = InventoryItem(1, 1); // Sword
                slots.at(1).item = InventoryItem(2, 1); // Shield
                slots.at(2).item = InventoryItem(3, 5); // Potion (qty 5)
            }

            int getSlotIndexAtPosition(int mouseX, int mouseY) const
            {
                for (size_t i = 0; i < slots.size(); ++i) {
                    if (slots.at(i).contains(mouseX, mouseY)) {
                        return static_cast<int>(i);
                    }
                }
                return -1;
            }

            bool isPlayerSlot(int slotIndex) const
            {
                if (slotIndex < 0 || std::cmp_greater_equal(slotIndex, slots.size())) {
                    return false;
                }
                return slots.at(slotIndex).isPlayerInventory;
            }

            bool tryDrop(int sourceSlotIndex, int targetSlotIndex)
            {
                if (sourceSlotIndex < 0 || std::cmp_greater_equal(sourceSlotIndex, slots.size()) ||
                    targetSlotIndex < 0 || std::cmp_greater_equal(targetSlotIndex, slots.size()) ||
                    sourceSlotIndex == targetSlotIndex) {
                    return false;
                }

                // Can't drop on player inventory in this demo (stash only)
                if (isPlayerSlot(targetSlotIndex)) {
                    return false;
                }

                InventorySlot& sourceSlot = slots.at(sourceSlotIndex);
                InventorySlot& targetSlot = slots.at(targetSlotIndex);

                // The source must still hold the dragged item and the target
                // must be empty.
                if (!sourceSlot.item.has_value() || targetSlot.item.has_value()) {
                    return false;
                }

                targetSlot.item = sourceSlot.item;
                sourceSlot.item.reset();
                return true;
            }

            bool isValidDrop(int slotIndex) const
            {
                if (slotIndex < 0 || std::cmp_greater_equal(slotIndex, slots.size())) {
                    return false;
                }
                if (isPlayerSlot(slotIndex)) {
                    return false;
                }
                return !slots.at(slotIndex).item.has_value();
            }

            fcn::Rectangle getSlotRectangle(int slotIndex) const
            {
                if (slotIndex < 0 || std::cmp_greater_equal(slotIndex, slots.size())) {
                    return {};
                }
                auto const & slot = slots.at(slotIndex);
                return fcn::Rectangle(slot.x, slot.y, slot.width, slot.height);
            }

            size_t getSlotCount() const
            {
                return slots.size();
            }

            std::string getTooltipContent(int slotIndex) const
            {
                if (slotIndex < 0 || std::cmp_greater_equal(slotIndex, slots.size())) {
                    return "";
                }
                auto const & slot = slots.at(slotIndex);
                if (!slot.item.has_value()) {
                    return "";
                }

                auto const * itemData = findItemData(slot.item->itemId);
                if (itemData == nullptr) {
                    return "";
                }

                std::string content = itemData->name + "\n";
                content += "Damage: " + std::to_string(itemData->damage) + "\n";
                content += "Armor: " + std::to_string(itemData->armor) + "\n";
                content += "Weight: " + std::to_string(itemData->weight);
                return content;
            }

            std::string getExtendedTooltipContent(int slotIndex) const
            {
                if (slotIndex < 0 || std::cmp_greater_equal(slotIndex, slots.size())) {
                    return "";
                }
                auto const & slot = slots.at(slotIndex);
                if (!slot.item.has_value()) {
                    return "";
                }

                auto const * itemData = findItemData(slot.item->itemId);
                if (itemData == nullptr) {
                    return "";
                }

                std::string content = getTooltipContent(slotIndex) + "\n\n";
                content += "Description:\n" + itemData->description;
                return content;
            }

            int getItemIdAtSlot(int slotIndex) const
            {
                if (slotIndex < 0 || std::cmp_greater_equal(slotIndex, slots.size())) {
                    return 0;
                }
                auto const & slot = slots.at(slotIndex);
                if (!slot.item.has_value()) {
                    return 0;
                }
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

            void render(SDL_Renderer* renderer) const
            {
                // Clear screen
                SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
                SDL_RenderClear(renderer);

                // Draw player inventory background
                SDL_FRect const playerBg = {
                    .x = static_cast<float>(PLAYER_INV_START_X - 10),
                    .y = static_cast<float>(PLAYER_INV_START_Y - 10),
                    .w = static_cast<float>((PLAYER_COLS * SLOT_SIZE) + 20),
                    .h = static_cast<float>((PLAYER_ROWS * SLOT_SIZE) + 20)};
                SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
                SDL_RenderFillRect(renderer, &playerBg);

                // Draw "Player Inventory" label background
                SDL_FRect const playerLabelBg = {
                    .x = static_cast<float>(PLAYER_INV_START_X),
                    .y = static_cast<float>(PLAYER_INV_START_Y - 25),
                    .w = static_cast<float>(PLAYER_COLS * SLOT_SIZE),
                    .h = 20.0F};
                SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
                SDL_RenderFillRect(renderer, &playerLabelBg);

                // Draw stash background
                SDL_FRect const stashBg = {
                    .x = static_cast<float>(STASH_INV_START_X - 10),
                    .y = static_cast<float>(STASH_INV_START_Y - 10),
                    .w = static_cast<float>((STASH_COLS * SLOT_SIZE) + 20),
                    .h = static_cast<float>((STASH_ROWS * SLOT_SIZE) + 20)};
                SDL_SetRenderDrawColor(renderer, 30, 30, 50, 255);
                SDL_RenderFillRect(renderer, &stashBg);

                // Draw "Stash Box" label background
                SDL_FRect const stashLabelBg = {
                    .x = static_cast<float>(STASH_INV_START_X),
                    .y = static_cast<float>(STASH_INV_START_Y - 25),
                    .w = static_cast<float>(STASH_COLS * SLOT_SIZE),
                    .h = 20.0F};
                SDL_SetRenderDrawColor(renderer, 50, 50, 80, 255);
                SDL_RenderFillRect(renderer, &stashLabelBg);

                // Draw slots and items
                for (auto& slot : slots) {
                    // Draw slot border
                    SDL_FRect const rect = {
                        .x = static_cast<float>(slot.x),
                        .y = static_cast<float>(slot.y),
                        .w = static_cast<float>(slot.width),
                        .h = static_cast<float>(slot.height)};
                    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
                    SDL_RenderRect(renderer, &rect);

                    // Draw item if present
                    if (slot.item.has_value()) {
                        SDL_FRect const itemRect = {
                            .x = static_cast<float>(slot.x + 2),
                            .y = static_cast<float>(slot.y + 2),
                            .w = static_cast<float>(slot.width - 4),
                            .h = static_cast<float>(slot.height - 4)};

                        fcn::Color const color = getItemColor(slot.item->itemId);
                        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
                        SDL_RenderFillRect(renderer, &itemRect);
                    }
                }
            }
    };

    // Widget placed over each slot so DragHandler can hit test it. It draws
    // nothing, but must stay visible so findWidgetAt() still considers it.
    class SlotWidget : public fcn::Widget
    {
        public:
            explicit SlotWidget(int slotIndex) : m_slotIndex(slotIndex)
            {
            }

            void draw(fcn::Graphics* /*graphics*/) override
            {
            }

            int getSlotIndex() const
            {
                return m_slotIndex;
            }

        private:
            int m_slotIndex;
    };

    // Listener shared by all slot widgets. The event source is the slot widget,
    // so the target slot is derived from the event instead of listener state.
    class InventoryDropTarget : public fcn::DropTargetListener
    {
        public:
            explicit InventoryDropTarget(InventorySystem& inventory) : m_inventory(inventory)
            {
            }

            bool dragEntered(fcn::DragEvent& /*dragEvent*/) override
            {
                // Accept every slot so invalid targets can be highlighted red
                // and then reject the drop itself.
                return true;
            }

            void dragExited(fcn::DragEvent& /*dragEvent*/) override
            {
            }

            void dragHovered(fcn::DragEvent& /*dragEvent*/) override
            {
            }

            bool dragDropped(fcn::DragEvent& dragEvent) override
            {
                SlotWidget const * const slotWidget = dynamic_cast<SlotWidget const *>(dragEvent.getSource());
                fcn::DragPayload const * payload    = dragEvent.getPayload();
                if (slotWidget == nullptr || payload == nullptr) {
                    return false;
                }

                auto data = payload->getShared<DragData>();
                if (!data) {
                    return false;
                }

                return m_inventory.tryDrop(data->sourceSlot, slotWidget->getSlotIndex());
            }

        private:
            InventorySystem& m_inventory;
    };

    // Full-size, otherwise invisible widget that lets the GUI draw pass render
    // the drag ghost and target highlight through DragHandler::render.
    class DragOverlay : public fcn::Widget
    {
        public:
            explicit DragOverlay(fcn::DragHandler& handler) : m_handler(handler)
            {
            }

            void draw(fcn::Graphics* graphics) override
            {
                m_handler.render(*graphics);
            }

        private:
            fcn::DragHandler& m_handler;
    };

} // namespace

int main(int /*argc*/, char* /*argv*/[])
try {
    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << '\n';
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

    // Append library versions to window title
    std::string const fifeguiVersion = fcn::fifechanVersion();

    int const sdlVersion            = SDL_GetVersion();
    std::string const sdlVersionStr = std::format(
        "{}.{}.{}",
        SDL_VERSIONNUM_MAJOR(sdlVersion),
        SDL_VERSIONNUM_MINOR(sdlVersion),
        SDL_VERSIONNUM_MICRO(sdlVersion));

    std::string const title =
        std::format("FifeGUI v{} using SDL {}: Drag-and-Drop Example", fifeguiVersion, sdlVersionStr);

    // Create window
    SDL_Window* window = SDL_CreateWindow(title.c_str(), 640, 360, SDL_WINDOW_HIGH_PIXEL_DENSITY);

    if (window == nullptr) {
        std::cerr << "Failed to create SDL_Window: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    if (renderer == nullptr) {
        std::cerr << "Failed to create SDL_Renderer: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialize fifechan GUI system
    auto graphics = std::make_unique<fcn::sdl3::Graphics>();
    graphics->setTarget(renderer, 640, 360);

    auto input = std::make_unique<fcn::sdl3::Input>();

    // Set up image loader for fifechan
    auto imageLoader = std::make_unique<fcn::sdl3::ImageLoader>();
    imageLoader->setRenderer(renderer);
    fcn::Image::setImageLoader(imageLoader.get());

    auto gui = std::make_unique<fcn::Gui>();
    gui->setGraphics(std::move(graphics));
    gui->setInput(input.get());

    // Prefer a TrueType font so text color can be tinted by the backend.
    if (!TTF_Init()) {
        std::cerr << "[ERROR] Failed to initialize SDL3_ttf: " << SDL_GetError() << '\n';
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 2;
    }

    // Use the new font loading API to find ArchitectsDaughter font
    std::filesystem::path const fontPath = fcn::font::FontLoader::findFontFile("ArchitectsDaughter.ttf");
    if (fontPath.empty()) {
        std::cerr << "[ERROR] Could not find ArchitectsDaughter.ttf in search paths\n";
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 5;
    }

    int const fontSize = std::max(12, std::min(640, 360) / 64);
    try {
        gui->setGlobalFont(fontPath.string(), fontSize);
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

    // Every slot gets an invisible widget so DragHandler can hit test it. The
    // grid itself is still drawn with raw SDL below the GUI.
    InventoryDropTarget dropTarget(inventory);
    std::vector<std::unique_ptr<SlotWidget>> slotWidgets;
    slotWidgets.reserve(inventory.getSlotCount());
    for (size_t i = 0; i < inventory.getSlotCount(); ++i) {
        auto slotWidget           = std::make_unique<SlotWidget>(static_cast<int>(i));
        fcn::Rectangle const rect = inventory.getSlotRectangle(static_cast<int>(i));
        slotWidget->setPosition(rect.x, rect.y);
        slotWidget->setSize(rect.width, rect.height);
        slotWidget->addDropTargetListener(&dropTarget);
        slotWidgets.push_back(std::move(slotWidget));
    }

    // Ghost and highlight are drawn by the handler inside the GUI draw pass.
    fcn::DragRenderConfig renderConfig;
    renderConfig.ghost = [&inventory](
                             fcn::Graphics& graphics, fcn::DragPayload const & payload, fcn::DragPoint const & pos) {
        auto data = payload.getShared<DragData>();
        if (!data) {
            return;
        }
        fcn::Color const color = getItemColor(data->itemId);
        graphics.setColor(fcn::Color(color.r, color.g, color.b, static_cast<Uint8>(180)));
        int const size = inventory.getSlotSize();
        graphics.fillRectangle(fcn::Rectangle(pos.x - data->offsetX, pos.y - data->offsetY, size, size));
    };
    // Signature is fixed by HighlightRenderer; a const pointer would not match.
    // cppcheck-suppress constParameterPointer
    renderConfig.highlight = [&inventory](fcn::Graphics& graphics, fcn::Widget* target, bool /*isValid*/) {
        SlotWidget const * const slotWidget = dynamic_cast<SlotWidget const *>(target);
        if (slotWidget == nullptr) {
            return;
        }
        bool const valid = inventory.isValidDrop(slotWidget->getSlotIndex());
        graphics.setColor(valid ? fcn::Color(0, 255, 0, 255) : fcn::Color(255, 0, 0, 255));
        int x = 0;
        int y = 0;
        target->getAbsolutePosition(x, y);
        graphics.drawRectangle(fcn::Rectangle(x, y, target->getWidth(), target->getHeight()));
    };
    dragHandler.setRenderConfig(std::move(renderConfig));

    // Drawn first so the ghost stays below the labels and tooltip, and hit
    // tested last so it never shadows a slot.
    auto dragOverlay = std::make_unique<DragOverlay>(dragHandler);
    dragOverlay->setPosition(0, 0);
    dragOverlay->setSize(640, 360);

    // Create a Tooltip widget and spec to show item info with ALT-extended view
    auto tooltip = std::make_unique<fcn::Tooltip>();
    fcn::TooltipSpec spec;
    spec.delayMs   = 300;
    spec.placement = fcn::TooltipPlacement::Cursor;
    spec.content   = [&inventory](int widgetId) {
        return inventory.getTooltipContent(widgetId);
    };
    spec.modifierBehavior.enabled         = true;
    spec.modifierBehavior.modifier        = SDL_KMOD_ALT;
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

    // Create a TextBox with usage rules underneath the drag and drop areas
    auto rulesTextBox = std::make_unique<fcn::TextBox>(
        "Use the mouse to drag and drop your inventory items into your stash box.\n"
        "You are not allowed to put them back.\n"
        "When you press ALT you can see additional stats in the tooltip overlay.");
    rulesTextBox->setPosition(60, 260);
    rulesTextBox->setWidth(540);
    rulesTextBox->setHeight(80);
    rulesTextBox->setOpaque(false);
    rulesTextBox->setBackgroundColor(fcn::Color(50, 50, 50));
    rulesTextBox->setForegroundColor(fcn::Color(255, 255, 255));
    rulesTextBox->setEditable(false);

    // Create top container and add labels
    auto topContainer = std::make_unique<fcn::Container>();
    topContainer->setLayout(fcn::Container::LayoutPolicy::Absolute);
    topContainer->setDimension(fcn::Rectangle(0, 0, 640, 360));

    // The overlay is added first so the ghost is drawn below the labels and
    // tooltip. Slot widgets are added last so hit testing finds them first.
    topContainer->add(dragOverlay.get(), dragOverlay->getX(), dragOverlay->getY());
    topContainer->add(playerLabel.get(), playerLabel->getX(), playerLabel->getY());
    topContainer->add(stashLabel.get(), stashLabel->getX(), stashLabel->getY());
    topContainer->add(rulesTextBox.get(), rulesTextBox->getX(), rulesTextBox->getY());

    // Add tooltip widget to the top container so it's drawn by gui->draw()
    topContainer->add(tooltip.get(), tooltip->getX(), tooltip->getY());

    // Slot widgets draw nothing; they exist only so DragHandler can find the
    // slot under the pointer and deliver drop events to its listener.
    for (auto& slotWidget : slotWidgets) {
        topContainer->add(slotWidget.get(), slotWidget->getX(), slotWidget->getY());
    }

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

        Uint32 const currentTime = SDL_GetTicks();
        lastTime                 = currentTime;

        float mouseXf = NAN;
        float mouseYf = NAN;
        SDL_GetMouseState(&mouseXf, &mouseYf);
        int const mouseX = static_cast<int>(mouseXf);
        int const mouseY = static_cast<int>(mouseYf);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    // The grid is drawn with raw SDL, so the pressed slot comes
                    // from the slot rectangles; the library takes over from
                    // beginDrag() and resolves the drop target itself.
                    int const slotIdx = inventory.getSlotIndexAtPosition(
                        static_cast<int>(event.button.x), static_cast<int>(event.button.y));
                    if (slotIdx >= 0) {
                        int const itemId = inventory.getItemIdAtSlot(slotIdx);
                        if (itemId != 0) {
                            auto const slotRect = inventory.getSlotRectangle(slotIdx);
                            auto data           = std::make_shared<DragData>(DragData{
                                itemId,
                                slotIdx,
                                static_cast<int>(event.button.x) - slotRect.x,
                                static_cast<int>(event.button.y) - slotRect.y});
                            auto payload        = std::make_unique<fcn::DragPayload>(data);
                            dragHandler.beginDrag(
                                slotWidgets.at(slotIdx).get(),
                                std::move(payload),
                                static_cast<int>(event.button.x),
                                static_cast<int>(event.button.y));
                        }
                    }
                }
            } else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                if (event.button.button == SDL_BUTTON_LEFT && dragHandler.getState() == fcn::DragState::Dragging) {
                    // The handler re-runs hit testing at the drop position and
                    // delivers Drop to the slot listener, which moves the item.
                    dragHandler.drop(static_cast<int>(event.button.x), static_cast<int>(event.button.y));
                }
            }
        }

        // Update drag manager with current mouse position
        dragHandler.update(mouseX, mouseY);

        // Tooltip handling: map mouse->slot and update tooltip widget
        static int prevHoverSlot = -1;
        int const hoverSlot      = inventory.getSlotIndexAtPosition(mouseX, mouseY);
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
        int const modifierState = ((SDL_GetModState() & SDL_KMOD_ALT) != 0) ? 0x100 : 0;
        tooltip->update(frameDelay, modifierState);

        // Position the tooltip near the mouse cursor and size it to content
        std::string const & content = tooltip->getCurrentContent();
        int const lines             = static_cast<int>(std::count(content.begin(), content.end(), '\n')) + 1;
        int const lineHeight        = 18;
        int padding                 = 8;
        int width                   = 220;
        int height                  = (padding * 2) + (lines * lineHeight);
        // Place tooltip slightly offset from cursor to avoid occluding
        // Use a tighter offset so the tooltip appears closer to hover position
        tooltip->setPosition(mouseX + 8, mouseY + 8);
        tooltip->setWidth(width);
        tooltip->setHeight(height);

        prevHoverSlot = hoverSlot;

        // Render custom inventory UI
        inventory.render(renderer);

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
            width   = ttLabelPtr->getWidth() + (padding * 2);
            height  = ttLabelPtr->getHeight() + (padding * 2);
            tooltip->setWidth(width);
            tooltip->setHeight(height);
        }

        // Render fifechan GUI (for labels and tooltip)
        gui->draw();

        SDL_RenderPresent(renderer);
    }

    // Cleanup
    fcn::Widget::resetGlobalFont();
    fcn::Image::resetImageLoader();

    gui.reset();
    input.reset();
    graphics.reset();
    imageLoader.reset();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
} catch (...) {
    return 1;
}
