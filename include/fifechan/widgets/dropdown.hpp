// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_DROPDOWN_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_DROPDOWN_HPP_

#include <list>
#include <memory>

#include "fifechan/actionlistener.hpp"
#include "fifechan/focushandler.hpp"
#include "fifechan/focuslistener.hpp"
#include "fifechan/keylistener.hpp"
#include "fifechan/mouselistener.hpp"
#include "fifechan/platform.hpp"
#include "fifechan/selectionlistener.hpp"
#include "fifechan/widget.hpp"

namespace fcn
{
    class ListBox;
    class ListModel;
    class ScrollArea;

    /**
     * A dropdown list widget allowing single item selection from a list.
     *
     * The drop down consists of an internal ScrollArea and an internal ListBox.
     * The drop down also uses an internal FocusHandler to handle the focus of
     * the internal ScrollArea and the internal ListBox.
     * The scroll area and the list box can be passed to the drop down
     * if a custom scroll area and or a custom list box is preferable.
     *
     * To be able display a list the drop down uses a user provided list model.
     * A list model can be any class that implements the ListModel interface.
     *
     * If an item is selected in the drop down a select event will be sent to
     * all selection listeners of the drop down. If an item is selected by
     * using a mouse click or by using the enter or space key an action event
     * will be sent to all action listeners of the drop down.
     *
     * @ingroup widgets
     */
    class FIFEGUI_API DropDown :
        public ActionListener,
        public KeyListener,
        public MouseListener,
        public FocusListener,
        public SelectionListener,
        public Widget
    {
    public:
        /**
         * Constructor.
         *
         * @param listModel the ListModel to use.
         * @param scrollArea the ScrollArea to use.
         * @param listBox the listBox to use.
         * @see ListModel, ScrollArea, ListBox.
         */
        explicit DropDown(ListModel* listModel = nullptr, ScrollArea* scrollArea = nullptr, ListBox* listBox = nullptr);

        ~DropDown() override;

        DropDown(DropDown const &)            = delete;
        DropDown& operator=(DropDown const &) = delete;
        DropDown(DropDown&&)                  = delete;
        DropDown& operator=(DropDown&&)       = delete;

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
         * Sets the list model to use when displaying the list.
         *
         * @param listModel the list model to use.
         * @see getListModel
         */
        void setListModel(ListModel* listModel);

        /**
         * Gets the list model used.
         *
         * @return the ListModel used.
         * @see setListModel
         */
        ListModel* getListModel() const;

        /**
         * Adjusts the height of the drop down to fit the height of the
         * drop down's parent's height. It's used to not make the drop down
         * draw itself outside of it's parent if folded down.
         */
        void adjustHeight();

        /**
         * Adds a selection listener to the drop down. When the selection
         * changes an event will be sent to all selection listeners of the
         * drop down.
         *
         * If you delete your selection listener, be sure to also remove it
         * using removeSelectionListener().
         *
         * @param selectionListener the selection listener to add.
         */
        void addSelectionListener(SelectionListener* selectionListener);

        /**
         * Removes a selection listener from the drop down.
         *
         * @param selectionListener the selection listener to remove.
         */
        void removeSelectionListener(SelectionListener* selectionListener);

        // Inherited from Widget

        using Widget::resizeToContent;

        void resizeToContent(bool recursion) override;
        void adjustSize() override;
        void draw(Graphics* graphics) override;

        /** Set the base color used for the dropdown background/controls. */
        void setBaseColor(Color const & color) override;

        /** Set the explicit background color for the dropdown. */
        void setBackgroundColor(Color const & color) override;

        /** Set the foreground/text color used in the dropdown. */
        void setForegroundColor(Color const & color) override;

        /** Set the font used to render items in the dropdown. */
        void setFont(Font* font) override;

        /** Set the color used for the selected item highlight. */
        void setSelectionColor(Color const & color) override;

        // Inherited from Container

        Rectangle getChildrenArea() override;

        // Inherited from FocusListener

        void focusLost(Event const & event) override;

        // Inherited from ActionListener

        void action(ActionEvent const & actionEvent) override;

        // Inherited from DeathListener

        /** DeathListener callback invoked when a observed widget is destroyed. */
        virtual void death(Event const & event);

        // Inherited from KeyListener

        void keyPressed(KeyEvent& keyEvent) override;

        // Inherited from MouseListener

        void mousePressed(MouseEvent& mouseEvent) override;

        void mouseReleased(MouseEvent& mouseEvent) override;

        void mouseWheelMovedUp(MouseEvent& mouseEvent) override;

        void mouseWheelMovedDown(MouseEvent& mouseEvent) override;

        void mouseDragged(MouseEvent& mouseEvent) override;

        // Inherited from SelectionListener

        void valueChanged(SelectionEvent const & event) override;

    protected:
        /**
         * Draws the button of the drop down.
         *
         * @param graphics a Graphics object to draw with.
         */
        virtual void drawButton(Graphics* graphics);

        /**
         * Sets the drop down to be dropped down.
         */
        virtual void dropDown();

        /**
         * Sets the drop down to be folded up.
         */
        virtual void foldUp();

        /**
         * Distributes a value changed event to all selection listeners
         * of the drop down.
         *
         */
        void distributeValueChangedEvent();

        /**
         * True if the drop down is dropped down, false otherwise.
         */
        bool mDroppedDown{false};

        /**
         * True if the drop down has been pushed with the mouse, false
         * otherwise.
         */
        bool mPushed{false};

        /**
         * Holds what the height is if the drop down is folded up. Used when
         * checking if the list of the drop down was clicked or if the upper
         * part of the drop down was clicked on a mouse click.
         */
        int mFoldedUpHeight{0};

        /**
         * Owned internal scroll area when not supplied externally.
         */
        std::unique_ptr<ScrollArea> mOwnedScrollArea;

        /**
         * Owned internal list box when not supplied externally.
         */
        std::unique_ptr<ListBox> mOwnedListBox;

        /**
         * The scroll area used.
         */
        ScrollArea* mScrollArea;

        /**
         * The list box used.
         */
        ListBox* mListBox;

        /**
         * The internal focus handler used to keep track of focus for the
         * internal list box.
         */
        FocusHandler mInternalFocusHandler;

        /**
         * True if an internal scroll area is used, false if a scroll area
         * has been passed to the drop down which the drop down should not
         * deleted in it's destructor.
         */
        bool mInternalScrollArea{false};

        /**
         * True if an internal list box is used, false if a list box
         * has been passed to the drop down which the drop down should not
         * deleted in it's destructor.
         */
        bool mInternalListBox{false};

        /**
         * True if the drop down is dragged.
         */
        bool mIsDragged{false};

        /** List of SelectionListener. */
        using SelectionListenerList = std::list<SelectionListener*>;

        /**
         * The selection listener's of the drop down.
         */
        SelectionListenerList mSelectionListeners;

        /** Iterator for SelectionListenerList. */
        using SelectionListenerIterator = SelectionListenerList::iterator;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_DROPDOWN_HPP_
