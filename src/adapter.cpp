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
#include "adapter.hpp"
#include <thread>
#include <chrono>
#include "device_datum.hpp"
#include "cutting_tool.hpp"
#include "logger.hpp"


Adapter::Adapter(int port, int scanDelayMs) :
	mServer(nullptr),
	mScanDelay{scanDelayMs},
	mPort(port),
	mDisableFlush(false),
	mHeartbeatFrequency(10000),
	mRunning(false),
	mInitializeClient(nullptr)
{

#ifdef THREADED
	mServerThread = nullptr;
#endif

	mDeviceData.reserve(INITIAL_MAX_DEVICE_DATA);
}


Adapter::~Adapter()
{
	mRunning = false;
	mServer.reset();

	for(auto value : mDeviceData)
		delete value;
	mDeviceData.clear();
}


// Add a data value to the list of data values
void Adapter::addDatum(DeviceDatum &value)
{
	// If we are approaching our capacity then resize
	if(mDeviceData.size() == mDeviceData.capacity())
		mDeviceData.reserve(mDeviceData.capacity() * 2u);

	mDeviceData.push_back(&value);
}


void Adapter::sleepMs(std::chrono::milliseconds ms)
{
	std::this_thread::sleep_for(ms);
}

#if THREADED

#ifdef WIN32

static int ServerThread(void *arg)
{
	auto adapter = (Adapter*)arg;
	adapter->serverThread();
	return 0;
}

bool Adapter::startServerThread()
{
	if (!gLogger)
		gLogger = new Logger();

	mServer = std::make_unique<Server>(mPort, mHeartbeatFrequency);
	mRunning = true;

	mServerThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE) &ServerThread, this, 0, 0);
	if (!mServerThread)
	{
		fprintf(stderr, "Cannot create server thread");
		mServer.reset();
		return false;
	}
	else
	{
		sleepMs(std::chrono::milliseconds(10));
	}

	return true;
}


int Adapter::waitUntilDone()
{
	int ret = 1;
	if (mServerThread != 0)
	{
		auto res = WaitForSingleObject(mServerThread, INFINITE);
		if (res == WAIT_OBJECT_0)
		{
			if (GetExitCodeThread(mServerThread, &res))
				ret = res;
		}
	}
	return ret;
}

#else

static void *ServerThread(void *arg)
{
	auto adapter = (Adapter*)arg;
	adapter->serverThread();

	static int res = 0;
	return &res;
}


bool Adapter::startServerThread()
{
	if (!gLogger)
		gLogger = new Logger();

	mServer = std::make_unique<Server>(mPort, mHeartbeatFrequency);
	mRunning = true;

	auto res = pthread_create(&mServerThread, nullptr, ::ServerThread, this);
	if (res != 0)
	{
		fprintf(stderr, "Cannot create server thread");
		mServer.reset();
		return false;
	}

	return true;
}


int Adapter::waitUntilDone()
{
	int ret = 1;
	if (mServerThread != 0)
	{
		int *value(nullptr);
		auto res = pthread_join(mServerThread, (void**) &value);
		if (res != 0)
			ret = *value;
	}
	return ret;
}

#endif

// Poll every second for activity. We could do blocking but it complicates
// list management and locking. May refactor this when we have more time.
//
void Adapter::serverThread()
{
	while (mRunning)
	{
		connectToClients();
		readFromClients();

		periodicWork();

		sleepMs(mScanDelay);
	}
}

#endif


//
// Reads from clients, either blocking or polling.
//
void Adapter::readFromClients()
{
	mServer->readFromClients();
}


//
// Checks for new client connections, either blocking or polling.
//
void Adapter::connectToClients()
{
	// Check if we have any new clients
	auto client = mServer->connectToClients();
	if (client)
		sendInitialData(client);
}


