#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "SDL.h"

enum {
   Button_Released=0,
   Button_Pressed=1,
};

enum {
   Scope_Pad1,
   Scope_Pad2,
   Scope_Pad3,
   Scope_Pad4,
   Scope_Any
};

// use our own enum for ALL the buttons!
enum {
   Ctrl_A=0,
   Ctrl_B,
   Ctrl_X,
   Ctrl_Y,
   Ctrl_Start,
   Ctrl_Back,
   Ctrl_LTrig,
   Ctrl_RTrig,
   Ctrl_Black,
   Ctrl_White,
   Ctrl_Up,
   Ctrl_Down,
   Ctrl_Left,
   Ctrl_Right,
   Ctrl_LStickY,
   Ctrl_LStickX,
   Ctrl_RStickY,
   Ctrl_RStickX,
};

extern char *buttonNames[];

#define NUM_BUTTONS 18

class Controller {

public:

   Controller();
   ~Controller();
   
   void update(SDL_Event *fEvent);
   static int threadStart(void *fController);
   
   void setScope(int fScope);
   
   static int getButtonByName(char *fButtonName);
   
   int getScope();
   int *getStates();
   int getState(int fButton);
   void setState(int fButton, int fState);
   
protected:

   int mScope;
   int mState[22];



};

#endif
