#include "hsInterface.h"

//---------------- buttons

string report="report: System ready";
	
void hsButton::draw(int _x, int _y)
{
	x=_x, y=_y;
	ofSetColor(0xffffff);
	if(bPressed) ofSetColor(0x777777),pressImage.draw(x,y);
	else background.draw(x,y);
}

//-------------- Interface


void hsInterface::draw(int x, int y, int w, int h)
{
	ofSetColor(0xCCCCCC);
	ofShadeBox(activate.x-100,y,50,50,OF_DOWN,.4);
	ofShadeBox(activate.x-100,y+300,50,50,OF_DOWN,.4);
	ofShadeBox(activate.x-100,y+600,50,50,OF_DOWN,.4);

	//--------- instruction box
	ofSetColor(0xFD8D21);
	ofRect(25, y-50,activate.x-125,h-100);
	ofSetColor(0xD2232A);
	ofRect(35, y-40,activate.x-145,h-120);

	ofSetColor(0xffffff);
	rep.drawString("Operator Instructions",45,y-35);


	//---------- background image behind the camera and buttons
	ofSetColor(0xFD8D21);
	ofRect(activate.x-50, y-50,save.x+save.w-activate.x+100,h+150);
	ofSetColor(0xD2232A);
	ofRect(activate.x-40, y-40,save.x+save.w-activate.x+80,h+130);
	ofSetColor(0xFD8D21);
	ofRect(x-10, y-10,w+20,h+20);

	//--------- notification area
	ofSetColor(0xFD8D21);
	ofRect(x-10, y+h+15,w+20,70);
	ofSetColor(0x000000);
	ofRect(x, y+h+25,w,50);

	vector<string> spl= ofSplitString(report,":");

	if(spl[0]=="report") ofSetColor(0,255,0);
	if(spl[0]=="warning") ofSetColor(255,255,0);
	if(spl[0]=="error") ofSetColor(255,0,0);

	rep.drawString(report,x+10,y+h+30);

	//--------- buttons
	ofRoundShadow(activate.x-10,activate.y-10,activate.w+30, activate.h+20,10,.4);
	activate.draw(x-activate.w-50,y+h/3);
	ofRoundShadow(capture.x-10,capture.y-10,capture.w+20, capture.h+20,10,.4);
	capture.draw(x-capture.w-50,y+2*h/3);
	ofRoundShadow(review.x-10,review.y-10,review.w+30, review.h+20,10,.4);
	review.draw(x+w+50,y+h/3);
	ofRoundShadow(save.x-10,save.y-10,save.w+20, save.h+20,10,.4);
	save.draw(x+w+50, y+2*h/3);

	cam->draw(x,y);

	//--------- circle around relevant button
	int _x=0,_y=0;
	if(cam->framesReviewed()) _x=save.x+save.w/2,_y=save.y+save.h/2;
	else if(cam->framesCaptured()) _x=review.x+review.w/2,_y=review.y+review.h/2;
	else if(cam->isRecording()) _x=capture.x+capture.w/2,_y=capture.y+capture.h/2;
	else _x=activate.x+activate.w/2,_y=activate.y+activate.h/2;

	ofSetColor(255,255,0);
	ofRing(_x,_y,save.w/1.5,save.w/1.5+5);

	if(cam->isFetching()){
		ofSetColor(0,0,0,128);
		ofRect(0,0,ofGetWidth(),ofGetHeight());
		ofSetColor(0xffffff);
		spin.draw(ofGetWidth()/2,ofGetHeight()/2,ofGetHeight()/2-200);
		ofSetColor(0xCCCCCC);
		ofRect(ofGetWidth()/4,7*ofGetHeight()/8,ofGetWidth()/2,24);
		ofSetColor(0x0);
		ofRect(ofGetWidth()/4+3,7*ofGetHeight()/8+3,ofGetWidth()/2-5,18);
		ofSetColor(255,0,0);
		ofRect(ofGetWidth()/4+3,7*ofGetHeight()/8+3,(ofGetWidth()/2-5)*cam->percentPlayed(),18);
	}
}

