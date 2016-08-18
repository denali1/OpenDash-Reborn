#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "List.h"

#include "Resources.h"
#include "Resource.h"

#include "Elements.h"
#include "Element.h"



Elements::Elements()
{
    mElements = 0;
}

Elements::~Elements()
{
    if (mElements) {
       delete mElements;
    }
}

void Elements::xmlConfigure(xmlNode *fNode)
{
    while (fNode) {
       if (!strcmp((char *) fNode->name, "Element") && fNode->children) {
          Element *t = new Element();
	  t->xmlConfigure(fNode->children->next);
	  addElement(t);
       }
       fNode = fNode->next;
    }
}

void Elements::associateResources(Resources *fResources)
{
    if (fResources) {
       List<Element> *t = mElements;
       while (t) {
          t->getData()->setResource(fResources->getResourceByName(t->getData()->getResourceName()));
	  t = t->getLink();
       }
    }
}

void Elements::addElement(Element *fElement)
{
    if (mElements) {
       mElements->addItem(fElement);
    } else {
       mElements = new List<Element> (fElement, 0);
    }
}

Element *Elements::getElementByContent(int fContent, char *fSkinID)
{
	List<Element> *t = mElements;
	Element *fCur = 0;
	while (t) {
	   if (t->getData() && t->getData()->getContent() == fContent) {
	      if (!fCur) {
	         fCur = t->getData();
	      } else {
	         if (!fSkinID && !t->getData()->getSkinID()) {
		    fCur = t->getData();
		 } else if (fSkinID && t->getData()->getSkinID() && !strcmp(fSkinID, t->getData()->getSkinID())) {
		    fCur = t->getData();
		 }
	      }
	   }
	   t = t->getLink();
	}
	return fCur;
}

void Elements::setElements(List<Element> *fElements)
{
    mElements = fElements;
}

List<Element> *Elements::getElements()
{
    return mElements;
}
