/** 
 * @file llstrider.h
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

#ifndef LL_LLSTRIDER_H
#define LL_LLSTRIDER_H

#include "stdtypes.h"

template <class Object> class LLStrider
{
	union
	{
		Object* mObjectp;
		U8*		mBytep;
	};
	U32     mSkip;
public:

	LLStrider()  { mObjectp = NULL; mSkip = sizeof(Object); } 
	~LLStrider() { } 

	const LLStrider<Object>& operator =  (Object *first)    { mObjectp = first; return *this;}
	void setStride (S32 skipBytes)	{ mSkip = (skipBytes ? skipBytes : sizeof(Object));}

	void skip(const U32 index)     { mBytep += mSkip*index;}

	Object* get()                  { return mObjectp; }
	Object* operator->()           { return mObjectp; }
	Object& operator *()           { return *mObjectp; }
	Object* operator ++(int)       { Object* old = mObjectp; mBytep += mSkip; return old; }
	Object* operator +=(int i)     { mBytep += mSkip*i; return mObjectp; }
	Object& operator[](U32 index)  { return *(Object*)(mBytep + (mSkip * index)); }
};

#endif // LL_LLSTRIDER_H
