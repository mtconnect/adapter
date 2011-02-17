/*
* Copyright (c) 2008, AMT – The Association For Manufacturing Technology (“AMT”)
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the AMT nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* DISCLAIMER OF WARRANTY. ALL MTCONNECT MATERIALS AND SPECIFICATIONS PROVIDED
* BY AMT, MTCONNECT OR ANY PARTICIPANT TO YOU OR ANY PARTY ARE PROVIDED "AS IS"
* AND WITHOUT ANY WARRANTY OF ANY KIND. AMT, MTCONNECT, AND EACH OF THEIR
* RESPECTIVE MEMBERS, OFFICERS, DIRECTORS, AFFILIATES, SPONSORS, AND AGENTS
* (COLLECTIVELY, THE "AMT PARTIES") AND PARTICIPANTS MAKE NO REPRESENTATION OR
* WARRANTY OF ANY KIND WHATSOEVER RELATING TO THESE MATERIALS, INCLUDING, WITHOUT
* LIMITATION, ANY EXPRESS OR IMPLIED WARRANTY OF NONINFRINGEMENT,
* MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. 

* LIMITATION OF LIABILITY. IN NO EVENT SHALL AMT, MTCONNECT, ANY OTHER AMT
* PARTY, OR ANY PARTICIPANT BE LIABLE FOR THE COST OF PROCURING SUBSTITUTE GOODS
* OR SERVICES, LOST PROFITS, LOSS OF USE, LOSS OF DATA OR ANY INCIDENTAL,
* CONSEQUENTIAL, INDIRECT, SPECIAL OR PUNITIVE DAMAGES OR OTHER DIRECT DAMAGES,
* WHETHER UNDER CONTRACT, TORT, WARRANTY OR OTHERWISE, ARISING IN ANY WAY OUT OF
* THIS AGREEMENT, USE OR INABILITY TO USE MTCONNECT MATERIALS, WHETHER OR NOT
* SUCH PARTY HAD ADVANCE NOTICE OF THE POSSIBILITY OF SUCH DAMAGES.
*/

#include "internal.hpp"
#include "audio_adapter.hpp"

static int recordCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
  return ((AudioAdapter*) userData)->recordCallback(inputBuffer, outputBuffer,
						    framesPerBuffer, timeInfo,
						    statusFlags);
}

int AudioAdapter::recordCallback(const void *inputBuffer, void *outputBuffer,
				 unsigned long framesPerBuffer,
				 const PaStreamCallbackTimeInfo* timeInfo,
				 PaStreamCallbackFlags statusFlags)
{
  if (mServer->numClients() > 0)
  {
    int i;
    char timestamp[64], mant[16];
    double now = mStartTime + timeInfo->inputBufferAdcTime;
    double sec, frac;
    frac = modf(now, &sec);

    time_t t = (time_t) (sec);
    strftime(timestamp, 64, "%Y-%m-%dT%H:%M:%S", gmtime(&t));
    sprintf(mant, "%.9f", frac);
    for (i = 10; i > 0 && mant[i] == '0'; i--)
      ;
    mant[i++] = 'Z';
    mant[i] = '\0';
    strcat(timestamp, mant + 1);
    //printf("Time: %s\n", timestamp);
    
    begin();
    mBuffer.setTimestamp(timestamp);
    mAudio.clear();

    float *data = (float*) inputBuffer;
    for (int i = 0; i < framesPerBuffer; i++)
    {
      mAudio.addValue(data[i]);
    }

    sendChangedData();
    mBuffer.reset();
    cleanup();
  }
  return 0;
}

AudioAdapter::AudioAdapter(int aPort)
  : Adapter(aPort, 1000), mAvailability("avail"), mAudio("audio")
{
  addDatum(mAvailability);
  addDatum(mAudio);
}

AudioAdapter::~AudioAdapter()
{
}

void AudioAdapter::initialize(int aArgc, const char *aArgv[])
{
  MTConnectService::initialize(aArgc, aArgv);
  if (aArgc > 1) {
    mPort = atoi(aArgv[1]);
  }
}

void AudioAdapter::start()
{
  PaStreamParameters  inputParameters;
  PaError err = Pa_Initialize();
  if( err != paNoError ) {
    printf("Could not initialize the audio\n");
    exit(1);
  }

  inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
  inputParameters.channelCount = 1;                    /* stereo input */
  inputParameters.sampleFormat = paFloat32;
  inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
  inputParameters.hostApiSpecificStreamInfo = NULL;

  /* Record some audio. -------------------------------------------- */
  err = Pa_OpenStream(&mStream,
		      &inputParameters,
		      NULL,                  /* &outputParameters, */
		      44100,
		      10240,
		      paClipOff,      /* we won't output out of range samples so don't bother clipping them */
		      ::recordCallback,
		      this );
  if( err != paNoError ) 
  {
    printf("Cannot open stream\n");
    exit(1);
  }

  err = Pa_StartStream( mStream );
  if( err != paNoError ) 
  {
    printf("Cannot start stream\n");
    exit(1);
  }
  struct timeval tv;
  gettimeofday(&tv, NULL);

  mStartTime = (double) tv.tv_sec + ((double) tv.tv_usec / 1000000.0);

  startServerThread();
}

void AudioAdapter::stop()
{
  PaError err = Pa_StopStream( mStream );
  if( err != paNoError ) 
  {
    printf("Cannot stop stream\n");
    exit(1);
  }
  
  stopServer();
}

void AudioAdapter::gatherDeviceData()
{
}

