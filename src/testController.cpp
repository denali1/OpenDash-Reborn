#ifdef ENABLE_XBOX
extern "C" {
   #include <hal/xbox.h>
   #include <hal/fileio.h>
   #include <openxdk/debug.h>
   #include <xboxkrnl/xboxkrnl.h>
}
#endif

#include "SDL.h"
#include "Controller.h"
#include "Environment.h"

#ifndef ENABLE_XBOX
   #define debugPrint printf
   #define XSleep SDL_Delay
#endif

Environment *theEnv;
#ifdef ENABLE_XBOX
extern "C" void XBoxStartup()
#else
extern "C" int main()
#endif
{
   SDL_Event event;
   
   theEnv = new Environment();
   
   Controller *c = new Controller();

#ifdef ENABLE_XBOX
   SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_VIDEO);
   SDL_JoystickEventState(SDL_ENABLE);
   SDL_Joystick *joystick = SDL_JoystickOpen(0);
#else
   SDL_Init(SDL_INIT_VIDEO);
#endif

   
   int old[NUM_BUTTONS];
   
   debugPrint("TestController.XBE!\n");
   
   while (1) {
      for (int i = 0; i < NUM_BUTTONS; i++) {
         old[i] = c->getState(i);
      }
      while (SDL_PollEvent(&event)) {
         debugPrint(".");
         c->update(&event);
      }
      for (int i = 0; i < NUM_BUTTONS; i++) {
         if (old[i] != c->getState(i)) {
	    debugPrint("   %d : %d\n", i, c->getState(i));
	 }
      }
      XSleep(40);
   }
   
   XSleep(100000);
   //XReboot();
}

