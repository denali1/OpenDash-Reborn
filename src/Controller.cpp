#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef ENABLE_XBOX
   #include <openxdk/debug.h>
   #include <hal/xbox.h>
#endif

#include "SDL.h"

#include "Controller.h"

#ifndef ENABLE_XBOX
   #define XSleep SDL_Delay
   #define debugPrint printf
#endif

char *buttonNames[] = {
   "Ctrl_A",
   "Ctrl_B",
   "Ctrl_X",
   "Ctrl_Y",
   "Ctrl_Start",
   "Ctrl_Back",
   "Ctrl_LTrig",
   "Ctrl_RTrig",
   "Ctrl_Black",
   "Ctrl_White",
   "Ctrl_Up",
   "Ctrl_Down",
   "Ctrl_Left",
   "Ctrl_Right",
   "Ctrl_LStickY",
   "Ctrl_LStickX",
   "Ctrl_RStickY",
   "Ctrl_RStickX"
};

Controller::Controller()
{
	mScope = Scope_Any;
	memset(mState, 0, sizeof(int)*NUM_BUTTONS);
}

Controller::~Controller()
{
	// yeah
}

int Controller::threadStart(void *cptr)
{
	SDL_Event event;
	Controller *fController = (Controller *) cptr;
	while (1) {
	  while (SDL_PollEvent(&event)) {
            fController->update(&event);
          }
	  XSleep(45);
        }
}

void Controller::update(SDL_Event *fEvent)
{
	int iCare = 0;
	int fAction = Button_Released;
//	if (mScope == Scope_Any) {
	   iCare = 1;
/*	} else if (mScope == Scope_Pad1 && fEvent->which == 0) {
	   iCare = 1;
	} else if (mScope == Scope_Pad2 && fEvent->which == 1) {
	   iCare = 1;
	} else if (mScope == Scope_Pad3 && fEvent->which == 2) {
	   iCare = 1;
	} else if (mScope == Scope_Pad4 && fEvent->which == 3) {
	   iCare = 1;
	} */
	if (iCare) {
	   
  	   switch(fEvent->type) {
	     case SDL_JOYBUTTONDOWN:
	       fAction = Button_Pressed; // PURPOSELY FALLING THROUGH! (SAVES CUT&PASTE CODE)
	     case SDL_JOYBUTTONUP:
	       if (fEvent->jbutton.button == 0) {
	          setState(Ctrl_A, fAction);
	       } else if (fEvent->jbutton.button == 1) {
	          setState(Ctrl_B, fAction);
	       } else if (fEvent->jbutton.button == 2) {
	          setState(Ctrl_X, fAction);
	       } else if (fEvent->jbutton.button == 3) {
	          setState(Ctrl_Y, fAction);
	       } else if (fEvent->jbutton.button == 6) {
	          setState(Ctrl_LTrig, fAction);
	       } else if (fEvent->jbutton.button == 7) {
	          setState(Ctrl_RTrig, fAction);
	       } else if (fEvent->jbutton.button == 5) {
	          setState(Ctrl_White, fAction);
	       } else if (fEvent->jbutton.button == 4) {
	          setState(Ctrl_Black, fAction);
	       } else if (fEvent->jbutton.button == 8) {
	          setState(Ctrl_Start, fAction);
	       } else if (fEvent->jbutton.button == 9) {
	          setState(Ctrl_Back, fAction);
	       }
	       break;
	     case SDL_JOYHATMOTION:
	       setState(Ctrl_Up, Button_Released);
	       setState(Ctrl_Down, Button_Released);
	       setState(Ctrl_Left, Button_Released);
	       setState(Ctrl_Right, Button_Released);
	       if (fEvent->jhat.value & SDL_HAT_UP) {
	          setState(Ctrl_Up, Button_Pressed);
	       } else if (fEvent->jhat.value & SDL_HAT_DOWN) {
	          setState(Ctrl_Down, Button_Pressed);
	       } else if (fEvent->jhat.value & SDL_HAT_LEFT) {
	          setState(Ctrl_Left, Button_Pressed);
	       } else if (fEvent->jhat.value & SDL_HAT_RIGHT) {
	          setState(Ctrl_Right, Button_Pressed);
	       }
	       break;
	     case SDL_JOYAXISMOTION:
	       if (fEvent->jaxis.axis == 0) {
	          setState(Ctrl_LStickX, fEvent->jaxis.value);
	       } else if (fEvent->jaxis.axis == 1) {
	          setState(Ctrl_LStickY, fEvent->jaxis.value);
	       } else if (fEvent->jaxis.axis == 2) {
	          setState(Ctrl_RStickX, fEvent->jaxis.value);
	       } else if (fEvent->jaxis.axis == 3) {
	          setState(Ctrl_RStickY, fEvent->jaxis.value);
	       }
	       break;
             case SDL_KEYDOWN:
               fAction = Button_Pressed; // PURPOSELY FALLING THROUGH (SAVES CUT&PASTE CODE AGAIN)
             case SDL_KEYUP:
               if (fEvent->key.keysym.sym == SDLK_LEFT) {
                  setState(Ctrl_Left, fAction);
               } else if (fEvent->key.keysym.sym == SDLK_RIGHT) {
                  setState(Ctrl_Right, fAction);
               } else if (fEvent->key.keysym.sym == SDLK_UP) {
                  setState(Ctrl_Up, fAction);
               } else if (fEvent->key.keysym.sym == SDLK_DOWN) {
                  setState(Ctrl_Down, fAction);
               } else if (fEvent->key.keysym.sym == SDLK_RSHIFT) {
                  setState(Ctrl_RTrig, fAction);
               } else if (fEvent->key.keysym.sym == SDLK_LSHIFT) {
                  setState(Ctrl_LTrig, fAction);
               } else if (fEvent->key.keysym.sym == SDLK_RETURN) {
                  setState(Ctrl_Start, fAction);
               } else if (fEvent->key.keysym.sym == SDLK_BACKSPACE) {
                  setState(Ctrl_Back, fAction);
               } else if (fEvent->key.keysym.sym == SDLK_z) {
                  setState(Ctrl_A, fAction);
               } else if (fEvent->key.keysym.sym == SDLK_a) {
                  setState(Ctrl_X, fAction);
               } else if (fEvent->key.keysym.sym == SDLK_x) {
                  setState(Ctrl_B, fAction);
               } else if (fEvent->key.keysym.sym == SDLK_s) {
                  setState(Ctrl_Y, fAction);
               } else if (fEvent->key.keysym.sym == SDLK_q) {
	          exit(1);
	       }
               break;
	   }
	}
}

int Controller::getButtonByName(char *fButtonName)
{
    int i;
    if (fButtonName) {
       for (i = 0; i < NUM_BUTTONS; i++) {
          if (!strcmp(fButtonName, buttonNames[i])) {
             return i;
          }
       }
    }
    return -1;
}
          
       
void Controller::setScope(int fScope)
{
	mScope = fScope;
}

int Controller::getScope()
{
	return mScope;
}

int Controller::getState(int fButton)
{
	return mState[fButton];
}

int *Controller::getStates()
{
	return mState;
}

void Controller::setState(int fButton, int fState)
{
	mState[fButton] = fState;
}
