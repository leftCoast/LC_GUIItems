#include "bmpObj.h"
#include "resizeBuff.h"



// *************************************
//     The -tiny- mask base class.
// *************************************


mask::mask(void) {  }
mask::~mask(void) {  }
bool mask::checkPixel(int x,int y) { return true; }



// *************************************
//          The bmpObj class
// *************************************


bmpObj::bmpObj(int inX,int inY,int inWidth,int inHeight,char* bmpPath)
	: drawObj(inX,inY,inWidth,inHeight) {
	
	mMask = NULL;	
	mRowArray = NULL;
	setSourceRect(0,0,inWidth,inHeight);			// Default source is top left of bitmap, our size.
	mBMPObj = new bmpImage();
	mPath = NULL;
	if (bmpPath) {
		resizeBuff(strlen(bmpPath)+1,&mPath);
		strcpy(mPath,bmpPath);
	}
}

		
bmpObj::~bmpObj(void) {

	if (mBMPObj) delete(mBMPObj);
	resizeBuff(0,(void**)&mRowArray);
}

	
// Some stuff must wait 'till our hardware is up and running. like SD cards.	
bool bmpObj::begin(void) {

	if (mBMPObj->openDocFile(mPath)) {	// If we can set it up as this image..
		resizeBuff(0,&mPath);				// Then we can dump the copy of our path. Its in the bmpImage thing now.
		return true;							// And we return success.
	}
	return false;								// If we land here? Something went wrong.
}



// Setup the source rect for reading from the .bmp file.
void bmpObj::setSourceRect(int sX,int sY,int sWidth,int sHeight) {
	mSourceRect.setRect(sX,sY,sWidth,sHeight);
	resizeBuff(sizeof(RGBpack)*sWidth,(void**)&mRowArray);
}


// Set a pointer to a mask object. You do NOT own this mask. You can only use it. So don't
// go deleting it!
void bmpObj::setMask(mask* aMaskPtr) { mMask = aMaskPtr; }


void	bmpObj::drawSelf(void) {

	File		bmpFile;
	int		localY;
	int		syMax;
	colorObj	aColor;
	char*		filePath;
	
	filePath = mBMPObj->getDocFilePath();
	bmpFile = SD.open(filePath,FILE_READ);
	if (bmpFile) {
		mBMPObj->setGFile(&bmpFile);
		syMax = mSourceRect.y+mSourceRect.height;
		localY = y;
		for (int sourceY=mSourceRect.y;sourceY<syMax;sourceY++) {
			mBMPObj->getRow(sourceY,mRowArray,mSourceRect.width,mSourceRect.x);
			if (mMask) {
				for (int i=0;i<mSourceRect.width;i++) {
					if (mMask->checkPixel(i,localY-y)) {
						aColor.setColor(&(mRowArray[i]));
						screen->drawPixel(x+i,localY,&aColor);
					}
				}
			} else {
				for (int i=0;i<mSourceRect.width;i++) {
					aColor.setColor(&(mRowArray[i]));
					screen->drawPixel(x+i,localY,&aColor);
				}
			}
			localY++;
		}
		bmpFile.close();
		mBMPObj->setGFile(NULL);
	}
}