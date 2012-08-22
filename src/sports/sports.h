#pragma once

#include "ofMain.h"
#include "ofExtended.h"

void Black();
void Red();
void Orange();
void Blue();
void Gray();
void Yellow();

ofColor & black();
ofColor & red();
ofColor & orange();
ofColor & yellow();
ofColor & gray();
ofColor & blue();

ofFont & label();
void label(string str, int x, int y);