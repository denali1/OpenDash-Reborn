#ifdef ENABLE_XBOX
extern "C" {
   #include <hal/fileio.h>
   #include <openxdk/debug.h>
}
#endif

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include "debug.h"
#include "MenuNode.h"
#include "FileAction.h"
#include "Environment.h"
#include "Generic.h"
#include "SubMenuNode.h"
#include "Script.h"


extern Environment *theEnv;

FileAction::FileAction()
{
    LOG_ENTRY("FileAction::FileAction()")
    mSource = 0;
    mDestination = 0;
    mAction = 0;
}

FileAction::~FileAction()
{
    LOG_ENTRY("FileAction::~FileAction()")
    if (mSource) {
       delete[] mSource;
    }
    if (mDestination) {
       delete[] mDestination;
    }
    LOG_EXIT("FileAction::~FileAction()")
}

void FileAction::xmlConfigure(xmlNode *fNode)
{
    LOG_ENTRY("FileAction::xmlConfigure")
    MenuNode::xmlConfigure(fNode);
    while (fNode) {
       if (!strcmp((char *) fNode->name, "Source") && fNode->children) {
          setSource((char *) fNode->children->content);
       } else if (!strcmp((char *) fNode->name, "Destination") && fNode->children) {
          setDestination((char *) fNode->children->content);
       } else if (!strcmp((char *) fNode->name, "Action") && fNode->children && fNode->children->content) {
          if (!strcmp((char *) fNode->children->content, "Delete")) {
             setAction(File_Delete);
          } else if (!strcmp((char *) fNode->children->content, "Move")) {
             setAction(File_Move);
          } else if (!strcmp((char *) fNode->children->content, "Copy")) {
             setAction(File_Copy);
          } else if (!strcmp((char *) fNode->children->content, "CreateDirectory")) {
             setAction(File_CreateDirectory);
          } else if (!strcmp((char *) fNode->children->content, "DeleteDirectory")) {
             setAction(File_DeleteDirectory);
          } else if (!strcmp((char *) fNode->children->content, "CopyDirectory")) {
             setAction(File_CopyDirectory);
          } else if (!strcmp((char *) fNode->children->content, "Rename")) {
             setAction(File_Rename);
          }
       }
       fNode = fNode->next;
    }
}

void FileAction::Execute()
{
    LOG_ENTRY("FileAction::Execute");

    if (mAction == File_Delete) {
       deleteFile(mSource);
    } else if (mAction == File_Move) {
       copyFile(mSource, mDestination);
       deleteFile(mSource);
    } else if (mAction == File_Copy) {
       this->copyFile(mSource, mDestination);
    } else if (mAction == File_CreateDirectory) {
       //XCreateDirectory(mSource);
    } else if (mAction == File_DeleteDirectory) {
       //XDeleteDirectory(mSource);
    } else if (mAction == File_CopyDirectory) {
       copyDirectory(mSource, mDestination);
    } else if (mAction == File_Rename) {
       //XRenameFile(mSource, mDestination);
    }
    LOG_EXIT("FileAction::Execute")
}

void FileAction::copyFile(char *fSource, char *fDestination)
{
//    LOG_ENTRY("FileAction::copyFile")
    struct stat sBuf;
    char msgbuf[255];
    char *fbuf;
    int readSize;
    int writeSize;
    int fDrawFreq;
    int fCounter;
    int fLastDraw;
    int tmp;
    
    FILE *fpIn;
    FILE *fpOut;
    
    memset(&sBuf, 0, sizeof(struct stat));
    // check for source file.
    stat(fSource, &sBuf);
    if (!(sBuf.st_mode & S_IFREG)) {
       if (theEnv->getDialog() && theEnv->getDialog()->getValue("CopyFailed_SourceMissing")) {
          sprintf(msgbuf, theEnv->getDialog()->getValue("CopyFailed_SourceMissing"), fSource);
       } else {
          sprintf(msgbuf, "File Copy Failed\nMissing source file\n%s.", fSource);
       }
       theEnv->drawMessage("File Copy Failed", msgbuf);
       return;
    }
    
    
    // open source file
    fpIn = fopen(fSource, "rb");
    if (!fpIn) {
       if (theEnv->getDialog() && theEnv->getDialog()->getValue("CopyFailed_OpenSource")) {
          sprintf(msgbuf, theEnv->getDialog()->getValue("CopyFailed_OpenSource"), fSource);
       } else {
          sprintf(msgbuf, "File Copy Failed\ncould not open source file\n%s.", fSource);
       }
       theEnv->drawMessage("File Copy Failed", msgbuf);
       return;
    }
    
    // open destination file
    fpOut = fopen(fDestination, "wb");
    if (!fpOut) {
       fclose(fpIn);
       if (theEnv->getDialog() && theEnv->getDialog()->getValue("CopyFailed_OpenDestination")) {
          sprintf(msgbuf, theEnv->getDialog()->getValue("CopyFailed_OpenDestination"), fDestination);
       } else {
          sprintf(msgbuf, "File Copy Failed: could not open destination file %s.", fDestination);
       }
       theEnv->drawMessage("File Copy Failed", msgbuf);
       return;
    }
    
    fbuf = new char[4096];
    // how often draw the progress bar?
    if (sBuf.st_size < 100000) {
       fDrawFreq = 50;
    } else if (sBuf.st_size < 10000000) {
       fDrawFreq = 10;
    } else {
       fDrawFreq = 5;
    }
    fCounter = 0;
    fLastDraw = 0;
    
    sprintf(msgbuf, "Copying\n%s\nto\n%s", fSource, fDestination);
    
    readSize = fread(fbuf, sizeof(char), 4096, fpIn);
    while (readSize != 0) {
       fCounter += readSize;
       if ((tmp = (double) fCounter/sBuf.st_size*100) - fLastDraw > fDrawFreq) {
          fLastDraw += fDrawFreq;
          theEnv->renderScreen();
	  theEnv->drawProgressBar("File Copy", msgbuf, fLastDraw);
       }
	  
       writeSize = fwrite(fbuf, sizeof(char), readSize, fpOut);
       if (readSize != writeSize) {
          fclose(fpIn);
	  fclose(fpOut);
	  delete[] fbuf;
          if (theEnv->getDialog() && theEnv->getDialog()->getValue("CopyFailed_Unknown")) {
             sprintf(msgbuf, theEnv->getDialog()->getValue("CopyFailed_Unknown"), fSource, fDestination);
          } else {
             sprintf(msgbuf, "File Copy Failed: Error during copy from %s to %s.", fSource, fDestination);
          }
          theEnv->drawMessage("File Copy Failed", msgbuf);
          return;	  
       }
       readSize = fread(fbuf, sizeof(char), 4096, fpIn);
    }
    
    fclose(fpIn);
    fclose(fpOut);
    delete[] fbuf;
    LOG_EXIT("FileAction::copyFile")
}

