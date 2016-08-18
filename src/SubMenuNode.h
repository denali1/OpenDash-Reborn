#ifndef __SUBMENUNODE_H__
#define __SUBMENUNODE_H__

#include <libxml/tree.h>
#include <libxml/parser.h>

#include "List.h"
class MenuNode;

class SubMenuNode: public MenuNode {

public:

   SubMenuNode();
   ~SubMenuNode();
   
   virtual void xmlConfigure(xmlNode *fNode);
   virtual void Execute();
   
   List<MenuNode> *getChildren();
   void setChildren(List<MenuNode> *fChildren);
   void addChild(MenuNode *fNode);
   
protected:
  
   List<MenuNode> *mChildren;

};

#endif
