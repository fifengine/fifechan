#ifndef GCN_RECTANGLE_HPP
#define GCN_RECTANGLE_HPP

namespace gcn
{
  /**
   * 
   */
  class Rectangle
  {
  public:

    /**
     * 
     */
    Rectangle();
    /**
     * 
     */
    Rectangle(int x, int y, int width, int height);
    
    /**
     * 
     */
    void setAll(int x, int y, int width, int height);

    /**
     *
     */
    bool intersect(const Rectangle& rectangle);

    bool isPointInRect(int x, int y);
    
    int x;
    int y;
    int width;
    int height;
    
  }; // end Rectangle
  
} // end gcn

#endif // end GCN_RECTANGEL_HPP
