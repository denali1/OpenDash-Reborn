#ifndef __FILEACTION_H__
#define __FILEACTION_H__

#include <libxml/tree.h>
#include <libxml/parser.h>

enum {
  File_Delete = 1,
  File_Move = 2,
  File_Copy = 3,
  File_CreateDirectory = 4,
  File_DeleteDirectory = 5,
  File_CopyDirectory = 6,
  File_Rename = 7
};

class FileAction : public MenuNode {

public:

   FileAction();
   ~FileAction();
   
   virtual void xmlConfigure (xmlNode *fNode);
   virtual void Execute();
   
   void copyFile(char *fSource, char *fDestination);
   void deleteFile(char *fFile);
   void copyDirectory(char *fSource, char *fDestination);
   void createDirectory(char *fSource);
   
   void setSource(char *fSource);
   void setDestination(char *fTarget);
   void setAction(int fAction);
   
   char *getSource();
   char *getDestination();
   int getAction();
   
private:

   
   char *mSource;
   char *mDestination;
   int mAction;

};

#endif
