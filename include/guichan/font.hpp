#ifndef GCN_FONT_HPP
#define GCN_FONT_HPP

#include <string>
#include <pair>
#include <vector>

namespace gcn
{
  /**
   * 
   */
  class Font
  {
  public:
    /**
     * 
     */
    int getWidth(std::string str);
    /**
     * 
     */
    int getWidth(char character);    

  private:
    Image* mImage;
    std::vector<std::pair<int,int> > mLetters;
    int mHeight;
    
  }; // end Font
  
} // end gcn

#endif // end GCN_FONT_HPP
