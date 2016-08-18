#ifndef __ELEMENTS_H__
#define __ELEMENTS_H__

#include <libxml/tree.h>
#include <libxml/parser.h>

class Element;
class Resources;

#include "List.h"

class Elements {

public:
   Elements();
   ~Elements();
   
   virtual void xmlConfigure(xmlNode *fNode);
   
   void associateResources(Resources *fResources);
   
   void addElement(Element *fElement);
   Element *getElementByContent(int fContent, char *fSkinID);
   
   
   void setElements(List<Element> *fElements);
   
   List<Element> *getElements();
   
   
private:

  List<Element> *mElements;
  
};

#endif
