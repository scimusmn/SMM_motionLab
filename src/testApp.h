#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofExtended.h"
#include "XStrmAPI.h"
#include "highSpeed.h"
#include "interface\hsInterface.h"
#include "threadedImageSave\threadImageSave.h"

class testApp : public ofBaseApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		void exit();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		
		ofVideoGrabber 		vidGrabber;
		unsigned char * 	videoInverted;
		
		int 				camWidth;
		int 				camHeight;

		ofImage				imageSave;
		ofTexture			videoTexture;

		highSpeed cam;

		ofThreadImageSave	save;
		hsInterface hsCam;
};

#endif	
