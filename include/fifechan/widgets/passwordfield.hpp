/***************************************************************************
 *   Copyright (c) 2017-2019 by the fifechan team                               *
 *   https://github.com/fifengine/fifechan                                 *
 *   This file is part of fifechan.                                        *
 *                                                                         *
 *   fifechan is free software; you can redistribute it and/or             *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA          *
 ***************************************************************************/

#ifndef FCN_PASSWORDFIELD_HPP
#define FCN_PASSWORDFIELD_HPP

#include <fifechan/widgets/textfield.hpp>

namespace fcn
{
    class Text;
    
    /**
     * Password input widget.
     */
    class FCN_CORE_DECLSPEC PasswordField : public TextField
    {
    public:
        
        /**
         * Constructor.
         *
         * @param text Initializes password field's text. 
         */
        PasswordField(const std::string& text = "");
        
        ~PasswordField();
        
        // Inherited from TextField
     
        virtual void keyPressed(KeyEvent& keyEvent);
        
        virtual void setText(const std::string& text);
        
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
};

#endif //FCN_PASSWORDFIELD_HPP
