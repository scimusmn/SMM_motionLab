#include "hsInterface.h"
#include "../config.h"
#include "../sports/sports.h"

//---------------- buttons

string report="report: System ready";
	
void hsButton::draw(int _x, int _y)
{
	x=_x, y=_y;
	ofSetColor(0xffffff);
	if(bPressed) ofSetColor(0x777777),pressImage.draw(x,y,w,h);
	else background.draw(x,y,w,h);
}

//-------------- Interface

hsInterface::~hsInterface(){
	if(TCP.close()){
		//cout << "Closing nicely\n";
	}
}

void hsInterface::draw(int x, int y, int w, int h)
{
	ofPoint border(10,10);
	ofPoint pad(60,40);
	int btnWid=activate.w;
	int btnHgt=activate.h;

	//Main background section
	Black();
	ofRect(x,y,w,h);

	//Camera image size
	float camWid=cam->width;
	float camHgt=cam->height;
	float camWidNew=w-(btnWid*2+border.x*2+pad.x*4);
	float camHgtNew=h-(border.y*4+pad.y*3+repBar.height+title.height);
	if(camWidNew<camWid){
		camHgt=camHgt*(camWidNew/camWid);
		camWid=camWidNew;
	}
	if(camHgtNew<camHgt){
		camWid=camWid*(camHgtNew/camHgt);
		camHgt=camHgtNew;
	}
	if((camWidNew>camWid)&&!(camHgtNew<camHgt*(camWidNew/camWid))){
		camHgt=camHgt*((camWidNew-50)/camWid);
		camWid=camWidNew;
	}
	if((camHgtNew>camHgt)&&!(camWidNew<camWid*(camHgtNew/camHgt))){
		camWid=camWid*((camHgtNew-50)/camHgt);
		camHgt=camHgtNew;
	}

	int camX=x+(w-camWid)/2;
	int camY=y+border.y*2+pad.y*2+title.height;

	ofSetColor(255,255,255);
	title.draw(x+pad.x,y+pad.y);

	//Draw the camera image border
	Gray();
	ofRect(camX-border.x,camY-border.y,camWid+border.x*2,camHgt+border.y*2);
	cam->draw(camX,camY,camWid,camHgt);

	//--------- notification area
	repBar.x=x+border.x+pad.x;
	repBar.y=camY+camHgt+pad.y;
	repBar.width=w-(border.x+pad.x*2);
	repBar.height=70;

	drawReportBar();

	//--------- buttons
	int shade=7;
	ofSetShadowDarkness(.4);
	ofShadowRounded(activate.x,activate.y,activate.w, activate.h,shade);
	ofShadowRounded(capture.x,capture.y,capture.w, capture.h,shade);
	ofShadowRounded(review.x,review.y,review.w, review.h,shade);
	ofShadowRounded(save.x,save.y,save.w, save.h,shade);

	int _x=0,_y=0;
	if(cam->framesReviewed()) _x=save.x,_y=save.y;
	else if(cam->framesCaptured()) _x=review.x,_y=review.y;
	else if(cam->isRecording()) _x=capture.x,_y=capture.y;
	else _x=activate.x,_y=activate.y;

	_x-=5,_y-=5;

	ofSetColor(yellow().opacity((ofGetElapsedTimeMillis()/500)%2));
	ofRoundedRect(_x,_y,activate.w+10,activate.h+10,5);

	activate.draw(camX-(border.x+pad.x+activate.w),camY-border.y);
	capture.draw(camX-(border.x+pad.x+activate.w),camY+camHgt-capture.h-pad.y);
	review.draw(camX+camWid+border.x+pad.x,camY-border.y);
	save.draw(camX+camWid+border.x+pad.x, camY+camHgt-save.h-pad.y);

	if(cam->isFetching()||cam->retrieved()){
		ofSetColor(0,0,0,128);
		ofRect(0,0,ofGetWidth(),ofGetHeight());
		ofSetColor(0xffffff);
		spin.draw(ofGetWidth()/2,ofGetHeight()/2,ofGetHeight()/2-200);
		Gray();
		ofRect(ofGetWidth()/4,7*ofGetHeight()/8,ofGetWidth()/2,24);
		Black();
		ofRect(ofGetWidth()/4+3,7*ofGetHeight()/8+3,ofGetWidth()/2-5,18);
		Orange();
		ofRect(ofGetWidth()/4+3,7*ofGetHeight()/8+3,(ofGetWidth()/2-5)*cam->percentPlayed(),18);
	}
}

void hsInterface::drawReportBar(){
	ofPoint border(10,10);

	Gray();
	ofRect(repBar.x,repBar.y,repBar.width,repBar.height);
	Black();
	ofRect(repBar.x+border.x,repBar.y+border.y,repBar.width-border.x*2,repBar.height-border.y*2);

	vector<string> spl= ofSplitString(report,":");

	if(spl[0]=="report") Blue();
	if(spl[0]=="warning") Yellow();
	if(spl[0]=="error") Red();

	rep.drawString(report,repBar.x+border.x+10,repBar.y+border.y+10);
}

