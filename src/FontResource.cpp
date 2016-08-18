#include <stdio.h>
#include <string.h>

#include "SDL.h"
#include "SDL_ttf.h"

#include "Resource.h"
#include "FontResource.h"

#include "Environment.h"
#ifdef ENABLE_XBOX
extern "C" {
   #include <openxdk/debug.h>
   #include <hal/xbox.h>
}
#else
   #define debugPrint printf
   #define XSleep SDL_Delay
#endif

extern Environment *theEnv;

FontResource::FontResource() : Resource()
{
	mFontFile = 0;
	mFont = 0;
	mFontSize = 20;
	mFontStyle = 0;
}

FontResource::~FontResource()
{
        debugPrint("In FontResource destructor\n");
	XSleep(200);
	if (mFontFile) {
	   delete[] mFontFile;
	}
	if (mFont) {
	   TTF_CloseFont(mFont);
	}
}

void FontResource::xmlConfigure(xmlNode *fNode)
{
	Resource::xmlConfigure(fNode);
	while (fNode) {
	   if (!strcmp((char *) fNode->name, "File") && fNode->children) {
	      setFontFile((char *) fNode->children->content);
	   } else if (!strcmp((char *) fNode->name, "Size") && fNode->children) {
	      setFontSize(atoi((char *) fNode->children->content));
	   } else if (!strcmp((char *) fNode->name, "Style") && fNode->children && fNode->children->content) {
	      if (!strcmp((char *) fNode->children->content, "Bold")) {
	         setFontStyle(getFontStyle() | TTF_STYLE_BOLD);
              } else if (!strcmp((char *) fNode->children->content, "Italic")) {
	         setFontStyle(getFontStyle() | TTF_STYLE_ITALIC);
	      } else if (!strcmp((char *) fNode->children->content, "Underline")) {
	         setFontStyle(getFontStyle() | TTF_STYLE_UNDERLINE);
              }
	   }
	   fNode = fNode->next;
	}
}

void FontResource::loadResource()
{
	char *fSkinPath = theEnv->getSkinPath();
        char buf[255];
        
	if (!mFontFile || !mFontSize) {
	   return;
	}
        if (fSkinPath) {
           sprintf(buf, "%s%s", fSkinPath, mFontFile);
        } else {
#ifdef ENABLE_XBOX
           sprintf(buf, "d:/%s", mFontFile);
#else
           sprintf(buf, "%s", mFontFile);
#endif
        }
        debugPrint("Loading font %s\n", buf);
	mFont = TTF_OpenFont(buf, mFontSize);
	TTF_SetFontStyle(mFont, mFontStyle);
}

void FontResource::setFontFile(char *fFontFile)
{
	if (fFontFile) {
	   if (mFontFile) {
	      delete[] mFontFile;
	   }
	   mFontFile = new char[strlen(fFontFile)+1];
	   strcpy(mFontFile, fFontFile);
	}
}

void FontResource::setFontSize(int fFontSize)
{
	mFontSize = fFontSize;
}

void FontResource::setFontStyle(int fFontStyle)
{
	mFontStyle = fFontStyle;
}

void FontResource::setFont(TTF_Font *fFont)
{
	mFont = fFont;
}

char *FontResource::getFontFile()
{
	return mFontFile;
}

int FontResource::getFontSize()
{
	return mFontSize;
}

TTF_Font *FontResource::getFont()
{
	return mFont;
}

int FontResource::getFontStyle()
{
	return mFontStyle;
}

