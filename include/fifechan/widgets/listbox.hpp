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
     * An implementation of a list box where an item can be selected.
     *
     * To be able display a list the list box uses a user provided list model.
     * A list model can be any class that implements the ListModel interface.
     *
     * If an item is selected in the list box a select event will be sent to
     * all selection listeners of the list box. If an item is selected by using
     * a mouse click or by using the enter or space key an action event will be
     * sent to all action listeners of the list box.
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

        virtual ~ListBox() { }

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

        virtual void resizeToContent(bool recursiv = true);
        virtual void adjustSize();
        virtual void draw(Graphics* graphics);

        virtual void logic();

        // Inherited from KeyListener

        virtual void keyPressed(KeyEvent& keyEvent);

        // Inherited from MouseListener

        virtual void mousePressed(MouseEvent& mouseEvent);

        virtual void mouseWheelMovedUp(MouseEvent& mouseEvent);

        virtual void mouseWheelMovedDown(MouseEvent& mouseEvent);

        virtual void mouseDragged(MouseEvent& mouseEvent);

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
        int mSelected;

        /**
         * The list model to use.
         */
        ListModel* mListModel;

        /**
         * True if wrapping is enabled, false otherwise.
         */
        bool mWrappingEnabled;

        /**
         * Typdef.
         */
        typedef std::list<SelectionListener*> SelectionListenerList;

        /**
         * The selection listeners of the list box.
         */
        SelectionListenerList mSelectionListeners;

        /**
         * Typedef.
         */
        typedef SelectionListenerList::iterator SelectionListenerIterator;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_LISTBOX_HPP_
