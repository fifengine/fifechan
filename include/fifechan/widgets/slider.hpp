// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_SLIDER_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_SLIDER_HPP_

#include <memory>

#include "fifechan/keylistener.hpp"
#include "fifechan/mouselistener.hpp"
#include "fifechan/platform.hpp"
#include "fifechan/widget.hpp"

namespace fcn
{
    /**
     * A control for selecting a numeric value by sliding along a scale.
     *
     * An implementation of a slider where a user can select different values by
     * sliding between a start value and an end value of a scale.
     * If the selected value is changed an action event will be sent to all
     * action listeners of the slider.
     *
     * @ingroup widgets
     */
    class FIFEGUI_API Slider : public Widget, public MouseListener, public KeyListener
    {
    public:
        /**
         * Draw orientations for the slider. A slider can be drawn vertically or
         * horizontally.
         */
        enum class Orientation : uint8_t
        {
            Horizontal = 0,
            Vertical
        };

        /**
         * Constructor. The default start value of the slider scale is zero.
         *
         * @param scaleEnd The end value of the slider scale.
         */
        explicit Slider(double scaleEnd = 1.0);

        /**
         * Constructor.
         *
         * @param scaleStart The start value of the slider scale.
         * @param scaleEnd The end value of the slider scale.
         */
        Slider(double scaleStart, double scaleEnd);

        ~Slider() override = default;

        /**
         * Sets the scale of the slider.
         *
         * @param scaleStart The start value of the scale.
         * @param scaleEnd tThe end of value the scale.
         * @see getScaleStart, getScaleEnd
         */
        void setScale(double scaleStart, double scaleEnd);

        /**
         * Gets the start value of the scale.
         *
         * @return The start value of the scale.
         * @see setScaleStart, setScale
         */
        double getScaleStart() const;

        /**
         * Sets the start value of the scale.
         *
         * @param scaleStart The start value of the scale.
         * @see getScaleStart
         */
        void setScaleStart(double scaleStart);

        /**
         * Gets the end value of the scale.
         *
         * @return The end value of the scale.
         * @see setScaleEnd, setScale
         */
        double getScaleEnd() const;

        /**
         * Sets the end value of the scale.
         *
         * @param scaleEnd The end value of the scale.
         * @see getScaleEnd
         */
        void setScaleEnd(double scaleEnd);

        /**
         * Gets the current selected value.
         *
         * @return The current selected value.
         * @see setValue
         */
        double getValue() const;

        /**
         * Sets the current selected value.
         *
         * @param value The current selected value.
         * @see getValue
         */
        void setValue(double value);

        /**
         * Sets the length of the marker.
         *
         * @param length The length for the marker.
         * @see getMarkerLength
         */
        void setMarkerLength(int length);

        /**
         * Gets the length of the marker.
         *
         * @return The length of the marker.
         * @see setMarkerLength
         */
        int getMarkerLength() const;

        /**
         * Sets the orientation of the slider. A slider can be drawn vertically
         * or horizontally.
         *
         * @param orientation The orientation of the slider.
         * @see getOrientation
         */
        void setOrientation(Orientation orientation);

        /**
         * Gets the orientation of the slider. A slider can be drawn vertically
         * or horizontally.
         *
         * @return The orientation of the slider.
         * @see setOrientation
         */
        Orientation getOrientation() const;

        /**
         * Sets the step length. The step length is used when the keys LEFT
         * and RIGHT are pressed to step in the scale.
         *
         * @param length The step length.
         * @see getStepLength
         */
        void setStepLength(double length);

        /**
         * Gets the step length. The step length is used when the keys LEFT
         * and RIGHT are pressed to step in the scale.
         *
         * @return the step length.
         * @see setStepLength
         */
        double getStepLength() const;

        // Inherited from Widget

        void draw(Graphics* graphics) override;

        // Inherited from MouseListener.

        void mousePressed(MouseEvent& mouseEvent) override;
        void mouseDragged(MouseEvent& mouseEvent) override;
        void mouseWheelMovedUp(MouseEvent& mouseEvent) override;
        void mouseWheelMovedDown(MouseEvent& mouseEvent) override;
        void mouseWheelMovedRight(MouseEvent& mouseEvent) override;
        void mouseWheelMovedLeft(MouseEvent& mouseEvent) override;

        // Inherited from KeyListener

        void keyPressed(KeyEvent& keyEvent) override;

    protected:
        /**
         * Draws the marker.
         *
         * @param graphics A graphics object to draw with.
         */
        virtual void drawMarker(Graphics* graphics);

        /**
         * Converts a marker position to a value in the scale.
         *
         * @param position The position to convert.
         * @return A scale value corresponding to the position.
         * @see valueToMarkerPosition
         */
        virtual double markerPositionToValue(int position) const;

        /**
         * Converts a value to a marker position.
         *
         * @param value The value to convert.
         * @return A marker position corresponding to the value.
         * @see markerPositionToValue
         */
        virtual int valueToMarkerPosition(double value) const;

        /**
         * Gets the marker position of the current selected value.
         *
         * @return The marker position of the current selected value.
         */
        virtual int getMarkerPosition() const;

        /**
         * True if the slider is dragged, false otherwise.
         */
        bool mDragged{false};

        /**
         * Holds the current selected value.
         */
        double mValue{0.0};

        /**
         * Holds the step length. The step length is used when the keys LEFT
         * and RIGHT are pressed to step in the scale.
         */
        double mStepLength{0.5};

        /**
         * Holds the length of the marker.
         */
        int mMarkerLength{10};

        /**
         * Holds the start value of the scale.
         */
        double mScaleStart{0.0};

        /**
         * Holds the end value of the scale.
         */
        double mScaleEnd{1.0};

        /**
         * Holds the orientation of the slider. A slider can be drawn
         * vertically or horizontally.
         */
        Orientation mOrientation{Orientation::Horizontal};
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_SLIDER_HPP_
