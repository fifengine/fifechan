#ifndef GCN_WIDGET_HPP
#define GCN_WIDGET_HPP

#include <string>

namespace gcn
{

  /**
   * 
   */
  class Widget
  {
  public:
    /**
     * 
     */
    virtual ~Widget();
    /**
     * 
     */
    void setParent(Widget* parent);
    /**
     * 
     */
    Widget* getParent();

    /**
     * 
     */
    void setWidth(int width);
    /**
     * 
     */
    int getWidth();

    /**
     * 
     */
    void setHeight(int height);
    /**
     * 
     */
    int getHeight();

    /**
     * 
     */
    void setX(int x);
    /**
     * 
     */
    int getX();

    /**
     * 
     */
    void setY(int y);
    /**
     * 
     */
    int getY();

    /**
     * 
     */
    void setPosition(int x, int y);
    
    /**
     * 
     */
    void setDimension(const Rectangle& dimension);
    /**
     * 
     */
    const Dimension& getDimension();

    /**
     * 
     */
    void setFocusable(bool focusable);
    /**
     * 
     */
    bool hasFocus();
    /**
     * 
     */
    bool isFocusable();
    /**
     * 
     */
    void requestFocus();
    
    /**
     * 
     */
    bool isVisible();
    /**
     * 
     */
    void setVisible(bool visible);

    /**
     * 
     */
    void setForegroundColor(const Color& color);
    /**
     * 
     */
    void setBackgroundColor(const Color& color);
    /**
     * 
     */
    void setDefaultBackgroundColor(const Color& color);
    /**
     * 
     */
    void setDefaultForegroundColor(const Color& color);
    /**
     * 
     */
    void setFocusHandler(FocusHandler* focusHandler);

    /**
     * 
     */
    void keyMessage();
    /**
     * 
     */
    void keyDownMessage(const Key& key);
    /**
     * 
     */
    void keyReleaseMessage(const Key& key);
    /**
     * 
     */
    void keyTypeMessage(const Key& key);
    
    /**
     * 
     */
    void mouseInMessage();
    /**
     * 
     */
    void mouseOutMessage();
    /**
     * 
     */
    void mouseDownMessage();
    /**
     * 
     */
    void mouseReleaseMessage();
    /**
     * 
     */
    void mouseWheelUpMessage();
    /**
     * 
     */
    void mouseWheelDownMessage();
    /**
     * 
     */
    void mouseMotionMessage(int x, int y);

    /**
     * 
     */
    const std::string& getMouseType();
    /**
     * 
     */
    void setMouseType(str::string& mouseType);

    /**
     * 
     */
    void setEventId(const std::string& eventId);
    /**
     * 
     */
    const std::string& getEventId();

    /**
     * 
     */
    void setActionListener(ActionListener* actionListener);
    /**
     * 
     */
    void setMouseListener(MouseListener* mouseListener);
    /**
     * 
     */
    void setKeyListener(KeyListener* keyListener);
    
    /**
     * 
     */
    virtual void logic() = 0;
    
    /**
     * 
     */
    virtual void draw(Grapics* grapics) = 0;

  protected:
    std::string mEventId;
    FocusHandler* mFocusHandler;
    MouseListener* mMouseListener;
    ActionListener* mActionListener;
    Rectangle mDimension;
    bool mFocused;
    
  }; // end Widget
  
} // end gcn

#endif // end GCN_WIDGET_HPP
