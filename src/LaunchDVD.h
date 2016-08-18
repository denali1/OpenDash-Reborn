#ifndef __LAUNCHDVD_H__
#define __LAUNCHDVD_H__

#include <libxml/tree.h>
#include <libxml/parser.h>

class LaunchDVD : public MenuNode {

public:
   
   LaunchDVD();
   ~LaunchDVD();
   
   virtual void xmlConfigure(xmlNode *fNode);
   virtual void Execute();
   
   void setPath(char *fPath);
   char *getPath();
   
private:
   
   char *mPath;
   
};

#endif
