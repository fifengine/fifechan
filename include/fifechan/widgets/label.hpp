// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_LABEL_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_LABEL_HPP_

// Standard library includes
#include <string>

// Platform config include
#include "fifechan/platform.hpp"

// Project headers (subdirs before local)
#include "fifechan/graphics.hpp"
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
         * Constructor.
         *
         * The label will be automatically resized to fit the caption.
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
         * Sets the caption of the label.
         *
         * After updating the caption, call adjustSize() to ensure
         * the label resizes to fit the new content.
         *
         * @param caption The caption of the label.
         * @see getCaption, adjustSize
         */
        void setCaption(std::string const & caption);

        /**
         * Sets the alignment of the caption.
         *
         * The alignment is relative to the center of the label.
         *
         * @param alignment The alignment of the caption of the label.
         * @see getAlignment, Graphics
         */
        void setAlignment(Graphics::Alignment alignment);

        /**
         * Gets the alignment of the caption.
         *
         * The alignment is relative to the center of the label.
         *
         * @return The alignment of caption of the label.
         * @see setAlignment Graphics
         */
        Graphics::Alignment getAlignment() const;

        /**
         * Sets the vertical alignment of the caption.
         *
         * @param alignment The vertical alignment of the caption.
         * @see getVerticalAlignment
         */
        void setVerticalAlignment(Graphics::VerticalAlignment alignment);

        /**
         * Gets the vertical alignment of the caption.
         *
         * @return The vertical alignment of the caption.
         * @see setVerticalAlignment
         */
        Graphics::VerticalAlignment getVerticalAlignment() const;

        // Inherited from Widget

        using Widget::resizeToContent;

        void resizeToContent(bool recursion = true) override;
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
         * Holds the horizontal alignment of the caption.
         */
        Graphics::Alignment mAlignment{Graphics::Alignment::Left};

        /**
         * Holds the vertical alignment of the caption.
         */
        Graphics::VerticalAlignment mVerticalAlignment{Graphics::VerticalAlignment::Center};
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_LABEL_HPP_
