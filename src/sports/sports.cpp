#include "sports.h"

ofColor blck(0,0,0);
ofColor rd(0xD2,0x23,0x2A);
ofColor orng(0xFD,0x8D,0x21);

void Black()
{
	ofSetColor(blck);
}

void Red()
{
	ofSetColor(rd);
}

void Orange()
{
	ofSetColor(orng);
}

ofColor & black()
{
	return blck;
}

ofColor & red()
{
	return rd;
}

ofColor & orange()
{
	return orng;
}

ofFont lbl;

ofFont & label()
{
	return lbl;
}

void label(string str, int x, int y)
{
	lbl.drawString(str,x,y);
}
