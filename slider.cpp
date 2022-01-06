#include <slider.h>

//#include <debug.h>



// **************************************************************
// ************************    knobObj    ***********************
// **************************************************************


knobObj::knobObj(int length,int inThickness,int inRange,bool inVertical,colorObj* bkColor)
	: colorRect(0,0,20,20,-2) {
	
	range			= inRange;
	vertical		= inVertical;
	thickness	= inThickness;
	prevY			= -1;
	prevX			= -1;
	if (vertical) {
		x			= 0;
		y			= 0;
		width		= length;
		height	= thickness;
		posToVal.setValues(range-thickness,0,0,100);
		valtoPos.setValues(0,100,range-thickness,0);
	} else {
		x			= 0;
		y			= 0;
		width		= thickness;
		height	= length;
		posToVal.setValues(0,range-thickness,0,100);
		valtoPos.setValues(0,100,0,range-thickness);
	}		
	setColor(DEF_SLIDER_KNBCOLOR);
	backColor.setColor(bkColor);
}
	
	
knobObj::~knobObj(void) {  }


void knobObj::eraseSelf(void) {
	
	rect aRect(this);
	
	aRect.x = lastX;
	aRect.y = lastY;
	screen->fillRect(&aRect,&backColor);
}


void knobObj::setPos(point* inPt) {

	int limited;
	
	if (vertical) {
		if (inPt->y>range-thickness) limited = range-thickness;
		else if (inPt->y<0) limited = 0;
		else limited = inPt->y;
		if (limited==prevY) return;
		setLocation(0,limited);
		prevY = limited;
	} else {
		if (inPt->x>range-thickness) limited = range-thickness;
		else if (inPt->x<0) limited = 0;
		else limited = inPt->x;
		if (limited==prevX) return;
		setLocation(limited,0);
		prevX = limited;
	}
}


float knobObj::getValue(void) {

	if (vertical) return posToVal.map(y);
	else return posToVal.map(x);
}


void knobObj::setValue(float value) {

	if (vertical) setLocation(0,valtoPos.map(value));
	else setLocation(valtoPos.map(value),0);
}


				
// **************************************************************
// *************************    slider    ***********************
// **************************************************************	


 slider::slider(int x,int y,int width,int height,bool inVertical) 
   : drawGroup(x,y,width,height,touchNDrag) {
   
   int	range;
   int	length;
   
   vertical = inVertical;
   backColor.setColor(DEF_SLIDER_BCOLOR);
   if (vertical) {
   	range = height;
   	length = width;
   } else {
   	range = width;
   	length = height;
   }
   knob = new knobObj(length,DEF_SLIDER_KNOB_T,range,vertical,&backColor);
   addObj(knob);
}


slider::~slider(void) { }


float slider::getValue(void) { return knob->getValue(); }


void slider::setValue(float value) { knob->setValue(value); }


void slider::drawSelf(void) {

	this->insetRect(-1);
	screen->drawRect(this,&black);
	this->insetRect(1);
	screen->fillRect(this,&backColor);
}
	
	
void slider::doAction(event* inEvent,point* localPt) {

	point offsetPt;

	switch(inEvent->mType) {			// We have an incoming event. Check the type.
		case touchEvent	:				// THE FINGER!  
		case dragBegin		:				// A starting off a drag action.
		case dragOn			:				// Drag on event.
			offsetPt.x = localPt->x-x;
 			offsetPt.y = localPt->y-y;
			knob->setPos(&offsetPt);	// Set our new position..
		break;								// Done here.
		default : break;					// Shut up compiler!
	}
}

