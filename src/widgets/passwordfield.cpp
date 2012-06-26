
#include <fifechan/key.hpp>
#include <fifechan/text.hpp>
#include <fifechan/widgets/passwordfield.hpp>

namespace fcn
{
    PasswordField::PasswordField(const std::string &text)
    {
        setText(text);
    }
    
    PasswordField::~PasswordField()
    {
        delete mActualText;
    }
    
    void PasswordField::keyPressed(KeyEvent& keyEvent)
    {
        Key key = keyEvent.getKey();

        if (key.getValue() == Key::Left)
        {
            mText->setCaretPosition(mText->getCaretPosition() - 1);
            mActualText->setCaretPosition(mText->getCaretPosition() - 1);
        }
        else if (key.getValue() == Key::Right)
        {
            mText->setCaretPosition(mText->getCaretPosition() + 1);
            mActualText->setCaretPosition(mText->getCaretPosition() + 1);
        }
        else if (key.getValue() == Key::Delete && mEditable)
        {
            mText->remove(1);
            mActualText->remove(1);
        }
        else if (key.getValue() == Key::Backspace && mEditable)
        {
            mText->remove(-1);
            mActualText->remove(-1);
        }
        else if (key.getValue() == Key::Enter)
        {
            distributeActionEvent();
        }
        else if (key.getValue() == Key::Home)
        {
            mText->setCaretColumn(0);
            mActualText->setCaretColumn(0);
        }
        else if (key.getValue() == Key::End)
        {
            mText->setCaretColumn(mText->getNumberOfCharacters(0));
            mActualText->setCaretColumn(mText->getNumberOfCharacters(0));
        }
        else if (key.isCharacter() || key.getValue() > 255
                 && key.getValue() != Key::Tab
                 && mEditable)
        {
            mText->insert('*');
            mActualText->insert(key.getValue());
        }
 
        if (key.getValue() != Key::Tab)
        {
            keyEvent.consume();
        }

        fixScroll();
    }
    
    void PasswordField::setText(const std::string& text)
    {
        std::string asterisks;
        asterisks.assign(text.size(), '*');
        
        mText = new Text(asterisks);
        mActualText = new Text(text);
    }
    
    std::string PasswordField::getText() const
    {
        return mActualText->getContent();
    }
    
    
};