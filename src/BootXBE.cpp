#include <string.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef ENABLE_XBOX
   #include <hal/xbox.h>
extern "C" {
   #include <hal/fileio.h>
}
#endif

#include "MenuNode.h"
//#include "ActionNode.h"
#include "BootXBE.h"
#include "Environment.h"
#include "debug.h"

extern Environment *theEnv;

BootXBE::BootXBE() : MenuNode()
{
	mPath = 0;
}

BootXBE::~BootXBE()
{
	if (mPath) {
	   delete[] mPath;
	}
}

void BootXBE::Execute()
{
	LOG_ENTRY("BootXBE::Execute()")
	
	struct stat sBuf;
	char msgbuf[255];
	
	if (mPath) {
	   stat(mPath, &sBuf);
	   if (!(sBuf.st_mode & S_IFREG)) {
	      sprintf(msgbuf, "File not found:\n%s", mPath);
	      theEnv->drawMessage("File not found", msgbuf);
	      return;
	   }
#ifdef ENABLE_XBOX
	   char path[255];
	   strcpy(path, mPath);
	   *strrchr(path, '/') = 0x00;
	   SDL_Quit();
	   XMountDrive('d', path);
	   XLaunchXBE(mPath);
	   
/*	   ANSI_STRING a1, a2;
	   	   
           RtlInitAnsiString(&a1, "\\??\\D:");
           RtlInitAnsiString(&a2, "\\Device\\CdRom0");
	   
           IoDeleteSymbolicLink(&a1);	      
           IoCreateSymbolicLink(&a1, &a2);
	      
           if (LaunchDataPage == NULL)
              LaunchDataPage = (_LAUNCH_DATA_PAGE *) MmAllocateContiguousMemory(0x1000);

           if (LaunchDataPage == NULL)
	      return;

           MmPersistContiguousMemory(LaunchDataPage, 0x1000, TRUE);

           memset((void*)LaunchDataPage, 0, 0x1000);
	
           LaunchDataPage->Header.dwLaunchDataType = 0xFFFFFFFF;
           LaunchDataPage->Header.dwTitleId = 0;

           strcpy(LaunchDataPage->Header.szLaunchPath, "\\Device\\CdRom0");
           strcat(LaunchDataPage->Header.szLaunchPath, ";");
           strcat(LaunchDataPage->Header.szLaunchPath, "default.xbe");

           HalReturnToFirmware(ReturnFirmwareQuickReboot);
*/
#endif
	}
}

void BootXBE::xmlConfigure(xmlNode *fNode)
{
	LOG_ENTRY("BootXBE::xmlConfigure")
	
        MenuNode::xmlConfigure(fNode);
	while (fNode) {
	   if (!strcmp((char *) fNode->name, "Path") && fNode->children) {
	      setPath((char *) fNode->children->content);
	   }
	   fNode = fNode->next;
	}
        if (!mName && mPath) {
           extractName(mPath);
        }
}

void BootXBE::extractName(char *fPath)
{
	LOG_ENTRY("BootXBE::extractName")
	
        char tmp[80];
        char *p, *q;
        unsigned int offset;
        unsigned int base;
       
#ifdef ENABLE_XBOX
	XSleep(5);
#endif
        FILE *fp = fopen(fPath, "r");
        fseek(fp, 0x104, SEEK_SET);
        fread(&base, sizeof(unsigned int), 1, fp);
        
        fseek(fp, 0x118, SEEK_SET);
        fread(&offset, sizeof(unsigned int), 1, fp);
        
        offset = offset - base + 0x0C;
        
        fseek(fp, offset, SEEK_SET);
        
        fread(tmp, sizeof(char), 80, fp);
        fclose(fp);
        

        if (tmp[0] == 0x00) {
	   extractDebugName(fPath);
	   return;
	}
	
	if (mName) {
           delete[] mName;
        }
        mName = new char[80];
        p = (char *) tmp;
        q = (char *) mName;
        while (*p != 0x00) {
           *(q++) = *(p++);
           p++;
        }
        *q = 0x00;
}

void BootXBE::extractDebugName(char *fPath)
{
	LOG_ENTRY("BootXBE::extractDebugName")
	
#ifdef ENABLE_XBOX
	XSleep(5);
#endif

        char tmp[80];
        char *p, *q;
        unsigned int offset;
        unsigned int base;
        
        FILE *fp = fopen(fPath, "r");
        fseek(fp, 0x104, SEEK_SET);
        fread(&base, sizeof(unsigned int), 1, fp);
        
        fseek(fp, 0x154, SEEK_SET);
        fread(&offset, sizeof(unsigned int), 1, fp);
        
        offset = offset - base;
        
        fseek(fp, offset, SEEK_SET);
        
        fread(tmp, sizeof(char), 80, fp);
        fclose(fp);
        	
	if (mName) {
           delete[] mName;
        }
        mName = new char[80];
        p = (char *) tmp;
        q = (char *) mName;
        while (*p != 0x00) {
           *(q++) = *(p++);
           p++;
        }
        *q = 0x00;
}	
	
void BootXBE::setPath(char *fPath)
{
	LOG_ENTRY("BootXBE::setPath")
	
	if (fPath) {
	   if (mPath) {
	      delete[] mPath;
	   }
	   mPath = new char[strlen(fPath)+1];
	   strcpy(mPath, fPath);
	}
}

char *BootXBE::getPath()
{
	return mPath;
}

