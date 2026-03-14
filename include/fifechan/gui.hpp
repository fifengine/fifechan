// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_GUI_HPP_
#define INCLUDE_FIFECHAN_GUI_HPP_

#include <algorithm>
#include <iterator>
#include <list>
#include <memory>
#include <queue>
#include <set>
#include <string>

#include "fifechan/keyevent.hpp"
#include "fifechan/mouseevent.hpp"
#include "fifechan/mouseinput.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    class FocusHandler;
    class Font;
    class GuiDeathListener;
    class Graphics;
    class Input;
    class KeyListener;
    class VisibilityEventHandler;
    class Widget;

    // The following comment will appear in the doxygen main page.
    /**
     * @mainpage FifeGUI API Reference
     *
     * @section intro_sec Introduction
     * Welcome to the <b>FifeGUI</b> API documentation. FifeGUI is a flexible,
     * backend-agnostic GUI library designed to be easily integrated into games
     * and applications. It provides a rich set of widgets, event handling, and
     * theming capabilities while remaining decoupled from specific rendering or
     * input libraries.
     *
     * @section arch_sec Architecture & Backends
     * FifeGUI operates on an abstraction layer that allows it to work with various
     * underlying technologies. The core logic is separated from the implementation
     * details via abstract interfaces:
     * - <b>Graphics:</b> Supports multiple backends (e.g., @ref fcn::opengl::Graphics "OpenGL", @ref
     * fcn::sdl2::Graphics "SDL2").
     * - <b>Input:</b> Abstracts keyboard and mouse input (@ref fcn::Input "Input" interface).
     * - <b>Fonts & Images:</b> Pluggable loaders and renderers for assets.
     *
     * This design allows you to swap rendering engines without changing your GUI logic.
     *
     * @section nav_sec Navigating this Documentation
     * This documentation is primarily structured as an API reference. To navigate effectively:
     * - <b>Modules:</b> Use the <a href="modules.html">Modules</a> tab to browse classes by logical category
     *   (e.g., @ref widgets "Widgets", @ref events "Events", @ref graphics "Graphics & Rendering").
     * - <b>Class List:</b> Use the <a href="annotated.html">Class List</a> for a complete alphabetical index.
     * - <b>Hierarchy:</b> Use the <a href="hierarchy.html">Class Hierarchy</a> to understand inheritance relationships.
     *
     * @section start_sec Getting Started
     * While this reference details every class and method, the best way to learn
     * FifeGUI is by examining practical implementations:
     * - Check the <code>examples/</code> directory for minimal, standalone demonstrations.
     * - Explore <code>tests/integration/</code> for comprehensive usage scenarios and edge cases.
     *
     * @section contrib_sec Contributing & Feedback
     * This documentation is a living project. If you encounter:
     * - Errors, typos, or broken links
     * - Inconsistent descriptions or missing parameters
     * - Features that require more detailed explanation
     *
     * Please do not hesitate to contribute! Submit issues or pull requests to our
     * repository to help improve the quality of this documentation for everyone.
     *
     */

    /**
     * The central GUI manager. Integrates backend implementations and manages the root widget tree.
     *
     * GUI is the core class of FifeGUI to which implementations of backends
     * are passed, to make FifeGUI work with a specific library, and to where
     * the top widget (root widget of GUI) is added.
     * If you want to be able to have more then one widget in your
     * GUI, the top widget should be a container.
     *
     * A Gui object cannot work properly without passing backend
     * implementations to it. A Gui object must have an implementation of a
     * Graphics and an implementation of Input.
     *
     * NOTE: A complete GUI also must have the ability to load images.
     *       Images are loaded with the Image class, so to make Fifechan
     *       able to load images an implementation of ImageLoader must be
     *       passed to Image.
     *
     * @see Graphics, Input, Image
     *
     * @ingroup core
     */
    class FIFEGUI_API Gui
    {
    public:
        Gui();

        virtual ~Gui();

        /**
         * Sets the top widget. The top widget is the root widget
         * of the GUI. If you want a GUI to be able to contain more
         * than one widget the top widget should be a container.
         *
         * @param top The top widget.
         * @see Container
         */
        virtual void setTop(Widget* top);

        /**
         * Sets the root widget and transfers ownership to Gui.
         *
         * @param top The root widget to own.
         */
        virtual void setTop(std::unique_ptr<Widget> top);

        /**
         * Alias for setTop.
         *
         * @param top The root widget.
         */
        virtual void setRoot(Widget* top);

        /**
         * Alias for ownership-based setTop.
         *
         * @param top The root widget to own.
         */
        virtual void setRoot(std::unique_ptr<Widget> top);

        /**
         * Gets the top widget. The top widget is the root widget
         * of the GUI.
         *
         * @return The top widget. NULL if no top widget has been set.
         */
        virtual Widget* getTop() const;

        /**
         * Sets the graphics object to use for drawing.
         *
         * @param graphics The graphics object to use for drawing.
         * @see getGraphics, OpenGLGraphics, SDLGraphics
         */
        virtual void setGraphics(Graphics* graphics);

        /**
         * Sets and owns the graphics object used for drawing.
         *
         * @param graphics The graphics object.
         */
        virtual void setGraphics(std::unique_ptr<Graphics> graphics);

        /**
         * Gets the graphics object used for drawing.
         *
         *  @return The graphics object used for drawing. NULL if no
         *          graphics object has been set.
         * @see setGraphics, OpenGLGraphics, SDLGraphics
         */
        virtual Graphics* getGraphics() const;

        /**
         * Sets the input object to use for input handling.
         *
         * @param input The input object to use for input handling.
         * @see getInput, SDLInput
         */
        virtual void setInput(Input* input);

        /**
         * Sets and owns the input object used for input handling.
         *
         * @param input The input object.
         */
        virtual void setInput(std::unique_ptr<Input> input);

        /**
         * Gets the input object being used for input handling.
         *
         *  @return The input object used for handling input. NULL if no
         *          input object has been set.
         * @see setInput, SDLInput
         */
        virtual Input* getInput() const;

        /**
         * Initializes GUI backends in one call.
         *
         * @param graphics Owned graphics backend.
         * @param input Owned input backend.
         * @param width Unused by the core implementation.
         * @param height Unused by the core implementation.
         */
        virtual void initialize(
            std::unique_ptr<Graphics> graphics, std::unique_ptr<Input> input, int width, int height);

        /**
         * Returns the focus handler used by this GUI.
         *
         * @return Focus handler.
         */
        virtual FocusHandler* getFocusHandler() const;

        /**
         * Loads a font using the active graphics backend and sets it as global widget font.
         *
         * @param filename Path to font file.
         * @param size Font size.
         */
        virtual void setGlobalFont(std::string const & filename, int size);

        /**
         * Performs logic of the GUI. By calling this function all logic
         * functions down in the GUI heirarchy will be called. When logic
         * is called for Gui, user input will be handled.
         *
         * @see Widget::logic
         */
        virtual void logic();

        /**
         * Draws the GUI. By calling this function all draw functions
         * down in the GUI hierarchy will be called. When draw is called
         * the used Graphics object will be initialised and drawing of
         * the top widget will commence.
         *
         * @see Widget::draw
         */
        virtual void draw();

        /**
         * Focuses none of the widgets in the Gui.
         *
         */
        virtual void focusNone();

        /**
         * Sets tabbing enabled, or not. Tabbing is the usage of
         * changing focus by utilizing the tab key.
         *
         * @param tabbing True if tabbing should be enabled, false
         *                otherwise.
         * @see isTabbingEnabled
         */
        virtual void setTabbingEnabled(bool tabbing);

        /**
         * Checks if tabbing is enabled.
         *
         * @return True if tabbing is enabled, false otherwise.
         * @see setTabbingEnabled
         */
        virtual bool isTabbingEnabled();

        /**
         * Adds a global key listener to the Gui. A global key listener
         * will receive all key events generated from the GUI and global
         * key listeners will receive the events before key listeners
         * of widgets.
         *
         * @param keyListener The key listener to add.
         * @see removeGlobalKeyListener
         */
        virtual void addGlobalKeyListener(KeyListener* keyListener);

        /**
         * Removes global key listener from the Gui.
         *
         * @param keyListener The key listener to remove.
         * @throws Exception if the key listener hasn't been added.
         * @see addGlobalKeyListener
         */
        virtual void removeGlobalKeyListener(KeyListener* keyListener);

        /**
         * Inform gui that a widget was hidden.
         *
         * @param widget Hidden widget.
         */
        void enqueueHiddenWidget(Widget* widget);

        /**
         * Inform gui that a widget was shown.
         *
         * @param widget Shown widget.
         */
        void enqueueShownWidget(Widget* widget);

        /**
         * Inform gui that a widget was deleted.
         *
         * @param widget Deleted widget.
         */
        void widgetDied(Widget* widget);

    protected:
        /**
         * Handles all mouse input.
         *
         */
        virtual void handleMouseInput();

        /**
         * Handles key input.
         *
         */
        virtual void handleKeyInput();

        /**
         * Handles mouse moved input.
         *
         * @param mouseInput The mouse input to handle.
         */
        virtual void handleMouseMoved(MouseInput const & mouseInput);

        /**
         * Handles mouse pressed input.
         *
         * @param mouseInput The mouse input to handle.
         */
        virtual void handleMousePressed(MouseInput const & mouseInput);

        /**
         *
         * Handles mouse wheel moved down input.
         *
         * @param mouseInput The mouse input to handle.
         */
        virtual void handleMouseWheelMovedDown(MouseInput const & mouseInput);

        /**
         * Handles mouse wheel moved up input.
         *
         * @param mouseInput The mouse input to handle.
         */
        virtual void handleMouseWheelMovedUp(MouseInput const & mouseInput);

        /**
         * Handles mouse wheel moved right input.
         *
         * @param mouseInput The mouse input to handle.
         */
        virtual void handleMouseWheelMovedRight(MouseInput const & mouseInput);

        /**
         * Handles mouse wheel moved left input.
         *
         * @param mouseInput The mouse input to handle.
         */
        virtual void handleMouseWheelMovedLeft(MouseInput const & mouseInput);

        /**
         * Handles mouse released input.
         *
         * @param mouseInput The mouse input to handle.
         */
        virtual void handleMouseReleased(MouseInput const & mouseInput);

        /**
         * Handles modal focus. Modal focus needs to be checked at
         * each logic iteration as it might be necessary to distribute
         * mouse entered or mouse exited events.
         *
         */
        virtual void handleModalFocus();

        /**
         * Handles modal mouse input focus. Modal mouse input focus needs
         * to be checked at each logic iteration as it might be necessary to
         * distribute mouse entered or mouse exited events.
         *
         */
        virtual void handleModalMouseInputFocus();

        /**
         * Handles modal focus gained. If modal focus has been gained it might
         * be necessary to distribute mouse entered or mouse exited events.
         *
         */
        virtual void handleModalFocusGained();

        /**
         * Handles modal focus released. If modal focus has been
         * released it might be necessary to distribute mouse entered or mouse
         * exited events.
         *
         */
        virtual void handleModalFocusReleased();

        /**
         * Handles modal mouse input focus gained. If modal focus has been gained it might
         * be necessary to distribute mouse entered or mouse exited events.
         *
         */
        virtual void handleModalMouseInputFocusGained();

        /**
         * Handles modal mouse input focus released. If modal focus has been
         * released it might be necessary to distribute mouse entered or mouse
         * exited events.
         *
         */
        virtual void handleModalMouseInputFocusReleased();

        /**
         * Handles hidden widgets.
         */
        virtual void handleHiddenWidgets();

        /**
         * Handles shown widgets.
         */
        virtual void handleShownWidgets();

        /**
         * Convenience overload: distribute a mouse event forwarding
         * to the full overload with `force` and `toSourceOnly` set to false.
         */
        void distributeMouseEvent(Widget* source, MouseEvent::Type type, MouseEvent::Button button, int x, int y)
        {
            distributeMouseEvent(source, type, button, x, y, false, false);
        }

        /** Convenience overload: forward and allow specifying `force` while
         *  `toSourceOnly` remains false.
         */
        void distributeMouseEvent(
            Widget* source, MouseEvent::Type type, MouseEvent::Button button, int x, int y, bool force)
        {
            distributeMouseEvent(source, type, button, x, y, force, false);
        }

        /**
         * Distributes a mouse event to the GUI handling code.
         *
         * @param source The source widget of the event.
         * @param type The type of the event to distribute.
         * @param button The button of the event (if any) to distribute.
         * @param x The x coordinate of the event.
         * @param y The y coordinate of the event.
         * @param force If true, distributes the event even if the receiving widget
         *              is not enabled, not visible, or another widget has modal focus.
         * @param toSourceOnly If true, only the source widget receives the event,
         *                     otherwise parent listeners may also receive it.
         */
        virtual void distributeMouseEvent(
            Widget* source,
            MouseEvent::Type type,
            MouseEvent::Button button,
            int x,
            int y,
            bool force,
            bool toSourceOnly);

        /**
         * Distributes a key event.
         *
         * @param keyEvent The key event to distribute.

         */
        virtual void distributeKeyEvent(KeyEvent& keyEvent);

        /**
         * Distributes a key event to the global key listeners.
         *
         * @param keyEvent The key event to distribute.
         *
         */
        virtual void distributeKeyEventToGlobalKeyListeners(KeyEvent& keyEvent);

        /**
         * Gets the widget at a certain position.
         *
         * @return The widget at a certain position.
         */
        Widget* getWidgetAt(int x, int y)
        {
            return getWidgetAt(x, y, nullptr);
        }

        /**
         * Gets the widget at a certain position, optionally excluding one widget.
         * @param x The x coordinate.
         * @param y The y coordinate.
         * @param exclude Widget to ignore when searching (may be nullptr).
         * @return The widget at the specified position or nullptr if none.
         */
        virtual Widget* getWidgetAt(int x, int y, Widget* exclude);

        /**
         * Gets the source of the mouse event.
         *
         * @return The source widget of the mouse event.
         */
        virtual Widget* getMouseEventSource(int x, int y);

        /**
         * Gets the source of the key event.
         *
         * @return The source widget of the key event.
         */
        virtual Widget* getKeyEventSource();

        /**
         * Gets all widgets a certain coordinate in the Gui.
         *
         * @param x The x coordinate.
         * @param y The y coordinate.
         * @return A set of all widgets at the specified coordinate.
         */
        virtual std::set<Widget*> getWidgetsAt(int x, int y);

        /**
         * Holds the top widget.
         */
        Widget* mTop;

        /**
         * Holds hidden widgets.
         */
        std::queue<Widget*> mHiddenWidgets;

        /**
         * Holds shown widgets.
         */
        std::queue<Widget*> mShownWidgets;

        /**
         * Holds the graphics implementation used.
         */
        Graphics* mGraphics;

        /**
         * Holds the input implementation used.
         */
        Input* mInput;

        /**
         * Holds the focus handler for the Gui.
         */
        FocusHandler* mFocusHandler;

        /**
         * Holds the visibility event handler for the Gui.
         */
        VisibilityEventHandler* mVisibilityEventHandler;

        /**
         * True if tabbing is enabled, false otherwise.
         */
        bool mTabbing;

        /**
         * Typedef.
         */
        using KeyListenerList = std::list<KeyListener*>;

        /**
         * Typedef.
         */
        using KeyListenerListIterator = KeyListenerList::iterator;

        /**
         * Holds the global key listeners of the Gui.
         */
        KeyListenerList mKeyListeners;

        /**
         * True if shift is pressed, false otherwise.
         */
        bool mShiftPressed;

        /**
         * True if meta is pressed, false otherwise.
         */
        bool mMetaPressed;

        /**
         * True if control is pressed, false otherwise.
         */
        bool mControlPressed;

        /**
         * True if alt is pressed, false otherwise.
         */
        bool mAltPressed;

        /**
         * Holds the last mouse button pressed.
         */
        unsigned int mLastMousePressButton;

        /**
         * Holds the last mouse press time stamp.
         */
        int mLastMousePressTimeStamp;

        /**
         * Holds the last mouse x coordinate.
         */
        int mLastMouseX;

        /**
         * Holds the last mouse y coordinate.
         */
        int mLastMouseY;

        /**
         * Holds the current click count. Used to keep track
         * of clicks for a the last pressed button.
         */
        int mClickCount;

        /**
         * Holds the last button used when a drag of a widget
         * was initiated. Used to be able to release a drag
         * when the same button is released.
         */
        int mLastMouseDragButton;

        /** Listener notified when the GUI or top widget is destroyed. */
        GuiDeathListener* mDeathListener;

        /** Optional owned top widget (when Gui takes ownership). */
        std::unique_ptr<Widget> mOwnedTop;

        /** Optional owned graphics backend instance. */
        std::unique_ptr<Graphics> mOwnedGraphics;

        /** Optional owned input backend instance. */
        std::unique_ptr<Input> mOwnedInput;

        /** Shared global font used by widgets when not overridden. */
        std::shared_ptr<Font> mGlobalFont;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_GUI_HPP_
