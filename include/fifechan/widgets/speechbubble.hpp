// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_SPEECHBUBBLE_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_SPEECHBUBBLE_HPP_

// Standard library includes
#include <cstdint>

// Platform config include
#include "fifechan/platform.hpp"

// Project headers (subdirs before local)
#include "fifechan/graphics.hpp"
#include "fifechan/widgets/container.hpp"

namespace fcn
{
    class Color;

    class FIFEGUI_API SpeechBubble : public Container
    {
        public:
            enum class TailDirection : uint8_t
            {
                None,
                Up,
                Down,
                Left,
                Right,
                BottomLeft,
                BottomRight,
                TopLeft,
                TopRight,
                Auto
            };

            //! Comic bubble body style (classic / round / thought / shout / whisper).
            enum class BubbleStyle : uint8_t
            {
                Classic, //!< oval bubble, pointy triangular tail
                Round,   //!< soft ellipse, short shallow tail
                Thought, //!< puffy cloud bubble, tail of trailing dots
                Shout,   //!< jagged burst bubble, spiky tail
                Whisper  //!< dashed outline, small pointy tail
            };

            struct TailProfile
            {
                    float tipWidthRatio = 0.0f; // width at tip / width at body (0 = sharp point)
                    float curvature     = 0.0f; // spine bend (0 = straight, <1 = gentle curve)
                    float hook          = 0.0f; // tip hook (0 = none, <1 = moderate hook)
                    float taperExponent =
                        1.0f; // 1 = linear taper, <1 = slow-initial taper (stays wide longer), >1 = fast-initial taper
                    float jaggedness = 0.0f; // tooth amplitude (0 = smooth)
                    int teeth        = 5;    // number of teeth (jagged)
                    float prongDepth = 0.0f; // fork depth (0 = none)
                    int segments     = 6;    // spine subdivision (more = smoother curves, but noisier at 1px)

                    static TailProfile sharp()
                    {
                        return {};
                    }
                    static TailProfile rounded()
                    {
                        return {0.3f};
                    }
                    static TailProfile curved()
                    {
                        return {0.0f, 0.3f};
                    }
                    static TailProfile wide()
                    {
                        return {0.55f, 0.0f, 0.0f, 0.8f};
                    }
                    static TailProfile jagged()
                    {
                        return {0.0f, 0.0f, 0.0f, 1.0f, 0.5f};
                    }
                    static TailProfile pronged()
                    {
                        return {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 5, 0.5f};
                    }
            };

            SpeechBubble();

            ~SpeechBubble() override;

            SpeechBubble(SpeechBubble const &)            = delete;
            SpeechBubble& operator=(SpeechBubble const &) = delete;
            SpeechBubble(SpeechBubble&&)                  = delete;
            SpeechBubble& operator=(SpeechBubble&&)       = delete;

            void setCornerRadius(unsigned int radius);
            unsigned int getCornerRadius() const;

            void setTailWidth(unsigned int width);
            unsigned int getTailWidth() const;

            void setTailHeight(unsigned int height);
            unsigned int getTailHeight() const;

            void setBubbleStyle(BubbleStyle style);
            BubbleStyle getBubbleStyle() const;

            void setTailDirection(TailDirection direction);
            TailDirection getTailDirection() const;

            void setTailProfile(TailProfile const & profile);
            TailProfile const & getTailProfile() const;

            void setTailColor(Color color);
            Color getTailColor() const;

            void setTailOffset(float offset);
            float getTailOffset() const;

            void setBorderSize(unsigned int borderSize);

            Rectangle getChildrenArea() override;

            void adjustSize() override;

            void draw(Graphics* graphics) override;

        protected:
            unsigned int mCornerRadius{10};
            unsigned int mTailWidth{16};
            unsigned int mTailHeight{12};
            BubbleStyle mBubbleStyle{BubbleStyle::Classic};
            TailDirection mTailDirection{TailDirection::Auto};
            TailProfile mTailProfile;
            float mTailOffset{0.5f};
            Color mTailColor{};
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_SPEECHBUBBLE_HPP_
