#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Environment.h"
#include "MenuNode.h"

#include "DialogBox.h"
#include "QuestionBox.h"

extern Environment *theEnv;

QuestionBox::QuestionBox() : DialogBox()
{
    mNo = 0;
    mYes = 0;
    mTimeout = 0;
    mDefaultYes = -1;
}

QuestionBox::~QuestionBox()
{
    if (mNo) {
       delete mNo;
    }
    if (mYes) {
       delete mYes;
    }
}

void QuestionBox::xmlConfigure(xmlNode *fNode)
{
    DialogBox::xmlConfigure(fNode);
    
    while (fNode) {
       if (!strcmp((char *) fNode->name, "No") && fNode->children) {
          xmlNode *t = fNode->children;
          while (t) {
             MenuNode *tmp = theEnv->loadNode(t);
             if (tmp) {
                setNo(tmp);
             }
             t = t->next;
          }
          
       } else if (!strcmp((char *) fNode->name, "Yes") && fNode->children) {
          xmlNode *t = fNode->children;
          while (t) {
             MenuNode *tmp = theEnv->loadNode(t);
             if (tmp) {
                setYes(tmp);
             }
             t = t->next;
          }
       } else if (!strcmp(( char *) fNode->name, "Timeout") && fNode->children) {
          if (fNode->children->content) {
	     mTimeout = atoi((char *) fNode->children->content);
	  }
       } else if (!strcmp((char *) fNode->name, "Default") && fNode->children) {
          if (fNode->children->content) {
	     if (!strcmp((char *) fNode->children->content, "Yes")) {
	        mDefaultYes = 1;
             } else if (!strcmp((char *) fNode->children->content, "No")) {
	        mDefaultYes = 0;
	     }
	  }
       }
       fNode = fNode->next;
    }
}

void QuestionBox::Execute()
{
    int fResult = 0;
    if (mMessage) {
       if (mTitle) {
          fResult = theEnv->askTimedQuestion(mTitle, mMessage, mTimeout, mDefaultYes);
       } else {
          fResult = theEnv->askTimedQuestion("", mMessage, mTimeout, mDefaultYes);
       }
    }
    theEnv->drawScreen();
    if (fResult == 0 && mNo) {
       mNo->Execute();
    }
    if (fResult == 1 && mYes) {
       mYes->Execute();
    }
}

void QuestionBox::setNo(MenuNode *fNo)
{
    mNo = fNo;
}

void QuestionBox::setYes(MenuNode *fYes)
{
    mYes = fYes;
}
