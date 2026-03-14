// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_LISTBOX_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_LISTBOX_HPP_

#include <list>

#include "fifechan/keylistener.hpp"
#include "fifechan/listmodel.hpp"
#include "fifechan/mouselistener.hpp"
#include "fifechan/platform.hpp"
#include "fifechan/widget.hpp"

namespace fcn
{
    class SelectionListener;

    /**
     * A scrollable list box allowing item selection.
     *
     * To be able display a list the list box uses a user provided list model.
     * A list model can be any class that implements the ListModel interface.
     *
     * If an item is selected in the list box a select event will be sent to
     * all selection listeners of the list box. If an item is selected by using
     * a mouse click or by using the enter or space key an action event will be
     * sent to all action listeners of the list box.
     *
     * @ingroup widgets
     */
    class FIFEGUI_API ListBox : public Widget, public MouseListener, public KeyListener
    {
    public:
        ListBox();

        /**
         * Constructor.
         *
         * @param listModel the list model to use.
         */
        explicit ListBox(ListModel* listModel);

        ~ListBox() override = default;

        /**
         * Gets the selected item as an index in the list model.
         *
         * @return the selected item as an index in the list model.
         * @see setSelected
         */
        int getSelected() const;

        /**
         * Sets the selected item. The selected item is represented by
         * an index from the list model.
         *
         * @param selected the selected item as an index from the list model.
         * @see getSelected
         */
        void setSelected(int selected);

        /**
         * Sets the list model to use.
         *
         * @param listModel the list model to use.
         * @see getListModel
         */
        void setListModel(ListModel* listModel);

        /**
         * Gets the list model used.
         *
         * @return the list model used.
         * @see setListModel
         */
        ListModel* getListModel() const;

        /**
         * Checks whether the list box wraps when selecting items with a
         * keyboard.
         *
         * Wrapping means that the selection of items will be wrapped. That is,
         * if the first item is selected and up is pressed, the last item will
         * get selected. If the last item is selected and down is pressed, the
         * first item will get selected.
         *
         * @return true if wrapping is enabled, false otherwise.
         * @see setWrappingEnabled
         */
        bool isWrappingEnabled() const;

        /**
         * Sets the list box to wrap or not when selecting items with a
         * keyboard.
         *
         * Wrapping means that the selection of items will be wrapped. That is,
         * if the first item is selected and up is pressed, the last item will
         * get selected. If the last item is selected and down is pressed, the
         * first item will get selected.
         *
         * @see isWrappingEnabled
         */
        void setWrappingEnabled(bool wrappingEnabled);

        /**
         * Adds a selection listener to the list box. When the selection
         * changes an event will be sent to all selection listeners of the
         * list box.
         *
         * If you delete your selection listener, be sure to also remove it
         * using removeSelectionListener().
         *
         * @param selectionListener The selection listener to add.
         */
        void addSelectionListener(SelectionListener* selectionListener);

        /**
         * Removes a selection listener from the list box.
         *
         * @param selectionListener The selection listener to remove.
         */
        void removeSelectionListener(SelectionListener* selectionListener);

        /**
         * Gets the height of a row. Should be overridden if another row
         * height than the font height is preferred.
         *
         * @return The height of a row.
         */
        virtual unsigned int getRowHeight() const;

        // Inherited from Widget

        using Widget::resizeToContent;

        void resizeToContent(bool recursion) override;
        void adjustSize() override;
        void draw(Graphics* graphics) override;

        void logic() override;

        // Inherited from KeyListener

        void keyPressed(KeyEvent& keyEvent) override;

        // Inherited from MouseListener

        void mousePressed(MouseEvent& mouseEvent) override;

        void mouseWheelMovedUp(MouseEvent& mouseEvent) override;

        void mouseWheelMovedDown(MouseEvent& mouseEvent) override;

        void mouseDragged(MouseEvent& mouseEvent) override;

    protected:
        /**
         * Distributes a value changed event to all selection listeners
         * of the list box.
         *
         */
        void distributeValueChangedEvent();

        /**
         * The selected item as an index in the list model.
         */
        int mSelected{-1};

        /**
         * The list model to use.
         */
        ListModel* mListModel{nullptr};

        /**
         * True if wrapping is enabled, false otherwise.
         */
        bool mWrappingEnabled{false};

        /**
         * Typdef.
         */
        using SelectionListenerList = std::list<SelectionListener*>;

        /**
         * The selection listeners of the list box.
         */
        SelectionListenerList mSelectionListeners;

        /**
         * Typedef.
         */
        using SelectionListenerIterator = SelectionListenerList::iterator;

        /**
         * Concrete implementation of adjustSize. Constructors and
         * internal methods should call this non-virtual helper to
         * avoid virtual dispatch during construction.
         */
        void adjustSizeImpl();
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_LISTBOX_HPP_
