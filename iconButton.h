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
// this piece of code and twist it into a multi purpose, icon based, UI thing.



class iconButton :	public bmpObj {

	public:
				iconButton(int xLoc,int yLoc,char* path,int pix=32);
	virtual	~iconButton(void);
	
	virtual	void	doAction(void);
};


// HAVE NOT TESTED THESE YET!!!

class iconButton32 :	public iconButton {

	public:
				iconButton32(int xLoc, int yLoc,char* path);
	virtual	~iconButton32(void);
	
};


class iconButton22 : public iconButton {

	public:
				iconButton22(int xLoc, int yLoc,char* path);
	virtual	~iconButton22(void);
};


#endif