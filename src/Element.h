#ifndef __ELEMENT_H__
#define __ELEMENT_H__

#include "SDL.h"

#include <libxml/tree.h>
#include <libxml/parser.h>

class Resource;

enum {
   CONTENT_BACKGROUND_IMAGE=1,
   CONTENT_MENU,
   CONTENT_MENUENTRY_BG,
   CONTENT_MENUENTRY_SELECTED_BG,
   CONTENT_MENUFONT,
   CONTENT_MENUFONT_SELECTED,
   CONTENT_MENUBROWSE_SOUND,
   CONTENT_MENUSELECT_SOUND,
   CONTENT_IMAGE,
   CONTENT_BOARDTEMP,
   CONTENT_CPUTEMP,
   CONTENT_TRAYSTATE,
   CONTENT_FANSPEED,
   CONTENT_MESSAGETEXT,
   CONTENT_MESSAGETITLE,
   CONTENT_MESSAGEBACKGROUND,
   CONTENT_QUESTIONTEXT,
   CONTENT_QUESTIONTITLE,
   CONTENT_QUESTIONBACKGROUND,
   CONTENT_QUESTIONTIMER,
   CONTENT_PROGRESSBACKGROUND,
   CONTENT_PROGRESSMESSAGE,
   CONTENT_PROGRESSTITLE,
   CONTENT_PROGRESSBAR,
   CONTENT_PROGRESSTEXT,
   CONTENT_STATUS,
};

enum {
   Align_Left,
   Align_Right,
   Align_Center
};

class Element {

public:

   Element();
   ~Element();
   
   virtual void xmlConfigure(xmlNode *fNode);
   void drawText(char *fText, SDL_Surface *screen);
   void drawImage(SDL_Surface *screen);
   
   
   void setDimensions(SDL_Rect fDimensions);
   void setLayer(int fLayer);
   void setContent(int fContent);
   void setSkinID(char *fSkinID);
   void setResourceName(char *fResourceName);
   void setResource(Resource *fResource);
   void setColor(SDL_Color fColor);
   void setAlign(int fAlign);
   void setRotation(int fRotation);
   
   SDL_Rect getDimensions();
   int getLayer();
   int getContent();
   char *getSkinID();
   char *getResourceName();
   Resource *getResource();
   SDL_Color getColor();
   int getAlign();
   int getRotation();
   
protected:
   
   SDL_Rect mDimensions;
   int mLayer;
   int mContent;
   char *mSkinID;
   char *mResourceName;
   Resource *mResource;
   SDL_Color mColor;
   int mAlign;
   int mRotation;
   SDL_Surface *mLastRender;
   char *mLastText;

};

#endif
