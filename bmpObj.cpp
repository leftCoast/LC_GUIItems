#include "bmpObj.h"
#include "resizeBuff.h"

// We only draw one at a time. Maybe, this is better used as a global so there is no
// chance or multiples out there in RAM. Also it seems that it's constructor is really
// slow!
File		bmpFile;


// *************************************
//     	A .bmp file mask class
// *************************************


bmpMask::bmpMask(void) { maskBits = NULL; }

bmpMask::~bmpMask(void) { if (maskBits) resizeBuff(0,&maskBits); }


void	bmpMask::readFromBMP(char* filePath) {

	bmpImage*	alphaFile;
	int			numBytes;
	colorObj		aColor;
	byte			alpha;

	alphaFile = new bmpImage(filePath);							// Have a go at creating the .bmp file object.
	if (alphaFile) {													// If we can create the .bmp file object..
		if (alphaFile->openDocFile(FILE_READ)) {				// If we can open the .bmp file..
			width = alphaFile->getWidth();						// We read and save the width.
			height = alphaFile->getHeight();						// We read and save the height.
			numBytes = calcBuffSize();								// Calculate our buff size.
			if (resizeBuff(numBytes,&maskBits)) {				// If we can allocate the bit buffer..
				for(int y=0;y<height;y++) {						// For every line..
					for(int x=0;x<width;x++) {						// For every pixel in that line..
						aColor = alphaFile->getPixel(x,y);		// Grab the pixel.
						alpha = aColor.getGreyscale();			// Calculate the brightness of the pixel.
						writeBit(x,y,alpha<128);					// Set the bitmap bit accordingly.
					}
				}
			}
			alphaFile->closeDocFile();									// All done, clean up shop.
		}
		delete(alphaFile);												// Recycle the .bmp file object.
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



bmpObj::bmpObj(int inX,int inY,int inWidth,int inHeight,char* bmpPath)
	: drawObj(inX,inY,inWidth,inHeight),
	bmpImage(bmpPath) {
	
	mMask		= NULL;		// No mask yet.
	offsetX	= 0;			// No offset. Typically there will be none.
	offsetY	= 0;
}

		
bmpObj::~bmpObj(void) {  }


// Setup the source offset for reading from the .bmp file.
void bmpObj::setSourceOffset(int offstX,int offstY) {

		offsetX = offstX;
		offsetY = offstY;
}


// Set a pointer to a mask object. You do NOT own this mask. You can only use it. So don't
// go deleting it!
void bmpObj::setMask(mask* aMaskPtr) { mMask = aMaskPtr; }


// Here we draw! If the file stuff is a bust.. We'll draw some default thing. 
void	bmpObj::drawSelf(void) {

	colorObj	aColor;
	
	if (openDocFile(FILE_READ)) {
		if(mMask) {
			for(int ty=0;ty<drawObj::height;ty++) {
				for(int tx=0;tx<drawObj::width;tx++) {
					if (mMask->checkPixel(tx,ty)) {
						aColor = getPixel(tx+offsetX,ty+offsetY);
						screen->drawPixel(tx+x,ty+y,&aColor);
					} 	
				}
			}
		} else {
			for(int ty=0;ty<drawObj::height;ty++) {
				for(int tx=0;tx<drawObj::width;tx++) {
					aColor = getPixel(tx+offsetX,ty+offsetY);
					screen->drawPixel(tx+x,ty+y,&aColor);	
				}
			}
		}
		closeDocFile();
	} else {
		drawObj::drawSelf();
	}
}