#include <string.h>
#include <stdio.h>

#ifdef ENABLE_XBOX
   #include <hal/xbox.h>
   #include <hal/fileio.h>
   #include <openxdk/debug.h>
   #include <xboxkrnl/xboxkrnl.h>
#endif

#include "MenuNode.h"

#include "XboxAction.h"

XboxAction::XboxAction() : MenuNode()
{
    mAction = 0;
    mOption = 0;
}

XboxAction::~XboxAction()
{
    // yeah
}

void XboxAction::xmlConfigure(xmlNode *fNode)
{
    MenuNode::xmlConfigure(fNode);
    while (fNode) {
       if (!strcmp((char *) fNode->name, "Action") && fNode->children && fNode->children->content) {
          if (!strcmp((char *) fNode->children->content, "Shutdown")) {
             setAction(Xbox_Shutdown);
          } else if (!strcmp((char *) fNode->children->content, "Reboot")) {
             setAction(Xbox_Reboot);
          } else if (!strcmp((char *) fNode->children->content, "OpenTray")) {
             setAction(Xbox_OpenTray);
          } else if (!strcmp((char *) fNode->children->content, "CloseTray")) {
             setAction(Xbox_CloseTray);
          } else if (!strcmp((char *) fNode->children->content, "BlinkLEDs")) {
             setAction(Xbox_BlinkLEDs);
          } else if (!strcmp((char *) fNode->children->content, "FanSpeed")) {
             setAction(Xbox_FanSpeed);
          }
       } else if (!strcmp((char *) fNode->name, "Option") && fNode->children) {
          setOption(atoi((char *) fNode->children->content));
       }
       fNode = fNode->next;
    }
}

void XboxAction::Execute()
{
#ifdef ENABLE_XBOX

    if (mAction == Xbox_Reboot) {
       XReboot();
    } else if (mAction == Xbox_Shutdown) {
	HalWriteSMBusValue(0x20, 0x02, 0, 0x80);
    } else if (mAction == Xbox_OpenTray) {
	HalWriteSMBusValue(0x20, 0x0C, 0, 0x00);
    } else if (mAction == Xbox_CloseTray) {
        HalWriteSMBusValue(0x20, 0x0C, 0, 0x01);
    } else if (mAction == Xbox_BlinkLEDs) {
        HalWriteSMBusValue(0x20, 0x08, 0, mOption);
        HalWriteSMBusValue(0x20, 0x07, 0, 1);
    } else if (mAction == Xbox_FanSpeed) {
        HalWriteSMBusValue(0x20, 0x06, 0, mOption);
        HalWriteSMBusValue(0x20, 0x07, 0, 1);
    }
    
#else
    if (mAction == Xbox_Reboot || mAction == Xbox_Shutdown) {
        exit(0);
    }
#endif
}

void XboxAction::setAction(int fAction)
{
    mAction = fAction;
}

void XboxAction::setOption(int fOption)
{
    mOption = fOption;
}

int XboxAction::getAction()
{
    return mAction;
}

int XboxAction::getOption()
{
    return mOption;
}
