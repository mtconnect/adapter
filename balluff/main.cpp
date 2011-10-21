//
//  main.c
//  cstream
//
//  Created by William Sobel on 10/17/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "internal.hpp"
#include "balluff_adapter.hpp"

int main(int aArgc, const char *aArgv[])
{    
  /* Construct the adapter and start the server */
  BalluffAdapter *adapter = new BalluffAdapter(7878);
  adapter->setName("MTConnect Balluff Adapter");
  adapter->main(aArgc, aArgv);
  return adapter->waitUntilDone();
}
