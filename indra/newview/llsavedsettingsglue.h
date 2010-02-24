/** 
 * @file llsavedsettingsglue.h
 * @author James Cook
 * @brief LLSavedSettingsGlue class definition
 *
 * $LicenseInfo:firstyear=2006&license=viewergpl$
 * 
 * Copyright (c) 2006-2010, Linden Research, Inc.
 * 
 * Second Life Viewer Source Code
 * The source code in this file ("Source Code") is provided by Linden Lab
 * to you under the terms of the GNU General Public License, version 2.0
 * ("GPL"), unless you have obtained a separate licensing agreement
 * ("Other License"), formally executed by you and Linden Lab.  Terms of
 * the GPL can be found in doc/GPL-license.txt in this distribution, or
 * online at http://secondlifegrid.net/programs/open_source/licensing/gplv2
 * 
 * There are special exceptions to the terms and conditions of the GPL as
 * it is applied to this Source Code. View the full text of the exception
 * in the file doc/FLOSS-exception.txt in this software distribution, or
 * online at
 * http://secondlifegrid.net/programs/open_source/licensing/flossexception
 * 
 * By copying, modifying or distributing this software, you acknowledge
 * that you have read and understood your obligations described above,
 * and agree to abide by those obligations.
 * 
 * ALL LINDEN LAB SOURCE CODE IS PROVIDED "AS IS." LINDEN LAB MAKES NO
 * WARRANTIES, EXPRESS, IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY,
 * COMPLETENESS OR PERFORMANCE.
 * $/LicenseInfo$
 */

#ifndef LL_LLSAVEDSETTINGSGLUE_H
#define LL_LLSAVEDSETTINGSGLUE_H

class LLUICtrl;

// Helper to change gSavedSettings from UI widget commit callbacks.
// Set the widget callback to be one of the setFoo() calls below,
// and assign the control name as a const char* to the userdata.
class LLSavedSettingsGlue
{
public:
	static void setBOOL(LLUICtrl* ctrl, const std::string& name);
	static void setS32(LLUICtrl* ctrl, const std::string& name);
	static void setF32(LLUICtrl* ctrl, const std::string& name);
	static void setU32(LLUICtrl* ctrl, const std::string& name);
	static void setString(LLUICtrl* ctrl, const std::string& name);
};

#endif
