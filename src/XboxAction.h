#ifndef __XBOXACTION_H__
#define __XBOXACTION_H__

#include <libxml/tree.h>
#include <libxml/parser.h>

enum {
   Xbox_Shutdown = 1,
   Xbox_Reboot = 2,
   Xbox_OpenTray = 3,
   Xbox_CloseTray = 4,
   Xbox_BlinkLEDs = 5,
   Xbox_FanSpeed = 6,
};

class XboxAction : public MenuNode {

public:

   XboxAction();
   ~XboxAction();
   
   virtual void xmlConfigure(xmlNode *fNode);
   virtual void Execute();
   
   void setAction(int fAction);
   void setOption(int fOption);
   
   int getAction();
   int getOption();

protected:
   
   int mAction;
   int mOption;
   
};

#endif
