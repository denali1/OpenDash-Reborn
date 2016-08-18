#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef ENABLE_XBOX
extern "C" {
   #include <openxdk/debug.h>
}
#else
   #define debugPrint printf
#endif

#include "List.h"

#include "Resource.h"
#include "FontResource.h"
#include "ImageResource.h"
#include "Resources.h"


Resources::Resources()
{
	mResources = 0;
}

Resources::~Resources()
{
	if (mResources) {
	   delete mResources;
	}
}

void Resources::xmlConfigure(xmlNode *fNode)
{
	while (fNode) {
	   if (!strcmp((char *) fNode->name, "Font")) {
	      Resource *r = new FontResource();
	      r->xmlConfigure(fNode->children->next);
	      addResource(r);
	   } else if (!strcmp((char *) fNode->name, "Image")) {
	      Resource *t = new ImageResource();
	      t->xmlConfigure(fNode->children->next);
	      addResource(t);
	   }
	   fNode = fNode->next;
	}
	
}

void Resources::loadResources()
{
	List<Resource> *t = mResources;
	while (t) {
	   if (t->getData()) {
	      t->getData()->loadResource();
	   }
	   t = t->getLink();
	}
}

void Resources::setResources(List<Resource> *fResources)
{
	mResources = fResources;
}

void Resources::addResource(Resource *fResource)
{
	if (mResources) {
	   mResources->addItem(fResource);
	} else {
	   mResources = new List<Resource> (fResource, 0);
	}
}

Resource *Resources::getResourceByName(char *fName)
{
	if (fName) {
	   List<Resource> *t = mResources;
	   while (t) {
	      if (t->getData() && !strcmp(fName, t->getData()->getName())) {
	         return t->getData();
	      }
	      t = t->getLink();
	   }
	}
	return 0;
}

List<Resource> *Resources::getResources()
{
	return mResources;
}

