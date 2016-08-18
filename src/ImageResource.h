#ifndef __IMAGERESOURCE_H__
#define __IMAGERESOURCE_H__

#include <libxml/tree.h>
#include <libxml/parser.h>

class Resource;

class ImageResource : public Resource {

public:

   ImageResource();
   ~ImageResource();
   
   virtual void xmlConfigure(xmlNode *fNode);
   
   virtual void loadResource();
   
   void setFile(char *fFile);
   void setImage(SDL_Surface *fImage);
   
   char *getFile();
   SDL_Surface *getImage();
   
private:

   char *mFile;
   SDL_Surface *mImage;

};

#endif
