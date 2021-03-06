/** 
 * @file llversioninfo.cpp
 * @brief Routines to access the viewer version and build information
 * @author Martin Reddy
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

#include "llviewerprecompiledheaders.h"
#include "llversioninfo.h"

#include "llversionviewer.h"

//
// Set the version numbers in indra/llcommon/llversionviewer.h
//

//static
S32 LLVersionInfo::getMajor()
{
	return LL_VERSION_MAJOR;
}

//static
S32 LLVersionInfo::getMinor()
{
	return LL_VERSION_MINOR;
}

//static
S32 LLVersionInfo::getPatch()
{
	return LL_VERSION_PATCH;
}

//static
S32 LLVersionInfo::getBuild()
{
	return LL_VERSION_BUILD;
}

//static
const std::string &LLVersionInfo::getVersion()
{
	static std::string version("");

	if (version.empty())
	{
		// cache the version string
		std::ostringstream stream;
		stream << LL_VERSION_MAJOR << "."
		       << LL_VERSION_MINOR << "."
		       << LL_VERSION_PATCH << "."
		       << LL_VERSION_BUILD;
		version = stream.str();
	}

	return version;
}

//static
const std::string &LLVersionInfo::getShortVersion()
{
	static std::string version("");

	if (version.empty())
	{
		// cache the version string
		std::ostringstream stream;
		stream << LL_VERSION_MAJOR << "."
		       << LL_VERSION_MINOR << "."
		       << LL_VERSION_PATCH;
		version = stream.str();
	}

	return version;
}

//static
const std::string &LLVersionInfo::getChannel()
{
	static std::string name(LL_CHANNEL);
	return name;
}
