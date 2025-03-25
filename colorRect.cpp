#include "colorRect.h"


colorRect::colorRect(void)
	: drawObj(),colorObj() {
	
	setInset(0);
}


colorRect::colorRect(rect* inRect,colorObj* inColor,int inset)
	: drawObj(inRect),colorObj() {
	
	setColor(inColor);
	setInset(inset);
}


colorRect::colorRect(int inLocX,int inLocY,int inWidth,int inHeight,colorObj* inColor,int inset)
  : drawObj(inLocX,inLocY,inWidth,inHeight),colorObj() {
	
	setColor(inColor);
	setInset(inset);
}	

	
colorRect::colorRect(int inLocX, int inLocY, int inWidth,int inHeight,int inset)
  : drawObj(inLocX,inLocY,inWidth,inHeight),
  colorObj(LC_WHITE) { setInset(inset); }
  
  
colorRect::~colorRect(void) { }


void colorRect::setInset(int inset) {

    mInset = inset;
    needRefresh = true;
}


void colorRect::drawSelf(void) {

	colorObj  insetTLColor;
	colorObj  insetBRColor;
  
	screen->fillRect(x, y, width, height, (colorObj*)this);
	if (mInset) {
		if (mInset>0) {
    		insetTLColor = mixColors(&black,50);
    		insetBRColor = mixColors(&white,50);
    	} else {
    		insetTLColor = mixColors(&white,50);
    		insetBRColor = mixColors(&black,50);
    	}

    for (int i=0;i<abs(mInset);i++) {
      screen->drawVLine(x+i,y+i,height-2*i,&insetTLColor);
      screen->drawHLine(x+i,y+i,width-2*i,&insetTLColor);
      screen->drawVLine(x+(width-1)-i,y+i,height-i,&insetBRColor);	// I think it needs (width-1) in there.
      screen->drawHLine(x+i,y+height-i,width-i,&insetBRColor);
    }
  }
}


void colorRect::setColor(byte r,byte g,byte b) {

  colorObj::setColor(r,g,b);
  needRefresh = true;
}


void colorRect::setColor(word color16) {

  colorObj::setColor(color16);
  needRefresh = true;
}


void colorRect::setColor(colorObj* inColor) {

  colorObj::setColor(inColor);
  needRefresh = true;
}


void colorRect::setLocation(int inX, int inY)								{ rect::setLocation(inX,inY); setNeedRefresh(); }
void colorRect::setSize(int inWidth,int inHeight)							{ rect::setSize(inWidth,inHeight); setNeedRefresh(); }
void colorRect::setRect(rect* inRect)											{ rect::setRect(inRect); setNeedRefresh(); }
void colorRect::setRect(point* inPt1,point* inPt2)							{ rect::	setRect(inPt1,inPt2); setNeedRefresh(); }			
void colorRect::setRect(int inX, int inY, int inWidth,int inHeight)	{ rect::setRect(inX,inY,inWidth,inHeight); setNeedRefresh(); }
void colorRect::insetRect(int inset)											{ rect::insetRect(inset); setNeedRefresh(); }
void colorRect::addRect(rect* inRect)											{ rect::addRect(inRect); setNeedRefresh(); }


