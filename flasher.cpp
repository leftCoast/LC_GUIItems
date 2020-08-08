#include "flasher.h"

#define	DEF_FLASH_PERIOD	1000
#define	DEF_FLASH_PULSE	500


// *****************************************************
//                        flasher
// *****************************************************


flasher::flasher(rect* inRect,colorObj* onColor,colorObj* offColor)
  : drawObj(inRect), 
  squareWave() {

	mFlash = false;						// State of the flash, light on or light off.
	setPeriod(DEF_FLASH_PERIOD);		// Setup default period.
	setPulse(DEF_FLASH_PULSE);			// Setup default pulse.
	setColors(onColor,offColor);		// Setup the colors.
}


flasher::flasher(int inX,int inY,int inWidth,int inHeight,colorObj* onColor,colorObj* offColor)
  : drawObj(inX,inY,inWidth,inHeight), 
  squareWave() {
  
	mFlash = false;						// State of the flash, light on or light off.
	setPeriod(DEF_FLASH_PERIOD);		// Setup default period.
	setPulse(DEF_FLASH_PULSE);			// Setup default pulse.
	setColors(onColor,offColor);		// Setup the colors.
}

                     
flasher::~flasher(void) { }


void flasher::setColors(colorObj* onColor,colorObj* offColor) {

	if (onColor) mForeColor.setColor(onColor);	// What is "on"?
	if (offColor) mBackColor.setColor(offColor);	// What is "off"?
	setNeedRefresh();
}


// What to do when pulse goes high.
void flasher::pulseOn(void) {
	
	mFlash = true;
	setNeedRefresh();
}


// What to do when pulse goes low.                                                      
void flasher::pulseOff(void) {

	mFlash = false;
	setNeedRefresh();
}                                                      
   
   
void flasher::drawSelf(void) {

  if (mFlash) {
    screen->fillRect(x,y,width,height,&mForeColor);
  } else {
    screen->fillRect(x,y,width,height,&mBackColor);
  }
}



// *****************************************************
//                     bmpFlasher
// *****************************************************


bmpFlasher::bmpFlasher(int inX,int inY, int width,int height,char* onBmp, char* offBmp)
  : flasher(inX,inY,width,height,&black) { 
 
  mReady = false;
  setup(onBmp,offBmp);
}

    
bmpFlasher::bmpFlasher(rect* inRect,char* onBmp, char* offBmp)
  : flasher(inRect,&black) { 
  
  mReady = false;
  setup(onBmp,offBmp);
}  

    
bmpFlasher::~bmpFlasher(void) {

  if (mOnBmp) delete(mOnBmp);
  if (mOffBmp) delete(mOffBmp);
}


void bmpFlasher::setup(char* onBmp,char* offBmp) {

  rect  sourceRect(0,0,width,height);
  mOnBmp = new bmpPipe(&sourceRect);
  mOffBmp = new bmpPipe(&sourceRect);
  if (mOnBmp&&mOffBmp) {
    if (mOnBmp->openPipe(onBmp) && mOffBmp->openPipe(offBmp)) {
      mReady = true;
    }
  }
}


void bmpFlasher::drawSelf(void) {

  if (mReady) {
    if (mFlash) {
      mOnBmp->drawImage(x,y);
    } else {
      mOffBmp->drawImage(x,y);
    }
  }
}
