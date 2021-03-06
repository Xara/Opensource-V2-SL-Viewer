/**
 * @file llnotificationofferhandler.cpp
 * @brief Notification Handler Class for Simple Notifications and Notification Tips
 *
 * $LicenseInfo:firstyear=2000&license=viewergpl$
 * 
 * Copyright (c) 2000-2010, Linden Research, Inc.
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


#include "llviewerprecompiledheaders.h" // must be first include

#include "llnotificationhandler.h"
#include "lltoastnotifypanel.h"
#include "llviewercontrol.h"
#include "llviewerwindow.h"
#include "llnotificationmanager.h"
#include "llnotifications.h"
#include "llscriptfloater.h"
#include "llimview.h"
#include "llnotificationsutil.h"

using namespace LLNotificationsUI;

//--------------------------------------------------------------------------
LLOfferHandler::LLOfferHandler(e_notification_type type, const LLSD& id)
{
	mType = type;

	// Getting a Channel for our notifications
	mChannel = LLChannelManager::getInstance()->createNotificationChannel();
	mChannel->setControlHovering(true);

	LLScreenChannel* channel = dynamic_cast<LLScreenChannel*>(mChannel);
	if(channel)
		channel->setOnRejectToastCallback(boost::bind(&LLOfferHandler::onRejectToast, this, _1));
}

//--------------------------------------------------------------------------
LLOfferHandler::~LLOfferHandler()
{
}

//--------------------------------------------------------------------------
void LLOfferHandler::initChannel()
{
	S32 channel_right_bound = gViewerWindow->getWorldViewRectScaled().mRight - gSavedSettings.getS32("NotificationChannelRightMargin");
	S32 channel_width = gSavedSettings.getS32("NotifyBoxWidth");
	mChannel->init(channel_right_bound - channel_width, channel_right_bound);
}

//--------------------------------------------------------------------------
bool LLOfferHandler::processNotification(const LLSD& notify)
{
	if(!mChannel)
	{
		return false;
	}

	LLNotificationPtr notification = LLNotifications::instance().find(notify["id"].asUUID());

	if(!notification)
		return false;

	// arrange a channel on a screen
	if(!mChannel->getVisible())
	{
		initChannel();
	}

	if(notify["sigtype"].asString() == "add" || notify["sigtype"].asString() == "change")
	{


		if( notification->getPayload().has("give_inventory_notification")
			&& !notification->getPayload()["give_inventory_notification"] )
		{
			// This is an original inventory offer, so add a script floater
			LLScriptFloaterManager::instance().onAddNotification(notification->getID());
		}
		else
		{
			notification->setReusable(LLHandlerUtil::isNotificationReusable(notification));

			LLUUID session_id;
			if (LLHandlerUtil::canSpawnIMSession(notification))
			{
				const std::string name = LLHandlerUtil::getSubstitutionName(notification);

				LLUUID from_id = notification->getPayload()["from_id"];

				session_id = LLHandlerUtil::spawnIMSession(name, from_id);
			}

			bool show_toast = true;
			bool add_notid_to_im = LLHandlerUtil::canAddNotifPanelToIM(notification);
			if (add_notid_to_im)
			{
				LLHandlerUtil::addNotifPanelToIM(notification);
				if (LLHandlerUtil::isIMFloaterOpened(notification))
				{
					show_toast = false;
				}
			}

			if (notification->getPayload().has("SUPPRESS_TOAST")
						&& notification->getPayload()["SUPPRESS_TOAST"])
			{
				LLNotificationsUtil::cancel(notification);
			}
			else if(show_toast)
			{
				LLToastNotifyPanel* notify_box = new LLToastNotifyPanel(notification);
				// don't close notification on panel destroy since it will be used by IM floater
				notify_box->setCloseNotificationOnDestroy(!add_notid_to_im);
				LLToast::Params p;
				p.notif_id = notification->getID();
				p.notification = notification;
				p.panel = notify_box;
				p.on_delete_toast = boost::bind(&LLOfferHandler::onDeleteToast, this, _1);
				// we not save offer notifications to the syswell floater that should be added to the IM floater
				p.can_be_stored = !add_notid_to_im;

				LLScreenChannel* channel = dynamic_cast<LLScreenChannel*>(mChannel);
				if(channel)
					channel->addToast(p);

				// if we not add notification to IM - add it to notification well
				if (!add_notid_to_im)
				{
					// send a signal to the counter manager
					mNewNotificationSignal();
				}
			}

			if (LLHandlerUtil::canLogToIM(notification))
			{
				// log only to file if notif panel can be embedded to IM and IM is opened
				if (add_notid_to_im && LLHandlerUtil::isIMFloaterOpened(notification))
				{
					LLHandlerUtil::logToIMP2P(notification, true);
				}
				else
				{
					LLHandlerUtil::logToIMP2P(notification);
				}
			}
		}
	}
	else if (notify["sigtype"].asString() == "delete")
	{
		if( notification->getPayload().has("give_inventory_notification")
			&& !notification->getPayload()["give_inventory_notification"] )
		{
			// Remove original inventory offer script floater
			LLScriptFloaterManager::instance().onRemoveNotification(notification->getID());
		}
		else
		{
			if (LLHandlerUtil::canAddNotifPanelToIM(notification)
					&& !LLHandlerUtil::isIMFloaterOpened(notification))
			{
				LLHandlerUtil::decIMMesageCounter(notification);
			}
			mChannel->killToastByNotificationID(notification->getID());
		}
	}

	return true;
}

//--------------------------------------------------------------------------

void LLOfferHandler::onDeleteToast(LLToast* toast)
{
	if (!LLHandlerUtil::canAddNotifPanelToIM(toast->getNotification()))
	{
		// send a signal to the counter manager
		mDelNotificationSignal();
	}

	// send a signal to a listener to let him perform some action
	// in this case listener is a SysWellWindow and it will remove a corresponding item from its list
	mNotificationIDSignal(toast->getNotificationID());
}

//--------------------------------------------------------------------------
void LLOfferHandler::onRejectToast(LLUUID& id)
{
	LLNotificationPtr notification = LLNotifications::instance().find(id);

	if (notification
			&& LLNotificationManager::getInstance()->getHandlerForNotification(
					notification->getType()) == this
					// don't delete notification since it may be used by IM floater
					&& !LLHandlerUtil::canAddNotifPanelToIM(notification))
	{
		LLNotifications::instance().cancel(notification);
	}
}
