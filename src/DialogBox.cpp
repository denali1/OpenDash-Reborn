#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Environment.h"
#include "MenuNode.h"

#include "DialogBox.h"

extern Environment *theEnv;

DialogBox::DialogBox() : MenuNode()
{
    mMessage = 0;
    mTitle = 0;
}

DialogBox::~DialogBox()
{
    if (mMessage) {
       delete[] mMessage;
    }
    if (mTitle) {
       delete[] mTitle;
    }
}

void DialogBox::xmlConfigure(xmlNode *fNode)
{
    MenuNode::xmlConfigure(fNode);
    
    while (fNode) {
       if (!strcmp((char *) fNode->name, "Title") && fNode->children) {
          setTitle((char *) fNode->children->content);
       } else if (!strcmp((char *) fNode->name, "Message") && fNode->children) {
          setMessage((char *) fNode->children->content);
       }
       fNode = fNode->next;
    }
}

void DialogBox::Execute()
{
    if (mMessage) {
       if (mTitle) {
          theEnv->drawMessage(mTitle, mMessage);
       } else {
          theEnv->drawMessage("", mMessage);
       }
    }
}

void DialogBox::setTitle(char *fTitle)
{
    if (fTitle) {
       if (mTitle) {
          delete[] mTitle;
       }
       mTitle = new char[strlen(fTitle)+1];
       strcpy(mTitle, fTitle);
    }
}

void DialogBox::setMessage(char *fMessage)
{
    if (fMessage) {
       if (mMessage) {
          delete[] mMessage;
       }
       mMessage = new char[strlen(fMessage)+1];
       strcpy(mMessage, fMessage);
    }
}
        
char *DialogBox::getTitle()
{
    return mTitle;
}

char *DialogBox::getMessage()
{
    return mMessage;
}
