#ifndef __XBEMENU_H__
#define __XBEMENU_H__

#include <libxml/tree.h>
#include <libxml/parser.h>


class XBEMenu : public FileMenu {

public:

   XBEMenu();
   ~XBEMenu();
   
   virtual void xmlConfigure(xmlNode *fNode);
   
   virtual void Execute();
   
};

#endif

