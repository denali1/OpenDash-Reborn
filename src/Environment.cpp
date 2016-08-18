#include <string.h>
#include <stdio.h>
#ifndef NO_TYPEINFO
  #include <typeinfo>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef ENABLE_XBOX
   #include <openxdk/debug.h>
extern "C" {
   #include <hal/xbox.h>
}
#else
   #define debugPrint printf
#endif

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "MenuNode.h"
#include "SubMenuNode.h"
#include "Resources.h"
#include "Resource.h"
#include "Elements.h"
#include "Element.h"
#include "FontResource.h"
#include "ImageResource.h"
#include "XboxInfo.h"
#include "Controller.h"
#include "HotKey.h"
#include "Environment.h"
#include "Generic.h"
#include "Setting.h"

#include "FileMenu.h"
#include "XBEMenu.h"
#include "SkinMenu.h"
#include "BootXBE.h"
#include "LoadSkin.h"
#include "FileAction.h"
#include "XboxAction.h"
#include "LaunchDVD.h"
#include "DialogBox.h"
#include "QuestionBox.h"
#include "Script.h"
#include "LoadKey.h"

Environment::Environment()
{
    mMenu = 0;
    mTop = 0;
    mResources = 0;
    mScreen = 0;
    mMenuSurface = 0;
    mMenuBackground = 0;
    mBuffer = 0;
    mElements = 0;
    mSkinPath = 0;
    mXboxInfo = 0;
    mController = 0;
    mHotKeys = 0;
    mControllerThread = 0;
    mSelectedNode = 0;
    mMenuOffset = 0;
    mMenuSpeed = 0;
    mMenuSelected = 0;
    mDialog = 0;
    mMenuState = State_Normal;
    mLogFileName = 0;

}

Environment::~Environment()
{
    if (mTop) {
       delete mTop;
    }
    if (mResources) {
       delete mResources;
    }
    if (mResources) {
       delete mResources;
    }
    if (mElements) {
       delete mElements;
    }
    if (mSkinPath) {
       delete[] mSkinPath;
    }
    if (mXboxInfo) {
       delete mXboxInfo;
    }
    if (mController) {
       delete mController;
    }
    if (mHotKeys) {
       delete mHotKeys;
    }
    if (mBuffer) {
       SDL_FreeSurface(mBuffer);
    }
    if (mDialog) {
       delete mDialog;
    }
}

MenuNode *Environment::loadNode(xmlNode *fNode)
{
   MenuNode *tmp = 0;
   if (!strcmp((char *) fNode->name, "XBEMenu") && fNode->children) {
      tmp = new XBEMenu();
      tmp->xmlConfigure(fNode->children);
   } else if (!strcmp((char *) fNode->name, "SubMenu") && fNode->children) {
      tmp = new SubMenuNode();
      tmp->xmlConfigure(fNode->children);
   } else if (!strcmp((char *) fNode->name, "SkinMenu") && fNode->children) {
      tmp = new SkinMenu();
      tmp->xmlConfigure(fNode->children);
   } else if (!strcmp((char *) fNode->name, "BootXBE") && fNode->children) {
      tmp = new BootXBE();
      tmp->xmlConfigure(fNode->children);
   } else if (!strcmp((char *) fNode->name, "LoadSkin") && fNode->children) {
      tmp = new LoadSkin();
      tmp->xmlConfigure(fNode->children);
   } else if (!strcmp((char *) fNode->name, "FileAction") && fNode->children) {
      tmp = new FileAction();
      tmp->xmlConfigure(fNode->children);
   } else if (!strcmp((char *) fNode->name, "XboxAction") && fNode->children) {
      tmp = new XboxAction();
      tmp->xmlConfigure(fNode->children);
   } else if (!strcmp((char *) fNode->name, "LaunchDVD") && fNode->children) {
      tmp = new LaunchDVD();
      tmp->xmlConfigure(fNode->children);
   } else if (!strcmp((char *) fNode->name, "DialogBox") && fNode->children) {
      tmp = new DialogBox();
      tmp->xmlConfigure(fNode->children);
   } else if (!strcmp((char *) fNode->name, "QuestionBox") && fNode->children) {
      tmp = new QuestionBox();
      tmp->xmlConfigure(fNode->children);
   } else if (!strcmp((char *) fNode->name, "Script") && fNode->children) {
      tmp = new Script();
      tmp->xmlConfigure(fNode->children);
   } else if (!strcmp((char *) fNode->name, "LoadKey") && fNode->children) {
      tmp = new LoadKey();
      tmp->xmlConfigure(fNode->children);
   }
   return tmp;
}

