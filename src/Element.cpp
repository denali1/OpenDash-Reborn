#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"
#include "Resource.h"
#include "FontResource.h"
#include "ImageResource.h"
#include "Element.h"

Element::Element()
{
    mDimensions.w = 0;
    mDimensions.h = 0;
    mDimensions.x = 0;
    mDimensions.y = 0;
    mLayer = 0;
    mContent = 0;
    mSkinID = 0;
    mResourceName = 0;
    memset(&mColor, 0x00, sizeof(SDL_Color));
    mAlign = Align_Left;
    mLastRender = 0;
    mLastText = 0;
    mResource = 0;
}

Element::~Element()
{
    if (mSkinID) {
       delete[] mSkinID;
    }
    if (mResourceName) {
       delete[] mResourceName;
    }
    if (mLastRender) {
       SDL_FreeSurface(mLastRender);
    }
    if (mLastText) {
       delete[] mLastText;
    }
}

void Element::xmlConfigure(xmlNode *fNode)
{
    while (fNode) {
       if (!strcmp((char *) fNode->name, "Dimensions") && fNode->children) {
          xmlNode *t = fNode->children->next;
          while (t) {
             if (!strcmp((char *) t->name, "Width") && t->children) {
                mDimensions.w = atoi((char *) t->children->content);
             } else if (!strcmp((char *) t->name, "Height") && t->children) {
                mDimensions.h = atoi((char *) t->children->content);
             } else if (!strcmp((char *) t->name, "Top") && t->children) {
                mDimensions.y = atoi((char *) t->children->content);
             } else if (!strcmp((char *) t->name, "Left") && t->children) {
                mDimensions.x = atoi((char *) t->children->content);
             }
             t = t->next;
          }
       } else if (!strcmp((char *) fNode->name, "Layer") && fNode->children) {
          setLayer(atoi((char *) fNode->children->content));
       } else if (!strcmp((char *) fNode->name, "SkinID") && fNode->children) {
          setSkinID((char *) fNode->children->content);
       } else if (!strcmp((char *) fNode->name, "Resource") && fNode->children) {
          setResourceName((char *) fNode->children->content);
       } else if (!strcmp((char *) fNode->name, "Content") && fNode->children && fNode->children->content) {
          if (!strcmp((char *) fNode->children->content, "BackgroundImage")) {
             setContent(CONTENT_BACKGROUND_IMAGE);
          } else if (!strcmp((char *) fNode->children->content, "MenuEntryBG")) {
             setContent(CONTENT_MENUENTRY_BG);
          } else if (!strcmp((char *) fNode->children->content, "MenuEntrySelectedBG")) {
             setContent(CONTENT_MENUENTRY_SELECTED_BG);
          } else if (!strcmp((char *) fNode->children->content, "MenuFont")) {
             setContent(CONTENT_MENUFONT);
          } else if (!strcmp((char *) fNode->children->content, "MenuSelectedFont")) {
             setContent(CONTENT_MENUFONT_SELECTED);
          } else if (!strcmp((char *) fNode->children->content, "MenuBrowseSound")) {
             setContent(CONTENT_MENUBROWSE_SOUND);
          } else if (!strcmp((char *) fNode->children->content, "MenuSelectSound")) {
             setContent(CONTENT_MENUSELECT_SOUND);
          } else if (!strcmp((char *) fNode->children->content, "Menu")) {
             setContent(CONTENT_MENU);
          } else if (!strcmp((char *) fNode->children->content, "BoardTemp")) {
	     setContent(CONTENT_BOARDTEMP);
	  } else if (!strcmp((char *) fNode->children->content, "CPUTemp")) {
	     setContent(CONTENT_CPUTEMP);
	  } else if (!strcmp((char *) fNode->children->content, "TrayState")) {
	     setContent(CONTENT_TRAYSTATE);
	  } else if (!strcmp((char *) fNode->children->content, "FanSpeed")) {
	     setContent(CONTENT_FANSPEED);
	  } else if (!strcmp((char *) fNode->children->content, "MessageText")) {
             setContent(CONTENT_MESSAGETEXT);
          } else if (!strcmp((char *) fNode->children->content, "MessageTitle")) {
             setContent(CONTENT_MESSAGETITLE);
          } else if (!strcmp((char *) fNode->children->content, "MessageBackground")) {
             setContent(CONTENT_MESSAGEBACKGROUND);
          } else if (!strcmp((char *) fNode->children->content, "QuestionText")) {
             setContent(CONTENT_QUESTIONTEXT);
          } else if (!strcmp((char *) fNode->children->content, "QuestionTitle")) {
             setContent(CONTENT_QUESTIONTITLE);
          } else if (!strcmp((char *) fNode->children->content, "QuestionBackground")) {
             setContent(CONTENT_QUESTIONBACKGROUND);
          } else if (!strcmp((char *) fNode->children->content, "QuestionTimer")) {
	     setContent(CONTENT_QUESTIONTIMER);
	  } else if (!strcmp((char *) fNode->children->content, "Status")) {
             setContent(CONTENT_STATUS);
          } else if (!strcmp((char *) fNode->children->content, "ProgressBarBackground")) {
	     setContent(CONTENT_PROGRESSBACKGROUND);
	  } else if (!strcmp((char *) fNode->children->content, "ProgressBarPercent")) {
	     setContent(CONTENT_PROGRESSTEXT);
	  } else if (!strcmp((char *) fNode->children->content, "ProgressBarTitle")) {
	     setContent(CONTENT_PROGRESSTITLE);
	  } else if (!strcmp((char *) fNode->children->content, "ProgressBar")) {
	     setContent(CONTENT_PROGRESSBAR);
	  } else if (!strcmp((char *) fNode->children->content, "ProgressBarText")) {
	     setContent(CONTENT_PROGRESSMESSAGE);
	  }
       } else if (!strcmp((char *) fNode->name, "Color") && fNode->children) {
          xmlNode *t = fNode->children->next;
	  SDL_Color j;
	  memset(&j, 0x00, sizeof(SDL_Color));
	  while (t) {
	     if (!strcmp((char *) t->name, "Red") && t->children) {
	        j.r = atoi((char *) t->children->content);
             } else if (!strcmp((char *) t->name, "Green") && t->children) {
	        j.g = atoi((char *) t->children->content);
             } else if (!strcmp((char *) t->name, "Blue") && t->children) {
	        j.b = atoi((char *) t->children->content);
             } else if (!strcmp((char *) t->name, "Alpha") && t->children) {
	        j.unused = atoi((char *) t->children->content);
             }
	     t = t->next;
	  }
	  
	  
          setColor(j);
       } else if (!strcmp((char *) fNode->name, "Align") && fNode->children) {
          if (!strcmp((char *) fNode->children->content, "Center") || !strcmp((char *) fNode->children->content, "Centre")) {
             setAlign(Align_Center);
          } else if (!strcmp((char *) fNode->children->content, "Left")) {
             setAlign(Align_Left);
          } else if (!strcmp((char *) fNode->children->content, "Right")) {
             setAlign(Align_Right);
          }
       } else if (!strcmp((char *) fNode->name, "Rotation") && fNode->children) {
          setRotation(atoi((char *) fNode->children->content));
       }
       fNode = fNode->next;
    }
}

