#ifndef bmpObj_h
#define bmpObj_h

#include <drawObj.h>
#include <bmpImage.h>
#include <bitmap.h>
#include <mask.h>


// This reads a .bmp file showing the silhouette of the shape you would like to draw. This
// silhouette is read in as a greyscale image. GREY_CUTOFF is used to decide whether each
// pixel is masked or drawn.
//
// Anything greater than this is NOT drawn. IE mask-white is for masking. Mask-black is for
// drawing.
#define GREY_CUTOFF	127


class bmpMask :	public mask {

	public:
				bmpMask(void);
	virtual	~bmpMask(void);
	
	virtual	void	readFromBMP(char* filePath);
	virtual	bool	checkPixel(int x,int y);
	
	protected:	
				long 	calcBuffSize(void);
				void	writeBit(int x, int y,bool trueFalse);
				bool	readBit(int x,int y);

				byte*	maskBits;
				int	width;
				int	height;
};



// Years later after so much messing about.
//
// This is to be the BASE CLASS for bitmap file drawing objects. Its a rect location on
// the screen that can spat a bitmap to itself. Want to do a picture, a button or an icon?
// Start with this. (Or just draw a bitmap)
//
// Basically this is the glue that hooks a .bmp filepath to a drawObj.

class bmpObj :	public drawObj,
					public bmpImage {

	public:
				bmpObj(int inX,int inY,int inWidth,int inHeight,char* bmpPath);
				bmpObj(rect* inRect,char* bmpPath);
	virtual	~bmpObj(void);
	
				void		setSourceOffset(int offstX,int offstY);
				void		setMask(mask* aMaskPtr);
	virtual	void		setGreyedOut(bool trueFalse);
	virtual	colorObj	greyscale(colorObj* inColor);
	virtual	void		drawSelf(void);
				
				int	offsetX;
				int	offsetY;
				mask*	mMask;
				bool	greyOut;
};


#endif