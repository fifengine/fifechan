#include "guichan/color.hpp"

namespace gcn
{
  Color::Color()
  {
    r = 0;
    g = 0;
    b = 0;
    a = 0;
  }

  Color::Color(int color)
  {
    a = 0;
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
    result.a = a + color.a;

    result.r = (result.r>255?255:(result.r<0?0:result.r));
    result.g = (result.g>255?255:(result.g<0?0:result.g));
    result.b = (result.b>255?255:(result.b<0?0:result.b));
    result.a = (result.a>255?255:(result.a<0?0:result.a));

    return result;
  }
  
  Color Color::operator-(const Color& color) const
  {
    Color result;
    result.r = r - color.r;
    result.g = g - color.g;
    result.b = b - color.b;
    result.a = a - color.a;

    result.r = (result.r>255?255:(result.r<0?0:result.r));
    result.g = (result.g>255?255:(result.g<0?0:result.g));
    result.b = (result.b>255?255:(result.b<0?0:result.b));
    result.a = (result.a>255?255:(result.a<0?0:result.a));

    return result;
  }

  Color Color::operator*(float amount) const
  {
    Color result;
    result.r = (int)(r * amount);
    result.g = (int)(g * amount);
    result.b = (int)(b * amount);
    result.a = (int)(a * amount);

    result.r = (result.r>255?255:(result.r<0?0:result.r));
    result.g = (result.g>255?255:(result.g<0?0:result.g));
    result.b = (result.b>255?255:(result.b<0?0:result.b));
    result.a = (result.a>255?255:(result.a<0?0:result.a));

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
