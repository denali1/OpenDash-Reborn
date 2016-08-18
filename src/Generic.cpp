#include <string.h>
#include <stdio.h>

#include "Generic.h"
#include "List.h"
#include "Setting.h"

Generic::Generic()
{
	mSettings = 0;
}

Generic::~Generic()
{
	if (mSettings) {
	   delete mSettings;
	}
}

void Generic::xmlConfigure(xmlNode *fNode)
{

	while (fNode) {
	   if (fNode->type == XML_ELEMENT_NODE && fNode->name && fNode->children && fNode->children->content) {
	      addSetting(new Setting((char *) fNode->name, (char *) fNode->children->content));
	   }
	   fNode = fNode->next;
	}
}

void Generic::addSetting(Setting *fSetting)
{
	if (mSettings) {
	   mSettings->addItem(fSetting);
	} else {
	   mSettings = new List<Setting> (fSetting, 0);
	}
}

char *Generic::getValue(char *fVariable)
{
	List<Setting> *t = mSettings;
	while (t) {
	   if (t->getData() && !strcmp(t->getData()->getVariable(), fVariable)) {
	      return t->getData()->getStringValue();
	   }
	   t = t->getLink();
	}
	return 0;
}

List<Setting> *Generic::getSettings()
{
	return mSettings;
}
