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

/*
 * For comments regarding functions please see the header file. 
 */

#include "guichan/opengl/openglgraphics.hpp"
#include "guichan/exception.hpp"
#include <GL/gl.h>
#include <string>

namespace gcn
{
  OpenGLGraphics::OpenGLGraphics()
  {
    setTargetPlane(640, 480);

  } // end OpenGLGraphics
  
  OpenGLGraphics::OpenGLGraphics(int width, int height)
  {
    setTargetPlane(width, height);

  } // end OpenGLGrapics
  
  OpenGLGraphics::~OpenGLGraphics()
  {

  } // end ~OpenGLGraphics

  void OpenGLGraphics::_beginDraw()
  {
    glPushAttrib(
      GL_COLOR_BUFFER_BIT |
      GL_CURRENT_BIT |
      GL_DEPTH_BUFFER_BIT |
      GL_ENABLE_BIT |
      GL_FOG_BIT |
      GL_LIGHTING_BIT |
      GL_LINE_BIT |
      GL_POINT_BIT |
      GL_POLYGON_BIT |
      GL_SCISSOR_BIT |
      GL_STENCIL_BUFFER_BIT	|
      GL_TEXTURE_BIT |
      GL_TRANSFORM_BIT
      );

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    
    glOrtho(0.0, (double)mWidth, (double)mHeight, 0.0, -1.0, 1.0);

    glEnable(GL_SCISSOR_TEST);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    pushClipArea(Rectangle(0, 0, mWidth, mHeight));

  } // end _beginDraw
  
  void OpenGLGraphics::_endDraw()
  {
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    glPopAttrib();
    
    popClipArea();

  } // _endDraw

  bool OpenGLGraphics::pushClipArea(Rectangle area)
  {
    bool result = Graphics::pushClipArea(area);
    
    glScissor(mClipStack.top().x,
              mHeight - mClipStack.top().y - mClipStack.top().height,
              mClipStack.top().width,
              mClipStack.top().height);
    
    return result;

  } // end pushClipArea

  void OpenGLGraphics::popClipArea()
  {
    Graphics::popClipArea();

    if (mClipStack.empty())
    {
      return;
    }
    
    glScissor(mClipStack.top().x,
              mHeight - mClipStack.top().y - mClipStack.top().height,
              mClipStack.top().width,
              mClipStack.top().height);

  } // end popClipArea
  
  void OpenGLGraphics::setTargetPlane(int width, int height)
  {
    mWidth = width;
    mHeight = height;

  } // end setTargetPlane
  
  void OpenGLGraphics::drawImage(const Image* image, int srcX, int srcY,
                                         int dstX, int dstY, int width,
                                         int height)
  {
    dstX += mClipStack.top().xOffset;
    dstY += mClipStack.top().yOffset;
    
    // The following code finds the real width and height of the texture.
    // OpenGL only supports texture sizes that are powers of two
    int realImageWidth = 1;
    int realImageHeight = 1;
    while (realImageWidth < image->getWidth())
    {
      realImageWidth *= 2;
    }
    while (realImageHeight < image->getHeight())
    {
      realImageHeight *= 2;
    }
    
    // Find OpenGL texture coordinates
    float texX1 = srcX / (float)realImageWidth;
    float texY1 = srcY / (float)realImageHeight;
    float texX2 = (srcX+width) / (float)realImageWidth;
    float texY2 = (srcY+height) / (float)realImageHeight;
    
    // Please dont look too closely at the next line, it is not pretty.
    // It uses the image data as a pointer to a GLuint
    glBindTexture(GL_TEXTURE_2D, *((GLuint *)(image->_getData())));

    glEnable(GL_TEXTURE_2D);
    
    glColor4f(1.0f, 1.0f, 1.0f, 0.5f);

    // Draw a textured quad -- the image
    glBegin(GL_QUADS);
    glTexCoord2f(texX1, texY1);
    glVertex3i(dstX, dstY, 0);

    glTexCoord2f(texX2, texY1);
    glVertex3i(dstX + width, dstY, 0);

    glTexCoord2f(texX2, texY2);
    glVertex3i(dstX + width, dstY + height, 0);

    glTexCoord2f(texX1, texY2);
    glVertex3i(dstX, dstY + height, 0);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);      

  } // drawImage
  
  void OpenGLGraphics::drawPoint(int x, int y)
  {
    x += mClipStack.top().xOffset;
    y += mClipStack.top().yOffset;    

    glBegin(GL_POINTS);
    glVertex3i(x, y, 0);
    glEnd();

  } // end drawPoint
  
  void OpenGLGraphics::drawLine(int x1, int y1, int x2, int y2)
  {
    x1 += mClipStack.top().xOffset;
    y1 += mClipStack.top().yOffset;
    x2 += mClipStack.top().xOffset;
    y2 += mClipStack.top().yOffset;
    
    glBegin(GL_LINES);
    glVertex3i(x1, y1, 0);
    glVertex3i(x2, y2, 0);
    glEnd();

  } // end drawLine
  
  void OpenGLGraphics::drawRectangle(const Rectangle& rectangle)
  {    
    glBegin(GL_LINE_LOOP);
    glVertex3i(rectangle.x + mClipStack.top().xOffset, rectangle.y + mClipStack.top().yOffset, 0);
    glVertex3i(rectangle.x + rectangle.width + mClipStack.top().xOffset, rectangle.y + mClipStack.top().yOffset, 0);
    glVertex3i(rectangle.x + rectangle.width + mClipStack.top().xOffset, rectangle.y + rectangle.height + mClipStack.top().yOffset, 0);
    glVertex3i(rectangle.x + mClipStack.top().xOffset, rectangle.y + rectangle.height + mClipStack.top().yOffset, 0);
    glEnd();

  } // end drawRectangle
  
  void OpenGLGraphics::fillRectangle(const Rectangle& rectangle)
  {
    glBegin(GL_QUADS);
    glVertex3i(rectangle.x + mClipStack.top().xOffset, rectangle.y + mClipStack.top().yOffset, 0);
    glVertex3i(rectangle.x + rectangle.width + mClipStack.top().xOffset, rectangle.y + mClipStack.top().yOffset, 0);
    glVertex3i(rectangle.x + rectangle.width + mClipStack.top().xOffset, rectangle.y + rectangle.height + mClipStack.top().yOffset, 0);
    glVertex3i(rectangle.x + mClipStack.top().xOffset, rectangle.y + rectangle.height + mClipStack.top().yOffset, 0);
    glEnd();

  } // end fillRectangle
  
  void OpenGLGraphics::setColor(const Color& color)
  {
    glColor4f(color.r/255.0,
              color.g/255.0,
              color.b/255.0,
              color.a/255.0);

  } // end setColor
  
//     void setHorizontalGradient(const Color& color1, const Color& color2){}
  
//     void setVerticalGradient(const Color& color1, const Color& color2){}  

} // end gcn