void hsInterface::update()
{
	if(!cam->isRecording()&&activate.pressed()) activate.clickUp();
	if(!cam->isPlaying()&&review.pressed()) review.clickUp();
	if(cam->retrieved()&&save.pressed()){
		save.clickUp();
		string imgFldr="C:\\Users\\Public\\Pictures\\highSpeed\\set";
		report="report: Frames saved to network in position "+ofToString(folderIndex);
		char dir[1024];
		sprintf(dir,"%s %02i","data\\bin\\handleImages.bat", folderIndex);
		cout << dir << endl;
		call.run(dir);
		/*sprintf(dir,"%s \"%s\\_%02i\\300.jpg\"",ofToDataPath("bin\\PhotoResize141x161IO.exe").c_str(),"C:\\Users\\Public\\Pictures\\highSpeed\\set", folderIndex);
		call.run(dir);
		sprintf(dir,"move \"%s\\_%02i\\300.jpg\" \"%s\\_%02i\\thumb.jpg\"","C:\\Users\\Public\\Pictures\\highSpeed\\set", folderIndex,"C:\\Users\\Public\\Pictures\\highSpeed\\set", folderIndex);
		call.run(dir);*/
		//call.run(dir2);
		//call2.run(dir3);
		if(++folderIndex>10) folderIndex=0; 
		cam->resetFlags();
	}
	if(cam->isFetching()) spin.spin();
}
	
void hsInterface::clickDown( int x, int y)
{
	if(!cam->isFetching()){
	//---------- activate button
	if(!cam->isRecording()&&!activate.pressed()&&activate.clickDown(x,y)){
		cam->beginRecord(clipLength);
		report="report: Began recording a " + ofToString(clipLength,1) + " second loop";
		timer=ofGetElapsedTimef();
	}
	else if(cam->isRecording()&&activate.pressed()&&activate.clickDown(x,y)){
		cam->stopRecord();
		report="report: Camera deactivated";
	}

	//--------- capture button
	if(cam->isRecording()&&capture.clickDown(x,y)&&ofGetElapsedTimef()>timer+clipLength/2){
		cam->trigger();
		report="report: Begin capture";
	}
	else if(cam->isRecording()&&capture.clickDown(x,y)&&ofGetElapsedTimef()<timer+clipLength/2){
		report="warning: Pretrigger frames not captured, try again";
	}
	else if(!cam->isRecording()&&capture.clickDown(x,y)){
		report="warning: You must activate camera first";
	}

	//---------- review button
	if(cam->framesCaptured()&&!cam->isPlaying()&&review.clickDown(x,y)){
		cam->play();
	}
	else if(!cam->framesCaptured()&&review.clickDown(x,y)){
		review.clickUp();
		report="warning: You must capture frames first";
	}
	else if(cam->framesCaptured()&&cam->isPlaying()&&review.clickDown(x,y)){
		report="warning: rejected grabbed frames";
		cam->pause();
		cam->resetPlayback();
		cam->resetFlags();
	}

	//--------- save button
	if(cam->framesReviewed()&&save.clickDown(x,y)){
		cam->fetch();
		cam->resetPlayback();
	}
	else if(!cam->framesReviewed()&&save.clickDown(x,y)){
		save.clickUp();
		report="warning: You must review frames first";
	}
	}
}
	
void hsInterface::clickUp()
{
	capture.clickUp();
}
	
void hsInterface::setup(highSpeed * cm)
{
	cam=cm;
	activate.setup(150,99,"buttons/1regular.jpg","buttons/1selected.jpg");
	capture.setup(150,99,"buttons/2regular.jpg","buttons/2selected.jpg");
	review.setup(150,99,"buttons/3regular.jpg","buttons/3selected.jpg");
	save.setup(150,99,"buttons/4regular.jpg","buttons/4selected.jpg");

	folderIndex=0;

	rep.loadFont("fonts/Arial.ttf");
	rep.setSize(24);
	rep.setMode(OF_FONT_TOP);

	clipLength=2.0;
	timer=0;
}