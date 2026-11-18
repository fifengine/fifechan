// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef FFLISTBOX_HPP
#define FFLISTBOX_HPP

// Standard library includes
#include <memory>

// Third-party library includes
#include <fifechan.hpp>

class FFListBox : public fcn::ListBox
{
    public:
        FFListBox();
        ~FFListBox() override;
        FFListBox(FFListBox const &)            = delete;
        FFListBox& operator=(FFListBox const &) = delete;
        FFListBox(FFListBox&&)                  = delete;
        FFListBox& operator=(FFListBox&&)       = delete;

        void draw(fcn::Graphics* graphics) override;
        void logic() override;
        void setSelected(int i);

    private:
        /**
         * Horizontal space reserved in front of each label for the hand icon.
         */
        static constexpr int kHandIconOffset = 16;

        static std::unique_ptr<fcn::Image> mHand;
        static int mInstances;
};

#endif // FFLISTBOX_HPP
