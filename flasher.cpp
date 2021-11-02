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

    
bmpFlasher::bmpFlasher(rect* inRect,char* onBmpPath, char* offBmpPath)
  : flasher(inRect,&black) { 
  
  mReady = false;
  setup(onBmpPath,offBmpPath);
}  

    
bmpFlasher::~bmpFlasher(void) {

  if (mOnBmp) delete(mOnBmp);
  if (mOffBmp) delete(mOffBmp);
}


void bmpFlasher::setup(char* onBmpPath,char* offBmpPath) {

  mOnBmp = new bmpObj(this,onBmpPath);
  mOffBmp = new bmpObj(this,offBmpPath);
  mReady = mOnBmp && mOffBmp;
}


void bmpFlasher::drawSelf(void) {

  if (mReady) {							// All ducks in row?
    if (mFlash) {							// We want it on?
    	mOnBmp->setLocation(x,y);		// Just in case. 
      mOnBmp->draw();					// Do it.
    } else {								// Or off?
    	mOffBmp->setLocation(x,y);		// Just in case. 
      mOffBmp->draw();					// Do it.
    }
  }
}