int Environment::loadSkin(char *fSkinDirectory)
{
	
	Elements *fOldElements = mElements;
	Resources *fOldResources = mResources;
	
	struct stat sbuf;
	
	char buf[255];
	sprintf(buf, "%sskin.xml", fSkinDirectory);
	
	printf("Loading %s\n", buf);
	if (stat(buf, &sbuf)) {
	   return 0;
	}
	
       xmlDoc *theDoc = 0;
       xmlNode *theRoot = 0;
       
       theDoc = xmlReadFile(buf, NULL, 0);
       if (theDoc == NULL) {
          return 0;
       }
       
       theRoot = xmlDocGetRootElement(theDoc);

       xmlNode *fNode = theRoot->children;
	
       while (fNode) {
  	  if (!strcmp((char *) fNode->name, "Resources") && fNode->children) {
             Resources *t = new Resources();
	     t->xmlConfigure(fNode->children);
	     setResources(t);
          } else if (!strcmp((char *) fNode->name, "Elements") && fNode->children) {
             Elements *t = new Elements();
	     t->xmlConfigure(fNode->children);
	     setElements(t);
          }
	  fNode = fNode->next;
       }
	
       if (fOldElements != mElements && fOldElements) {
	  delete fOldElements;
       }
       if (fOldResources != mResources && fOldResources) {
	  delete fOldResources;
       }
       
       setSkinPath(fSkinDirectory);
       
       debugPrint("     Loading resources\n");
       
       mResources->loadResources();
       debugPrint("     Associating resources & elements\n");
       
       mElements->associateResources(mResources);
       
       xmlFreeDoc(theDoc);
       return 1;
}

void Environment::loadSkinPath()
{
    char buf[255];
    char buf2[255];
    int len = 0;
#ifdef ENABLE_XBOX
    sprintf(buf, "d:/skin.pth");
#else
    sprintf(buf, "skin.pth");
#endif
    printf("Reading in skin path..\n\n");
    FILE *fp = fopen(buf, "r");
    if (fp) {
       len = fread(buf2, sizeof(char), 100, fp);
       fclose(fp);
       buf2[len] = 0x00;
       printf("Read in skin path %s\n", buf2);
       while (strchr(buf2, '\n')) {
          *strchr(buf2, '\n') = 0x00;
       }
       while (strchr(buf2, '\r')) {
          *strchr(buf2, '\r') = 0x00;
       }
    } else {
#ifdef ENABLE_XBOX
       sprintf(buf2, "d:/");
#else
       sprintf(buf2, "");
#endif
    }
       
#ifdef ENABLE_XBOX
    debugPrint("     Read in skin path: %s\n", buf2);
#endif
    setSkinPath(buf2);
    
}

void Environment::saveSkinPath(char *fSkinPath)
{
    char buf[255];
#ifdef ENABLE_XBOX
    sprintf(buf, "d:/skin.pth");
#else
    sprintf(buf, "skin.pth");
#endif
    
    FILE *fp = fopen(buf, "w");
    if (fp) {
       fwrite(fSkinPath, sizeof(char), strlen(fSkinPath), fp);
       fclose(fp);
    }
}   
 
void Environment::ensureSkin()
{
    if (mResources && mElements) {
       return;
    }
    
    // we're missing resources or elements.  That is equivalent to a missing skin.
    // use our internal generated skin in that case.
    
    if (mResources) {
       delete mResources;
    }
    
    if (mElements) {
       delete mElements;
    }
    
    // generate our own resources.
    mResources = new Resources();
    mElements = new Elements();
    
    SDL_Surface *t = SDL_CreateRGBSurface(SDL_HWSURFACE, 640,480,32,mScreen->format->Rmask, mScreen->format->Gmask, mScreen->format->Bmask, 0);
    SDL_FillRect(t, 0, 0);
    
    ImageResource *f = new ImageResource();
    
    f->setName("Background");
    f->setImage(t);
    mResources->addResource(f);
    
    Element *e = new Element();
    e->setContent(CONTENT_MENU);
    SDL_Rect r;
    r.x = 70;
    r.y = 60;
    r.h = 420;
    r.w = 500;
    e->setDimensions(r);
    SDL_Color c;
    c.r = 250;
    c.g = 250;
    c.b = 250;
    e->setColor(c);
    e->setLayer(1);
    e->setSkinID("");

    // set the loaded font to e.
    
    mElements->addElement(e);

    
    e = new Element();
    e->setContent(CONTENT_BACKGROUND_IMAGE);
    e->setLayer(1);
    e->setResourceName("Background");
    e->setSkinID("");
    r.x = 0;
    r.y = 0;
    r.h = 480;
    r.w = 640;
    e->setDimensions(r);
    mElements->addElement(e);
    
        
    e = new Element();
    e->setContent(CONTENT_MENUENTRY_BG);
    e->setLayer(1);
    e->setResourceName("Background");
    e->setSkinID("");
    mElements->addElement(e);
    
    e = new Element();
    e->setContent(CONTENT_MENUENTRY_SELECTED_BG);
    e->setLayer(1);
    e->setResourceName("Background");
    e->setSkinID("");
    mElements->addElement(e);

    
    
}

