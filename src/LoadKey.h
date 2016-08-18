#ifndef __LOADKEY_H__
#define __LOADKEY_H__

#include <libxml/tree.h>
#include <libxml/parser.h>

class LoadKey : public MenuNode {

public:
   
   LoadKey();
   ~LoadKey();
   
   virtual void xmlConfigure(xmlNode *fNode);
   virtual void Execute();
   
   void setKeyFile(char *fKeyFile);
   char *getKeyFile();
   
private:
   
   void disableWP();
   void enableWP();
   
   char *mKeyFile;
   
};

#endif
