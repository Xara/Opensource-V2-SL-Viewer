/** 
 * @file llfloaterauction.h
 * @author James Cook, Ian Wilkes
 * @brief llfloaterauction class header file
 *
 * $LicenseInfo:firstyear=2004&license=viewergpl$
 * 
 * Copyright (c) 2004-2010, Linden Research, Inc.
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

#ifndef LL_LLFLOATERAUCTION_H
#define LL_LLFLOATERAUCTION_H

#include "llfloater.h"
#include "lluuid.h"
#include "llpointer.h"
#include "llviewertexture.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Class LLFloaterAuction
//
// Class which holds the functionality to start auctions.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class LLParcelSelection;
class LLParcel;
class LLViewerRegion;

class LLFloaterAuction : public LLFloater
{
	friend class LLFloaterReg;
public:
	// LLFloater interface
	/*virtual*/ void onOpen(const LLSD& key);
	/*virtual*/ void draw();

private:
	
	LLFloaterAuction(const LLSD& key);
	~LLFloaterAuction();
	
	void initialize();

	static void onClickSnapshot(void* data);
	static void onClickResetParcel(void* data);
	static void onClickSellToAnyone(void* data);		// Sell to anyone clicked
	bool onSellToAnyoneConfirmed(const LLSD& notification, const LLSD& response);	// Sell confirmation clicked
	static void onClickStartAuction(void* data);

	/*virtual*/ BOOL postBuild();

	void doResetParcel();
	void doSellToAnyone();
	void clearParcelAccessLists( LLParcel* parcel, LLViewerRegion* region );
	void cleanupAndClose();

private:

	LLTransactionID mTransactionID;
	LLAssetID mImageID;
	LLPointer<LLViewerTexture> mImage;
	LLSafeHandle<LLParcelSelection> mParcelp;
	S32 mParcelID;
	LLHost mParcelHost;

	std::string mParcelUpdateCapUrl;	// "ParcelPropertiesUpdate" capability
};


#endif // LL_LLFLOATERAUCTION_H