void Environment::initialize()
{
    
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
	
    SDL_JoystickEventState(SDL_ENABLE);
    SDL_Joystick *joystick = SDL_JoystickOpen(0);
    
    mXboxInfo = new XboxInfo();
    // creating xboxinfo thread
    //SDL_CreateThread(XboxInfo::createThread, mXboxInfo);
    
    mController = new Controller();
    mController->setScope(Scope_Any);
    // executing controller startup thread
#ifndef WINDOWS
    mControllerThread = SDL_CreateThread(Controller::threadStart, mController);
#endif
    
    mScreen = SDL_SetVideoMode(640, 480, 32, SDL_DOUBLEBUF | SDL_HWSURFACE);
    mBuffer = SDL_CreateRGBSurface(SDL_HWSURFACE, 640, 480, mScreen->format->BitsPerPixel,
    mScreen->format->Rmask, mScreen->format->Gmask, mScreen->format->Bmask, 0);
       
    TTF_Init();
    
/*    if (mResources) {
       mResources->loadResources();
    }
    
    if (mElements) {
       mElements->associateResources(mResources);
    } */
    

}


void Environment::update()
{
    int fMenuHeight = 0;
    int fMenuEntryHeight = 0;
    int fTarget = 0;
    int i,j;
    Element *t;
        
    // update the menu state position if we're in a changemenu.
    if (mMenuState == State_ChangeMenu) {

       mMenuStatePosition++;
       
       if (mMenuStatePosition == 4) {
          mMenuState = State_Normal;
          mMenuStatePosition = 0;
       }
    }
    
    // fix out of range mMenuSelected values.
    if (mMenuSelected < 0) {
       mMenuSelected = 0;
       renderMenu();
    }
    
    if (mMenuSelected >= mMenu->getChildren()->getLength()) {
       mMenuSelected = mMenu->getChildren()->getLength()-1;
       renderMenu();
    }
    
    t = mElements->getElementByContent(CONTENT_MENU, mMenu->getSkinID());
    if (t) {
       fMenuHeight = t->getDimensions().h;
    }
    t = mElements->getElementByContent(CONTENT_MENUENTRY_BG, mMenu->getSkinID());
    if (t) {
       fMenuEntryHeight = t->getDimensions().h;
    }
    
    fTarget = mMenuSelected * fMenuEntryHeight;
    
/*    if (abs(fTarget - mMenuOffset) < 10) {
       fTarget = mMenuOffset;
    } else {
       int fDiff = fTarget - mMenuOffset;
       fDiff /= 10;
       mMenuOffset += fDiff;
    } */
    
    if (mController->getState(Ctrl_RTrig) == Button_Pressed) {
       j = 30;
    } else {
       j = 20;
    }
    
    if (fTarget + (fMenuEntryHeight*3) > (fMenuHeight + mMenuOffset)) {

       mMenuOffset += j;
    } else if (fTarget < mMenuOffset) {
       mMenuOffset -= j;
       if (mMenuOffset < 0) {
          mMenuOffset = 0;
       }
    }
    
}

