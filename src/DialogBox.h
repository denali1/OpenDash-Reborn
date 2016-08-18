#ifndef __DIALOGBOX_H__
#define __DIALOGBOX_H__

#include <libxml/tree.h>
#include <libxml/parser.h>

class MenuNode;

class DialogBox : public MenuNode
{

public:

   DialogBox();
   virtual ~DialogBox();
   
   virtual void xmlConfigure(xmlNode *fNode);
   virtual void Execute();
   
   void setTitle(char *fTitle);
   void setMessage(char *fMessage);
   
   char *getTitle();
   char *getMessage();
   
protected:
   
   char *mMessage;
   char *mTitle;
   
};

#endif
