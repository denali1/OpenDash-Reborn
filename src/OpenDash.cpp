#ifndef NO_TYPEINFO
#include <typeinfo>
#endif

#include <libxml/tree.h>
#include <libxml/parser.h>

#include "MenuNode.h"
#include "SubMenuNode.h"
#include "FileMenu.h"
#include "XBEMenu.h"
#include "SkinMenu.h"
#include "BootXBE.h"
#include "Environment.h"
#include "XboxInfo.h"
#include "Controller.h"
#include "HotKey.h"

#include "SDL.h"

#ifdef ENABLE_XBOX
   #include <hal/xbox.h>
   #include <hal/fileio.h>
   #include <openxdk/debug.h>
   #include <xboxkrnl/xboxkrnl.h>
#endif

#ifdef WINDOWS
   #define main WinMain
#endif


#ifdef ENABLE_XBOX
   #define CONFIG_FILE "d:/config.xml"
#else
   #define CONFIG_FILE "config.xml"
#endif

#define DEADZONE 25000

Environment *theEnv;

extern "C" {
#ifdef ENABLE_XBOX
#define printf debugPrint
void XBoxStartup()
#else
#define debugPrint printf
int main(int argc, char **argv)
#endif
{
       SDL_Event event;
       theEnv = new Environment();
       int last_clock;
       	
       xmlDoc *theDoc = 0;
       xmlNode *theRoot = 0;
       
       theEnv->initialize();

       theDoc = xmlReadFile(CONFIG_FILE, NULL, 0);
       
       if (theDoc == NULL) {
          printf("Error opening file %s\n", CONFIG_FILE);
          exit(1);
       }
       
       theRoot = xmlDocGetRootElement(theDoc);
       
       theEnv->xmlConfigure(theRoot->children);
       
       theEnv->loadSkinPath();
      
       if (! theEnv->loadSkin(theEnv->getSkinPath())) {
          printf("Cannot load skin\n");
          xmlFreeDoc(theDoc);
          exit(1);
       }
       
/*       theEnv->initialize(); */
       
       theEnv->ensureSkin();
       
       theEnv->renderMenu();
       Controller *c = theEnv->getController();
       
       while (1) {
          theEnv->update();
          theEnv->drawScreen();
          //theEnv->getXboxInfo()->readState();
          
	      // event handling now done in controller thread
#ifdef WINDOWS
          while (SDL_PollEvent(&event)) {
              theEnv->getController()->update(&event);
          }
#endif          
          // test for the hotkey key-combo (both triggers)
          if (c->getState(Ctrl_RTrig) == Button_Pressed && c->getState(Ctrl_LTrig) == Button_Pressed) {
             // go through hotkey list, update menu if hotkey pressed.
             int fUsed = 0;
             List<HotKey> *tmp = theEnv->getHotKeys();
             while (tmp) {
                if (tmp->getData() && tmp->getData()->getNode() && c->getState(tmp->getData()->getButton()) == Button_Pressed) {
		   tmp->getData()->getNode()->Execute();
		   theEnv->renderMenu();
                   fUsed = 1;
                }
		tmp = tmp->getLink();
             }
             
             if (fUsed) {
                c->setState(Ctrl_LTrig, Button_Released);
                c->setState(Ctrl_RTrig, Button_Released);
             }
          } else if (c->getState(Ctrl_Up) == Button_Pressed || c->getState(Ctrl_LStickY) > DEADZONE) {
             theEnv->setMenuSelected(theEnv->getMenuSelected()-1);
             theEnv->renderMenu();
          } else if (c->getState(Ctrl_Down) == Button_Pressed || c->getState(Ctrl_LStickY) < -1 * DEADZONE) {
             theEnv->setMenuSelected(theEnv->getMenuSelected()+1);
             theEnv->renderMenu();
          } else if (c->getState(Ctrl_A) == Button_Pressed) {

             MenuNode *t = theEnv->getCurrentMenu()->getChildren()->getData(theEnv->getMenuSelected());

             if (t) {

                t->Execute();

                theEnv->renderMenu();
             }

             c->setState(Ctrl_A, Button_Released);
          } else if (c->getState(Ctrl_B) == Button_Pressed || c->getState(Ctrl_Y) == Button_Pressed) {
             MenuNode *t = theEnv->getCurrentMenu()->getParent();
             if (t) {
                t->Execute();
                theEnv->renderMenu();
             }
             c->setState(Ctrl_B, Button_Released);
             c->setState(Ctrl_Y, Button_Released);
          }
       
#ifdef ENABLE_XBOX
	  XSleep(1);
#else
	  SDL_Delay(30);
#endif
       }
       
       
       xmlFreeDoc(theDoc);
}

}

