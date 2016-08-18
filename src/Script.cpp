#include <string.h>
#include "MenuNode.h"
#include "SubMenuNode.h"
#include "Script.h"

#include "Environment.h"

extern Environment *theEnv;

Script::Script() : SubMenuNode()
{
    // yeah
}

Script::~Script()
{
    // yeah
}

void Script::Execute()
{
    List<MenuNode> *fNode = mChildren;
    while (fNode) {
       if (fNode->getData()) {
          fNode->getData()->Execute();
          theEnv->renderScreen();
       }
       fNode = fNode->getLink();
    }
}

