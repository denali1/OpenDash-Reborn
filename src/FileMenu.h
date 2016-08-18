#ifndef __FILEMENU_H__
#define __FILEMENU_H__

#include <libxml/tree.h>
#include <libxml/parser.h>


class FileMenu : public SubMenuNode {

public:

   FileMenu();
   ~FileMenu();
   
   virtual void xmlConfigure(xmlNode *fNode);
   virtual void Execute();
   
   List<char> *findFiles();
   
   void setDirectory(char *fDirectory);
   void setTarget(char *fTarget);
   
   char *getDirectory();
   char *getTarget();
protected:
   
   char *mDirectory;
   char *mTarget;
   

};

#endif
