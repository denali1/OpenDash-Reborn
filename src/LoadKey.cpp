#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef ENABLE_XBOX
   #include <hal/xbox.h>
   #include <hal/fileio.h>
   #include <openxdk/debug.h>
   #include <xboxkrnl/xboxkrnl.h>
#endif

#include "MenuNode.h"
#include "LoadKey.h"

LoadKey::LoadKey() : MenuNode()
{
    mKeyFile = 0;
}

LoadKey::~LoadKey()
{
    if (mKeyFile) {
       delete[] mKeyFile;
    }
}

void LoadKey::xmlConfigure(xmlNode *fNode)
{
    MenuNode::xmlConfigure(fNode);
    
    while (fNode) {
       if (!strcmp((char *) fNode->name, "KeyFile") && fNode->children) {
          setKeyFile((char *) fNode->children->content);
       }
       fNode = fNode->next;
    }
}

void LoadKey::Execute()
{
#ifdef ENABLE_XBOX
    if (!mKeyFile) {
       return;
    }
    char *buf;
    struct stat sBuf;
    buf = new char[276];
    memset(&sBuf, 0x00, sizeof(struct stat));
    stat(mKeyFile, &sBuf);
    if (sBuf.st_mode & S_IFREG) {
       FILE *fp = fopen(mKeyFile, "r");
       int size = fread(buf, sizeof(char), 276, fp);
       fclose(fp);
       if (size == 276) {
          char *p = (char *) XePublicKeyData;
	  int i = 0;

          disableWP();
	  
          while (i < 276) {
             *(p++) = buf[i++];
	  }
	  
          enableWP();

       } else {
          // error : keyfile wrong size
	  delete[] buf;
	  return;
       }
       
    } else {
       // error: could not find keyfile
       delete[] buf;
       return;
    }
    delete[] buf;
#endif
}

void LoadKey::disableWP()
{
#ifdef ENABLE_XBOX
    __asm__("\
            cli; \
            mov %cr0, %ecx; \
            and $0x0FFFEFFFF, %ecx; \
            mov %ecx, %cr0; \
	    ");
#endif
}

void LoadKey::enableWP()
{
#ifdef ENABLE_XBOX
   __asm__("\
            mov %cr0, %ecx; \
	    or  $0x10000, %ecx; \
            mov %ecx, %cr0; \
            sti; \
	    ");
#endif
}

void LoadKey::setKeyFile(char *fKeyFile)
{
    if (fKeyFile) {
       if (mKeyFile) {
          delete[] mKeyFile;
       }
       mKeyFile = new char[strlen(fKeyFile)+1];
       strcpy(mKeyFile, fKeyFile);
    }
}

char *LoadKey::getKeyFile()
{
    return mKeyFile;
}
