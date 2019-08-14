//
// Copyright (c) 2008, AMT - The Association For Manufacturing Technology ("AMT")
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the AMT nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// DISCLAIMER OF WARRANTY. ALL MTCONNECT MATERIALS AND SPECIFICATIONS PROVIDED
// BY AMT, MTCONNECT OR ANY PARTICIPANT TO YOU OR ANY PARTY ARE PROVIDED "AS IS"
// AND WITHOUT ANY WARRANTY OF ANY KIND. AMT, MTCONNECT, AND EACH OF THEIR
// RESPECTIVE MEMBERS, OFFICERS, DIRECTORS, AFFILIATES, SPONSORS, AND AGENTS
// (COLLECTIVELY, THE "AMT PARTIES") AND PARTICIPANTS MAKE NO REPRESENTATION OR
// WARRANTY OF ANY KIND WHATSOEVER RELATING TO THESE MATERIALS, INCLUDING, WITHOUT
// LIMITATION, ANY EXPRESS OR IMPLIED WARRANTY OF NONINFRINGEMENT,
// MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
//
// LIMITATION OF LIABILITY. IN NO EVENT SHALL AMT, MTCONNECT, ANY OTHER AMT
// PARTY, OR ANY PARTICIPANT BE LIABLE FOR THE COST OF PROCURING SUBSTITUTE GOODS
// OR SERVICES, LOST PROFITS, LOSS OF USE, LOSS OF DATA OR ANY INCIDENTAL,
// CONSEQUENTIAL, INDIRECT, SPECIAL OR PUNITIVE DAMAGES OR OTHER DIRECT DAMAGES,
// WHETHER UNDER CONTRACT, TORT, WARRANTY OR OTHERWISE, ARISING IN ANY WAY OUT OF
// THIS AGREEMENT, USE OR INABILITY TO USE MTCONNECT MATERIALS, WHETHER OR NOT
// SUCH PARTY HAD ADVANCE NOTICE OF THE POSSIBILITY OF SUCH DAMAGES.
//
#include "internal.hpp"
#include "condition.hpp"
#include <algorithm>
#include "string_buffer.hpp"

// Condition


Condition::Condition(const char *name, bool simple) :
	DeviceDatum(name),
	mBegun(false),
	mPrepared(false),
	mSimple(simple)
{
	mActiveList.reserve(mInitialActiveListSize);
	mHasValue = true;
	unavailable();
}


Condition::~Condition()
{
	removeAll();
}


void Condition::removeAll()
{
	for(auto condition : mActiveList)
		delete condition;
	mActiveList.clear();
}


bool Condition::requiresFlush() const
{
	return true;
}


bool Condition::unavailable()
{
	return add(eUNAVAILABLE);
}


char *Condition::toString(char *, int )
{
	return nullptr;
}


void Condition::begin()
{
	if (!mSimple)
	{
		for(auto condition : mActiveList)
			condition->clear();
	}

	mPrepared = false;
	mBegun = true;
	mChanged = false;
}


void Condition::cleanup()
{
	mBegun = false;
	mPrepared = false;
}


void Condition::initialize()
{
	normal();
}


void Condition::append(
	StringBuffer &stringBuffer,
	char *buffer,
	ActiveCondition *condition,
	bool &first,
	int maxLen)
{
	if (!first)
		stringBuffer.newline();
	else
		first = false;

	auto cp = buffer + strlen(buffer);
	condition->toString(cp, maxLen);

	appendText(buffer, const_cast<char *>(condition->getText()), maxLen);
	stringBuffer.append(buffer);
}


void Condition::prepare()
{
	if (!mBegun)
		return;

	bool marked = false;

	// Check to see if we have no marked conditions
	for(auto condition : mActiveList)
	{
		if (condition->isPlaceHolder() || condition->isMarked())
		{
			marked = true;
			break;
		}
	}

	if (!marked)
		normal();

	// Sweep old conditions
	for(auto conditionIter = mActiveList.crbegin();conditionIter != mActiveList.crend(); conditionIter++)
	{
		auto condition = *conditionIter;
		if (!condition->isPlaceHolder() && !condition->isMarked())
			condition->setValue(eNORMAL, "", condition->getNativeCode());

		if (condition->hasChanged())
			mChanged = true;
	}

	mPrepared = true;
}


bool Condition::append(StringBuffer &stringBuffer)
{
	if (!mBegun)
	{
		char buffer[1024] = {0};
		bool first = true;
		buffer[0] = '|';
		strcpy(buffer + 1, mName);
		auto cp = buffer + strlen(buffer);
		auto max = 1024u - strlen(buffer);

		for(auto condition : mActiveList)
		{
			*cp = '\0';
			append(stringBuffer, buffer, condition, first, max);
		}
	}
	else if (mBegun && mPrepared)
	{
		char buffer[1024]  {0};
		bool first = true;
		buffer[0] = '|';
		strcpy(buffer + 1, mName);
		auto cp = buffer + strlen(buffer);
		auto max = 1024u - strlen(buffer);
		
		// Sweep old conditions
		// Update any that have changed
		for(auto conditionIter = mActiveList.crbegin(); conditionIter != mActiveList.crend(); conditionIter++)
		{
			auto condition = *conditionIter;
			if(condition->hasChanged())
			{
				*cp = '\0';
				append(stringBuffer, buffer, condition, first, max);
			}
		}

		// Update our container to move items to be deleted to the end
		auto removeRange = std::remove_if(mActiveList.begin(), mActiveList.end(), [](ActiveCondition *condition)
		{
			if( !condition->isPlaceHolder() && !condition->isMarked() )
			{
				delete condition; condition = nullptr;
				return true;
			}
			else
				return false;
		});

		// Remove items from the container if required
		if(removeRange != mActiveList.end())
			mActiveList.erase(removeRange);
	}

	return mChanged;
}


