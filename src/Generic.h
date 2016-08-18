#ifndef __GENERIC_H__
#define __GENERIC_H__

#include <libxml/tree.h>
#include <libxml/parser.h>

#include "List.h"

class Setting;

class Generic {

public:

   Generic();
   ~Generic();
   
   virtual void xmlConfigure(xmlNode *fNode);
   
   void addSetting(Setting *fSetting);
   char *getValue(char *fVariable);
   
   List<Setting> *getSettings();
   
protected:
   
   List<Setting> *mSettings;
   
};

#endif
   
