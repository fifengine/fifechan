/*
 *    _aaaa,  _aa.  sa,  aaa              _aaaa,_  ac  .aa.   .aa.  .aa,  _a, sa
 *  .wWV!!!T  |Wm;  dQ[  $WF            _mWT!"?Y  ]QE  :Q#:   ]QW[  :WWk. ]Q[ dW
 * .jWf       :WW: .dQ[  dQ[           .mW(       )WE  :Q#:  .mSQh. :mWQa.]W[ dQ
 * |QW:       :Wm;  mQ[  dQ[           ]Qk        )Qmi_aQW:  <B:$Qc :WBWQ()W[ dQ
 * |W#:  .ww  ;WW;  dQ[  dQ[  .......  ]Qk        )QB?YYW#:  jf ]Qp.:mE)Qm]Q[ )W
 * +WQ;  :Wm  |Wm; .mQ[  dQ[ :qgggggga ]Qm.       ]WE  :Q# :=QasuQm;:Wk 3QQW[ )Y
 *  ]Wmi.:Wm  +$Q; .mW(  dQ[  !"!!"!!^ dQk,  ._   ]WE  :Q# :3D"!!$Qc.Wk -$WQ[   
 *   "?????? ` "?!=m?!   ??'            -??????!  -?!  -?? -?'   "?"-?"  "??' "?
 *
 * Copyright (c) 2004 darkbits                              Js_./
 * Per Larsson a.k.a finalman                          _RqZ{a<^_aa
 * Olof Naessén a.k.a jansem/yakslem                _asww7!uY`>  )\a//
 *                                                 _Qhm`] _f "'c  1!5m
 * Visit: http://guichan.darkbits.org             )Qk<P ` _: :+' .'  "{[
 *                                               .)j(] .d_/ '-(  P .   S
 * License: (BSD)                                <Td/Z <fP"5(\"??"\a.  .L
 * Redistribution and use in source and          _dV>ws?a-?'      ._/L  #'
 * binary forms, with or without                 )4d[#7r, .   '     )d`)[
 * modification, are permitted provided         _Q-5'5W..j/?'   -?!\)cam'
 * that the following conditions are met:       j<<WP+k/);.        _W=j f
 * 1. Redistributions of source code must       .$%w\/]Q  . ."'  .  mj$
 *    retain the above copyright notice,        ]E.pYY(Q]>.   a     J@\
 *    this list of conditions and the           j(]1u<sE"L,. .   ./^ ]{a
 *    following disclaimer.                     4'_uomm\.  )L);-4     (3=
 * 2. Redistributions in binary form must        )_]X{Z('a_"a7'<a"a,  ]"[
 *    reproduce the above copyright notice,       #}<]m7`Za??4,P-"'7. ).m
 *    this list of conditions and the            ]d2e)Q(<Q(  ?94   b-  LQ/
 *    following disclaimer in the                <B!</]C)d_, '(<' .f. =C+m
 *    documentation and/or other materials      .Z!=J ]e []('-4f _ ) -.)m]'
 *    provided with the distribution.          .w[5]' _[ /.)_-"+?   _/ <W"
 * 3. Neither the name of darkbits nor the     :$we` _! + _/ .        j?
 *    names of its contributors may be used     =3)= _f  (_yQmWW$#(    "
 *    to endorse or promote products derived     -   W,  sQQQQmZQ#Wwa]..
 *    from this software without specific        (js, \[QQW$QWW#?!V"".
 *    prior written permission.                    ]y:.<\..          .
 *                                                 -]n w/ '         [.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT       )/ )/           !
 * HOLDERS AND CONTRIBUTORS "AS IS" AND ANY         <  (; sac    ,    '
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING,               ]^ .-  %
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF            c <   r
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR            aga<  <La
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE          5%  )P'-3L
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR        _bQf` y`..)a
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          ,J?4P'.P"_(\?d'.,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES               _Pa,)!f/<[]/  ?"
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT      _2-..:. .r+_,.. .
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     ?a.<%"'  " -'.a_ _,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION)                     ^
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef GCN_WIDGET_HPP
#define GCN_WIDGET_HPP

#include <string>

#include "guichan/actionlistener.hpp"
#include "guichan/color.hpp"
#include "guichan/graphics.hpp"
#include "guichan/keyinput.hpp"
#include "guichan/keylistener.hpp"
#include "guichan/mouseinput.hpp"
#include "guichan/mouselistener.hpp"
#include "guichan/rectangle.hpp"

namespace gcn
{
  /*
   * This class definiton exists for the widget class to compile.
   * For more information regarding class FocusHandler please see
   * the FocusHandler header file.
   */
  class FocusHandler;

  /**
   * This is the base class for all widgets. It is abstract.
   * It handles the common logic for all widgets such as
   * user input.
   *
   * NOTE: Functions begining with underscore "_" should not
   * be overloaded unless you know what you are doing.
   */
  class Widget
  {
  public:
    /**
     * Constructor. Resets member variables. Noteable, a widget
     * is not focusable as default, therefore, widgets that are
     * supposed to be focusable should overide this default in
     * their own constructor.
     */
    Widget();
    
    /**
     * Default destructor.
     */
    virtual ~Widget(){}

    /**
     * This function should draw the widget (No kidding!).
     * It is called by the parent widget when it is time
     * for the widget to draw itself. The graphics object
     * is set up so that all drawing is relative to the
     * widget, i.e coordinate (0,0) is the top-left corner
     * of the widget. It is not possible to draw outside of
     * a widgets dimension.
     */
    virtual void draw(Graphics* graphics) = 0;

    /**
     * This functions gets called for all widgets in the gui
     * each time Gui::logic is called. You can do logic stuff
     * here like playing an animation.
     *
     * @see Gui
     */
    virtual void logic(){};
    
    /**
     * @return a pointer to the widgets parent container. Returns
     *         null if the widget has no parent for example if the
     *         widget is the gui top widget.
     */
    virtual Widget* getParent() const;
    
    /**
     * @param width the widget width in pixels
     */
    virtual void setWidth(int width);

    /**
     * @return the widget with in pixels
     */
    virtual int getWidth();
    
    /**
     * @param height the widget height in pixels
     */
    virtual void setHeight(int height);

    /**
     * @return the widget height in pixels
     */
    virtual int getHeight();

    /**
     * @param x the widgets x coordinate relative to its
     *          container.
     */
    virtual void setX(int x);

    /**
     * @return the widgets x coordinate relative to its
     *         container.
     */
    virtual int getX();

    /**
     * @param y the widgets y coordinate relative to its
     *          container.
     */
    virtual void setY(int y);

    /**
     * @return the widgets y coordinate relative to its
     *         container.
     */
    virtual int getY();

    /**
     * @param x the widgets x coordinate relative to its
     *          container.
     * @param y the widgets y coordinate relative to its
     *          container.
     */
    virtual void setPosition(int x, int y);
    
    /**
     * @param dimension the widgets dimension relative to is
     *                  container.
     * @see Rectangle
     */
    virtual void setDimension(const Rectangle& dimension);
    
    /**
     * @return the widgets dimension relative to its
     *         container.
     * @see Rectangle
     */
    virtual const Rectangle& getDimension();

    /**
     * @param focusable indicates whether the widget is focusable
     *                  or not.
     */
    void setFocusable(bool focusable);

    /**
     * @return true if the widget is focusable.
     */
    bool isFocusable();

    /**
     * @return true if the widget currently has focus.
     */
    bool hasFocus();

    /**
     * @return true if the widget currently has the mouse.
     */
    virtual bool hasMouse();

    
    /**
     * @param visible indicates whether the widget is visible or
     *                not.
     */
    void setVisible(bool visible);

    /**
     * @return true if the widget is visible.
     */
    bool isVisible();

    /**
     * @param color the foreground color.
     * @see Color
     */
    virtual void setForegroundColor(const Color& color);

    /**
     * @return the widgets foreground color.
     * @see Color
     */
    virtual const Color& getForegroundColor() const;
    
    /**
     * @param color the background color.
     * @see Color
     */
    virtual void setBackgroundColor(const Color& color);

    /**
     * @return the widgets background color.
     * @see Color
     */
    virtual const Color& getBackgroundColor() const;    
    
    /**
     * This function is called if a key is pressed when
     * the widget has keyboard focus. Overload this function
     * if you want your widget to handle key presses.
     *
     * If a key is held down the widget will recieve multiple
     * key presses.
     *
     * @param key the key pressed
     * @see Key
     */
    virtual void keyPressMessage(const Key& key){}

    /**
     * This function is called if a key is released when
     * the widget has keyboard focus. Overload this function
     * if you want your widget to handle key releases.
     *
     * @param key the key released
     * @see Key
     */
    virtual void keyReleaseMessage(const Key& key){}
    
    /**
     * This function is called when the mouse enters into the
     * widget area. In addition to this function there is also
     * the function hasMouse with which you can check if the
     * widget currently has the mouse.
     */
    virtual void mouseInMessage(){}

    /**
     * This function is called when the mouse leaves the
     * widget area. In addition to this function there is also
     * the function hasMouse with which you can check if the
     * widget currently has the mouse.
     */
    virtual void mouseOutMessage(){}
    
    /**
     * This function is called when a mouse button is pressed
     * when the mouse is in the widget area or if the widget
     * has focus.
     *
     * NOTE: A mouse press is NOT equal to a mouse click.
     *       Use mouseClickMessage to check for mouse clicks.
     *
     * @param x the x coordinate of the mouse relative to the
     *          widget itself.
     * @param y the y coordinate of the mouse relative to the
     *          widget itself.
     * @param button the button pressed
     * @see mouseClickMessage
     */
    virtual void mousePressMessage(int x, int y, int button){}
    
    /**
     * This function is called when a mouse button is released
     * when the mouse is in the widget area or if the widget
     * has focus.
     *
     * @param x the x coordinate of the mouse relative to the
     *          widget itself.
     * @param y the y coordinate of the mouse relative to the
     *          widget itself.
     * @param button the button released
     */
    virtual void mouseReleaseMessage(int x, int y, int button){}
    
    /**
     * This function is called when a mouse button is pressed
     * and released (clicked) when the mouse is in the widget
     * area or if the widget has focus.
     * 
     * @param x the x coordinate of the mouse relative to the
     *          widget itself.
     * @param y the y coordinate of the mouse relative to the
     *          widget itself.
     * @param button the button clicked
     * @param count the number of clicks
     */
    virtual void mouseClickMessage(int x, int y, int button, int count){}
    
    /**
     * This function is called on a mouse wheel up when the mouse
     * is in the widget area or if the widget has focus.
     * 
     * @param x the x coordinate of the mouse relative to the
     *          widget itself.
     * @param y the y coordinate of the mouse relative to the
     *          widget itself.
     */
    virtual void mouseWheelUpMessage(int x, int y){}

    /**
     * This function is called on a mouse wheel down when the
     * mouse is in the widget area or if the widget has focus.
     * 
     * @param x the x coordinate of the mouse relative to the
     *          widget itself.
     * @param y the y coordinate of the mouse relative to the
     *          widget itself.
     */
    virtual void mouseWheelDownMessage(int x, int y){}

    /**
     * This function is called when the mouse moves and the
     * mouse is in the widget area or if the widget has focus.
     * 
     * @param x the x coordinate of the mouse relative to the
     *          widget itself.
     * @param y the y coordinate of the mouse relative to the
     *          widget itself.
     */
    virtual void mouseMotionMessage(int x, int y){}

    /**
     * This function sets the mouseType to be used as a mouse
     * pointer when the mouse is over this widget.
     *
     * @param mouseType the mouseType to be used.
     * @see MouseType
     */
    virtual void setMouseType(const std::string& mouseType);

    /**
     * This function returns the mouseType currently being used
     * as the mouse pointer when the mouse is over this widget.
     *
     * @return the mouseType currently used. 
     * @see MouseType
     */
    virtual const std::string& getMouseType() const;
        
    /*
     * Musmedelanden propageras till barnobjekt om de inte är
     * i fokus. Detta för att undkiva dubblarade medelanden då
     * medelanden alltid skickas till fokuserat object.
     * Föräldraobjektet har ansvar för att skicka _mouseIn/Out
     * till barnen.
     */
    
    /**
     * This function is used internally be the gui to handle all
     * mouse messages. Don't call or overload it unless you know
     * what you are doing.
     *
     * @param mouseInput
     * @see MouseInput
     */
    virtual void _mouseInputMessage(const MouseInput& mouseInput);

    /*
     * Skickas bara till fokuserat objekt.
     */
    /**
     * This function is used internally be the gui to handle all
     * key messages. Don't call or overload it unless you know
     * what you are doing.
     *
     * @param keyInput
     * @see KeyInput
     */
    virtual void _keyInputMessage(const KeyInput& keyInput);

    /**
     * This function is used internally be the gui to handle
     * mouse in messages. Don't call this function unless you
     * know what you are doing.
     */
    void _mouseInMessage();

    /**
     * This function is used internally be the gui to handle
     * mouse out messages. Don't call this function unless you
     * know what you are doing.
     */
    void _mouseOutMessage();
    
    /**
     * This function requests focus for the widget. If the widget
     * is focusable it will get focus. Otherwise, this function
     * does nothing.
     */
    void requestFocus();

    /**
     * This function sets the focus handler to be used by this
     * widget and should not be called or overloaded unless you
     * know what you are doing.
     *
     * @param focusHandler a pointer to a focus handler.
     * @see FocusHandler
     */
    virtual void _setFocusHandler(FocusHandler* focusHandler);

    /**
     * This function sets the action listener to be used by this
     * widget. When an action is triggered, the widget calls its
     * action listener. An action can be just about anything,
     * for example a click on a button or enter pressed in an
     * edit box.
     *
     * @param actionListener a pointer to an action listener.
     * @see ActionListener
     */
    void setActionListener(ActionListener* actionListener);

    /**
     * This function sets the mouse listener to be used by this
     * widget. When a mouse message is recieved its not just
     * sent to the widget but also the widgets mouse listener.
     *
     * @param mouseListener a pointer to a mouse listener.
     * @see MouseListener
     */
    void setMouseListener(MouseListener* mouseListener);
    
    /**
     * This function sets the key listener to be used by this
     * widget. When a key message is recieved its not just
     * sent to the widget but also the widgets key listener.
     *
     * @param keyListener a pointer to a key listener.
     * @see KeyListener
     */
    void setKeyListener(KeyListener* keyListener);
    
    /**
     * This function sets the widgets event identifier. An event
     * identifier is used with action events. If this widget
     * sends an action event, the widgets event identifier is
     * sent along with the action so that the action listener can
     * determine which widget sent the action event.
     *
     * NOTE: An event identifier should not be used to identify
     * a certain widget but rather a certain event in your
     * application. Several widgets can have the same event
     * identifer.
     * 
     * @param eventId the event identifier
     * @see ActionListener, getEventId, sendActionEvent
    */
    void setEventId(const std::string& eventId);

    /**
     * This function returns the widgets event identifier. An
     * event identifier is used with action events. If this
     * widget sends an action event, the widgets event identifier
     * is sent along with the action so that the action listener
     * can determine which widget sent the action event.
     *
     * @return the widgets event identifier
     */
    virtual const std::string& getEventId() const;

    /**
     * This function sends an action event to the widgets action
     * listener. An action can be just about anything, for
     * example a click on a button or enter pressed in an edit
     * box.
     *
     * @see setEventId, getEventId, ActionListener
     */
    void sendActionEvent();

    /**
     *
     * @todo Make a class point?
     */
    void getAbsolutePosition(int& x, int& y);
    
  protected:
    /**
     * This function sets the widgets parent. It should not be
     * called unless you know what you are doing.
     *
     * @param widget the parent widget.
     */
    void _setParent(Widget* parent);

    Rectangle mDimension;
    Widget* mParent;
    Color mForegroundColor;
    Color mBackgroundColor;
    std::string mMouseType;
    FocusHandler* mFocusHandler;
    
  private:
    MouseListener* mMouseListener;
    KeyListener* mKeyListener;
    ActionListener* mActionListener;
    std::string mEventId;
    int mClickTimeStamp;
    int mClickCount;
    int mClickButton;
    bool mHasMouse;
    bool mFocusable;
    bool mVisible;
    
  }; // end Widget
  
} // end gcn

#endif // end GCN_WIDGET_HPP

/*
 * yakslem  - "I have a really great idea! Why not have an
 *             interesting and funny quote or story at the
 *             end of every source file."
 * finalman - "Yeah - good idea! .... do you know any funny
 *             quotes?"
 * yakslem  - "Well.. I guess not. I just thought it would be
 *             fun to tell funny quotes."
 * finalman - "That's not a very funny quote. But i believe
 *             pointless quotes are funny in their own pointless
 *             way."
 */
