#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	 

	cam.intitialize();
	
	cam.setFPS(300);
	cam.setExposure(50);

	int midWid=894;
	int midHgt=784;
	cam.setROI(0,(cam.getMaxHeight()-midHgt)/2,midWid,midHgt);
	cam.setRotation(XS_ROT_90);

	videoTexture.allocate(cam.width, cam.height, GL_BGR);
	imageSave.allocate(cam.width, cam.height, OF_IMAGE_COLOR);

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
	hsCam.draw(800,180, imageSave.width, imageSave.height);
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
