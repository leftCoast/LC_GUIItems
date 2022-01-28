#ifndef iconButton_h
#define iconButton_h

#include <bmpObj.h>

// Make a button with a 32x32 pixel icon stored in a .bmp file.
// Seeing its expecting a 32x32 pixel file we can simplify a bunch.
//
// Well, so much for fixed size. Now its a square thing of any size.
//
// FINE! Now its any rectangle?! You try to make things simple for people.
// I'll just add some sub classes for the simple cases.
//
// 9/19 - Doing my own icons now. (So there's suddenly a lot of them!) I think I'll take 
// this piece of code and twist it into a multi purpose, icon based, clickable UI thing.
//
// 11/21 - OK. bmpObj is your "everything using a .bmp file" thing. Use that to base
// drawing on. THIS is going back to being used for doing square icon based buttons. OK?
// Fine!


class iconButton :	public bmpObj {

	public:
				iconButton(int xLoc,int yLoc,const char* path,int pix=32);
	virtual	~iconButton(void);
	
	virtual	void	setEventSet(eventSet inEventSet);
	virtual	void	doAction(void);
};


#endif