void FileAction::copyDirectory(char *fSource, char *fDestination)
{
    LOG_ENTRY("FileAction;:copyDirectory")
    
    struct stat sBuf;
    char msgbuf[255];
    char titlebuf[255];
    char buf[255];
    char *p, *q;
    char *source;
    char *destination;
    
    struct dirent *curDirent;
    DIR *curDir;
    
    if (!fSource || !fDestination) {
       return;
    }
    
    // does source directory exist?
    stat(fSource, &sBuf);
    
    if (!(sBuf.st_mode & S_IFDIR)) {
       sprintf(msgbuf, "Directory not found:\n%s", fSource);
       theEnv->drawMessage("Directory not found", msgbuf);
       return;
    }
    
    // create destination directory
    createDirectory(fDestination);
    
    // salright? salright.
    Script *fNode = new Script();
    
    p = fSource + strlen(fSource) - 1;
    if (strcmp(fSource + strlen(fSource) - 1, "/")) {
       source = new char[strlen(fSource)+2];
       sprintf(source, "%s/", fSource);
    } else {
       source = new char[strlen(fSource)+1];
       strcpy(source, fSource);
    }
    
    if (strcmp(fDestination + strlen(fDestination) - 1, "/")) {
       destination = new char[strlen(fDestination)+2];
       sprintf(destination, "%s/", fDestination);
    } else {
       destination = new char[strlen(fDestination)+1];
       strcpy(destination, fDestination);
    }
    
    curDir = opendir(source);
    while ((curDirent = readdir(curDir)) != NULL) {
       if (strcmp(curDirent->d_name, ".") && strcmp(curDirent->d_name, "..")) {
          sprintf(buf, "%s%s", source, curDirent->d_name);
          stat(buf, &sBuf);
          if (sBuf.st_mode & S_IFREG) {
             FileAction *f = new FileAction();
   	     f->setAction(File_Copy);
	     f->setSource(buf);
	     sprintf(buf, "%s%s", destination, curDirent->d_name);
	     f->setDestination(buf);
	     fNode->addChild(f);
          } else if (sBuf.st_mode & S_IFDIR) {
             FileAction *f = new FileAction();
	     f->setAction(File_CopyDirectory);
	     f->setSource(buf);
	     sprintf(buf, "%s%s/", destination, curDirent->d_name);
	     f->setDestination(buf);
	     fNode->addChild(f);
          }
       }
    }
    closedir(curDir);
    fNode->Execute();
    
    delete fNode;
    delete[] source;
    delete[] destination;
    
    LOG_EXIT("FileAction::copyDirectory")

}

void FileAction::createDirectory(char *fSource)
{
   LOG_ENTRY("FileAction::createDirectory")
   
#ifdef ENABLE_XBOX
   XCreateDirectory(fSource);
#else
#ifdef WINDOWS
   mkdir(fSource);
#else
   mkdir(fSource, 0777);
#endif
#endif
   LOG_EXIT("FileAction::createDirectory")
}

void FileAction::deleteFile(char *fFile)
{
   LOG_ENTRY("FileAction::deleteFile")
   
#ifdef ENABLE_XBOX
    XDeleteFile(fFile);
#endif

  LOG_EXIT("FileAction::deleteFile")
}

void FileAction::setSource(char *fSource)
{
    if (fSource) {
       if (mSource) {
          delete[] mSource;
       }
       mSource = new char[strlen(fSource)+1];
       strcpy(mSource, fSource);
    }
}

void FileAction::setDestination(char *fDestination)
{
    if (fDestination) {
       if (mDestination) {
          delete[] mDestination;
       }
       mDestination = new char[strlen(fDestination)+1];
       strcpy(mDestination, fDestination);
    }
}

void FileAction::setAction(int fAction)
{
    mAction = fAction;
}

char *FileAction::getSource()
{
    return mSource;
}

char *FileAction::getDestination()
{
    return mDestination;
}

int FileAction::getAction()
{
    return mAction;
}

