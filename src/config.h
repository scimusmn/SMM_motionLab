#pragma once

#include "ofMain.h"
#include "ofExtended.h"
#include "../../dallasEng/dallasEng.h"

struct configuration {
	string folderRoot;
	string fileRoot;
	int numSets;
	string dest;
	int startingNumber;
	int portNumber;
	int camWid;
	int camHgt;
	int framesPerSec;
	float recordTime;
	void readGeneralConfig(string cfgFile);
	configuration(){
		ofEnableDataPath();
	}
	void setup(){
		readGeneralConfig("config.txt");
	}
};

configuration & cfg();