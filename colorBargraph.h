#ifndef colorBargraph_h
#define colorBargraph_h

#include <colorRect.h>
#include <colorObj.h>


// A colored bar graph. The constructor rect sets the full value size. orientation sets the
// direction the bar graph grows as the value goes up. The value limits are set by adding
// colors at different values. The lowest and highest color values set the high and low
// limits of the bar graph. As the values cross the color values the bar changes color to
// match. Intermediate colors will be blends between their end point colors.
//
// There is no background color. You will have to set up the background yourself.


enum orientation {
	bottomUp,
	leftRight,
	topDown,
	rightLeft
};


class colorBargraph :	public colorRect,
								public colorMultiMap {
	public:
				colorBargraph(rect* inRect,orientation inOrientation=bottomUp);
	virtual	~colorBargraph(void);
	
	virtual	void	addColor(double inX, colorObj* color);
				void	setupMapper(void);
	virtual	void	clearMap(void);
				void	setValue(float inVal);
				float getValue(void);
	virtual	void	drawSelf(void);
	
	virtual	void  setLocation(int inX, int inY);
	virtual	void  setSize(int inWidth,int inHeight);
  	virtual	void  setRect(rect* inRect);                 					
  	virtual	void  setRect(point* inPt1,point* inPt2);							
  	virtual	void	setRect(int inX, int inY, int inWidth,int inHeight);	
  	virtual	void	insetRect(int inset);											
   virtual	void	addRect(rect* inRect);
   
   			rect			drawRect;
				mapper		sizeMapper;
				orientation	ourOrientation;
				float			minVal;
				float			maxVal;
				float			value;
				bool			empty;
};


#endif