void Environment::drawMessage(char *fTitle, char *fText)
{
    Element *fMessageBackground;
    Element *fMessageText;
    Element *fMessageTitle;
    
    fMessageBackground = mElements->getElementByContent(CONTENT_MESSAGEBACKGROUND, mMenu->getSkinID());
    fMessageText = mElements->getElementByContent(CONTENT_MESSAGETEXT, mMenu->getSkinID());
    fMessageTitle = mElements->getElementByContent(CONTENT_MESSAGETITLE, mMenu->getSkinID());
    
    // render background
    if (fMessageBackground) {
       fMessageBackground->drawImage(mScreen);
    }
        
    // render the text's
    if (fMessageText) {
       fMessageText->drawText(fText, mScreen);
    }
    
    if (fMessageTitle) {
       fMessageTitle->drawText(fTitle, mScreen);
    }
    
    SDL_Flip(mScreen);
    
    mController->setState(Ctrl_A, Button_Released);
    mController->setState(Ctrl_B, Button_Released);
    mController->setState(Ctrl_Y, Button_Released);
   
    SDL_Event event;
    
    while (mController->getState(Ctrl_A) == Button_Released &&
           mController->getState(Ctrl_B) == Button_Released &&
           mController->getState(Ctrl_Y) == Button_Released) {
#ifdef WINDOWS
        while (SDL_PollEvent(&event)) {
            mController->update(&event);
        }
#endif

#ifdef ENABLE_XBOX
       XSleep(30);
#else       
       SDL_Delay(30);
#endif
    }
    
}
    

int Environment::askQuestion(char *fTitle, char *fText)
{
    Element *fQuestionBackground;
    Element *fQuestionText;
    Element *fQuestionTitle;
    
    FontResource *tFont;
    
    fQuestionBackground = mElements->getElementByContent(CONTENT_QUESTIONBACKGROUND, mMenu->getSkinID());
    fQuestionText = mElements->getElementByContent(CONTENT_QUESTIONTEXT, mMenu->getSkinID());
    fQuestionTitle = mElements->getElementByContent(CONTENT_QUESTIONTITLE, mMenu->getSkinID());
    
    if (fQuestionBackground) {
       fQuestionBackground->drawImage(mScreen);
    }
    
    if (fQuestionTitle) {
       fQuestionTitle->drawText(fTitle, mScreen);
    }
    
    if (fQuestionText) {
       fQuestionText->drawText(fText, mScreen);
    }
    
    SDL_Flip(mScreen);
    
    mController->setState(Ctrl_A, Button_Released);
    mController->setState(Ctrl_B, Button_Released);
    mController->setState(Ctrl_Y, Button_Released);
    
    SDL_Event event;
    
    while (mController->getState(Ctrl_A) == Button_Released &&
           mController->getState(Ctrl_B) == Button_Released &&
           mController->getState(Ctrl_Y) == Button_Released) {
#ifdef WINDOWS
        while (SDL_PollEvent(&event)) {
            mController->update(&event);
        }
#endif

#ifdef ENABLE_XBOX
       XSleep(30);
#else       
       SDL_Delay(30);
#endif
    }
    if (mController->getState(Ctrl_A) == Button_Pressed) {
       return 1;
    } else {
       return 0;
    }
    
}

int Environment::askTimedQuestion(char *fTitle, char *fText, int fTimeout, int fDefault)
{
    Element *fQuestionBackground;
    Element *fQuestionText;
    Element *fQuestionTitle;
    Element *fQuestionTimer;
    
    char buf[255];
    char *fTimerText;
    
    FontResource *tFont;
    
    if (fDefault == -1 || fTimeout <= 0) {
       return askQuestion(fTitle, fText);
    }
    
    fQuestionBackground = mElements->getElementByContent(CONTENT_QUESTIONBACKGROUND, mMenu->getSkinID());
    fQuestionText = mElements->getElementByContent(CONTENT_QUESTIONTEXT, mMenu->getSkinID());
    fQuestionTitle = mElements->getElementByContent(CONTENT_QUESTIONTITLE, mMenu->getSkinID());
    fQuestionTimer = mElements->getElementByContent(CONTENT_QUESTIONTIMER, mMenu->getSkinID());
    
    fTimerText = mDialog->getValue("TimerText");
    
    
    mController->setState(Ctrl_A, Button_Released);
    mController->setState(Ctrl_B, Button_Released);
    mController->setState(Ctrl_Y, Button_Released);
    
    SDL_Event event;
#ifdef ENABLE_XBOX
    int start = XGetTickCount();
#else
    int start = SDL_GetTicks();
#endif
    int now;
    
    while (mController->getState(Ctrl_A) == Button_Released &&
           mController->getState(Ctrl_B) == Button_Released &&
           mController->getState(Ctrl_Y) == Button_Released) {
#ifdef WINDOWS
        while (SDL_PollEvent(&event)) {
            mController->update(&event);
        }
#endif

#ifdef ENABLE_XBOX    
	now = XGetTickCount();
#else
        now = SDL_GetTicks();
#endif

	if (now - start > (fTimeout*1000)) {
	   return fDefault;
	}
	
	renderScreen();
	
        if (fQuestionBackground) {
           fQuestionBackground->drawImage(mScreen);
        }
     
        if (fQuestionTitle) {
           fQuestionTitle->drawText(fTitle, mScreen);
        }
    
        if (fQuestionText) {
           fQuestionText->drawText(fText, mScreen);
        }
	
        if (fQuestionTimer && fTimerText) {
           sprintf(buf, fTimerText, fTimeout - ((int) (now - start) / 1000));
           fQuestionTimer->drawText(buf, mScreen);
        }
    
        SDL_Flip(mScreen);

#ifdef ENABLE_XBOX
       XSleep(30);
#else       
       SDL_Delay(30);
#endif
    }
    if (mController->getState(Ctrl_A) == Button_Pressed) {
       return 1;
    } else {
       return 0;
    }
    
}

