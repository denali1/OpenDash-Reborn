#include <stdio.h>
#include <string.h>

#include "MenuNode.h"
#include "LoadSkin.h"
#include "Environment.h"

extern Environment *theEnv;

LoadSkin::LoadSkin() : MenuNode()
{
    mPath = 0;
}

LoadSkin::~LoadSkin()
{
    if (mPath) {
       delete[] mPath;
    }
}

void LoadSkin::xmlConfigure(xmlNode *fNode)
{
    MenuNode::xmlConfigure(fNode);
    
    while (fNode) {
       if (!strcmp((char *) fNode->name, "Path")) {
          setPath((char *) fNode->children->content);
       }
       fNode = fNode->next;
    }
}

void LoadSkin::Execute()
{
    // yeah
    char buf[255];
    strcpy(buf, mPath);
    *(strrchr(buf, '/')+1) = 0x00;
    
    if (theEnv->loadSkin(buf)) {
       theEnv->saveSkinPath(buf);
    }
}

void LoadSkin::setPath(char *fPath)
{
    if (fPath) {
       if (mPath) {
          delete[] mPath;
       }
       mPath = new char[strlen(fPath)+1];
       strcpy(mPath, fPath);
    }
}

char *LoadSkin::getPath()
{
    return mPath;
}

void LoadSkin::extractName(char *fPath)
{
    char *p;
    char *buff = new char[strlen(fPath) + 1];
    strcpy(buff, fPath);
    if ((p = strrchr(buff, '/')) != NULL) {
       *p = '\0';
    }
    if ((p = strrchr(buff, '/')) != NULL) {
       this->setName(p + 1);
    } else {
       this->setName(buff);
    }
    delete [] buff;
}
