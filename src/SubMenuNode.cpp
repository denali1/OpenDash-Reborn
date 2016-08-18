#include <string.h>
#include <stdio.h>

#include "Environment.h"

#include "MenuNode.h"
#include "SubMenuNode.h"
#include "FileMenu.h"

#include "XBEMenu.h"
#include "SkinMenu.h"
#include "BootXBE.h"
#include "LoadSkin.h"
#include "FileAction.h"
#include "XboxAction.h"
#include "LaunchDVD.h"
#include "DialogBox.h"

#include "List.h"

extern Environment *theEnv;

SubMenuNode::SubMenuNode() : MenuNode()
{
	mChildren = 0;
}

SubMenuNode::~SubMenuNode()
{
	if (mChildren) {
	   delete mChildren;
	}
}

void SubMenuNode::xmlConfigure(xmlNode *fNode)
{
	MenuNode::xmlConfigure(fNode);
	
	while (fNode) {
	   if (!strcmp((char *) fNode->name, "Children") && fNode->children) {
	      xmlNode *t = fNode->children->next;
	      while (t) {
	         MenuNode *tmp = theEnv->loadNode(t);
                 if (tmp) {
                    addChild(tmp);
                 }
                 t = t->next;
              }
	   }
	   fNode = fNode->next;
	}
}

void SubMenuNode::Execute()
{
	theEnv->setCurrentMenu(this);
}

void SubMenuNode::setChildren(List<MenuNode> *fChildren)
{
	mChildren = fChildren;
}

void SubMenuNode::addChild(MenuNode *fChild)
{
	fChild->setParent(this);
	
	if (mChildren) {
	   mChildren->addItem(fChild);
	} else {
	   mChildren = new List<MenuNode> (fChild, 0);
	}
}

List<MenuNode> *SubMenuNode::getChildren()
{
	return mChildren;
}
