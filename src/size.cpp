#include "fifechan/size.hpp"

namespace fcn
{
    Size::Size(int width, int height)
    :
    mWidth(width),
    mHeight(height)
    {
    }
    
    Size::~Size()
    {
    }
    
    int Size::getWidth() const
    {
        return mWidth;
    }
    
    int Size::getHeight() const
    {
        return mHeight;
    }
    
    void Size::setWidth(int width)
    {
        mWidth = width;
    }
    
    void Size::setHeight(int height)
    {
        mHeight = height;
    }
};