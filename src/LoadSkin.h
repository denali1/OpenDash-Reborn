#ifndef __LOADSKIN_H__
#define __LOADSKIN_H__

class LoadSkin : public MenuNode {

public:
   
   LoadSkin();
   ~LoadSkin();
   
   virtual void xmlConfigure(xmlNode *fNode);
   virtual void Execute();
   
   void extractName(char *fPath);
   void setPath(char *fPath);
   char *getPath();
   
protected:
   
   char *mPath;
   
};

#endif
