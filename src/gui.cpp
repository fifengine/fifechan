/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004, 2005, 2006 Olof Naessén and Per Larsson
 *
 *                                                         Js_./
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
 * 3. Neither the name of Guichan nor the      :$we` _! + _/ .        j?
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

/*
 * For comments regarding functions please see the header file.
 */

#include "guichan/gui.hpp"

#include "guichan/basiccontainer.hpp"
#include "guichan/exception.hpp"
#include "guichan/focushandler.hpp"
#include "guichan/graphics.hpp"
#include "guichan/input.hpp"
#include "guichan/keyinput.hpp"
#include "guichan/keylistener.hpp"
#include "guichan/mouseinput.hpp"
#include "guichan/mouselistener.hpp"
#include "guichan/widget.hpp"

namespace gcn
{
    Gui::Gui()
            :mTop(NULL),
             mGraphics(NULL),
             mInput(NULL),
             mTabbing(true),
             mDraggedWidget(NULL),
             mLastWidgetWithMouse(NULL),
             mLastWidgetWithModalFocus(NULL),
             mLastWidgetWithModalMouseInputFocus(NULL),
             mIsShiftPressed(false),
             mIsMetaPressed(false),
             mIsControlPressed(false),
             mIsAltPressed(false),
             mMouseX(0),
             mMouseY(0),
             mMouseButton(0),
             mMousePressTimeStamp(0),
             mLastMousePressButton(0),
             mLastMousePressTimeStamp(0),
             mClickCount(0),
             mMouseEventSource(NULL),
             mMouseEventType(0),
             mKeyEventSource(NULL),
             mKeyEventIsNumericPad(false),
             mKeyEventType(0)
        
    {
        mFocusHandler = new FocusHandler();
    }

    Gui::~Gui()
    {
        if (Widget::widgetExists(mTop))
        {
            setTop(NULL);
        }

        delete mFocusHandler;
    }

    void Gui::setTop(Widget* top)
    {
        if (mTop != NULL)
        {
            mTop->_setFocusHandler(NULL);
        }
        if (top != NULL)
        {
            top->_setFocusHandler(mFocusHandler);
        }

        mTop = top;
    }

    Widget* Gui::getTop() const
    {
        return mTop;
    }

    void Gui::setGraphics(Graphics* graphics)
    {
        mGraphics = graphics;
    }

    Graphics* Gui::getGraphics() const
    {
        return mGraphics;
    }

    void Gui::setInput(Input* input)
    {
        mInput = input;
    }

    Input* Gui::getInput() const
    {
        return mInput;
    }

    void Gui::logic()
    {
        if (mTop == NULL)
        {
            throw GCN_EXCEPTION("No top widget set");
        }

        mFocusHandler->applyChanges();

        // Release of modal focus or modal mouse input focus might make it
        // necessary to distribute mouse events.
        handleModalFocusRelease();
        handleModalMouseInputFocusRelease();
        
        if (mInput != NULL)
        {
            mInput->_pollInput();

            handleKeyInput();
            handleMouseInput();

            // Apply changes even if no input has been processed.
            // A widget might has asked for focus.
            mFocusHandler->applyChanges();

        } // end if

        mTop->logic();
    }

    void Gui::draw()
    {
        if (mTop == NULL)
        {
            throw GCN_EXCEPTION("No top widget set");
        }
        if (mGraphics == NULL)
        {
            throw GCN_EXCEPTION("No graphics set");
        }

        if (!mTop->isVisible())
        {
            return;
        }

        mGraphics->_beginDraw();

        // If top has a border,
        // draw it before drawing top
        if (mTop->getBorderSize() > 0)
        {
            Rectangle rec = mTop->getDimension();
            rec.x -= mTop->getBorderSize();
            rec.y -= mTop->getBorderSize();
            rec.width += 2 * mTop->getBorderSize();
            rec.height += 2 * mTop->getBorderSize();
            mGraphics->pushClipArea(rec);
            mTop->drawBorder(mGraphics);
            mGraphics->popClipArea();
        }

        mGraphics->pushClipArea(mTop->getDimension());
        mTop->draw(mGraphics);
        mGraphics->popClipArea();


        mGraphics->_endDraw();
    }

    void Gui::focusNone()
    {
        mFocusHandler->focusNone();
    }

    void Gui::setTabbingEnabled(bool tabbing)
    {
        mTabbing = tabbing;
    }

    bool Gui::isTabbingEnabled()
    {
        return mTabbing;
    }

    void Gui::addGlobalKeyListener(KeyListener* keyListener)
    {
        mKeyListeners.push_back(keyListener);
    }

    void Gui::removeGlobalKeyListener(KeyListener* keyListener)
    {
        mKeyListeners.remove(keyListener);
    }

    void Gui::handleMouseInput()
    {
        while (!mInput->isMouseQueueEmpty())
         {
             MouseInput mouseInput = mInput->dequeueMouseInput();

             // Save the current mouse state. It will be needed if modal focus
             // changes, modal mouse input focus changes or a widget is deleted.
             mMouseX = mouseInput.getX();
             mMouseY = mouseInput.getY();
             mMouseButton = mouseInput.getButton();
             mMousePressTimeStamp = mouseInput.getTimeStamp();
             
             if (mMouseX < 0
                 || mMouseY < 0
                 || !mTop->getDimension().isPointInRect(mMouseX, mMouseY))
             {
                 if (mLastWidgetWithMouse != NULL)
                 {
                     mMouseEventSource = mLastWidgetWithMouse;
                     mMouseEventType = MouseEvent::MOUSE_EXITED;
                     distributeMouseEvent();
                     mLastWidgetWithMouse = NULL;
                 }                 
                 
                 continue;
             }

             Widget* widget = getMouseEventSource();
            
             switch (mouseInput.getType())
             {
               case MouseInput::MOUSE_PRESSED:
                   handleMousePressed(widget);
                   break;
               case MouseInput::MOUSE_RELEASED:
                   handleMouseReleased(widget);
                   break;                   
               case MouseInput::MOUSE_MOVED:
                   handleMouseMoved(widget);
                   break;
               case MouseInput::MOUSE_WHEEL_MOVED_DOWN:
                   handleMouseWheelMovedDown(widget);
                   break;
               case MouseInput::MOUSE_WHEEL_MOVED_UP:
                   handleMouseWheelMovedUp(widget);
                   break;
               default:                   
                   throw GCN_EXCEPTION("Unknown mouse input type.");
                   break;
             }                  
         }                   
    }

    void Gui::handleKeyInput()
    {
        while (!mInput->isKeyQueueEmpty())
        {
            KeyInput keyInput = mInput->dequeueKeyInput();

            mIsShiftPressed = keyInput.isShiftPressed();
            mIsMetaPressed = keyInput.isMetaPressed();
            mIsControlPressed = keyInput.isControlPressed();
            mIsAltPressed = keyInput.isAltPressed();

            mKeyEventSource = NULL;
            mKeyEventType = keyInput.getType();
            mKeyEventIsNumericPad = keyInput.isNumericPad();
            mKeyEventKey = keyInput.getKey();
            distributeKeyEventToGlobalKeyListeners();
            
            if (mTabbing
                && keyInput.getKey().getValue() == Key::TAB
                && keyInput.getType() == KeyInput::KEY_PRESSED)
            {
                if (keyInput.isShiftPressed())
                {
                    mFocusHandler->tabPrevious();
                }
                else
                {
                    mFocusHandler->tabNext();
                }
            }
            else
            {
                // Send key inputs to the focused widgets
                if (mFocusHandler->getFocused() != NULL)
                {
                    if (mFocusHandler->getFocused()->isFocusable())
                    {                        
                        mKeyEventSource = getKeyEventSource();
                        mKeyEventType = keyInput.getType();
                        mKeyEventIsNumericPad = keyInput.isNumericPad();
                        mKeyEventKey = keyInput.getKey();                        
                        distributeKeyEvent();
                    }
                    else
                    {
                        mFocusHandler->focusNone();
                    }
                }
            }
            
            mFocusHandler->applyChanges();
            
        } // end while        
    }
    
    void Gui::handleMouseMoved(Widget* widget)
    {        
        if (widget != mLastWidgetWithMouse)
        {
            if (mLastWidgetWithMouse != NULL)
            {
                mMouseEventSource = mLastWidgetWithMouse;
                mMouseEventType = MouseEvent::MOUSE_EXITED;
                distributeMouseEvent();
                mClickCount = 0;
                mLastMousePressTimeStamp = 0;
            }
            
            mMouseEventSource = widget;
            mMouseEventType = MouseEvent::MOUSE_ENTERED;
            distributeMouseEvent();
            
            mLastWidgetWithMouse = widget;
        }

        if (mDraggedWidget != NULL)
        {
            mMouseEventSource = mDraggedWidget;
            mMouseEventType = MouseEvent::MOUSE_DRAGGED;
            distributeMouseEvent();
        }
        else
        {
            mMouseEventSource = widget;
            mMouseEventType = MouseEvent::MOUSE_MOVED;
            distributeMouseEvent();
        }
    }
    
    void Gui::handleMousePressed(Widget* widget)
    {
        if (mDraggedWidget != NULL)
        {
            widget = mDraggedWidget;
        }

        mMouseEventSource = widget;
        mMouseEventType = MouseEvent::MOUSE_PRESSED;
        distributeMouseEvent();
        
        if (mFocusHandler->getModalFocused() != NULL
            && widget->hasModalFocus()
            || mFocusHandler->getModalFocused() == NULL)
        {
            widget->requestFocus();
        }
        
        mDraggedWidget = widget;
        
        if (mLastMousePressTimeStamp < 300
            && mLastMousePressButton == mMouseButton)
        {
            mClickCount++;
        }
        else
        {
            mClickCount = 0;
        }
        
        mLastMousePressButton = mMouseButton;
        mLastMousePressTimeStamp = mMousePressTimeStamp;
    }

    void Gui::handleMouseWheelMovedDown(Widget* widget)
    {
        if (mDraggedWidget != NULL)
        {
            widget = mDraggedWidget;
        }

        mMouseEventSource = widget;
        mMouseEventType = MouseEvent::MOUSE_WHEEL_MOVED_DOWN;
        distributeMouseEvent();
    }

    void Gui::handleMouseWheelMovedUp(Widget* widget)
    {
        if (mDraggedWidget != NULL)
        {
            widget = mDraggedWidget;
        }

        mMouseEventSource = widget;
        mMouseEventType = MouseEvent::MOUSE_WHEEL_MOVED_UP;
        distributeMouseEvent();
    }

    void Gui::handleMouseReleased(Widget* widget)
    {
        if (mDraggedWidget != NULL)
        {
            widget = mDraggedWidget;
        }

        mMouseEventSource = widget;
        mMouseEventType = MouseEvent::MOUSE_RELEASED;
        distributeMouseEvent();
        
        if (mMouseButton == mLastMousePressButton)
        {
            mMouseEventSource = widget;
            mMouseEventType = MouseEvent::MOUSE_CLICKED;
            distributeMouseEvent();
        }
        else
        {
            mLastMousePressButton = 0;
            mClickCount = 0;
        }

        if (mDraggedWidget != NULL)
        {
            mDraggedWidget = NULL;
        }
    }
    
    void Gui::handleModalFocusRelease()
    {
        if (mLastWidgetWithModalFocus != mFocusHandler->getModalFocused())
        {
            Widget* widgetWithMouse = getWidgetWithMouse();
            
            if (widgetWithMouse != mLastWidgetWithModalFocus
                && mLastWidgetWithModalFocus != NULL)
            {
                mMouseEventSource = widgetWithMouse;
                mMouseEventType = MouseEvent::MOUSE_ENTERED;
                distributeMouseEvent();
            }
            
            mLastWidgetWithModalFocus = mFocusHandler->getModalFocused();
        }        
    }

    void Gui::handleModalMouseInputFocusRelease()
    {
        if (mLastWidgetWithModalMouseInputFocus != mFocusHandler->getModalMouseInputFocused())
        {
            Widget* widgetWithMouse = getWidgetWithMouse();
            
            if (widgetWithMouse != mLastWidgetWithModalMouseInputFocus
                && mLastWidgetWithModalMouseInputFocus != NULL)
            {
                mMouseEventSource = widgetWithMouse;
                mMouseEventType = MouseEvent::MOUSE_ENTERED;
                distributeMouseEvent();
            }
            
            mLastWidgetWithModalMouseInputFocus = mFocusHandler->getModalMouseInputFocused();
        }        
    }

    Widget* Gui::getWidgetWithMouse()
    {
        // If the widget's parent has no child then we have found the widget..
        Widget* parent = mTop;
        Widget* child = mTop;
        
        while (child != NULL)
        {
            Widget* swap = child;
            int x, y;
            parent->getAbsolutePosition(x, y);
            child = parent->getWidgetAt(mMouseX - x, mMouseY - y);
            parent = swap;
        }

        return parent;        
    }

    Widget* Gui::getMouseEventSource()
    {

        Widget* widget = getWidgetWithMouse();
        
        if (mFocusHandler->getModalMouseInputFocused() != NULL
            && !widget->hasModalMouseInputFocus())
        {
            return mFocusHandler->getModalMouseInputFocused();
        }

        return widget;
    }

    Widget* Gui::getKeyEventSource()
    {
        Widget* widget = mFocusHandler->getFocused();

        while (widget->_getInternalFocusHandler() != NULL
               && widget->_getInternalFocusHandler()->getFocused() != NULL)
        {
            widget = widget->_getInternalFocusHandler()->getFocused();            
        }

        return widget;                
    }
    
    void Gui::distributeMouseEvent()
    {
        Widget* parent = mMouseEventSource;
        Widget* widget = mMouseEventSource;
        
        if (mFocusHandler->getModalFocused() != NULL
            && !widget->hasModalFocus())
        {
            return;
        }
        
        if (mFocusHandler->getModalMouseInputFocused() != NULL
            && !widget->hasModalMouseInputFocus())
        {
            return;
        }
        
        while (parent != NULL)
        {
            bool isEventConsumed = false;
            
            parent = (Widget*)widget->getParent();

            if (widget->isEnabled())
            {
                std::list<MouseListener*> mouseListeners = widget->_getMouseListeners();
                
                // Send the event to all mouse listeners of the widget.
                for (std::list<MouseListener*>::iterator it = mouseListeners.begin();
                     it != mouseListeners.end();
                     ++it)
                {
                    int x, y;
                    widget->getAbsolutePosition(x, y);
                    MouseEvent mouseEvent(mMouseEventSource,
                                          mIsShiftPressed,
                                          mIsControlPressed,
                                          mIsAltPressed,
                                          mIsMetaPressed,
                                          mMouseEventType,
                                          mMouseButton,
                                          mMouseX - x,
                                          mMouseY - y,
                                          mClickCount);                 
                    
                    switch (mouseEvent.getType())
                    {
                      case MouseEvent::MOUSE_ENTERED:
                          (*it)->mouseEntered(mouseEvent);
                          break;
                      case MouseEvent::MOUSE_EXITED:
                          (*it)->mouseExited(mouseEvent);
                          break;
                      case MouseEvent::MOUSE_MOVED:
                          (*it)->mouseMoved(mouseEvent);
                          break;
                      case MouseEvent::MOUSE_PRESSED:
                          (*it)->mousePressed(mouseEvent);
                          break;
                      case MouseEvent::MOUSE_RELEASED:
                          (*it)->mouseReleased(mouseEvent);
                          break;
                      case MouseEvent::MOUSE_WHEEL_MOVED_UP:
                          (*it)->mouseWheelMovedUp(mouseEvent);
                          break;
                      case MouseEvent::MOUSE_WHEEL_MOVED_DOWN:
                          (*it)->mouseWheelMovedDown(mouseEvent);
                          break;
                      case MouseEvent::MOUSE_DRAGGED:
                          (*it)->mouseDragged(mouseEvent);
                          break;
                      case MouseEvent::MOUSE_CLICKED:
                          (*it)->mouseClicked(mouseEvent);
                          break;
                      default:
                          throw GCN_EXCEPTION("Unknown mouse event type.");
                    }
                    
                    if (mouseEvent.isConsumed())
                    {
                        isEventConsumed = true;
                        break;
                    }
                }
            }

            // Check if event has been consumed by one of the widget's
            // mouse listeners. If it has been consumed no further distribution
            // of the event to the widget's parents should take place.
            if (isEventConsumed)
            {
                    break;
            }                
            
            Widget* swap = widget;
            widget = parent;
            parent = (Widget*)swap->getParent();
            
            if (mFocusHandler->getModalFocused() != NULL
                && !widget->hasModalFocus())
            {
                break;
            }
            
            if (mFocusHandler->getModalMouseInputFocused() != NULL
                && !widget->hasModalMouseInputFocus())
            {
                break;
            }            
        } 
    }

    void Gui::distributeKeyEvent()
    {
        Widget* parent = mKeyEventSource;
        Widget* widget = mKeyEventSource;
                    
        if (mFocusHandler->getModalFocused() != NULL
            && !widget->hasModalFocus())
        {
            return;
        }
        
        while (parent != NULL)
        {
            bool isEventConsumed = false;            
            parent = (Widget*)widget->getParent();

            if (widget->isEnabled())
            {
                std::list<KeyListener*> keyListeners = widget->_getKeyListeners();
                
                KeyEvent keyEvent(widget,
                                  mIsShiftPressed,
                                  mIsControlPressed,
                                  mIsAltPressed,
                                  mIsMetaPressed,
                                  mKeyEventType,
                                  mKeyEventIsNumericPad,
                                  mKeyEventKey);

                // Send the event to all key listeners of the widget.
                for (std::list<KeyListener*>::iterator it = keyListeners.begin();
                     it != keyListeners.end();
                     ++it)
                {
                    switch (keyEvent.getType())
                    {
                      case KeyEvent::KEY_PRESSED:
                          (*it)->keyPressed(keyEvent);
                          break;
                      case KeyEvent::KEY_RELEASED:
                          (*it)->keyReleased(keyEvent);
                          break;
                      default:
                          throw GCN_EXCEPTION("Unknown key event type.");
                    }
                    
                    if (keyEvent.isConsumed())
                    {
                        isEventConsumed = true;
                        break;
                    }
                }
            }

            // Check if event has been consumed by one of the widget's
            // key listeners. If it has been consumed no further distribution
            // of the event to the widget's parents should take place.
            if (isEventConsumed)
            {
                    break;
            }                
            
            Widget* swap = widget;
            widget = parent;
            parent = (Widget*)swap->getParent();
            
            if (mFocusHandler->getModalFocused() != NULL
                && !widget->hasModalFocus())
            {
                break;
            }            
        } 
    }

    void Gui::distributeKeyEventToGlobalKeyListeners()
    {        
        KeyListenerListIterator it;
        
        for (it = mKeyListeners.begin(); it != mKeyListeners.end(); it++)
        {
                KeyEvent keyEvent(mKeyEventSource,
                                  mIsShiftPressed,
                                  mIsControlPressed,
                                  mIsAltPressed,
                                  mIsMetaPressed,
                                  mKeyEventType,
                                  mKeyEventIsNumericPad,
                                  mKeyEventKey);
                
            switch (keyEvent.getType())
            {                
              case KeyEvent::KEY_PRESSED:
                  (*it)->keyPressed(keyEvent);
                  break;
              case KeyEvent::KEY_RELEASED:
                  (*it)->keyReleased(keyEvent);
                  break;
              default:
                  throw GCN_EXCEPTION("Unknown key event type.");                  
            }
            
            if (keyEvent.isConsumed())
            {
                break;
            }            
        }               
    }
}
