#ifndef __MENUNODE_H__
#define __MENUNODE_H__

#include <libxml/tree.h>
#include <libxml/parser.h>

class MenuNode {

public:

   MenuNode();
   virtual ~MenuNode();
   
   virtual void xmlConfigure(xmlNode *fNode);
   virtual void Execute();
   
   void setName(char *fName);
   void setSkinID(char *fSkinID);
   void setStatusText(char *fStatusText);
   void setParent(MenuNode *fParent);
   
   char *getName();
   char *getSkinID();
   char *getStatusText();
   MenuNode *getParent();
   
protected:
   
   char *mName;
   char *mSkinID;
   char *mStatusText;
   MenuNode *mParent;
   
};

#endif
