#ifndef __QUESTIONBOX_H__
#define __QUESTIONBOX_H__

#include <libxml/tree.h>
#include <libxml/parser.h>

class MenuNode;

class QuestionBox : public DialogBox
{

public:

   QuestionBox();
   virtual ~QuestionBox();
   
   virtual void xmlConfigure(xmlNode *fNode);
   virtual void Execute();
   
   void setNo(MenuNode *fNo);
   void setYes(MenuNode *fYes);
   
   MenuNode *getNo();
   MenuNode *getYes();
   
private:
   
   MenuNode *mNo;
   MenuNode *mYes;
   int mTimeout;
   int mDefaultYes;

};

#endif
