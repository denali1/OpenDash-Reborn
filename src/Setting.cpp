#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Setting.h"

Setting::Setting()
{
	mVariable = 0;
	mValue = 0;
}

Setting::Setting(char *fVariable, char *fValue)
{
	if (fVariable) {
	   mVariable = new char[strlen(fVariable)+1];
	   strcpy(mVariable, fVariable);
	} else {
	   mVariable = 0;
	}
	
	if (fValue) {
	   mValue = new char[strlen(fValue)+1];
	   strcpy(mValue, fValue);
	} else {
	   mValue = 0;
	}
}

Setting::~Setting()
{
	if (mValue) {
	   delete[] mValue;
	}
	if (mVariable) {
	   delete[] mVariable;
	}
}

void Setting::setVariable(char *fVariable)
{
	if (fVariable) {
	   if (mVariable) {
	      delete[] mVariable;
	   }
	   mVariable = new char[strlen(fVariable)+1];
	   strcpy(mVariable, fVariable);
	}
}

void Setting::setValue(char *fValue)
{
	if (fValue) {
	   if (mValue) {
	      delete[] mValue;
	   }
	   mValue = new char[strlen(fValue)+1];
	   strcpy(mValue, fValue);
	}
}

char *Setting::getVariable()
{
	return mVariable;
}

char *Setting::getStringValue()
{
	return mValue;
}

int Setting::getIntValue()
{
	return atoi(mValue);
}