void Environment::drawProgressBar(char *fTitle, char *fMessage, int fPercent)
{
    char buf[16];
    SDL_Rect src, dest;
    SDL_Surface *bar;
    int per_x;
    
    Element *fMessageBackground;
    Element *fMessageText;
    Element *fMessageTitle;
    Element *fProgressBar;
    Element *fProgressText;
    
    fMessageBackground = mElements->getElementByContent(CONTENT_PROGRESSBACKGROUND, mMenu->getSkinID());
    fMessageText = mElements->getElementByContent(CONTENT_PROGRESSMESSAGE, mMenu->getSkinID());
    fMessageTitle = mElements->getElementByContent(CONTENT_PROGRESSTITLE, mMenu->getSkinID());
    fProgressBar = mElements->getElementByContent(CONTENT_PROGRESSBAR, mMenu->getSkinID());
    fProgressText = mElements->getElementByContent(CONTENT_PROGRESSTEXT, mMenu->getSkinID());
    
    // render background
    if (fMessageBackground) {
       fMessageBackground->drawImage(mScreen);
    }
    
    // draw the progress bar.
    
    if (fProgressBar) {
       bar = ((ImageResource *)fProgressBar->getResource())->getImage();
       if (bar) {
          src.y = 0;
          src.x = 0;
          src.w = ((double) fPercent/100)*fProgressBar->getDimensions().w;
          dest = fProgressBar->getDimensions();
          dest.w = src.w;
          src.h = dest.h;
	  
          SDL_BlitSurface(bar, &src, mScreen, &dest);
       }
    }
        
    // render the text's
    if (fMessageText) {
       fMessageText->drawText(fMessage, mScreen);
    }
    
    if (fMessageTitle) {
       fMessageTitle->drawText(fTitle, mScreen);
    }
        
    // render the percent
    sprintf(buf, "%d%%", fPercent);
    if (fProgressText) {
       fProgressText->drawText(buf, mScreen);  
    }
    
    SDL_Flip(mScreen);
    
}

