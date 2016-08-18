#ifndef __Resource_H__
#define __Resource_H__

#include <libxml/tree.h>
#include <libxml/parser.h>

class Resource {

public:

   Resource();
   virtual ~Resource();
   
   virtual void xmlConfigure(xmlNode *fNode);
   virtual void loadResource();
   
   void setName(char *fName);
   void setAlpha(int fAlpha);
   char *getName();
   int getAlpha();
      
protected:
   
   char *mName;
   int mAlpha;
   
};

#endif
