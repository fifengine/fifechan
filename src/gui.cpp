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
    {
        mTop = NULL;
        mInput = NULL;
        mGraphics = NULL;
        mFocusHandler = new FocusHandler();
        mTabbing = true;
        mDraggedWidget = NULL;
        mLastWidgetWithMouse = NULL;
        mLastMousePressTimeStamp = 0;
        mLastMousePressButton = 0;
        mClickCount = 0;        
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
        if (mTop)
        {
            mTop->_setFocusHandler(NULL);
        }
        if (top)
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
        if (!mTop)
        {
            throw GCN_EXCEPTION("No top widget set");
        }

        mFocusHandler->applyChanges();

        if(mInput)
        {
            mInput->_pollInput();

            handleMouseInput();
            
            while (!mInput->isKeyQueueEmpty())
            {
                KeyInput ki = mInput->dequeueKeyInput();

                KeyListenerListIterator it;

                // Propagate key input to global KeyListeners
                switch(ki.getType())
                {
                  case KeyInput::PRESS:
                      for (it = mKeyListeners.begin(); it != mKeyListeners.end(); ++it)
                      {
                          (*it)->keyPress(ki.getKey());
                      }
                      break;

                  case KeyInput::RELEASE:
                      for (it = mKeyListeners.begin(); it != mKeyListeners.end(); ++it)
                      {
                          (*it)->keyRelease(ki.getKey());
                      }
                      break;
                }

                if (mTabbing
                    && ki.getKey().getValue() == Key::TAB
                    && ki.getType() == KeyInput::PRESS)
                {
                    if (ki.getKey().isShiftPressed())
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
                    if (mFocusHandler->getFocused())
                    {
                        if (mFocusHandler->getFocused()->isFocusable())
                        {
                            mFocusHandler->getFocused()->_keyInputMessage(ki);
                        }
                        else
                        {
                            mFocusHandler->focusNone();
                        }
                    }
                }

                mFocusHandler->applyChanges();

            } // end while

            // Apply changes even if no input has been processed.
            // A widget might has asked for focus.
            mFocusHandler->applyChanges();

        } // end if

        mTop->logic();
    }

    void Gui::draw()
    {
        if (!mTop)
        {
            throw GCN_EXCEPTION("No top widget set");
        }
        if (!mGraphics)
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
                        
             if (mouseInput.x < 0
                 || mouseInput.y < 0
                 || !mTop->getDimension().isPointInRect(mouseInput.x, mouseInput.y))
             {
                 if (mLastWidgetWithMouse != NULL)
                 {
                     int x, y;
                     mLastWidgetWithMouse->getAbsolutePosition(x, y);
                     MouseEvent mouseExitedEvent(mLastWidgetWithMouse,
                                                 MouseEvent::MOUSE_EXITED,
                                                 mIsShiftPressed,
                                                 mIsControlPressed,
                                                 mIsAltPressed,
                                                 mIsMetaPressed,
                                                 mouseInput.getButton(),
                                                 mouseInput.x - x,
                                                 mouseInput.y - y,
                                                 mClickCount);                 
                     distributeMouseEvent(mouseExitedEvent);
                     mLastWidgetWithMouse = NULL;
                 }                 
                 
                 continue;
             }

             Widget* widget = getMouseEventSource(mouseInput);
            
             switch (mouseInput.getType())
             {
               case MouseInput::PRESS:
                   handleMousePressed(widget, mouseInput);
                   break;
               case MouseInput::RELEASE:
                   handleMouseReleased(widget, mouseInput);
                   break;                   
               case MouseInput::MOTION:
                   handleMouseMoved(widget, mouseInput);
                   break;
               default:
                   throw GCN_EXCEPTION("Unknown mouse input type.");
                   break;
             }
                  
         }                   
    }

    void Gui::handleMouseMoved(Widget* widget, const MouseInput& mouseInput)
    {        
        if (widget != mLastWidgetWithMouse)
        {
            if (mLastWidgetWithMouse != NULL)
            {
                int x, y;
                mLastWidgetWithMouse->getAbsolutePosition(x, y);
                MouseEvent mouseExitedEvent(mLastWidgetWithMouse,
                                            MouseEvent::MOUSE_EXITED,
                                            mIsShiftPressed,
                                            mIsControlPressed,
                                            mIsAltPressed,
                                            mIsMetaPressed,
                                            mouseInput.getButton(),
                                            mouseInput.x - x,
                                            mouseInput.y - y,
                                            mClickCount);
                
                distributeMouseEvent(mouseExitedEvent);
                mClickCount = 0;
                mLastMousePressTimeStamp = 0;
            }
            
            int x, y;
            widget->getAbsolutePosition(x, y);
            MouseEvent mouseEnteredEvent(widget,
                                         MouseEvent::MOUSE_ENTERED,
                                         mIsShiftPressed,
                                         mIsControlPressed,
                                         mIsAltPressed,
                                         mIsMetaPressed,
                                         mouseInput.getButton(),
                                         mouseInput.x - x,
                                         mouseInput.y - y,
                                         mClickCount);
            distributeMouseEvent(mouseEnteredEvent);
            
            mLastWidgetWithMouse = widget;
        }

        if (mDraggedWidget != NULL)
        {
            int x, y;
            mDraggedWidget->getAbsolutePosition(x, y);
            MouseEvent mouseMovedEvent(mDraggedWidget,
                                       MouseEvent::MOUSE_DRAGGED,
                                       mIsShiftPressed,
                                       mIsControlPressed,
                                       mIsAltPressed,
                                       mIsMetaPressed,
                                       mouseInput.getButton(),
                                       mouseInput.x - x,
                                       mouseInput.y - y,
                                       mClickCount);
            distributeMouseEvent(mouseMovedEvent);            
        }
        else
        {
            int x, y;
            widget->getAbsolutePosition(x, y);            
            MouseEvent mouseMovedEvent(widget,
                                       MouseEvent::MOUSE_MOVED,
                                       mIsShiftPressed,
                                       mIsControlPressed,
                                       mIsAltPressed,
                                       mIsMetaPressed,
                                       mouseInput.getButton(),
                                       mouseInput.x - x,
                                       mouseInput.y - y,
                                       mClickCount);
            distributeMouseEvent(mouseMovedEvent);
        }
    }
    
    void Gui::handleMousePressed(Widget* widget, const MouseInput& mouseInput)
    {
        if (mDraggedWidget != NULL)
        {
            widget = mDraggedWidget;
        }

        int x, y;
        widget->getAbsolutePosition(x, y);        
        if (mouseInput.getButton() == MouseInput::WHEEL_DOWN)
        {
            MouseEvent mouseWheelMovedDownEvent(widget,
                                                MouseEvent::MOUSE_WHEEL_MOVED_DOWN,
                                                mIsShiftPressed,
                                                mIsControlPressed,
                                                mIsAltPressed,
                                                mIsMetaPressed,
                                                mouseInput.getButton(),
                                                mouseInput.x - x,
                                                mouseInput.y - y,
                                                mClickCount);
            distributeMouseEvent(mouseWheelMovedDownEvent);            
        }
        else if (mouseInput.getButton() == MouseInput::WHEEL_UP)
        {
            MouseEvent mouseWheelMovedUpEvent(widget,
                                              MouseEvent::MOUSE_WHEEL_MOVED_UP,
                                              mIsShiftPressed,
                                              mIsControlPressed,
                                              mIsAltPressed,
                                              mIsMetaPressed,
                                              mouseInput.getButton(),
                                              mouseInput.x - x,
                                              mouseInput.y - y,
                                              mClickCount);
            distributeMouseEvent(mouseWheelMovedUpEvent);
        }
        else
        {
            MouseEvent mousePressedEvent(widget,
                                         MouseEvent::MOUSE_PRESSED,
                                         mIsShiftPressed,
                                         mIsControlPressed,
                                         mIsAltPressed,
                                         mIsMetaPressed,
                                         mouseInput.getButton(),
                                         mouseInput.x - x,
                                         mouseInput.y - y,
                                         mClickCount);
            distributeMouseEvent(mousePressedEvent);

            if (mFocusHandler->getModalFocused() != NULL
                && widget->hasModalFocus()
                || mFocusHandler->getModalFocused() == NULL)
            {
                widget->requestFocus();
            }
            
            mDraggedWidget = widget;

            if (mLastMousePressTimeStamp < 300
                && mLastMousePressButton == mouseInput.getButton())
            {
                mClickCount++;
            }
            else
            {
                mClickCount = 0;
            }

            mLastMousePressButton = mouseInput.getButton();;
            mLastMousePressTimeStamp = mouseInput.getTimeStamp();
        }
    }
    
    void Gui::handleMouseReleased(Widget* widget, const MouseInput& mouseInput)
    {
        if (mDraggedWidget != NULL)
        {
            widget = mDraggedWidget;
        }

        int x, y;
        widget->getAbsolutePosition(x, y);            
        MouseEvent mouseReleasedEvent(widget,
                                      MouseEvent::MOUSE_RELEASED,
                                      mIsShiftPressed,
                                      mIsControlPressed,
                                      mIsAltPressed,
                                      mIsMetaPressed,
                                      mouseInput.getButton(),
                                      mouseInput.x - x,
                                      mouseInput.y - y,
                                      mClickCount);
        distributeMouseEvent(mouseReleasedEvent);
        
        if (mouseInput.getButton() == mLastMousePressButton)
        {
            MouseEvent mouseReleasedEvent(widget,
                                          MouseEvent::MOUSE_CLICKED,
                                          mIsShiftPressed,
                                          mIsControlPressed,
                                          mIsAltPressed,
                                          mIsMetaPressed,
                                          mouseInput.getButton(),
                                          mouseInput.x - x,
                                          mouseInput.y - y,
                                          mClickCount);
            distributeMouseEvent(mouseReleasedEvent);
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

    Widget* Gui::getMouseEventSource(const MouseInput& mouseInput)
    {
        // Find the widget the mouse input concerns. If the parent has
        // no child then we have found our widget.
        Widget* parent = mTop;
        Widget* child = mTop;
        
        while (child != NULL)
        {
            Widget* swap = child;
            int x, y;
            parent->getAbsolutePosition(x, y);
            child = parent->getWidgetAt(mouseInput.x - x, mouseInput.y - y);
            parent = swap;
        }

        if (mFocusHandler->getModalInputFocused() != NULL
            && !parent->hasModalInputFocus())
        {
            return mFocusHandler->getModalInputFocused();
        }
        
        return parent;        
    }

    void Gui::distributeMouseEvent(MouseEvent& mouseEvent)
    {
        Widget* parent = mouseEvent.getSource();
        Widget* widget = mouseEvent.getSource();
                    
        if (mFocusHandler->getModalFocused() != NULL &&
            !widget->hasModalFocus())
        {
            return;
        }
        
        if (mFocusHandler->getModalInputFocused() != NULL &&
            !widget->hasModalInputFocus())
        {
            return;
        }

        while (parent != NULL)
        {
            parent = (Widget*)widget->getParent();

            if (widget->isEnabled())
            {
                std::list<MouseListener*> mouseListeners = widget->_getMouseListeners();
                
                // Send the event to all mouse listeners of the widget.
                for (std::list<MouseListener*>::iterator it = mouseListeners.begin();
                     it != mouseListeners.end();
                     ++it)
                {
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
                        break;
                    }
                }
            }

            // Check if event has been consumed by one of the widget's
            // mouse listeners. If it has been consumed no further distribution
            // of the event to the widget's parents should take place.
            if (mouseEvent.isConsumed())
            {
                    break;
            }                
            
            Widget* swap = widget;
            widget = parent;
            parent = (Widget*)swap->getParent();
            
            if (mFocusHandler->getModalFocused() != NULL &&
                !widget->hasModalFocus())
            {
                break;
            }
            
            if (mFocusHandler->getModalInputFocused() != NULL &&
                !widget->hasModalInputFocus())
            {
                break;
            }            
        } 
    }
}
