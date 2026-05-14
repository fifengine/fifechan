# Listener Interfaces

Base path: include/fifechan/listeners

Listeners are used via multiple inheritance to add event-handling capabilities:

| Interface	             |  File	                            |         Key Method                |
|------------------------| -------------------------------------|-----------------------------------|
| ActionListener	     | listeners/actionlistener.hpp	        | action(ActionEvent const &)       |
| MouseListener	         | listeners/mouselistener.hpp	        | mousePressed, mouseReleased, etc. |
| KeyListener	         | listeners/keylistener.hpp	        | keyPressed, keyReleased           |
| FocusListener	         | listeners/focuslistener.hpp	        | focusLost, focusGained            |
| DeathListener	         | listeners/deathlistener.hpp	        | death                             |
| SelectionListener	     | listeners/selectionlistener.hpp	    | valueChanged                      |
| ContainerListener	     | listeners/containerlistener.hpp	    | widgetAdded, widgetRemoved        |
| WidgetListener	     | listeners/widgetlistener.hpp	        | ancestorHidden, etc.              |
| DropTargetListener	 | listeners/droptargetlistener.hpp	    | drag events                       |
| VisibilityEventHandler | listeners/visibilityeventhandler.hpp | visibility events                 |

# Events

Base path: include/fifechan/events

Events represent signals fired by widgets.

| Event Class            | File                      | Base Class  | Key Members                                    |
|------------------------|-------------------------- |-------------|------------------------------------------------|
| Event                  | events/event.hpp          | —           | Widget* getSource()                            |
| ActionEvent            | events/actionevent.hpp    | Event       | getId() — action identifier string             |
| ContainerEvent         | events/containerevent.hpp | Event       | getContainer() — affected Container            |
| DragEvent              | events/dragevent.hpp      | Event       | Type (Enter/Leave/Hover/Drop/Cancel), payload, x/y, screenX/screenY |
| InputEvent             | events/inputevent.hpp     | Event       | Modifier bitmask, consume(), isConsumed(), getDistributor() |
| KeyEvent               | events/keyevent.hpp       | InputEvent  | Type (Pressed/Released), Key, isNumericPad()   |
| MouseEvent             | events/mouseevent.hpp     | InputEvent  | Type (Moved/Pressed/Released/Wheel/Clicked/Entered/Exited/Dragged), Button (Empty/Left/Middle/Right/X1/X2), x/y, clickCount     |
| TextInputEvent         | events/textinputevent.hpp | InputEvent  | Type (Input/Editing), getText() — UTF-8 text   |
| SelectionEvent         | events/selectionevent.hpp | Event       | (no additional data beyond source)             |

# Visual Hierarchy of Widgets

Widget (abstract base)
├── Label
├── Icon
├── Spacer
├── Slider (also: MouseListener, KeyListener)
├── TextField (also: MouseListener, KeyListener)
│   └── PasswordField
├── TextBox (also: MouseListener, KeyListener)
├── ListBox (also: MouseListener, KeyListener)
│   └── uses ListModel interface
├── ScrollArea (also: MouseListener)
├── DropDown (also: ActionListener, KeyListener, MouseListener, FocusListener, SelectionListener)
│   └── uses ListModel interface
├── TabbedArea (also: ActionListener, KeyListener, MouseListener)
├── BarGraph
├── PieGraph
├── PointGraph
├── LineGraph
├── CurveGraph
├── ImageProgressBar
├── IconProgressBar
├── Button (also: MouseListener, KeyListener, FocusListener, WidgetListener)
│   ├── ImageButton
│   │   ├── ToggleButton
│   │   │   └── ActivityBarItem
│   │   └── CheckBox
│   │       └── RadioButton
│   └── MenuItem
└── Container
    ├── Window (also: MouseListener)
    ├── Panel (also: MouseListener)
    │   ├── PrimaryPanel
    │   └── SecondaryPanel
    ├── HorizontalBar
    │   ├── MenuBar (also: ActionListener, KeyListener, MouseListener)
    │   └── StatusBar
    ├── Tab (also: MouseListener)
    ├── FlowContainer
    ├── AdjustingContainer
    ├── ModalBackdrop (also: MouseListener)
    ├── Tooltip
    ├── BarSection
    ├── ActivityBar (also: ActionListener)
    └── MenuPopup (also: MouseListener, KeyListener, FocusListener, ActionListener)

## Container Widgets

**Container**
- the base composite widget that holds and manages child widgets;
- children are positioned relative to the container
- supports layout policies (fixed, manual resizing, or expanding content)

**FlowContainer**
- arranges children in rows or columns that wrap to the next line/column when space runs out
- configurable alignment per axis, like a word-wrap layout

**AdjustingContainer**
- organizes children in a table-like grid with a fixed number of columns, configurable column alignments, spacing, and padding
- automatically resizing itself as widgets are added or removed
