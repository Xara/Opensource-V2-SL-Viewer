/** 
 * @file llmorphview.h
 * @brief Container for character morph controls
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

#ifndef LL_LLMORPHVIEW_H
#define LL_LLMORPHVIEW_H

#include "llview.h"
#include "v3dmath.h"
#include "llframetimer.h"

class LLJoint;
class LLFloaterCustomize;

class LLMorphView : public LLView
{
public:
	struct Params : public LLInitParam::Block<Params, LLView::Params>
	{
		Params()
		{
			mouse_opaque(false);
			follows.flags(FOLLOWS_ALL);
		}
	};
	LLMorphView(const LLMorphView::Params&);
	
	void		initialize();
	void		shutdown();

	// inherited methods
	/*virtual*/ void	setVisible(BOOL visible);

	void		setCameraTargetJoint(LLJoint *joint)		{mCameraTargetJoint = joint;}
	LLJoint*	getCameraTargetJoint()						{return mCameraTargetJoint;}

	void		setCameraOffset(const LLVector3d& camera_offset)	{mCameraOffset = camera_offset;}
	void		setCameraTargetOffset(const LLVector3d& camera_target_offset) {mCameraTargetOffset = camera_target_offset;}
	void		setCameraDistToDefault()					{ mCameraDist = -1.f; }

	void		updateCamera();
	void		setCameraDrivenByKeys( BOOL b );

protected:
	LLJoint*	mCameraTargetJoint;
	LLVector3d	mCameraOffset;
	LLVector3d	mCameraTargetOffset;
	LLVector3d	mOldCameraPos;
	LLVector3d	mOldTargetPos;
	F32			mOldCameraNearClip;
	LLFrameTimer mCameraMoveTimer;

	// camera rotation
	F32			mCameraPitch;
	F32			mCameraYaw;

	// camera zoom
	F32			mCameraDist;

	BOOL		mCameraDrivenByKeys;
};

//
// Globals
//

extern LLMorphView *gMorphView;

#endif 
