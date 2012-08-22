#include "ofMain.h"
#include "ofExtended.h"
#include "XstrmAPI.h"
#include "../highSpeed.h"
#include "ofxSystemCall.h"
#include "ofxNetwork.h"

class hsButton : public ofButton {
public:
	void draw(int _x, int _y);
};

class hsInterface {
	highSpeed * cam;
	hsButton activate;
	hsButton review;
	ofButton capture;
	ofButton save;
	ofSystemCall call;
	ofFont rep;
	int folderIndex;
	//double timer;
	ofTimer timer;
	double clipLength;

	ofTimer delay;
	ofxTCPServer TCP;
	ofProgressSpinner spin;

	ofRectangle repBar;

	ofImage title;
public:
	~hsInterface();
	void draw(int x, int y, int w, int h);
	void drawInterface();
	void drawInstructions(int x, int y, int w, int h);
	void drawReportBar();
	void update();
	void clickDown( int x, int y);
	void clickUp();
	void setup(highSpeed * cam);
};