//
// This is the main loop of the application. Once the server is started and
// the application runs forever until it is killed. The process follows this flow:
//    See if any new clients have arrived.
//      If new clients are present, sent the initial values to them.
//    Read from the clients any data that is currently in the socket to keep anyone from
//      blocking.
//    If we have at least one client:
//      Timestamp the buffer
//      gather data from the API
//      send the data values that have changed to the clients
//    Sleep for a few millies and repeat.
//
void Adapter::startServer()
{
	if (!gLogger)
		gLogger = new Logger();

	mServer = std::make_unique<Server>(mPort, mHeartbeatFrequency);
	mRunning = true;

	// Process untill stopped
	while (mRunning)
	{
		// Check if we have any new clients
		connectToClients();

		// Read and all data from the clients
		readFromClients();

		// Don't bother getting data if we don't have anyone to read it
		if (mServer->numClients() > 0)
		{
			std::lock_guard<std::mutex> lock(mGatherLock);

			begin();
			mBuffer.timestamp();
			gatherDeviceData();
			prepare();
			sendChangedData();
			mBuffer.reset();
			cleanup();
		}
		else if (mServer->hasClients())
			clientsDisconnected();

		sleepMs(mScanDelay);
	}

	mServer.reset();
}


void Adapter::begin()
{
	for(auto value : mDeviceData)
		value->begin();
}


void Adapter::prepare()
{
	for(auto value : mDeviceData)
		value->prepare();
}


void Adapter::cleanup()
{
	for(auto value : mDeviceData)
		value->cleanup();
}


void Adapter::beginGather(const char *timestamp, bool sweep)
{
	mGatherLock.lock();

	if (sweep)
		begin();

	mBuffer.reset();
	if (timestamp)
		mBuffer.setTimestamp(timestamp);
	else
		mBuffer.timestamp();
}


void Adapter::completeGather()
{
	prepare();
	sendChangedData();
	cleanup();

	mGatherLock.unlock();
}


void Adapter::stopServer()
{
	mRunning = false;
}


// Send a single value to the buffer.
void Adapter::sendDatum(DeviceDatum *value)
{
	if (value->requiresFlush())
		sendBuffer();
	value->append(mBuffer);
	if (value->requiresFlush())
		sendBuffer();
}


// Send the buffer to the clients. Only sends if there is something in the buffer.
void Adapter::sendBuffer()
{
	if (mServer && mBuffer.length())
	{
		mBuffer.append("\n");
		if (mInitializeClient)
			mServer->sendToClient(mInitializeClient, mBuffer);
		else
			mServer->sendToClients(mBuffer);
		mBuffer.reset();
	}
}


// Send the initial values to a client
void Adapter::sendInitialData(Client *client)
{
	std::lock_guard<std::mutex> lock(mGatherLock);

	mInitializeClient = client;
	mDisableFlush = true;
	mBuffer.timestamp();
	gatherDeviceData();

	for(auto value : mDeviceData)
	{
		if(value->hasInitialValue())
			sendDatum(value);
	}

	sendBuffer();

	mDisableFlush = false;
	mInitializeClient = nullptr;
}


// Send the values that have changed to the clients
void Adapter::sendChangedData()
{
	for(auto value : mDeviceData)
	{
		if(value->changed())
			sendDatum(value);
	}

	sendBuffer();
}


void Adapter::flush()
{
	if (mDisableFlush)
		return;

	sendChangedData();
	mBuffer.reset();
	mBuffer.timestamp();
}


void Adapter::clientsDisconnected()
{
	// Do nothing for now ...
	gLogger->info("All clients have disconnected");
}


void Adapter::unavailable()
{
	for(auto value : mDeviceData)
		value->unavailable();

	flush();
}


void Adapter::initializeDeviceDatum()
{
	for(auto value : mDeviceData)
		value->initialize();

	flush();
}


void Adapter::addAsset(const char *id, const char *type, const char *data)
{
	sendBuffer();
	mBuffer.timestamp();

	mBuffer.append("|@ASSET@|");
	mBuffer.append(id);
	mBuffer.append("|");
	mBuffer.append(type);
	mBuffer.append("|--multiline--ABCD\n");
	mBuffer.append(data);
	mBuffer.append("\n--multiline--ABCD");

	sendBuffer();
	mBuffer.timestamp();
}


void Adapter::updateAsset(const char *id, const char *data)
{
	sendBuffer();
	mBuffer.timestamp();

	mBuffer.append("|@UPDATE_ASSET@|");
	mBuffer.append(id);
	mBuffer.append("|");
	mBuffer.append(data);

	sendBuffer();
	mBuffer.timestamp();
}


void Adapter::addAsset(CuttingTool *tool)
{
	addAsset(tool->getAssetId().c_str(), "CuttingTool", tool->toString().c_str());
}


void Adapter::updateAsset(CuttingTool *tool)
{
	updateAsset(tool->getAssetId().c_str(), tool->toString().c_str());
}

