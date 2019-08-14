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
#pragma once

#include <vector>

// The conditon items
#include "device_datum.hpp"


class Condition : public DeviceDatum
{
public:
	enum ELevels
	{
		eUNAVAILABLE,
		eNORMAL,
		eWARNING,
		eFAULT
	};

protected:
	class ActiveCondition
	{
	protected:
		ELevels mLevel;
		char mText[EVENT_VALUE_LEN];
		char mNativeCode[EVENT_VALUE_LEN];
		char mNativeSeverity[EVENT_VALUE_LEN];
		char mQualifier[EVENT_VALUE_LEN];

		bool mChanged;
		bool mMarked;
		bool mPlaceHolder;

	public:
		ActiveCondition() :
			mLevel(eUNAVAILABLE),
			mText{0},
			mNativeCode{0},
			mNativeSeverity{0},
			mQualifier{0},
			mChanged(true),
			mMarked(true),
			mPlaceHolder(false)
		{
		}

		ActiveCondition(
			ELevels level,
			const char *text = "",
			const char *code = "",
			const char *qualifier = "",
			const char *severity = "")
			:
			mText{0},
			mNativeCode{0},
			mNativeSeverity{0},
			mQualifier{0},
			mChanged(true),
			mMarked(true),
			mPlaceHolder(false)
		{
			setValue(level, text, code, qualifier, severity);
		}

		~ActiveCondition()
		{
		}

		bool setValue(
			ELevels level,
			const char *text = "",
			const char *code = "",
			const char *qualifier = "",
			const char *severity = "");

		char *toString(char *bBuffer, int maxLen);
		bool hasChanged() const {
			return mChanged; }

		ELevels getLevel() const {
			return mLevel; }
		const char *getText() const {
			return mText; }
		const char *getNativeCode() const {
			return mNativeCode; }
		const char *getNativeSeverity() const {
			return mNativeSeverity; }
		const char *getQualifier() const {
			return mQualifier; }

		void clear() {
			mMarked = false; }
		void mark() {
			mMarked = true; }
		bool isMarked() const {
			return mMarked; }
		bool isPlaceHolder() const {
			return mPlaceHolder; }
	};

protected:
	std::vector<ActiveCondition *> mActiveList;
	static const int mInitialActiveListSize = 16;

	bool mBegun;
	bool mPrepared;
	bool mSimple;

	void add(ActiveCondition *condition);

	static void append(
		StringBuffer &stringBuffer,
		char *buffer,
		ActiveCondition *cond,
		bool &first,
		int maxLen);

public:
	Condition(const char *name = "", bool simple = false);
	virtual ~Condition();

	bool add(
		ELevels level,
		const char *text = "",
		const char *code = "",
		const char *qualifier = "",
		const char *severity = "");
	void remove(const char *code);

	void removeAll();
	bool normal() {
		return add(eNORMAL); }
	bool isActive(const char *nativeCode) const;
	void setSimple() {
		mSimple = true; }

// Overrides from DeviceDatum
public:
	char *toString(char *buffer, int maxLen) override;
	bool requiresFlush() const override;
	bool unavailable() override;
	bool append(StringBuffer &buffer) override;
	void begin() override;
	void prepare() override;
	void cleanup() override;
	void initialize() override;
};

