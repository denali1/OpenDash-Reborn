#include <stdio.h>
#include <string.h>
#include "MenuNode.h"
#include "HotKey.h"
#include "Controller.h"
#include "Environment.h"

extern Environment *theEnv;

MenuNode::MenuNode()
{
    mName = 0;
    mStatusText = 0;
    mSkinID = 0;
    mParent = 0;
}

MenuNode::~MenuNode()
{
   if (mName) {
      delete[] mName;
   }
   if (mSkinID) { 
      delete[] mSkinID;
   }
   if (mStatusText) {
      delete[] mStatusText;
   }
}

void MenuNode::Execute()
{
	// yeah
}

void MenuNode::xmlConfigure(xmlNode *fNode)
{
	while (fNode != 0x00) {
	   if (!strcmp((char *) fNode->name, "Name") && fNode->children) {
	      setName((char *) fNode->children->content);
	   } else if (!strcmp((char *) fNode->name, "StatusText") && fNode->children) {
              setStatusText((char *) fNode->children->content);
           } else if (!strcmp((char *) fNode->name, "SkinID") && fNode->children) {
	      setSkinID((char *) fNode->children->content);
	   } else if (!strcmp((char *) fNode->name, "HotKey") && fNode->children) {
              theEnv->addHotKey(new HotKey(this, Controller::getButtonByName((char *) fNode->children->content)));
           }
	   fNode = fNode->next;
	}
}

void MenuNode::setName(char *fName)
{
	if (fName) {
	   if (mName) {
	      delete[] mName;
	   }
	   mName = new char[strlen(fName)+1];
	   strcpy(mName, fName);
	}
}

void MenuNode::setSkinID(char *fSkinID)
{
	if (fSkinID) {
	   if (mSkinID) {
	      delete[] mSkinID;
	   }
	   mSkinID = new char[strlen(fSkinID)+1];
	   strcpy(mSkinID, fSkinID);
	}
}

void MenuNode::setStatusText(char *fStatusText)
{
	if (fStatusText) {
    	   if (mStatusText) {
              delete[] mStatusText;
           }
           mStatusText = new char[strlen(fStatusText)+1];
           strcpy(mStatusText, fStatusText);
        }
}

void MenuNode::setParent(MenuNode *fParent)
{
	mParent = fParent;
}

char *MenuNode::getName()
{
	return mName;
}

char *MenuNode::getSkinID()
{
	return mSkinID;
}

char *MenuNode::getStatusText()
{
	return mStatusText;
}

MenuNode *MenuNode::getParent()
{
	return mParent;
}
