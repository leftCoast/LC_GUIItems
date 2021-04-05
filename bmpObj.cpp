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
	
	mMask			= NULL;											// Start pointer at NULL, just in case.. But this one is actually a flag.
	mRowArray	= NULL;											// NULL
	mPath			= NULL;											// NULL NULL.
	mBMPObj		= NULL;											// And NULL.
	setSourceRect(0,0,inWidth,inHeight);					// Default source is top left of bitmap, our size. 
	if (bmpPath) {													// If we got a bmpImage path..
		if (resizeBuff(strlen(bmpPath)+1,&mPath)) {		// If we can get the RAM to store said path..
			strcpy(mPath,bmpPath);								// Save the path for later.
		}
	}
}

		
bmpObj::~bmpObj(void) {

	if (mBMPObj) delete(mBMPObj);			// If we got a mBMPObj, delete it.
	resizeBuff(0,(void**)&mRowArray);	// Save to rall resizeBuff->0 on these.
	resizeBuff(0,(void**)&mPath);			// CYA memory!
}

	
// Some stuff must wait 'till our hardware is up and running. like SD cards.	
bool bmpObj::begin(void) {

	if (!mBMPObj && mPath) {							// If we don't have a bmpImage object. (Shouldn't) but DO have a path.. (Should)
		mBMPObj	= new bmpImage();						// Create our bmpImage object. We'll use it to draw stuff.
		if (mBMPObj) {										// If we had success in creating it.. (Should)
			if (mBMPObj->openDocFile(mPath)) {		// If we can open and read an image from the path we saved..
				resizeBuff(0,&mPath);					// Then we can dump the copy of our path. Its saved in the bmpImage thing now.
				return true;								// And we return success.
			}
		}
	}
	return false;											// If we land here? Something went wrong.
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
		syMax = mSourceRect.y+mSourceRect.height;
		localY = y;
		for (int sourceY=mSourceRect.y;sourceY<syMax;sourceY++) {
			if (!sourceY) {
				mBMPObj->getRow(0,mRowArray,mSourceRect.width,&bmpFile);
			} else {
				mBMPObj->getRow(-1,mRowArray,mSourceRect.width,&bmpFile);
			}
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
	}
}