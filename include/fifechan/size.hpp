#ifndef FCN_SIZE_HPP
#define FCN_SIZE_HPP

#include "fifechan/platform.hpp"

namespace fcn
{
    class FCN_CORE_DECLSPEC Size
    {
    public:
        
        /**
         * Constructor initializes size values.
         * 
         * @param width Width of size.
         * @param height Height of size.
         */
        Size(int width = 0, int height = 0);
        
        ~Size();
        
        /**
         * @return Width of size.
         */
        int getWidth() const;
        
        /**
         * @return Height of size.
         */
        int getHeight() const;
        
        /**
         * Sets the width of size.
         * 
         * @param width Desired width.
         */
        void setWidth(int width);
        
        /**
         * Sets the height of size.
         * 
         * @param height Desired height.
         */
        void setHeight(int height);
        
    private:
        
        int mWidth;
        
        int mHeight;
    };
};

#endif