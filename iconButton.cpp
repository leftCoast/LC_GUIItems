#include <iconButton.h>
#include <resizeBuff.h>


// Its a SQUARE icon. Used to be fixed at 32x32 but no..
// Now we want different sizes, don't we?


iconButton::iconButton(int xLoc,int yLoc,char* path,int pix)
	: bmpObj(xLoc,yLoc,pix,pix,path) {
	
	setEventSet(touchLift);
}
	
	
iconButton::~iconButton(void) { }


void iconButton::doAction(void) {  }



iconButton32::iconButton32(int xLoc, int yLoc,char* path)
	: iconButton(xLoc,yLoc,path,32) {  }


iconButton32::~iconButton32(void) {  }



iconButton22::iconButton22(int xLoc, int yLoc,char* path)
	: iconButton(xLoc,yLoc,path,22) {  }


iconButton22::~iconButton22(void) {  }
