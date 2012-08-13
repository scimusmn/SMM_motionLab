#include "testApp.h"
#include "config.h"

//--------------------------------------------------------------
void testApp::setup(){	 
	cfg().setup();

	cam.intitialize();
	
	cam.setFPS(cfg().framesPerSec);
	cam.setExposure(100);

	int midWid=cfg().camHgt;
	int midHgt=cfg().camWid;
	cam.setROI((cam.getMaxWidth()-midWid)/2,(cam.getMaxHeight()-midHgt)/2,midWid,midHgt);
	cam.setRotation(XS_ROT_270);

	//videoTexture.allocate(cam.width, cam.height, GL_BGR);
	//imageSave.allocate(cam.width, cam.height, OF_IMAGE_COLOR);

	hsCam.setup(&cam);
}


//--------------------------------------------------------------
void testApp::update(){
	cam.update();
	hsCam.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0,0,0);
	hsCam.drawInterface();
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	hsCam.clickDown(x,y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	hsCam.clickUp();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::exit(){
	cam.onExit();
}
