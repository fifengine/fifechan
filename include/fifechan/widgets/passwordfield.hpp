// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_PASSWORDFIELD_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_PASSWORDFIELD_HPP_

#include <fifechan/widgets/textfield.hpp>

#include <string>

namespace fcn
{
    class Text;

    /**
     * Password input widget.
     */
    class FIFEGUI_API PasswordField : public TextField
    {
    public:
        /**
         * Constructor.
         *
         * @param text Initializes password field's text.
         */
        explicit PasswordField(std::string const & text = "");

        ~PasswordField();

        // Inherited from TextField

        virtual void keyPressed(KeyEvent& keyEvent);

        virtual void setText(std::string const & text);

        virtual std::string getText() const;

    private:
        /**
         * @return Caret position of the actual text.
         */
        unsigned int getActualTextCaretPosition() const;

        /**
         * Sets the caret position of the actual text.
         */
        void setActualTextCaretPosition(unsigned int position);

        /**
         * Holds the actual text of the password field. The standard text
         * will hold asterisks instead.
         */
        Text* mActualText;
    };
}; // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_PASSWORDFIELD_HPP_
