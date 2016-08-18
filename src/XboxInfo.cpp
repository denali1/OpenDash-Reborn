#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "SDL.h"

#ifdef ENABLE_XBOX
   #include <hal/xbox.h>
   #include <hal/fileio.h>
   #include <openxdk/debug.h>
   #include <xboxkrnl/xboxkrnl.h>
#else
   #define XSleep SDL_Delay
#endif

#include "XboxInfo.h"
#include "MenuNode.h"

extern MenuNode *theMenu;

XboxInfo::XboxInfo()
{
	memset(mPICVersion, 0x00, sizeof(char)*3);
	
	mTrayState = 0;
	mAVPack = 0;
	mBoardTemp = 0;
	mCPUTemp = 0;
	mFanSpeed = 0;
}

XboxInfo::~XboxInfo()
{
	// yeah
}

int XboxInfo::createThread(void *fObj)
{
	XboxInfo *fXboxInfo = (XboxInfo *) fObj;
	
	while (1) {
	   XSleep(2000);
	   fXboxInfo->readState();
	}
	return(0);
}

void XboxInfo::readState()
{
#ifdef ENABLE_XBOX
	int i;
	// HalReadSMBusValue touches 4 bytes
	char t[4];
	// read PIC Version
	HalWriteSMBusValue(0x20, 0x01, 0, 0x00);
	XSleep(50);
	for (i = 0; i < 3; i++) {
	   HalReadSMBusValue(0x20, 0x01, 0, t);
	   mPICVersion[i] = t[0];
	   XSleep(50);
	}
	// read tray state
	HalReadSMBusValue(0x20, 0x03, 0, t);
	mTrayState = t[0];
	XSleep(50);

	// read AVPack State
	HalReadSMBusValue(0x20, 0x04, 0, t);
	mAVPack = t[0];
	XSleep(50);
	
	// read Board Temperature
	HalReadSMBusValue(0x20, 0x09, 0, t);
	mBoardTemp = t[0];
	XSleep(50);

	// read CPU Temperature
	HalReadSMBusValue(0x20, 0x0A, 0, t);
	mCPUTemp = t[0];
	XSleep(50);
	
	// read Fan Speed
	HalReadSMBusValue(0x20, 0x10, 0, t);
	mFanSpeed = t[0];
	XSleep(50);	
	

#endif
}

char *XboxInfo::getPICVersion()
{
	return mPICVersion;
}

int XboxInfo::getTrayState()
{
	if (mTrayState == 1) {
	   return Tray_Reading;
	} else if (mTrayState == 49 || mTrayState == 65 || mTrayState == 33) {
	   return Tray_Opening;
	} else if (mTrayState == 17 || mTrayState == 81) {
	   return Tray_Closing;
	} else if (mTrayState == 64) {
	   return Tray_NoDisk;
	} else if (mTrayState == 96) {
	   return Tray_Disk;
	} else if (mTrayState == 16) {
	   return Tray_Open;
	}
	return mTrayState;
}

int XboxInfo::getAVPack()
{
	return mAVPack;
}

int XboxInfo::getBoardTemp()
{
	return mBoardTemp;
}

int XboxInfo::getCPUTemp()
{
	return mCPUTemp;
}

int XboxInfo::getFanSpeed()
{
	return mFanSpeed;
}

