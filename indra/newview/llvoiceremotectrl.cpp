/* @file llvoiceremotectrl.cpp
 * @brief A remote control for voice chat
 *
 * $LicenseInfo:firstyear=2005&license=viewergpl$
 * 
 * Copyright (c) 2005-2010, Linden Research, Inc.
 * 
 * Second Life Viewer Source Code
 * The source code in this file ("Source Code") is provided by Linden Lab
 * to you under the terms of the GNU General Public License, version 2.0
 * ("GPL"), unless you have obtained a separate licensing agreement
 * ("Other License"), formally executed by you and Linden Lab.  Terms of
 * the GPL can be found in doc/GPL-license.txt in this distribution, or
 * online at http://secondlife.com/developers/opensource/gplv2
 * 
 * There are special exceptions to the terms and conditions of the GPL as
 * it is applied to this Source Code. View the full text of the exception
 * in the file doc/FLOSS-exception.txt in this software distribution, or
 * online at
 * http://secondlife.com/developers/opensource/flossexception
 * 
 * By copying, modifying or distributing this software, you acknowledge
 * that you have read and understood your obligations described above,
 * and agree to abide by those obligations.
 * 
 * ALL LINDEN LAB SOURCE CODE IS PROVIDED "AS IS." LINDEN LAB MAKES NO
 * WARRANTIES, EXPRESS, IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY,
 * COMPLETENESS OR PERFORMANCE.
 * $/LicenseInfo$
 * 
 */

#include "llviewerprecompiledheaders.h"

#include "llvoiceremotectrl.h"

#include "llagent.h"
#include "llui.h"
#include "llbutton.h"
#include "lluictrlfactory.h"
#include "llviewercontrol.h"
#include "llvoicechannel.h"
#include "llvoiceclient.h"
#include "llfloaterchatterbox.h"
#include "llfloaterreg.h"
#include "lliconctrl.h"
#include "lloverlaybar.h"
#include "lltextbox.h"

LLVoiceRemoteCtrl::LLVoiceRemoteCtrl ()
  : LLPanel()
{
	LLUICtrlFactory::getInstance()->buildPanel(this, "panel_voice_remote.xml");

	setIsChrome(TRUE);
	setFocusRoot(TRUE);
	mCommitCallbackRegistrar.add("VoiceRemote.Lock",     boost::bind(&LLVoiceRemoteCtrl::onBtnLock, this));	
	mCommitCallbackRegistrar.add("VoiceRemote.Popup",    boost::bind(&LLVoiceRemoteCtrl::onClickPopupBtn, this));	
	
}

LLVoiceRemoteCtrl::~LLVoiceRemoteCtrl()
{
}

void LLVoiceRemoteCtrl::expandOrCollapse()
{
	deleteAllChildren();

	if (gSavedSettings.getBOOL("ShowVoiceChannelPopup"))
	{
		LLUICtrlFactory::getInstance()->buildPanel(this, "panel_voice_remote_expanded.xml");
	}
	else
	{
		LLUICtrlFactory::getInstance()->buildPanel(this, "panel_voice_remote.xml");
	}
}

BOOL LLVoiceRemoteCtrl::postBuild()
{
	mTalkBtn = getChild<LLButton>("push_to_talk");
	mTalkBtn->setClickedCallback(onBtnTalkClicked, this);
	mTalkBtn->setHeldDownCallback(onBtnTalkHeld, this);
	mTalkBtn->setMouseUpCallback(onBtnTalkReleased, this);

	mTalkLockBtn = getChild<LLButton>("ptt_lock");

	mSpeakersBtn = getChild<LLButton>("speakers_btn");
		
	//childSetAction("end_call_btn", onClickEndCall, this);  not in use

	return TRUE;
}

