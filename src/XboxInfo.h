#ifndef __XBOXINFO_H__
#define __XBOXINFO_H__


enum {
  Video_SCART=0,
  Video_HDTV=1,
  Video_VGA=2,
  Video_RFU=3,
  Video_SVideo=4,
  Video_Undefined=5,
  Video_Standard=6,
  Video_Missing=7,
};

enum {
  Tray_Open=0,
  Tray_Closing,
  Tray_Opening,
  Tray_Empty,
  Tray_Reading,
  Tray_NoDisk,
  Tray_Disk
};


class XboxInfo {

public:
   XboxInfo();
   ~XboxInfo();

   static int createThread(void *fObj);
   
   void readState();
   
   char *getPICVersion();
   int getTrayState();
   int getAVPack();
   int getBoardTemp();
   int getCPUTemp();   
   int getFanSpeed();
   
private:
   
   char mPICVersion[3];
   int mTrayState;
   int mAVPack;
   int mBoardTemp;
   int mCPUTemp;
   int mFanSpeed;

};

#endif
