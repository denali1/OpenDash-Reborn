#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include <libxml/tree.h>
#include <libxml/parser.h>

class Resource;

class Resources {

public:

   Resources();
   ~Resources();
   
   void xmlConfigure(xmlNode *fNode);
   
   void loadResources();
   
   void setResources(List<Resource> *fResources);
   void addResource(Resource *fResource);
   
   Resource *getResourceByName(char *fName);
   List<Resource> *getResources();
   
protected:

   List<Resource> *mResources;

};

#endif
