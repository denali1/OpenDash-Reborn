#ifndef __SKINMENU_H__
#define __SKINMENU_H__

#include <libxml/tree.h>
#include <libxml/parser.h>

class SkinMenu : public FileMenu {

public:
  SkinMenu();
  ~SkinMenu();
  
  virtual void xmlConfigure (xmlNode *fNode);
  virtual void Execute();
  
protected:

};

#endif

