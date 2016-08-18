#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <SDL.h>

#ifdef ENABLE_XBOX
   #include <openxdk/debug.h>
   #include <hal/xbox.h>
#else
   #define debugPrint printf
#endif

#include "MenuNode.h"
#include "SubMenuNode.h"
#include "FileMenu.h"
#include "Environment.h"

extern Environment *theEnv;


FileMenu::FileMenu()
{
	mDirectory = 0;
	mTarget = 0;
}

FileMenu::~FileMenu()
{
	if (mDirectory) {
	   delete[] mDirectory;
	}
	if (mTarget) {
	   delete[] mTarget;
	}
}

void FileMenu::Execute()
{
	// yeah
}

void FileMenu::xmlConfigure(xmlNode *fNode)
{
        SubMenuNode::xmlConfigure(fNode);
        while (fNode) {
           if (!strcmp((char *) fNode->name, "Directory") && fNode->children) {
              // add trailing "/" if needed
              if (!strcmp((char *) (fNode->children->content + strlen((char *) fNode->children->content) - 1), "/")) {
                 setDirectory((char *) fNode->children->content);
              } else {
                 char buf[255];
                 sprintf(buf, "%s/", (char *) fNode->children->content);
                 setDirectory(buf);
              }
           } else if (!strcmp((char *) fNode->name, "Target") && fNode->children) {
              setTarget((char *) fNode->children->content);
           }
           fNode = fNode->next;
        }
}

List<char> *FileMenu::findFiles()
{
	List<char> *t = 0;
        struct stat sBuf;
        struct dirent *curDirent;
        DIR *curDir;
        char buf[256];
        char *p;
	int i = 0;
        
        if (!mDirectory || !mTarget) {
           return 0;
        }
	
        // does mDirectory exist?
        stat(mDirectory, &sBuf);
        if (!(sBuf.st_mode & S_IFDIR)) {
           return 0;
        }
        
        curDir = opendir(mDirectory);
	
        if (curDir == NULL) {
           return 0;
        }
	
        while ((curDirent = readdir(curDir)) != NULL) {
           sprintf(buf, "%s%s/%s", mDirectory, curDirent->d_name, mTarget);
           memset(&sBuf, 0x00, sizeof(struct stat));
           stat(buf, &sBuf);
           if (sBuf.st_mode & S_IFREG) {

              p = new char[strlen(buf)+1];
              strcpy(p, buf);
              if (t) {
                 t->addItem(p);
              } else {
                 t = new List<char> (p, 0x00);
              }
           }
#ifdef ENABLE_XBOX
	   if (i++ == 5) {
	   
	      // let the other thread catch up?
	      theEnv->drawScreen();
	      i = 0;
	   }
#endif
        }
	closedir(curDir);
        
        return t;
}

void FileMenu::setDirectory(char *fDirectory)
{
	if (fDirectory) {
	   if (mDirectory) {
	      delete[] mDirectory;
	   }
	   mDirectory = new char[strlen(fDirectory)+1];
	   strcpy(mDirectory, fDirectory);
	}
}

void FileMenu::setTarget(char *fTarget)
{
	if (fTarget) {
	   if (mTarget) {
	      delete[] mTarget;
	   }
	   mTarget = new char[strlen(fTarget)+1];
	   strcpy(mTarget, fTarget);
	}
}

char *FileMenu::getDirectory()
{
	return mDirectory;
}
	 
char *FileMenu::getTarget()
{
	return mTarget;
}