void Element::drawText(char *fText, SDL_Surface *screen)
{
    TTF_Font *tFont;
    int fY;
    char *p;
    char *q;
    char *buf;
    
    if (!mResource || !(tFont = ((FontResource *) mResource)->getFont())) {
       return;
    }
    
    SDL_Surface *fTextSurface;
    
    // use optimized blitting routine if we're using a single-line fText.
    if (!strchr(fText, '\n')) {
       // use buffered text render if we can
       if (mLastText && !strcmp(fText, mLastText) && mLastRender) {
          fTextSurface = mLastRender;
       } else {
          if (mLastRender) {
             SDL_FreeSurface(mLastRender);
          }
          fTextSurface = TTF_RenderText_Blended(tFont, fText, mColor);
          if (mLastText) {
             delete[] mLastText;
          }
          mLastText = new char[strlen(fText)+1];
          strcpy(mLastText, fText);
          mLastRender = fTextSurface;
       }
    
       // deal with max alpha, rotation, flip, tilt.
    
       if (fTextSurface) {
          SDL_Rect q,r;
          q = mDimensions;
          if (mAlign == Align_Right) {
             q.x = mDimensions.x + mDimensions.w - fTextSurface->w;
          }
          if (mAlign == Align_Center) {
             q.x = mDimensions.x + ((mDimensions.w - fTextSurface->w)/2);
          }
          r.x = 0;
          r.y = 0;
          r.w = fTextSurface->w;
          r.h = fTextSurface->h;
          SDL_BlitSurface(fTextSurface, &r, screen, &q);
       }
    } else {
       // cut up fText line by line, blitting each line.
       // keep track of the end of the text.
       buf = new char[strlen(fText)+1];
       strcpy(buf, fText);
       q = buf + strlen(buf);
       p = buf;
       fY = mDimensions.y;
       while (p < q || !(p >= q-1)) {
          if (strchr(p, '\n')) {
             *strchr(p, '\n') = 0x00;
	  }
	  fTextSurface = TTF_RenderText_Blended(tFont, p, mColor);

          if (fTextSurface) {
             SDL_Rect q,r;
             q = mDimensions;
	     q.y = fY;
	     q.h = fTextSurface->h;
             if (mAlign == Align_Right) {
                q.x = mDimensions.x + mDimensions.w - fTextSurface->w;
             }
             if (mAlign == Align_Center) {
                q.x = mDimensions.x + ((mDimensions.w - fTextSurface->w)/2);
             }
             r.x = 0;
             r.y = 0;
             r.w = fTextSurface->w;
             r.h = fTextSurface->h;
             SDL_BlitSurface(fTextSurface, &r, screen, &q);
	     fY += fTextSurface->h + 5;
	     SDL_FreeSurface(fTextSurface);
          }
	  p += strlen(p)+1;
       }
       
       // grab next line of text
       // render and blit it
       // increase Y position
       delete[] buf;
    }
    
}