void LLVoiceRemoteCtrl::draw()
{
	BOOL voice_active = FALSE;
	LLVoiceChannel* channelp = LLVoiceChannel::getCurrentVoiceChannel();
	if (channelp)
	{
		voice_active = channelp->isActive();
	}

	mTalkBtn->setEnabled(voice_active);
	mTalkLockBtn->setEnabled(voice_active);

	// propagate ptt state to button display,
	if (!mTalkBtn->hasMouseCapture())
	{
		// not in push to talk mode, or push to talk is active means I'm talking
		mTalkBtn->setToggleState(!gSavedSettings.getBOOL("PTTCurrentlyEnabled") || gVoiceClient->getUserPTTState());
	}
	mSpeakersBtn->setToggleState(LLFloaterReg::instanceVisible("active_speakers",LLSD()));
	mTalkLockBtn->setToggleState(!gSavedSettings.getBOOL("PTTCurrentlyEnabled"));

	std::string talk_blip_image;
	if (gVoiceClient->getIsSpeaking(gAgent.getID()))
	{
		F32 voice_power = gVoiceClient->getCurrentPower(gAgent.getID());

		if (voice_power > LLVoiceClient::OVERDRIVEN_POWER_LEVEL)
		{
			talk_blip_image = "icn_voice_ptt-on-lvl3.tga";
		}
		else
		{
			F32 power = gVoiceClient->getCurrentPower(gAgent.getID());
			S32 icon_image_idx = llmin(2, llfloor((power / LLVoiceClient::OVERDRIVEN_POWER_LEVEL) * 3.f));

			switch(icon_image_idx)
			{
			case 0:
				talk_blip_image = "icn_voice_ptt-on.tga";
				break;
			case 1:
				talk_blip_image = "icn_voice_ptt-on-lvl1.tga";
				break;
			case 2:
				talk_blip_image = "icn_voice_ptt-on-lvl2.tga";
				break;
			}
		}
	}
	else
	{
		talk_blip_image = "icn_voice_ptt-off.tga";
	}

	LLIconCtrl* icon = getChild<LLIconCtrl>("voice_volume");
	if (icon)
	{
		icon->setValue(talk_blip_image);
	}

	LLFloater* voice_floater = LLFloaterChatterBox::getCurrentVoiceFloater();
	std::string active_channel_name;
	if (voice_floater)
	{
		active_channel_name = voice_floater->getShortTitle();
	}

	LLVoiceChannel* current_channel = LLVoiceChannel::getCurrentVoiceChannel();
	//childSetEnabled("end_call_btn", LLVoiceClient::voiceEnabled() 
	//							&& current_channel
	//							&& current_channel->isActive()
	//							&& current_channel != LLVoiceChannelProximal::getInstance());

	childSetValue("channel_label", active_channel_name);
	childSetToolTip("voice_channel_bg", active_channel_name);

	if (current_channel)
	{
		if (voice_floater)
		{
			childSetValue("voice_channel_icon", voice_floater->getString("voice_icon"));
		}

		LLButton* voice_channel_bg = getChild<LLButton>("voice_channel_bg", FALSE);
		LLColor4 bg_color;
		if (current_channel->isActive())
		{
			bg_color = lerp(LLColor4::green, LLColor4::white, 0.7f);
		}
		else if (current_channel->getState() == LLVoiceChannel::STATE_ERROR)
		{
			bg_color = lerp(LLColor4::red, LLColor4::white, 0.7f);
		}
		else // active, but not connected
		{
			bg_color = lerp(LLColor4::yellow, LLColor4::white, 0.7f);
		}
		voice_channel_bg->setImageColor(bg_color);
	}

	LLButton* expand_button = getChild<LLButton>("show_channel");
	if (expand_button)
	{
		if (expand_button->getToggleState())
		{
			expand_button->setImageOverlay(std::string("arrow_down.tga"));
		}
		else
		{
			expand_button->setImageOverlay(std::string("arrow_up.tga"));
		}
	}

	LLPanel::draw();
}

void LLVoiceRemoteCtrl::onBtnTalkClicked(void *user_data)
{
	// when in toggle mode, clicking talk button turns mic on/off
	if (gSavedSettings.getBOOL("PushToTalkToggle"))
	{
		gVoiceClient->toggleUserPTTState();
	}
}

void LLVoiceRemoteCtrl::onBtnTalkHeld(void *user_data)
{
	// when not in toggle mode, holding down talk button turns on mic
	if (!gSavedSettings.getBOOL("PushToTalkToggle"))
	{
		gVoiceClient->setUserPTTState(true);
	}
}

void LLVoiceRemoteCtrl::onBtnTalkReleased(void* user_data)
{
	// when not in toggle mode, releasing talk button turns off mic
	if (!gSavedSettings.getBOOL("PushToTalkToggle"))
	{
		gVoiceClient->setUserPTTState(false);
	}
}

void LLVoiceRemoteCtrl::onBtnLock()
{
	gSavedSettings.setBOOL("PTTCurrentlyEnabled",mTalkLockBtn->getToggleState());
}

void LLVoiceRemoteCtrl::onClickPopupBtn()
{
    expandOrCollapse();
	gOverlayBar->layoutButtons();
}

//static
void LLVoiceRemoteCtrl::onClickEndCall(void* user_data)
{
	LLVoiceChannel* current_channel = LLVoiceChannel::getCurrentVoiceChannel();

	if (current_channel && current_channel != LLVoiceChannelProximal::getInstance())
	{
		current_channel->deactivate();
	}
}

