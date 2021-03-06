/** 
* @file llfloatertestinspectors.h
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
#ifndef LLFLOATERTESTINSPECTORS_H
#define LLFLOATERTESTINSPECTORS_H

#include "llfloater.h"

class LLSD;

class LLFloaterTestInspectors : public LLFloater
{
	friend class LLFloaterReg;
public:
	// nothing yet

private:
	// Construction handled by LLFloaterReg
	LLFloaterTestInspectors(const LLSD& seed);
	~LLFloaterTestInspectors();

	/*virtual*/ BOOL postBuild();

	// Button callback to show
	void showAvatarInspector(LLUICtrl*, const LLSD& avatar_id);
	void showObjectInspector(LLUICtrl*, const LLSD& avatar_id);
	
	// Debug function hookups for buttons
	void onClickAvatar2D();
	void onClickAvatar3D();
	void onClickObject2D();
	void onClickObject3D();
	void onClickGroup();
	void onClickPlace();
	void onClickEvent();
};

#endif