void Element::drawImage(SDL_Surface *screen)
{
    SDL_Surface *fImage;
    
    if (!mResource || !(fImage = ((ImageResource *) mResource)->getImage())) {
       return;
    }
    
    // deal with max alpha
    if (fImage) {
       SDL_Rect q,r;
       q = mDimensions;
       r.x = 0;
       r.y = 0;
       r.w = q.w;
       r.h = q.h;
       SDL_BlitSurface(fImage, &r, screen, &q);
    }
}

void Element::setDimensions(SDL_Rect fDimensions)
{
    mDimensions = fDimensions;
}

void Element::setLayer(int fLayer)
{
    mLayer = fLayer;
}

void Element::setContent(int fContent)
{
    mContent = fContent;
}

void Element::setSkinID(char *fSkinID)
{
    if (fSkinID) {
       if (mSkinID) {
          delete[] mSkinID;
       }
       
       mSkinID = new char[strlen(fSkinID)+1];
       strcpy(mSkinID, fSkinID);
    }
}

void Element::setResourceName(char *fResourceName)
{
    if (fResourceName) {
       if (mResourceName) {
          delete[] mResourceName;
       }
       
       mResourceName = new char[strlen(fResourceName)+1];
       strcpy(mResourceName, fResourceName);
    }
}

void Element::setResource(Resource *fResource)
{
    mResource = fResource;
}

void Element::setColor(SDL_Color fColor)
{
    mColor = fColor;
}

void Element::setAlign(int fAlign)
{
    mAlign = fAlign;
}

void Element::setRotation(int fRotation)
{
    mRotation = fRotation;
}

SDL_Rect Element::getDimensions()
{
    return mDimensions;
}

int Element::getLayer()
{
    return mLayer;
}

int Element::getContent()
{
    return mContent;
}

char *Element::getSkinID()
{
    return mSkinID;
}

char *Element::getResourceName()
{
    return mResourceName;
}

Resource *Element::getResource()
{
    return mResource;
}

SDL_Color Element::getColor()
{
    return mColor;
}

int Element::getAlign()
{
    return mAlign;
}

int Element::getRotation()
{
    return mRotation;
}
