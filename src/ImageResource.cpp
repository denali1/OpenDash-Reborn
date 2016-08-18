#include <stdio.h>
#include <string.h>

#include "SDL.h"
#include "SDL_image.h"

#include "Resource.h"
#include "ImageResource.h"

#include "Environment.h"

extern Environment *theEnv;
#ifdef ENABLE_XBOX
extern "C" {
   #include <openxdk/debug.h>
}
#else
   #define debugPrint printf
#endif

ImageResource::ImageResource() : Resource()
{
	mFile = 0;
	mImage = 0;
}

ImageResource::~ImageResource()
{
	debugPrint("In ImageResource destructor");
	if (mFile) {
	   delete[] mFile;
	}
	if (mImage) {
	   SDL_FreeSurface(mImage);
	}
}

void ImageResource::xmlConfigure(xmlNode *fNode)
{
	Resource::xmlConfigure(fNode);
	while (fNode) {
	   if (!strcmp((char *) fNode->name, "File") && fNode->children) {
	      setFile((char *) fNode->children->content);
	   }
	   fNode = fNode->next;
	}
}

void ImageResource::loadResource()
{
        SDL_Surface *tmp;
        char *fSkinPath = theEnv->getSkinPath();
        
	if (!mImage && mFile) {
           char buf[255];
           if (fSkinPath) {
              sprintf(buf, "%s%s", fSkinPath, mFile);
           } else {
#ifdef ENABLE_XBOX           
              sprintf(buf, "d:/%s", mFile);
#else
              sprintf(buf, mFile);
#endif
           }
	   debugPrint("Loading image %s", buf);
	   tmp = IMG_Load(buf);
           if (! tmp) {
                debugPrint("Error loading image\n");
                return;
           }
           mImage = SDL_DisplayFormatAlpha(tmp);
           if (mImage) {
                SDL_FreeSurface(tmp);
           } else {
                debugPrint("Error converting image to display format");
                mImage = tmp;
           }
	}
}

void ImageResource::setFile(char *fFile)
{
	if (fFile) {
	   if (mFile) {
	      delete[] mFile;
	   }
	   mFile = new char[strlen(fFile)+1];
	   strcpy(mFile, fFile);
	}
}

void ImageResource::setImage(SDL_Surface *fImage)
{
	mImage = fImage;
}

char *ImageResource::getFile()
{
	return mFile;
}

SDL_Surface *ImageResource::getImage()
{
	return mImage;
}


