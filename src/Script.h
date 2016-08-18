#ifndef __SCRIPT_H__
#define __SCRIPT_H__

class SubMenuNode;

class Script : public SubMenuNode {

public:
   Script();
   virtual ~Script();
   
   virtual void Execute();
      
protected:
   
};

#endif
