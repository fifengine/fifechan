#ifndef GCN_IMAGE_HPP
#define GCN_IMAGE_HPP

#include <string>

namespace gcn
{
  /**
   * 
   */
  class Image
  {
  public:
    /**
     * 
     */
    virtual const std::string& getFilename() = 0;
    /**
     * 
     */
    virtual int getWidth() const = 0;
    /**
     * 
     */
    virtual int getHeight() const = 0;
    
  }; // end Image
  
} // end gcn

#endif // end GCN_IMAGE_HPP
