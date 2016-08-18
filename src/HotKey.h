#ifndef __HOTKEY_H__
#define __HOTKEY_H__

class MenuNode;

class HotKey {

public:

   HotKey();
   HotKey(MenuNode *fNode, int fButton);
   ~HotKey();

   void setNode(MenuNode *fNode);
   void setButton(int fButton);
   
   MenuNode *getNode();
   int getButton();
   
private:
   
   MenuNode *mNode;
   int mButton;
   
};

#endif
