
#include <fifechan/key.hpp>
#include <fifechan/text.hpp>
#include <fifechan/utf8stringeditor.hpp>
#include <fifechan/widgets/passwordfield.hpp>

namespace fcn
{
    PasswordField::PasswordField(const std::string &text)
    :
    TextField(text)
    {
        mActualText = new Text;
        setText(text);
    }
    
    PasswordField::~PasswordField()
    {
        delete mActualText;
    }
    
    void PasswordField::keyPressed(KeyEvent& keyEvent)
    {
        Key key = keyEvent.getKey();

        if (key.getValue() == Key::Left && getCaretPosition() > 0)
        {
            mText->setCaretPosition(mText->getCaretPosition() - 1);
            setActualTextCaretPosition(mStringEditor->prevChar(getText(), static_cast<int>(getActualTextCaretPosition())));
        }
        else if (key.getValue() == Key::Right && getCaretPosition() < getText().size())
        {
            mText->setCaretPosition(mText->getCaretPosition() + 1);
            setActualTextCaretPosition(mStringEditor->nextChar(getText(), static_cast<int>(getActualTextCaretPosition())));
        }
        else if (mEditable && key.getValue() == Key::Delete && getCaretPosition() < getText().size() && mText->getNumberOfRows() > 0)
        {
            mText->remove(1);
            setActualTextCaretPosition(mStringEditor->eraseChar(mActualText->getRow(0), static_cast<int>(getActualTextCaretPosition())));
        }
        else if (mEditable && key.getValue() == Key::Backspace && getCaretPosition() > 0 && mText->getNumberOfRows() > 0)
        {
            mText->remove(-1);
            setActualTextCaretPosition(mStringEditor->prevChar(mActualText->getRow(0), static_cast<int>(getActualTextCaretPosition())));
            setActualTextCaretPosition(mStringEditor->eraseChar(mActualText->getRow(0), static_cast<int>(getActualTextCaretPosition())));
        }
        else if (key.getValue() == Key::Enter)
        {
            distributeActionEvent();
        }
        else if (key.getValue() == Key::Home)
        {
            mText->setCaretColumn(0);
            setActualTextCaretPosition(0);
        }
        else if (key.getValue() == Key::End)
        {
            mText->setCaretColumn(mText->getNumberOfCharacters(0));
            setActualTextCaretPosition(getText().size());
        }

        // Add character to text, if key is realy a ASCII character
        // or is greater than 8bits long and the character is not
        // the tab key.

        else if ((key.isCharacter() || key.getValue() > 255 && mText->getNumberOfRows() > 0)
            && key.getValue() != Key::Tab)
        {
            mText->insert('*');
            setActualTextCaretPosition(mStringEditor->insertChar(mActualText->getRow(0), getActualTextCaretPosition(), key.getValue()));
        }

        if (key.getValue() != Key::Tab)
        {
            // consume all characters except TAB which is needed
            // for traversing through widgets in a container.
            keyEvent.consume();
        }

        fixScroll();   
    }
    
    void PasswordField::setText(const std::string& text)
    {
        std::string asterisks;
        asterisks.assign(text.size(), '*');
        
        mText->setContent(asterisks);
        mActualText->setContent(text);
    }
    
    std::string PasswordField::getText() const
    {
        return mActualText->getContent();
    }
    
    void PasswordField::setActualTextCaretPosition(unsigned int position)
    {
        mActualText->setCaretPosition(position);
    }
    
    unsigned int PasswordField::getActualTextCaretPosition() const  
    {
        return mActualText->getCaretPosition();
    }
};