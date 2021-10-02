#include "bmpObj.h"
#include "resizeBuff.h"



// *************************************
//     The -tiny- mask base class.
// *************************************


mask::mask(void) {  }
mask::~mask(void) {  }
bool mask::checkPixel(int x,int y) { return true; }



// *************************************
//     	A .bmp file mask class
// *************************************


bmpMask::bmpMask(void) { maskBits = NULL; }

bmpMask::~bmpMask(void) { if (maskBits) resizeBuff(0,&maskBits); }

	
void	bmpMask::readFromBMP(char* filePath) {

	bmpImage	alphaFile;
	long		numBytes;
	colorObj	thePixel;
	byte		alpha;
	
	if (alphaFile.openDocFile(filePath)) {					// Ok, If we have a valid .bmp file..
		width = alphaFile.getWidth();							// We read and save the width.
		height = alphaFile.getHeight();						// We read and save the height.
		numBytes = calcBuffSize();								// Calculate our buff size.
		if (resizeBuff(numBytes,&maskBits)) {				// If we can allocate the bit buffer..
			for (int y=0;y<height;y++) {						// For each row in the image..
				for (int x=0;x<width;x++) {					// For each pixel in the row..
					thePixel = alphaFile.getPixel(x,y);		// Grab the pixel.					
					alpha = thePixel.getGreyscale();			// Calculate the brightness of the pixel.
					writeBit(x,y,alpha<128);					// Set the bitmap bit accordingly.
				}
			}		
		}
	}
}


bool	bmpMask::checkPixel(int x,int y) { return readBit(x,y); }
	

long 	bmpMask::calcBuffSize(void) {
	
	int	rowBytes;
	
	rowBytes = width/8;				// Making a bit array, divide by 8.
	if (width % 8) rowBytes++;		// Add one if there is a remainder.
	return rowBytes * height;		// Return result;
}
	

void	bmpMask::writeBit(int x, int y,bool trueFalse) {
	
	int	byteIndex;
	byte	bitIndex;
	
	if (maskBits) {											// Sanity check. Is there a bit buffer?..
		byteIndex = ((y * width) + x) / 8;				// Offset into the byte buffer.
		bitIndex = x % 8;										// Offset into the bit field.
		if (trueFalse) {										// Lets see what the user wants..
			bitSet(maskBits[byteIndex],bitIndex);		// If its true, set the bit.
		} else {													// Else, ,ust be false..
			bitClear(maskBits[byteIndex],bitIndex);	// Clear the bit.
		}
	}
}


bool	bmpMask::readBit(int x,int y) {
	
	int	byteIndex;
	byte	bitIndex;
	
	if (maskBits) {												// If we have a mask buffer..
		byteIndex = ((y * width) + x) / 8;					// Calc the byte index.
		bitIndex = x % 8;											// Calc the bit index.
		return bitRead(maskBits[byteIndex],bitIndex);	// return the result.
	} else {															// Else, we have no mask?..
		return true;												// Whatever. No mask means draw it all.
	}
}


	
// *************************************
//          The bmpObj class
// *************************************


// We only draw one at a time. Maybe, this is better used as a global so there is no
// chance or multiples out there in RAM. Also it seems that it's constructor is really
// slow!
File		bmpFile;


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
// This is also your second chance to set the image file's path.
bool bmpObj::begin(char* bmpPath) {

	
	if (!mBMPObj) {											// No bmpImage? (Shouldn't have one)
		mBMPObj	= new bmpImage();							// Create our bmpImage object. We'll use it to draw stuff.
	}
	if (mBMPObj) {												// If we were successful in creating one.
		if (bmpPath) {											// If we were passed in a path..
			resizeBuff(0,&mPath);							// If we have a saved path, dump it.
			return mBMPObj->openDocFile(bmpPath);		// Return if we can open and read the image.
		} else if (mPath) {									// Else if we have a saved path..
			if (mBMPObj->openDocFile(mPath)) {			// If we can open and read the image..
				resizeBuff(0,&mPath);						// Then we can dump our copy of the path.
				return true;									// And we return success.
			} else {												// Else we couldn't read the file..
				return false;									// Something is wrong. Return false.
			}
		}
		return true;											// We achieved what they asked for. Return true;
	}
	return false;												// Nothing but failure! Return false.
}
				

// Setup the source rect for reading from the .bmp file.
void bmpObj::setSourceRect(int sX,int sY,int sWidth,int sHeight) {
	mSourceRect.setRect(sX,sY,sWidth,sHeight);
	resizeBuff(sizeof(RGBpack)*sWidth,(void**)&mRowArray);
}


// Set the path after begin() has been called or change the path..
bool bmpObj::setBmpPath(char* bmpPath) {

	if (mBMPObj) {											// If we have the mBMPObj..
		return mBMPObj->openDocFile(bmpPath);		// return if we can open this new path.
	}
	return false;											// No mBMPObj? Thi is messed up! Return false.
}


// Set a pointer to a mask object. You do NOT own this mask. You can only use it. So don't
// go deleting it!
void bmpObj::setMask(mask* aMaskPtr) { mMask = aMaskPtr; }


void	bmpObj::drawSelf(void) {

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