void Environment::renderScreen()
{
    SDL_Rect q,r;
    Element *t;
    char buf[255];
    char *bufp;
    
    // if we're in a fadeout, use a faded mBuffer and just blit it to
    // the screen
    if (mMenuState == State_ChangeMenu && mMenuStatePosition == 1) {
       SDL_BlitSurface(mScreen, 0x00, mBuffer, 0x00);
       
       /*SDL_SetAlpha(mBuffer, SDL_SRCALPHA, 255-(mMenuStatePosition*10));
       SDL_FillRect(mScreen, 0, 0);
       SDL_BlitSurface(mBuffer, 0x00, mScreen, 0x00);
       SDL_Flip(mScreen);
       return; */
    }
    
    // draw background
    t = mElements->getElementByContent(CONTENT_BACKGROUND_IMAGE, mMenu->getSkinID());   
    if (t) {
       t->drawImage(mScreen);
    }

    
    //if (t && t->getResource() /*&& typeid(*t->getResource()) == typeid(ImageResource) */) {
    //   ImageResource *tmp = (ImageResource *) t->getResource();
    //   SDL_BlitSurface(tmp->getImage(), 0x00, mScreen, 0);
    //}
    
    
    // draw menu in proper dimensions
    t = mElements->getElementByContent(CONTENT_MENU, mMenu->getSkinID());
    if (mMenuSurface && t) {
       r = t->getDimensions();
       q.w = r.w;
       q.h = r.h;
       q.x = 0;
       q.y = mMenuOffset-50;
       if (q.y < 0) {
          q.y = 0;
       }
       SDL_BlitSurface(mMenuBackground, &q, mScreen, &r);
       SDL_BlitSurface(mMenuSurface, &q, mScreen, &r);
    }
        
    // draw the remaining Elements.
    List<Element> *e = mElements->getElements();
    while (e) {
       if (e->getData() && (!e->getData()->getSkinID() || !e->getData()->getSkinID() || !strcmp(e->getData()->getSkinID(), mMenu->getSkinID()))) {
          if (e->getData()->getContent() == CONTENT_STATUS) {
             if (mSelectedNode && mSelectedNode->getStatusText()) {
                e->getData()->drawText(mSelectedNode->getStatusText(), mScreen);
  	     }
          } else if (e->getData()->getContent() == CONTENT_BOARDTEMP) {
          
	     sprintf(buf, "%d C", mXboxInfo->getBoardTemp());
	     e->getData()->drawText(buf, mScreen);
          } else if (e->getData()->getContent() == CONTENT_CPUTEMP) {
          
	     sprintf(buf, "%d C", mXboxInfo->getCPUTemp());
	     e->getData()->drawText(buf, mScreen);
	  
          } else if (e->getData()->getContent() == CONTENT_TRAYSTATE) {
             FontResource *fRes = (FontResource *) e->getData()->getResource();
	     int i = mXboxInfo->getTrayState();
	     if (i == Tray_Open) {
	        if (mDialog && (bufp = mDialog->getValue("TrayOpen"))) {
	           strcpy(buf, bufp);
                } else {
	           sprintf(buf, "Open");
                }
	     } else if (i == Tray_NoDisk) {
	        if (mDialog && (bufp = mDialog->getValue("TrayNoDisk"))) {
	           strcpy(buf, bufp);
	        } else {
	           sprintf(buf, "No Disk");
                }
	     } else if (i == Tray_Disk) {
	        if (mDialog && (bufp == mDialog->getValue("TrayDisk"))) {
	           strcpy(buf, bufp);
	        } else {
	           sprintf(buf, "Disk");
	        }
	     } else if (i == Tray_Opening) {
	        if (mDialog && (bufp == mDialog->getValue("TrayOpening"))) {
	           strcpy(buf, bufp);
                } else {
	           sprintf(buf, "Opening");
	        }
	     } else if (i == Tray_Closing) {
	        if (mDialog && (bufp == mDialog->getValue("TrayClosing"))) {
	           strcpy(buf, bufp);
                } else {
  	           sprintf(buf, "Closing");
                }
	     } else if (i == Tray_Reading) {
	        if (mDialog && (bufp == mDialog->getValue("TrayBusy"))) {
	           strcpy(buf, bufp);
	        } else {
	           sprintf(buf, "Busy");
                }
	     }
	     if (fRes && fRes->getFont()) {
                e->getData()->drawText(buf, mScreen);
	     }
          }
       }
       e = e->getLink();
    }
}

void Environment::drawScreen()
{
    renderScreen();
    if (mMenuState == State_ChangeMenu) {
       //if (mMenuStatePosition == 25) {
       //   SDL_BlitSurface(mScreen, 0x00, mBuffer, 0x00);
       //} 
              
       // if we're in a fadeout, use a faded mBuffer and just blit it to
       // the screen
       //if (mMenuStatePosition < 50) {
          SDL_SetAlpha(mBuffer, SDL_SRCALPHA, (255-(mMenuStatePosition*60)));
          SDL_BlitSurface(mBuffer, 0x00, mScreen, 0x00);
          
          SDL_Flip(mScreen);
          return;
       //} 
          
    } else {
    
       SDL_Flip(mScreen);
    }
       
}