void hsInterface::drawInterface()
{
	int startX=ofGetWidth()/4-25;
	int ySpace=(ofGetHeight()-140)/3;
	ofSetColor(0xCCCCCC);
	ofSetShadowDarkness(.4);
	ofRect(startX,70,50,50);
	ofShade(startX,70,50,50,OF_DOWN);
	ofRect(startX,70+ySpace,50,50);
	ofShade(startX,70+ySpace,50,50,OF_DOWN);
	ofRect(startX,70+ySpace*2,50,50);
	ofShade(startX,70+ySpace*2,50,50,OF_DOWN);

	//drawInstructions(50,50,ofGetWidth()/4-75,ofGetHeight()-100);

	draw(0,0,ofGetWidth(), ofGetHeight());
}

void hsInterface::drawInstructions(int x, int y, int w, int h){
	

	//--------- instruction box
	ofSetColor(0xFD8D21);
	ofRect(x,y,w,h);
	ofSetColor(0xD2232A);
	ofRect(x+10,y+10,w-20,h-20);

	ofSetColor(0xffffff);
	rep.setMode(OF_FONT_CENTER);
	rep.drawString("Operator Instructions",x+w/2,y+20);
	rep.setMode(OF_FONT_LEFT);
}

void hsInterface::update()
{
	if(!cam->isRecording()&&activate.pressed()){
		activate.clickUp();
	}
	if(!cam->isPlaying()&&review.pressed()) review.clickUp();
	if(cam->retrieved()&&save.pressed()){
		save.clickUp();
		string imgFldr=cfg().dest;
		report="report: Beginning transfer to network.";
		char setNum[128];
		sprintf(setNum,"%02i", folderIndex);
		string cmd="data\\bin\\handleImages.bat "+string(setNum)+" "+cfg().dest+" "+cfg().folderRoot;
		call.run(cmd); 
	}
	if(cam->retrieved()&&!call.isRunning()){
		for(int i = 0; i < TCP.getLastID(); i++){
			if( !TCP.isClientConnected(i) )continue;
			report="report: Contacting client number "+ ofToString(i)+"\n"; 
			TCP.send(i,"<packet>");
			string setString="set="+cfg().folderRoot+ssprintf("%02i",folderIndex );
			report="report: Sending set name to client "+ ofToString(i)+"\n";
			//cout << setString<< endl;
			TCP.send(i, setString);
			string rootString="root="+cfg().dest.substr(3);
			//cout << rootString << endl;
			TCP.send(i,rootString);
			TCP.send(i,"</packet>");
			report="report: All set information sent to client "+ ofToString(i)+"\n";
		}
		report="report: Frames saved to network in position "+ofToString(folderIndex);
		if(++folderIndex>cfg().numSets) folderIndex=0;
		cam->resetFlags();
	}
	for(int i = 0; i < TCP.getLastID(); i++){
		if( !TCP.isClientConnected(i) )continue;
		string str = TCP.receive(i);
		if(str=="<mapRequest />"){
			TCP.send(i,"<mapRequest>");
			string rootString="map_root="+cfg().dest.substr(3);
			TCP.send(i,rootString);
			TCP.send(i,"</mapRequest>");
			cout << rootString << endl;
		}
		if(str=="<connectTest />"){
			//TCP.send(i,"<connectConfirm />");
		}
	}
	if(delay.justExpired()){
		cam->trigger();
		report="report: Begin capture";
	}
	if(timer.justExpired()){
		report="report: Ready to capture frames";
		if(cfg().autoMode){
			clickDown(capture.x,capture.y);
		}
	}
}
	
void hsInterface::clickDown( int x, int y)
{
	if(!cam->isFetching()&&!review.pressed()){
	//---------- activate button
		if(!cam->isRecording()&&!call.isRunning()&&!activate.pressed()&&activate.clickDown(x,y)){
			cam->resetFlags();
			cam->beginRecord(clipLength);
			report="report: Began recording a " + ofToString(clipLength,1) + " second loop";
			timer.set(clipLength/2.);
			timer.run();
		}
		else if(cam->isRecording()&&activate.pressed()&&activate.clickDown(x,y)){
			cam->stopRecord();
			report="report: Camera deactivated";
		}

	//--------- capture button
	if(cam->isRecording()&&capture.over(x,y)&&timer.expired()){
		report="report: Begin capture";
		cam->trigger();
	}
	else if(cam->isRecording()&&capture.clickDown(x,y)&&timer.running()){
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
	activate.setup(340,178,"buttons/1regular.png","buttons/1selected.png");
	capture.setup(340,178,"buttons/2regular.png","buttons/2selected.png");
	review.setup(340,178,"buttons/3regular.png","buttons/3selected.png");
	save.setup(340,178,"buttons/4regular.png","buttons/4selected.png");

	title.loadImage("buttons/title.png");

	folderIndex=cfg().startingNumber;

	rep.loadFont("fonts/Arial.ttf");
	rep.setSize(24);
	rep.setMode(OF_FONT_TOP);

	clipLength=cfg().recordTime;
	timer.set(0);

	TCP.setup(11999);
}