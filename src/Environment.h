#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include <libxml/tree.h>
#include <libxml/parser.h>

#include "List.h"
#include "SDL.h"
#include "SDL_thread.h"

class SubMenuNode;
class Resources;
class Elements;
class XboxInfo;
class Controller;
class HotKey;
class MenuNode;
class Element;
class Generic;

enum MenuState {
   State_Normal = 0,
   State_ChangeMenu = 1,
   State_FadeLogo = 2,
};

class Environment {

public:

  Environment();
  ~Environment();
  
  // loads a node from the current node pointer.
  MenuNode *loadNode(xmlNode *fNode);
  int loadSkin(char *fSkinDirectory);
  void loadSkinPath();
  void saveSkinPath(char *fSkinPath);
  void ensureSkin();
  
  void initialize();
  
  void update();
  
  // draw the usual screen
  void renderScreen();
  void drawScreen();
  void renderMenu();

  // draw a messagebox, wait for response.
  void drawMessage(char *fTitle, char *fText);
  
  // draw a dialog, returns 1 on an "A" press, 0 on a "B"/"Y" press.
  int askQuestion(char *fTitle, char *fText);
  
  int askTimedQuestion(char *fTitle, char *fText, int fTimeout, int fDefaultYes);
  
  // draw a progress bar, return immediately.
  void drawProgressBar(char *fTitle, char *fMessage, int fPercent);
  
  void xmlConfigure(xmlNode *fNode);
  
  void logMessage(char *str, ...);

  void setCurrentMenu(SubMenuNode *fNode);
  void setTopMenu(SubMenuNode *fNode);
  void setResources(Resources *fResources);
  void setElements(Elements *fElements);
  void setSkinPath(char *fSkinPath);
  void setXboxInfo(XboxInfo *fXboxInfo);
  void setController(Controller *fController);
  void setHotKeys(List<HotKey> *fHotKeys);
  void addHotKey(HotKey *fHotKey);
  void setDialog(Generic *fDialog);
  void setMenuSelected(int fMenuSelected);
  void setMenuState(int fMenuState);
  void setLogFileName(char *fLogFile);

  SubMenuNode *getCurrentMenu();
  SubMenuNode *getTopMenu();
  Resources *getResources();
  Elements *getElements();
  char *getSkinPath();
  XboxInfo *getXboxInfo();
  Controller *getController();
  List<HotKey> *getHotKeys();
  Generic *getDialog();
  int getMenuSelected();
  int getMenuState();
  char *getLogFileName();

private:

  // our screen object
  SDL_Surface *mScreen;
  
  // buffered screen renders
  SDL_Surface *mMenuSurface;
  SDL_Surface *mMenuBackground;
  
  SDL_Surface *mBuffer;
  
  int mMenuOffset;   // current y offset on menu
  int mMenuSpeed; // current speed we're moving through menu
  int mMenuSelected; // index of selected entry in menu
  int mMenuState; // fading in/out to a new menu, or interactive, or ..?
  int mMenuStatePosition; // how far into the fade, etc?
  
  
  // loaded from xml menu
  SubMenuNode *mMenu;
  SubMenuNode *mTop;
  MenuNode *mSelectedNode;
  
  Resources *mResources;
  Elements *mElements;
  char *mSkinPath;
  
  Generic *mDialog;
  
  XboxInfo *mXboxInfo;
  Controller *mController;
  SDL_Thread *mControllerThread;
  List<HotKey> *mHotKeys;
  char *mLogFileName;


};

#endif
  
