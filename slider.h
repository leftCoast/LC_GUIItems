#ifndef slider_h
#define slider_h

#include <mapper.h>
#include <drawObj.h>
#include <eventMgr.h>
#include <colorRect.h>

#define DEF_SLIDER_KNOB_T		8			// Knob thickness.
#define DEF_SLIDER_MIN			0
#define DEF_SLIDER_MAX			100
#define DEF_SLIDER_IVAL			50.0

#define DEF_SLIDER_BCOLOR		LC_WHITE
#define DEF_SLIDER_KNBCOLOR	LC_DARK_GREY



// **************************************************************
// ************************    knobObj    ***********************
// **************************************************************


class knobObj :	public colorRect {

	public:
					 knobObj(int length,int thickness,int inRange,bool vertical,colorObj* bkColor);
	virtual       ~knobObj(void);

	virtual	void	eraseSelf(void);
				void	setPos(point* inPt);
				float	getValue(void);
				void	setValue(float value);
				
				int		range;
				int		thickness;
				bool		vertical; 		
				colorObj	backColor;
				mapper	posToVal;
				mapper	valtoPos;
				int		prevX;
				int		prevY;
};



// **************************************************************
// *************************    slider    ***********************
// **************************************************************	


class slider :  public drawGroup {

	public:
					 slider(int x,int y,int width,int height,bool inVertical=false);
	virtual       ~slider(void);
	
				float getValue(void);                           // Just in case you were wondering.
				void  setValue(float value);							// Great for pre-loading.
	virtual	void  drawSelf(void);
	virtual	void  doAction(event* inEvent,point* localPt);
	
				bool			vertical;
				knobObj*		knob;
				colorObj		backColor;				
};
 
#endif
