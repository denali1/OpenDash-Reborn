#include "MenuNode.h"
#include "SubMenuNode.h"
#include "FileMenu.h"
#include "LoadSkin.h"
#include "SkinMenu.h"

SkinMenu::SkinMenu() : FileMenu ()
{
	
}

SkinMenu::~SkinMenu()
{
	// yeah
}

void SkinMenu::xmlConfigure(xmlNode *fNode)
{
	FileMenu::xmlConfigure(fNode);
}

void SkinMenu::Execute()
{
	List<char> *fTargets = findFiles();
	List<char> *t = fTargets;
	
	mChildren = 0;
	
	while (t) {
	   if (t->getData()) {
              LoadSkin *p = new LoadSkin();
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
        SubMenuNode::Execute();
}	
