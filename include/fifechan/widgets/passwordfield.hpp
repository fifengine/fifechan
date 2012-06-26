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
         * Holds the actual text of the password field. The standard text
         * will hold asterisks instead.
         */
        Text* mActualText;
    };
};

#endif //FCN_PASSWORDFIELD_HPP
