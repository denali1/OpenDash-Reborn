#include <stdio.h>
#include <string.h>

#include "Resource.h"

Resource::Resource()
{
	mName = 0;
	mAlpha = 0;
}

Resource::~Resource()
{
	if (mName) {
	   delete[] mName;
	}
}

void Resource::xmlConfigure(xmlNode *fNode)
{
	while (fNode) {
	   if (!strcmp((char *) fNode->name, "Name") && fNode->children) {
	      setName((char *) fNode->children->content);
	   } else if (!strcmp((char *) fNode->name, "Alpha") && fNode->children) {
	      setAlpha(atoi((char *) fNode->children->content));
	   }
	   fNode = fNode->next;
	}
}

void Resource::loadResource()
{
	// yeah
}

void Resource::setName(char *fName)
{
	if (fName) {
	   if (mName) {
	      delete[] mName;
	   }
	   mName = new char[strlen(fName)+1];
	   strcpy(mName, fName);
	}
}

void Resource::setAlpha(int fAlpha)
{
	mAlpha = fAlpha;
}

char *Resource::getName()
{
	return mName;
}

int Resource::getAlpha()
{
	return mAlpha;
}

