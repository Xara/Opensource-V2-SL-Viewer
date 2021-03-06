/** 
* @file llpanelprofile.h
* @brief Profile panel
*
* $LicenseInfo:firstyear=2009&license=viewergpl$
* 
* Copyright (c) 2009-2010, Linden Research, Inc.
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

#ifndef LL_LLPANELPROFILE_H
#define LL_LLPANELPROFILE_H

#include "llviewerprecompiledheaders.h"
#include "llpanel.h"
#include "llpanelavatar.h"

class LLTabContainer;

/**
* Base class for Profile View and My Profile.
*/
class LLPanelProfile : public LLPanel
{
	LOG_CLASS(LLPanelProfile);

public:
	/*virtual*/ BOOL postBuild();

	/*virtual*/ void onOpen(const LLSD& key);

	virtual void togglePanel(LLPanel*, const LLSD& key = LLSD());

	virtual void openPanel(LLPanel* panel, const LLSD& params);

	virtual void closePanel(LLPanel* panel);

	S32 notifyParent(const LLSD& info);

protected:

	LLPanelProfile();

	virtual void onTabSelected(const LLSD& param);

	virtual void setAllChildrenVisible(BOOL visible);

	LLTabContainer* getTabCtrl() { return mTabCtrl; }

	const LLUUID& getAvatarId() { return mAvatarId; }

	void setAvatarId(const LLUUID& avatar_id) { mAvatarId = avatar_id; }

	typedef std::map<std::string, LLPanelProfileTab*> profile_tabs_t;

	profile_tabs_t& getTabContainer() { return mTabContainer; }

private:

	LLTabContainer* mTabCtrl;	
	profile_tabs_t mTabContainer;
	LLUUID mAvatarId;
};

#endif //LL_LLPANELPROFILE_H
