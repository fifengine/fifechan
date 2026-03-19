// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_LABEL_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_LABEL_HPP_

#include <string>

#include "fifechan/graphics.hpp"
#include "fifechan/platform.hpp"
#include "fifechan/widget.hpp"

namespace fcn
{
    /**
     * Displays a text caption.
     *
     * @ingroup widgets
     */
    class FIFEGUI_API Label : public Widget
    {
    public:
        Label();

        /**
         * Constructor. The label will be automatically resized
         * to fit the caption.
         *
         * @param caption The caption of the label.
         */
        explicit Label(std::string caption);

        /**
         * Gets the caption of the label.
         *
         * @return The caption of the label.
         * @see setCaption
         */
        std::string const & getCaption() const;

        /**
         * Sets the caption of the label. It's advisable to call
         * adjustSize after setting of the caption to adjust the
         * label's size to fit the caption.
         *
         * @param caption The caption of the label.
         * @see getCaption, adjustSize
         */
        void setCaption(std::string const & caption);

        /**
         * Sets the alignment of the caption. The alignment is relative
         * to the center of the label.
         *
         * @param alignment The alignment of the caption of the label.
         * @see getAlignment, Graphics
         */
        void setAlignment(Graphics::Alignment alignment);

        /**
         * Gets the alignment of the caption. The alignment is relative to
         * the center of the label.
         *
         * @return The alignment of caption of the label.
         * @see setAlignment Graphics
         */
        Graphics::Alignment getAlignment() const;

        // Inherited from Widget

        using Widget::resizeToContent;

        void resizeToContent(bool recursion) override;
        void adjustSize() override;
        void draw(Graphics* graphics) override;

    protected:
        /**
         * Adjusts the size of the label to fit the caption.
         *
         * The public `adjustSize()` method serves as a virtual entry point
         * for polymorphism, while this `adjustSizeImpl()` method contains
         * the concrete implementation of the resizing logic.
         *
         * @see adjustSize (virtual entry point for polymorphism)
         */
        void adjustSizeImpl();

        /**
         * Holds the caption of the label.
         */
        std::string mCaption;

        /**
         * Holds the alignment of the caption.
         */
        Graphics::Alignment mAlignment;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_LABEL_HPP_
