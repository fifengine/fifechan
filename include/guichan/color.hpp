#ifndef GCN_COLOR_HPP
#define GCN_COLOR_HPP

namespace gcn
{
  /**
   * 
   */
  class Color
  {    
  public:
    /**
     * 
     */
    Color();
    
    /**
     * 
     */
    Color(int color);
    
    /**
     * 
     */
    Color(int r, int g, int b, int a = 0);

    /**
     * 
     */
    Color operator+(const Color& color) const;

    /**
     * 
     */
    Color operator-(const Color& color) const;

    /**
     *
     */
    Color operator*(float amount) const;

    int r;
    int g;
    int b;
    int a;
    
  }; // end Color
  
} // end gcn

#endif // end GCN_COLOR_HPP