void Environment::renderMenu()
{
    SDL_Color f;
    SDL_Rect r;
    int i;
    int entryHeight;
    int entryWidth;
    int useSelected;
    
    if (mMenu) {
    
       if (mMenuSurface) {
          SDL_FreeSurface(mMenuSurface);
	  mMenuSurface = 0;
       }
       
       if (mMenuBackground) {
          SDL_FreeSurface(mMenuBackground);
	  mMenuBackground = 0;
       }
       
       Element *elMenu = mElements->getElementByContent(CONTENT_MENU, mMenu->getSkinID());
       Element *elMenuEntryBG = mElements->getElementByContent(CONTENT_MENUENTRY_BG, mMenu->getSkinID());
       Element *elMenuSelectedBG = mElements->getElementByContent(CONTENT_MENUENTRY_SELECTED_BG, mMenu->getSkinID());
       
       if (elMenuEntryBG) {
          entryHeight = elMenuEntryBG->getDimensions().h;
	  entryWidth = elMenuEntryBG->getDimensions().w;
       } else {
          entryHeight = 40;
	  entryWidth = 200;
       }
       
       mMenuSurface = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, elMenu->getDimensions().w, mMenu->getChildren()->getLength()*entryHeight, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);       
       mMenuBackground = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, elMenu->getDimensions().w, mMenu->getChildren()->getLength()*entryHeight, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
       
       SDL_SetAlpha(mMenuSurface, 0, 255);
       SDL_SetAlpha(mMenuBackground, 0, 255);
              
       if (elMenuEntryBG && elMenuEntryBG->getResource()) {
          SDL_Surface *tmp = ((ImageResource *) elMenuEntryBG->getResource())->getImage();
          SDL_SetAlpha(tmp, 0, 255);
	  r.w = tmp->w;
	  r.h = tmp->h;
	  r.x = 0;
	  r.y = 0;
           
          useSelected = 0;
          SDL_Surface *selected;
          if (elMenuSelectedBG && elMenuSelectedBG->getResource()) {
             useSelected = 1;
             selected = ((ImageResource *) elMenuSelectedBG->getResource())->getImage();
             SDL_SetAlpha(selected, 0, 255);
          }
           
	  for (i = 0; i < mMenu->getChildren()->getLength(); i++) {
             if (useSelected && i == mMenuSelected) {
                SDL_BlitSurface(selected, 0x00, mMenuBackground, &r);
             } else {
                SDL_BlitSurface(tmp, 0x00, mMenuBackground, &r);
             }
	     r.y += entryHeight;
	  }
          SDL_SetAlpha(tmp, SDL_SRCALPHA, 255);
       }
       
       List<MenuNode> *t = mMenu->getChildren();
       
       Element *elMenuFont = mElements->getElementByContent(CONTENT_MENUFONT, mMenu->getSkinID());
       Element *elMenuSelectedFont = mElements->getElementByContent(CONTENT_MENUFONT_SELECTED, mMenu->getSkinID());
       
              
       TTF_Font *font = ((FontResource *) elMenuFont->getResource())->getFont();
       TTF_Font *selectFont;
       
       useSelected = 0;
       if (elMenuSelectedFont && elMenuSelectedFont->getResource()) {
          selectFont = ((FontResource *) elMenuSelectedFont->getResource())->getFont();
          useSelected = 1;
       }

              
       r.w = elMenu->getDimensions().w;
       r.h = 30;
       r.x = 0;
       r.y = 0;
       i = 0;
       
       while (t) {
          
	  SDL_Surface *tmp;
          if (useSelected && i == mMenuSelected) {
             tmp = TTF_RenderText_Blended(selectFont, t->getData()->getName(), elMenuSelectedFont->getColor());             
          } else {
             tmp = TTF_RenderText_Blended(font, t->getData()->getName(), elMenuFont->getColor());
          }
          if (i == mMenuSelected) {
             mSelectedNode = t->getData();
          }
	  
	  
	  if (elMenu->getAlign() == Align_Left || !tmp) {
             r.x = 5;
          } else if (elMenu->getAlign() == Align_Center) {
             r.x = (elMenu->getDimensions().w - tmp->w)/2;
          } else if (elMenu->getAlign() == Align_Right) {
             r.x = elMenu->getDimensions().w - tmp->w - 5;
          }
	  
	  if (tmp) {
  	     r.y += (entryHeight - tmp->h)/2;
	  
	     r.w = tmp->w;
	     r.h = tmp->h;
	  
	     SDL_SetAlpha(tmp, 0,255);
             SDL_BlitSurface(tmp, 0x00, mMenuSurface, &r);
	     r.y -= (entryHeight - tmp->h)/2;
	     SDL_FreeSurface(tmp);
	  }
          t = t->getLink();

	  r.y += entryHeight;
          i++;
       }
       
       SDL_SetAlpha(mMenuSurface, SDL_SRCALPHA, 255);
       SDL_SetAlpha(mMenuBackground, SDL_SRCALPHA, 255);
    }
    
}
       
