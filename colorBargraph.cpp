#include <colorBargraph.h>
#include <colorObj.h>
//#include "debug.h"

colorBargraph::colorBargraph(rect* inRect,orientation inOrientation)
	: colorRect(inRect,&white) {
	
	ourOrientation = inOrientation;
	sizeMapper.setValues(0,0,0,0);
	maxVal	= 0;
	minVal	= 0;
	value		= 0;
	empty = true;
}
	

colorBargraph::~colorBargraph(void) {  }
   
   	
void colorBargraph::addColor(double inX, colorObj* color) {

	
	if (empty) {
		maxVal = inX;
		minVal = inX;
		empty = false;
	} else {
		if (inX>maxVal) maxVal=inX;
		else if (inX<minVal) minVal = inX;
	}
	setupMapper();
	colorMultiMap::addColor(inX,color);
	setNeedRefresh();
}


void colorBargraph::setupMapper(void) {

	switch(ourOrientation) {
		case bottomUp	: sizeMapper.setValues(minVal,maxVal,height,0); break;
		case leftRight	: sizeMapper.setValues(minVal,maxVal,0,width); break;
		case topDown	: sizeMapper.setValues(minVal,maxVal,0,height); break;
		case rightLeft	: sizeMapper.setValues(minVal,maxVal,width,0); break;
	}
}


void colorBargraph::clearMap(void) {

	colorMultiMap::clearMap();
	sizeMapper.setValues(0,0,0,0);
	maxVal	= 0;
	minVal	= 0;
	value		= 0;
	empty = true;	
}

	
void colorBargraph::setValue(float inVal) {

	value = inVal;
	if (!empty) {
		setNeedRefresh();
	}
}
	
				
float colorBargraph::getValue(void) { return value; }


void  colorBargraph::drawSelf(void) {

	rect	saveRect(this);
	float mappedValue;
	int	limit;
	
	drawRect.setRect(this);
	colorObj	theColor;
	theColor = map(value);
	setColor(&theColor);
	mappedValue = sizeMapper.map(value);
	switch(ourOrientation) {
		case bottomUp	:
			drawRect.y			= drawRect.y + mappedValue;
			drawRect.height	= drawRect.height - mappedValue;
		break;
		case leftRight	:
			drawRect.width		= mappedValue;
		break;
		case topDown	:
			drawRect.height	= mappedValue; 
		break;
		case rightLeft	:
			drawRect.x			= drawRect.x + mappedValue;
			drawRect.width		= drawRect.width - mappedValue;
		break;
	}
	rect::setRect(&drawRect);	// Our children can use draw rect to know where we drew.
	colorRect::drawSelf();
	rect::setRect(&saveRect);
	
}

		
void colorBargraph::setLocation(int inX, int inY)								{ colorRect::setLocation(inX,inY); setupMapper(); }
void colorBargraph::setSize(int inWidth,int inHeight)							{ colorRect::setSize(inWidth,inHeight); setupMapper(); }
void colorBargraph::setRect(rect* inRect)											{ colorRect::setRect(inRect); setupMapper(); }
void colorBargraph::setRect(point* inPt1,point* inPt2)						{ colorRect::	setRect(inPt1,inPt2); setupMapper(); }			
void colorBargraph::setRect(int inX, int inY, int inWidth,int inHeight)	{ colorRect::setRect(inX,inY,inWidth,inHeight); setupMapper(); }
void colorBargraph::insetRect(int inset)											{ colorRect::insetRect(inset); setupMapper(); }
void colorBargraph::addRect(rect* inRect)											{ colorRect::addRect(inRect); setupMapper(); }
			