/** 
 * @file llfloatermap.cpp
 * @brief The "mini-map" or radar in the upper right part of the screen.
 *
 * $LicenseInfo:firstyear=2001&license=viewergpl$
 * 
 * Copyright (c) 2001-2010, Linden Research, Inc.
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

// self include
#include "llfloatermap.h"

// Library includes
#include "llfloaterreg.h"
#include "llfontgl.h"
#include "llglheaders.h"

// Viewer includes
#include "llagent.h"
#include "llviewercontrol.h"
#include "llnetmap.h"
#include "lltracker.h"
#include "llviewercamera.h"
#include "lldraghandle.h"
#include "lltextbox.h"
#include "llviewermenu.h"

//
// Constants
//
const F32 MAP_MINOR_DIR_THRESHOLD = 0.08f;

//
// Member functions
//

LLFloaterMap::LLFloaterMap(const LLSD& key) 
	: LLFloater(key),
	  mPopupMenu(NULL),
	  mTextBoxEast(NULL),
	  mTextBoxNorth(NULL),
	  mTextBoxWest(NULL),
	  mTextBoxSouth(NULL),
	  mTextBoxSouthEast(NULL),
	  mTextBoxNorthEast(NULL),
	  mTextBoxNorthWest(NULL),
	  mTextBoxSouthWest(NULL),
	  mMap(NULL)
{
	//Called from floater reg: LLUICtrlFactory::getInstance()->buildFloater(this, "floater_map.xml", FALSE);
}

LLFloaterMap::~LLFloaterMap()
{
}

BOOL LLFloaterMap::postBuild()
{
	mMap = getChild<LLNetMap>("Net Map");
	mMap->setScale(gSavedSettings.getF32("MiniMapScale"));
	mMap->setToolTipMsg(getString("ToolTipMsg"));	
	sendChildToBack(mMap);
	
	mTextBoxNorth = getChild<LLTextBox> ("floater_map_north");
	mTextBoxEast = getChild<LLTextBox> ("floater_map_east");
	mTextBoxWest = getChild<LLTextBox> ("floater_map_west");
	mTextBoxSouth = getChild<LLTextBox> ("floater_map_south");
	mTextBoxSouthEast = getChild<LLTextBox> ("floater_map_southeast");
	mTextBoxNorthEast = getChild<LLTextBox> ("floater_map_northeast");
	mTextBoxSouthWest = getChild<LLTextBox> ("floater_map_southwest");
	mTextBoxNorthWest = getChild<LLTextBox> ("floater_map_northwest");

	LLUICtrl::CommitCallbackRegistry::ScopedRegistrar registrar;
	
	registrar.add("Minimap.Zoom", boost::bind(&LLFloaterMap::handleZoom, this, _2));
	registrar.add("Minimap.Tracker", boost::bind(&LLFloaterMap::handleStopTracking, this, _2));

	mPopupMenu = LLUICtrlFactory::getInstance()->createFromFile<LLMenuGL>("menu_mini_map.xml", gMenuHolder, LLViewerMenuHolderGL::child_registry_t::instance());
	if (mPopupMenu && !LLTracker::isTracking(0))
	{
		mPopupMenu->setItemEnabled ("Stop Tracking", false);
	}

	updateMinorDirections();

	// Get the drag handle all the way in back
	sendChildToBack(getDragHandle());

	setIsChrome(TRUE);
	getDragHandle()->setTitleVisible(TRUE);
	
	// keep onscreen
	gFloaterView->adjustToFitScreen(this, FALSE);

	return TRUE;
}

BOOL LLFloaterMap::handleDoubleClick( S32 x, S32 y, MASK mask )
{
	LLFloaterReg::showInstance("world_map");
	return TRUE;
}

BOOL LLFloaterMap::handleRightMouseDown(S32 x, S32 y, MASK mask)
{
	if (mPopupMenu)
	{
		mPopupMenu->buildDrawLabels();
		mPopupMenu->updateParent(LLMenuGL::sMenuContainer);
		LLMenuGL::showPopup(this, mPopupMenu, x, y);
	}
	return TRUE;
}

void LLFloaterMap::setDirectionPos( LLTextBox* text_box, F32 rotation )
{
	// Rotation is in radians.
	// Rotation of 0 means x = 1, y = 0 on the unit circle.

	F32 map_half_height = (F32)(getRect().getHeight() / 2) - getHeaderHeight()/2;
	F32 map_half_width = (F32)(getRect().getWidth() / 2) ;
	F32 text_half_height = (F32)(text_box->getRect().getHeight() / 2);
	F32 text_half_width = (F32)(text_box->getRect().getWidth() / 2);
	F32 radius = llmin( map_half_height - text_half_height, map_half_width - text_half_width );

	// Inset by a little to account for position display.
	radius -= 8.f;

	text_box->setOrigin( 
		llround(map_half_width - text_half_width + radius * cos( rotation )),
		llround(map_half_height - text_half_height + radius * sin( rotation )) );
}

void LLFloaterMap::updateMinorDirections()
{
	if (mTextBoxNorthEast == NULL)
	{
		return;
	}

	// Hide minor directions if they cover too much of the map
	bool show_minors = mTextBoxNorthEast->getRect().getHeight() < MAP_MINOR_DIR_THRESHOLD *
		llmin(getRect().getWidth(), getRect().getHeight());

	mTextBoxNorthEast->setVisible(show_minors);
	mTextBoxNorthWest->setVisible(show_minors);
	mTextBoxSouthWest->setVisible(show_minors);
	mTextBoxSouthEast->setVisible(show_minors);
}

// virtual
void LLFloaterMap::draw()
{
	F32 rotation = 0;

	static LLUICachedControl<bool> rotate_map("MiniMapRotate", true);
	if( rotate_map )
	{
		// rotate subsequent draws to agent rotation
		rotation = atan2( LLViewerCamera::getInstance()->getAtAxis().mV[VX], LLViewerCamera::getInstance()->getAtAxis().mV[VY] );
	}

	setDirectionPos( mTextBoxEast,  rotation );
	setDirectionPos( mTextBoxNorth, rotation + F_PI_BY_TWO );
	setDirectionPos( mTextBoxWest,  rotation + F_PI );
	setDirectionPos( mTextBoxSouth, rotation + F_PI + F_PI_BY_TWO );

	setDirectionPos( mTextBoxNorthEast, rotation +						F_PI_BY_TWO / 2);
	setDirectionPos( mTextBoxNorthWest, rotation + F_PI_BY_TWO +		F_PI_BY_TWO / 2);
	setDirectionPos( mTextBoxSouthWest, rotation + F_PI +				F_PI_BY_TWO / 2);
	setDirectionPos( mTextBoxSouthEast, rotation + F_PI + F_PI_BY_TWO + F_PI_BY_TWO / 2);

	// Note: we can't just gAgent.check cameraMouselook() because the transition states are wrong.
	if( gAgent.cameraMouselook())
	{
		setMouseOpaque(FALSE);
		getDragHandle()->setMouseOpaque(FALSE);
	}
	else
	{
		setMouseOpaque(TRUE);
		getDragHandle()->setMouseOpaque(TRUE);
	}
	
	if (LLTracker::isTracking(0))
	{
		mPopupMenu->setItemEnabled ("Stop Tracking", true);
	}
	
	LLFloater::draw();
}

void LLFloaterMap::reshape(S32 width, S32 height, BOOL called_from_parent)
{
	LLFloater::reshape(width, height, called_from_parent);
	updateMinorDirections();
}

void LLFloaterMap::handleZoom(const LLSD& userdata)
{
	std::string level = userdata.asString();
	
	F32 scale = 0.0f;
	if (level == std::string("close"))
		scale = LLNetMap::MAP_SCALE_MAX;
	else if (level == std::string("medium"))
		scale = LLNetMap::MAP_SCALE_MID;
	else if (level == std::string("far"))
		scale = LLNetMap::MAP_SCALE_MIN;
	if (scale != 0.0f)
	{
		gSavedSettings.setF32("MiniMapScale", scale );
		mMap->setScale(scale);
	}
}

void LLFloaterMap::handleStopTracking (const LLSD& userdata)
{
	if (mPopupMenu)
	{
		mPopupMenu->setItemEnabled ("Stop Tracking", false);
		LLTracker::stopTracking ((void*)LLTracker::isTracking(NULL));
	}
}

