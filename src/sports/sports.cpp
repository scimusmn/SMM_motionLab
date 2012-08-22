#include "sports.h"

ofColor blck(3,3,3);
ofColor rd(0xD2,0x23,0x2A);
ofColor orng(0xF2,0x66,0x49);
ofColor ylw(0xFD,0xEA,0x32);
ofColor blu(0x00,0xB5,0xAD);
ofColor gry(0xc4,0xc6,0xc8);

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

void Blue()
{
	ofSetColor(blu);
}

void Yellow()
{
	ofSetColor(ylw);
}

void Gray()
{
	ofSetColor(gry);
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

ofColor & blue()
{
	return blu;
}

ofColor & yellow()
{
	return ylw;
}

ofColor & gray()
{
	return gry;
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
