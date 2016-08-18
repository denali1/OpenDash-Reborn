#ifndef __BOOTXBE_H__
#define __BOOTXBE_H__

#include <libxml/tree.h>
#include <libxml/parser.h>

class BootXBE : public MenuNode {

public:

   BootXBE();
   ~BootXBE();
   
   virtual void Execute();
   virtual void xmlConfigure(xmlNode *fNode);
   
   void setPath(char *fPath);
   void extractName(char *fPath);
   void extractDebugName(char *fPath);
   
   char *getPath();

protected:
   
   char *mPath;
};

#endif
   
