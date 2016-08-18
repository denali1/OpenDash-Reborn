#ifndef __FONTRESOURCE_H__
#define __FONTRESOURCE_H__

#include "SDL_ttf.h"

class Resource;

class FontResource : public Resource {

public:

   FontResource();
   ~FontResource();
   
   virtual void loadResource();
   virtual void xmlConfigure(xmlNode *fNode);
   
   void setFontFile(char *fFontFile);
   void setFontSize(int fFontSize);
   void setFontStyle(int fFontStyle);
   void setFont(TTF_Font *fFont);
   
   TTF_Font *getFont();
   int getFontSize();
   int getFontStyle();
   char *getFontFile();
   
protected:
   
   char *mFontFile;
   int mFontSize;
   int mFontStyle;
   TTF_Font *mFont;
      
};

#endif