bool Condition::isActive(const char *nativeCode) const
{
	auto findPos = std::find_if(mActiveList.cbegin(), mActiveList.cend(), [nativeCode](const ActiveCondition *condition)
	{
		return !strcmp(nativeCode, condition->getNativeCode());
	});

	return findPos != mActiveList.cend();
}


bool Condition::add(
	ELevels level,
	const char *text,
	const char *code,
	const char *qualifier,
	const char *severity)
{
	bool res(false);

	// First check for a unassociated normal or a unavailable.
	if ((level == eNORMAL || level == eUNAVAILABLE) &&
		code[0] == '\0')
	{
		// See if we are already in this state.
		if (mActiveList.size() == 1u &&
			mActiveList[0]->getNativeCode()[0] == '\0' &&
			mActiveList[0]->getLevel() == level)
		{
			mActiveList[0]->mark();
			res = false;
		}
		else
		{
			// Clear all existing conditions and add one with this state.
			removeAll();
			add( new ActiveCondition(level) );
			res = mChanged = true;
		}
	}
	else
	{
		if (mActiveList.size() == 1u &&
			(mActiveList[0]->getLevel() == eNORMAL ||
			 mActiveList[0]->getLevel() == eUNAVAILABLE))
		{
			removeAll();
		}

		// We have a code specific condition or a ab-normal
		auto conditionPos = std::find_if(mActiveList.cbegin(), mActiveList.cend(), [code](ActiveCondition* condition)
		{
			return !strcmp(code, condition->getNativeCode());
		});

		if(conditionPos != mActiveList.cend())
		{
			auto condition = *conditionPos;
			res = mChanged = condition->setValue(level, text, code, qualifier, severity);
			condition->mark();
		}
		else
		{
			// New condition
			add( new ActiveCondition(level, text, code, qualifier, severity) );
			res = mChanged = true;
		}
	}

	return res;
}


void Condition::remove(const char *code)
{
	// We have a code specific condition or a ab-normal
	auto conditionPos = std::find_if(mActiveList.cbegin(), mActiveList.cend(), [code](ActiveCondition* condition)
	{
		return !strcmp(code, condition->getNativeCode());
	});

	if(conditionPos != mActiveList.cend())
	{
		if(mActiveList.size() == 1u)
			normal();
		else
		{
			(*conditionPos)->setValue(eNORMAL, "", code);
			(*conditionPos)->clear();
		}
	}
}


void Condition::add(ActiveCondition *condition)
{
	// If we are approaching our capacity then resize
	if(mActiveList.size() == mActiveList.capacity())
		mActiveList.reserve(mActiveList.capacity() * 2u);

	mActiveList.push_back(condition);
}


char *Condition::ActiveCondition::toString(char *buffer, int aMaxLen)
{
	const char *text = nullptr;

	switch (mLevel)
	{
	case eUNAVAILABLE:
		text = "UNAVAILABLE";
		break;

	case eNORMAL:
		text = "NORMAL";
		break;

	case eWARNING:
		text = "WARNING";
		break;

	case eFAULT:
		text = "FAULT";
		break;

	default:
		text = "";
		break;
	}

	snprintf(buffer, aMaxLen, "|%s|%s|%s|%s|", text, mNativeCode, mNativeSeverity, mQualifier);

	mChanged = false;
	return buffer;
}


bool Condition::ActiveCondition::setValue(
	ELevels level,
	const char *text,
	const char *code,
	const char *qualifier,
	const char *severity)
{
	if ((level == eNORMAL || level == eUNAVAILABLE) && code[0] == '\0')
		mPlaceHolder = true;

	if (mLevel != level ||
		strncmp(code, mNativeCode, EVENT_VALUE_LEN) ||
		strncmp(qualifier, mQualifier, EVENT_VALUE_LEN) ||
		strncmp(severity, mNativeSeverity, EVENT_VALUE_LEN) ||
		strncmp(text, mText, EVENT_VALUE_LEN))
	{
		mLevel = level;

		strncpy(mNativeCode, code, EVENT_VALUE_LEN);
		mNativeCode[EVENT_VALUE_LEN - 1] = '\0';

		strncpy(mQualifier, qualifier, EVENT_VALUE_LEN);
		mQualifier[EVENT_VALUE_LEN - 1] = '\0';

		strncpy(mNativeSeverity, severity, EVENT_VALUE_LEN);
		mNativeSeverity[EVENT_VALUE_LEN - 1] = '\0';

		strncpy(mText, text, EVENT_VALUE_LEN);
		mText[EVENT_VALUE_LEN - 1] = '\0';

		mChanged = true;
	}

	return mChanged;
}

