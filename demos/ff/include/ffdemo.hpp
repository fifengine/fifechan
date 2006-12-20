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

#ifndef __FFDEMO_HPP
#define __FFDEMO_HPP

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <guichan.hpp>
#include <guichan/sdl.hpp>

#include "ffcharacterchooser.hpp"
#include "ffcontainer.hpp"
#include "fflistbox.hpp"
#include "ffscrollarea.hpp"
#include "stringlistmodel.hpp"

class FFDemo : public gcn::ActionListener, public gcn::KeyListener
{
public:
	FFDemo();
	~FFDemo();
	void run();
	void action(const gcn::ActionEvent& actionEvent);
	void keyPressed(gcn::KeyEvent& keyEvent);

private:
	void input();
	void initMain();
	void cleanMain();
	void initStatus();
	void cleanStatus();
	void initMagicSkills();
	void cleanMagicSkills();
	void initItems();
	void cleanItems();
	void initAbout();
	void cleanAbout();

	bool mRunning;

	SDL_Surface* mScreen;
	SDL_Event mEvent;
	Mix_Chunk* mChooseSound;
	Mix_Chunk* mEscapeSound;

	gcn::SDLGraphics* mSDLGraphics;
	gcn::SDLInput* mSDLInput;
	gcn::SDLImageLoader* mSDLImageLoader;
	gcn::Gui* mGui;

	gcn::Container* mTop;
	FFContainer* mMain;
	FFContainer* mStatus;
	FFContainer* mItems;
	FFContainer* mMagicSkills;
	FFContainer* mTime;
	FFContainer* mGoldFootsteps;
	FFContainer* mMenu;
	FFContainer* mAbout;
	FFContainer* mItemsInfo;

	gcn::Icon* mPerIcon;
	gcn::Icon* mOlofIcon;
	gcn::Icon* mTomasIcon;
	gcn::Image* mPerImage;
	gcn::Image* mOlofImage;
	gcn::Image* mTomasImage;
	gcn::Image* mSplashImage;
	gcn::Font* mFontWhite;
	gcn::Font* mFontCyan;

	FFListBox *mMenuList;

	FFListBox *mMagicSkillsList;
	FFScrollArea *mMagicSkillsScroll;

	StringListModel *mPerSkills;
	StringListModel *mPerMagic;
	StringListModel *mOlofSkills;
	StringListModel *mOlofMagic;
	StringListModel *mTomasSkills;
	StringListModel *mTomasMagic;

	gcn::TextBox* mPerInfo1;
	gcn::TextBox* mOlofInfo1;
	gcn::TextBox* mTomasInfo1;
	gcn::TextBox* mPerInfo2;
	gcn::TextBox* mOlofInfo2;
	gcn::TextBox* mTomasInfo2;
	gcn::TextBox* mItemsInfoInfo;
	gcn::TextBox* mOlofStatus1;
	gcn::TextBox* mOlofStatus2;
	gcn::TextBox* mPerStatus1;
	gcn::TextBox* mPerStatus2;
	gcn::TextBox* mTomasStatus1;
	gcn::TextBox* mTomasStatus2;

	gcn::TextBox* mGoldFootstepsInfo1;
	gcn::TextBox* mGoldFootstepsInfo2;
	gcn::Label* mTimeLabel1;
	gcn::Label* mTimeLabel2;

	gcn::Label* mNavigationLabel;

	gcn::TextBox* mAboutInfo;
	FFScrollArea *mAboutScrollArea;

	FFListBox *mItemsList;
	FFScrollArea *mItemsScrollArea;
	StringListModel *mItemsListModel;
	StringListModel *mItemsInfoListModel;
	StringListModel *mMenuListModel;

	FFCharacterChooser* mCharacterChooser;
};

#endif
