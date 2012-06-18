#include "fifechan/widgets/spacer.hpp"
#include "fifechan/exception.hpp"

namespace fcn
{
    Spacer::Spacer(SizeConstraint* sizeConstraint)
    {
        if(sizeConstraint == NULL)
            throw FCN_EXCEPTION("Spacer cannot have a NULL size constraint!");
        
        setSizeConstraint(sizeConstraint);
    }
    
    Spacer::~Spacer()
    {
    }
    
    void Spacer::draw(Graphics *graphics)
    {
    }
};