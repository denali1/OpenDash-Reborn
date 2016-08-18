#include <stdio.h>

#include "MenuNode.h"
#include "BootXBE.h"
#include "Environment.h"

MenuNode *theMenu;

Environment *theEnv;

extern "C" int main()
{
   BootXBE *t = new BootXBE();
   t->setPath("/home/tburns/proj/od-beta/od.xbe");
   t->extractName("/home/tburns/proj/od-beta/od.xbe");
   
   printf("XBE name is: %s\n", t->getName());
   
  // XSleep(100000);
  // XReboot();
  return 0;
}

