#ifndef __SETTING_H__
#define __SETTING_H__

class Setting {
public:
  Setting();
  Setting(char *fVariable, char *fValue);
  
  ~Setting();
  
  void setVariable(char *fVariable);
  void setValue(char *fValue);
  
  char *getVariable();
  char *getStringValue();
  int getIntValue();
 
protected:

  char *mVariable;
  char *mValue;

};

#endif

