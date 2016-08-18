#ifdef ENABLE_XBOX
extern "C" {
   #include <hal/xbox.h>
   #include <hal/fileio.h>
   #include <openxdk/debug.h>
   #include <xboxkrnl/xboxkrnl.h>
}
#endif
#include <typeinfo>

#include "List.h"
#include "XboxInfo.h"
#include "MenuNode.h"

MenuNode *theMenu;

extern "C" void XBoxStartup()
{
   XboxInfo *t = new XboxInfo();
   List<XboxInfo> *o = new List<XboxInfo> (t, 0x00);
   t->readState();
   debugPrint("\n\n\nCPU Temp is %d\n", t->getCPUTemp());
   if (typeid(*t) == typeid(XboxInfo)) {
      
   }
   debugPrint("\n\n\n    Am I here?\n\n");
   XSleep(100000);
   XReboot();
}

