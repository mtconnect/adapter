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
#include <chrono>
#include <memory>
#include "server.hpp"
#include "string_buffer.hpp"
#include "threading.hpp"

class DeviceDatum;
class CuttingTool;

constexpr size_t INITIAL_MAX_DEVICE_DATA = 128u;

//
// Abstract adapter that manages all the data values and writing them
// to the clients.
//
// Subclasses of this class will add the data values and interact with the
// vendor specifc API. This class provides all the common functionality.
//
class Adapter
{
protected:
	std::unique_ptr<Server> mServer;		// The socket server
	StringBuffer mBuffer;					// A string buffer to hold the string we write to the streams
	std::vector<DeviceDatum *> mDeviceData;
	std::chrono::milliseconds mScanDelay;	// How long to sleep (in ms) between scans
	int mPort;								// The server port we bind to
	bool mDisableFlush;						// Used for initial data collection
	int mHeartbeatFrequency;				// The frequency (ms) to heartbeat server. Responds to Ping. Default 10 sec
	bool mRunning;
	Client *mInitializeClient;				// If we are sending initial data to a client

#ifdef THREADED
	#ifdef WIN32
		HANDLE mServerThread;
	#else
		pthread_t mServerThread;
	#endif
#endif
	 MTCMutex mGatherLock;

protected:
	void sleepMs(std::chrono::milliseconds ms);

	// Internal buffer sending methods
	void sendBuffer();
	void sendDatum(DeviceDatum *value);
	virtual void sendInitialData(Client *client);
	virtual void sendChangedData();
	virtual void flush();
	void timestamp() { mBuffer.timestamp(); }
	virtual void unavailable();
	virtual void initializeDeviceDatum();

	virtual void addAsset(const char *id, const char *type, const char *data);
	virtual void updateAsset(const char *id, const char *data);
	virtual void addAsset(CuttingTool *tool);
	virtual void updateAsset(CuttingTool *tool);

public:
	Adapter(int port, int scanDelayMs = 100);
	~Adapter();

	void readFromClients();
	void connectToClients();

	// Start the server and never return
#ifdef THREADED
	bool startServerThread();
	void serverThread();
	int  waitUntilDone();

	// allow for subclasses to do some work when threaded
	virtual void periodicWork() {}
#endif

	void startServer();
	void addDatum(DeviceDatum &value);

	// Stop server
	virtual void stopServer();

	// Pure virtual method to get the data from the device.
	virtual void gatherDeviceData() = 0;
	virtual void begin();
	virtual void prepare();
	virtual void cleanup();

	// For multithreaded async gathering
	virtual void beginGather(const char *timestamp = nullptr, bool sweep = true);
	virtual void completeGather();

	// Overload this method to handle situation when all clients disconnect
	virtual void clientsDisconnected();
};


class AutoGather
{
public:
	AutoGather(Adapter *adapter = nullptr, const char *timestamp = nullptr, bool sweep = true)
		: mAdapter(adapter)
	{
		if (mAdapter)
			mAdapter->beginGather(timestamp, sweep);
	}

	void begin(Adapter *adapter, const char *timestamp = nullptr, bool sweep = true)
	{
		mAdapter = adapter;
		if (mAdapter)
			mAdapter->beginGather(timestamp, sweep);
	}

	void complete()
	{
		if (mAdapter)
			mAdapter->completeGather();
		mAdapter = nullptr;
	}

	~AutoGather()
	{
		if (mAdapter)
			mAdapter->completeGather();
	}

protected:
	Adapter *mAdapter;
};
