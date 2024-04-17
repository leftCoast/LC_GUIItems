#include <iconButton.h>


iconButton::iconButton(int xLoc,int yLoc,const char* path,int pix)
	: bmpObj(xLoc,yLoc,pix,pix,path) { setEventSet(touchLift); }
	
	
iconButton::~iconButton(void) { }


void iconButton::setEventSet(eventSet inEventSet) {
	
	if (inEventSet==noEvents) {
		setGreyedOut(true);
	} else {
		setGreyedOut(false);
	}
	setNeedRefresh();
	bmpObj::setEventSet(inEventSet);
}
