#include "MenuNode.h"
#include "SubMenuNode.h"
#include "FileMenu.h"
#include "XBEMenu.h"
#include "BootXBE.h"
#include "Environment.h"

#ifdef ENABLE_XBOX
   #include <openxdk/debug.h>
#else
   #define debugPrint printf
#endif

#include "debug.h"

extern Environment *theEnv;

XBEMenu::XBEMenu() : FileMenu()
{
	// yeah
}

XBEMenu::~XBEMenu()
{
	
}

void XBEMenu::xmlConfigure(xmlNode *fNode)
{
	FileMenu::xmlConfigure(fNode);
}

void XBEMenu::Execute()
{
	LOG_ENTRY("XBEMenu::Execute")
	
	if (!mChildren) {
	
  	   List<char> *fTargets = findFiles();
	   List<char> *t = fTargets;
	   
	   while (t) {
	      if (t->getData()) {
                 BootXBE *p = new BootXBE();
                 p->setPath(t->getData());
	         p->extractName(t->getData());
                 if (mChildren) {
                    mChildren->addItem(p);
                 } else {
                    mChildren = new List<MenuNode>(p, 0x00);
                 }
                 delete[] t->getData();
                 t->setData(0);
              }
	      t = t->getLink();
	   }
           delete fTargets;
        
	}
	
        SubMenuNode::Execute();
	
	LOG_EXIT("XBEMenu::Execute")
}
