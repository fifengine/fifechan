#include "guichan/color.hpp"

namespace gcn
{
  Color::Color()
  {
    r = 0;
    g = 0;
    b = 0;
    a = 255;
  }

  Color::Color(int color)
  {
    a = 255;
    r = (color >> 16) & 0xFF;
    g = (color >> 8) & 0xFF;
    b = color & 0xFF;    
  }
    
  Color::Color(int r, int g, int b, int a)
  {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
  }

  Color Color::operator+(const Color& color) const
  {
    Color result;
    result.r = r + color.r;
    result.g = g + color.g;
    result.b = b + color.b;
    result.a = 255;

    result.r = (result.r>255?255:(result.r<0?0:result.r));
    result.g = (result.g>255?255:(result.g<0?0:result.g));
    result.b = (result.b>255?255:(result.b<0?0:result.b));

    return result;
  }
  
  Color Color::operator-(const Color& color) const
  {
    Color result;
    result.r = r - color.r;
    result.g = g - color.g;
    result.b = b - color.b;
    result.a = 255;

    result.r = (result.r>255?255:(result.r<0?0:result.r));
    result.g = (result.g>255?255:(result.g<0?0:result.g));
    result.b = (result.b>255?255:(result.b<0?0:result.b));

    return result;
  }

  Color Color::operator*(float value) const
  {
    Color result;
    result.r = (int)(r * value);
    result.g = (int)(g * value);
    result.b = (int)(b * value);
    result.a = a;

    result.r = (result.r>255?255:(result.r<0?0:result.r));
    result.g = (result.g>255?255:(result.g<0?0:result.g));
    result.b = (result.b>255?255:(result.b<0?0:result.b));

    return result;    
  }

  bool Color::operator==(const Color& color) const
  {
    return r == color.r && g == color.g && b == color.b && a == color.a;
  }

  bool Color::operator!=(const Color& color) const
  {
    return !(r == color.r && g == color.g && b == color.b && a == color.a);
  }

} // end gcn