void Environment::xmlConfigure(xmlNode *fNode)
{
    int fEmbeddedSkin = 0;
    while (fNode) {
       if (!strcmp((char *) fNode->name, "Menu")) {
          SubMenuNode *t = new SubMenuNode();
	  t->xmlConfigure(fNode->children);
	  setTopMenu(t);
	  setCurrentMenu(t);
       } else if (!strcmp((char *) fNode->name, "Resources")) {
          Resources *t = new Resources();
	  t->xmlConfigure(fNode->children);
	  setResources(t);
	  fEmbeddedSkin = 1;
       } else if (!strcmp((char *) fNode->name, "Elements")) {
          Elements *t = new Elements();
	  t->xmlConfigure(fNode->children);
	  setElements(t);
	  fEmbeddedSkin = 1;
       } else if (!strcmp((char *) fNode->name, "Dialog")) {
          Generic *t = new Generic();
	  t->xmlConfigure(fNode->children);
	  setDialog(t);
       } else if (!strcmp((char *) fNode->name, "LogFile")) {
          setLogFileName((char *) fNode->children->content);
       }
       fNode = fNode->next;
    }
    if (fEmbeddedSkin) {
       mResources->loadResources();
       mElements->associateResources(mResources);
    }
}


void Environment::logMessage(char *fMsg, ...)
{
	char buffer[1024];
	unsigned short len;
	va_list argList;
	va_start(argList, fMsg);
	vsprintf(buffer, fMsg, argList);
	va_end(argList);
	if (mLogFileName) {
	   FILE *fp = fopen(mLogFileName, "a");
	   fprintf(fp, buffer);
	   fclose(fp);
	}	

}

// ********************************************************************* TEH SETTORS

void Environment::setCurrentMenu(SubMenuNode *fNode)
{
    if (fNode->getChildren() != 0) {
       mMenu = fNode;
       mMenuSelected = 0;
       mMenuOffset = 0;
       mMenuState = State_ChangeMenu;
       mMenuStatePosition = 0;
    }
}

void Environment::setTopMenu(SubMenuNode *fTop)
{
    mTop = fTop;
}

void Environment::setResources(Resources *fResources)
{
    mResources = fResources;
}

void Environment::setElements(Elements *fElements)
{
    mElements = fElements;
}

void Environment::setSkinPath(char *fSkinPath)
{
    if (fSkinPath && fSkinPath != mSkinPath) {
       if (mSkinPath) {
          delete[] mSkinPath;
       }
       mSkinPath = new char[strlen(fSkinPath)+1];
       strcpy(mSkinPath, fSkinPath);
    }
}

void Environment::setXboxInfo(XboxInfo *fXboxInfo)
{
    mXboxInfo = fXboxInfo;
}

void Environment::setHotKeys(List<HotKey> *fHotKeys)
{
    mHotKeys = fHotKeys;
}

void Environment::setMenuSelected(int fMenuSelected)
{
    mMenuSelected = fMenuSelected;
}

void Environment::addHotKey(HotKey *fHotKey)
{
    if (mHotKeys) {
       mHotKeys->addItem(fHotKey);
    } else {
       mHotKeys = new List<HotKey> (fHotKey, 0x00);
    }
}

void Environment::setDialog(Generic *fDialog)
{
    mDialog = fDialog;
}

void Environment::setLogFileName(char *fLogFileName)
{
    if (fLogFileName) {
       if (mLogFileName) { 
          delete[] mLogFileName;
       }
       mLogFileName = new char[strlen(fLogFileName)+1];
       strcpy(mLogFileName, fLogFileName);
    }
}

// ********************************************************************* TEH GETTORZ

SubMenuNode *Environment::getCurrentMenu()
{
    return mMenu;
}

SubMenuNode *Environment::getTopMenu()
{
    return mTop;
}

Resources *Environment::getResources()
{
    return mResources;
}

Elements *Environment::getElements()
{
    return mElements;
}

char *Environment::getSkinPath()
{
    return mSkinPath;
}

XboxInfo *Environment::getXboxInfo()
{
    return mXboxInfo;
}

Controller *Environment::getController()
{
    return mController;
}

List<HotKey> *Environment::getHotKeys()
{
    return mHotKeys;
}

Generic *Environment::getDialog()
{
    return mDialog;
}

int Environment::getMenuSelected()
{
    return mMenuSelected;
}

char *Environment::getLogFileName()
{
    return mLogFileName;
}
