#include <xboxkrnl/xboxkrnl.h>
#include <stdio.h>
#include <openxdk/debug.h>
#include <hal/xbox.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

void disableWP()
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


void enableWP()
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

void Execute(char *mKeyFile)
{
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
       debugPrint("    Read File: %d bytes\n", size);
       if (size == 276) {
          char *p = (char *) XePublicKeyData;
	  int i = 0;
	  debugPrint("      About to Execute!!!");
#ifdef ENABLE_XBOX
          disableWP();
	  debugPrint("       DisabledWP!\n");
	  
          while (i < 276) {
             *(p++) = buf[i++];
	  }
	  
	  debugPrint("       Enabling WP!\n");
          enableWP();
#endif
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
}

extern "C" {
void XBoxStartup()
{
	char *mem;
	char buf[128];
	mem = (char*) XePublicKeyData;
	int *tmp;
	tmp = (int*) mem;
	debugPrint("\n\n\n\n\n      About to Start!\n");
	
        Execute("d:/test.key");
	
	debugPrint("\n\n\n\n\n      Key:");
	for (int i = 0; i < 69; i++) {
	   debugPrint("%08x", *(tmp+i));
	   if (i % 6 == 0) {
	      debugPrint("\n      ");
	   }
	}
	
	FILE *fp = fopen("c:/key.out", "w");
	fwrite(XePublicKeyData, sizeof(char), 256+20, fp);
	fclose(fp);
	
	while (1) {
	   XSleep(100);
	}
}

}
