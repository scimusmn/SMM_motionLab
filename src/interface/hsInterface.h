#include "ofMain.h"
#include "ofExtended.h"
#include "XStrmAPI.h"
#include "../highSpeed.h"
#include "ofxSystemCall.h"

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
	double timer;
	double clipLength;

	ofProgressSpinner spin;
public:
	void draw(int x, int y, int w, int h);
	void update();
	void clickDown( int x, int y);
	void clickUp();
	void setup(highSpeed * cam);
};