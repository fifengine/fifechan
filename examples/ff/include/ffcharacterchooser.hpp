// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FFCHARACTERCHOOSER_HPP
#define FFCHARACTERCHOOSER_HPP

#include <fifechan.hpp>

#include <memory>

class FFCharacterChooser : public fcn::Widget, fcn::KeyListener
{
public:
    FFCharacterChooser();
    ~FFCharacterChooser() override                            = default;
    FFCharacterChooser(FFCharacterChooser const &)            = delete;
    FFCharacterChooser& operator=(FFCharacterChooser const &) = delete;
    FFCharacterChooser(FFCharacterChooser&&)                  = delete;
    FFCharacterChooser& operator=(FFCharacterChooser&&)       = delete;

    void draw(fcn::Graphics* graphics) override;
    int getSelected() const;
    void setSelected(int selected);
    void setDistance(int distance);
    void keyPressed(fcn::KeyEvent& keyEvent) override;

private:
    int mSelected;
    int mDistance;
    std::unique_ptr<fcn::Image> mHand;
};

#endif
