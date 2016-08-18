#include <string.h>
#include <stdio.h>

#ifdef ENABLE_XBOX
   #include <hal/xbox.h>
   #include <hal/fileio.h>
   #include <openxdk/debug.h>
   #include <xboxkrnl/xboxkrnl.h>
#endif


#include "MenuNode.h"
#include "LaunchDVD.h"

LaunchDVD::LaunchDVD() : MenuNode()
{
    mPath = 0;
}

LaunchDVD::~LaunchDVD()
{
    if (mPath) {
       delete[] mPath;
    }
}

void LaunchDVD::xmlConfigure(xmlNode *fNode)
{
    MenuNode::xmlConfigure(fNode);
    
    while (fNode) {
       if (!strcmp((char *) fNode->name, "Path") && fNode->children) {
          setPath((char *) fNode->children->content);
       }
       fNode = fNode->next;
    }
}

void LaunchDVD::Execute()
{

#ifdef ENABLE_XBOX
    ANSI_STRING a1, a2;
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
    
#endif

}

void LaunchDVD::setPath(char *fPath)
{
    if (fPath) {
       if (mPath) {
          delete[] mPath;
       }
       mPath = new char[strlen(fPath)+1];
       strcpy(mPath, fPath);
    }
}

char *LaunchDVD::getPath()
{
    return mPath;
}
