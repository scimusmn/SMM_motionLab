#include "highSpeed.h"

extern string report;

void highSpeed::setRecordingVar(bool state)
{
	bRecording=state;
}

void highSpeed::trigger()
{
	justTriggered=true;
	XsTrigger(hCam);
}

void XSTREAMAPI recordCallback(void* nUserData, XS_ERROR nErrCode, unsigned long cbFlags)
{
	highSpeed & cam = *((highSpeed*)nUserData);
	cam.setRecordingVar(false);
	cam.getStartAddress();
	cam.setCaptured(true);

	report="report: Recorded "+ ofToString(int(cam.nFrames-10)) + " frames";
}

void highSpeed::beginRecord(double secondsToRecord, double portionBeforeTrigger)
{
	bRecording=true;
	nFrames=secondsToRecord*nFPS;
	nPreFrames=nFrames*portionBeforeTrigger;
	nFrames+=10;
	err = XsMemoryStartGrab(hCam,LOLONG(memOffset),HILONG(memOffset),nFrames,nPreFrames,recordCallback,XS_CF_DONE,this);
	if( err ) report="error: Start grab failed!";//ofLog(OF_LOG_WARNING,"Start Grab failed with error code %i\n",err);
	else handleError();
	bFramesCaptured=bFramesSaved=false;
}

void highSpeed::toggleRecord(double secondsToRecord, double portionBeforeTrigger)
{
	if(!bRecording) beginRecord(secondsToRecord,portionBeforeTrigger);
	else stopRecord();
}
	
void highSpeed::stopRecord()
{
	bRecording=false;
	unsigned long aqFrames;
	XsMemoryStopGrab(hCam,&aqFrames);
}

bool highSpeed::isRecording()
{
	return bRecording;
}

void highSpeed::play()
{
	bPlaying=true;
	bFramesSaved=false;
	getStartAddress();
	lastFrame=-1,curPlayFrame=0;
	XsGetParameter( hCam, &xsCfg, XSP_FRAMES, &nFrames);
}
	
void highSpeed::pause()
{
	bPlaying=false;
}
	
void highSpeed::resetPlayback()
{
	lastFrame=-1,curPlayFrame=0;
}
	
void highSpeed::fetchFrames()
{
	if(bOpen && !err && !bRecording && !bPlaying && bFetching && lastFrame != curPlayFrame){
		unsigned long numFrames=0;
		if(curPlayFrame<nFrames-10){
			//int strtInd=(trigIndex>nPreFrames)?trigIndex-nPreFrames:nFrames-(nPreFrames-trigIndex);
			int curFrame=nStartIndex+curPlayFrame;//
			curFrame = curFrame%nFrames;
			report="report: Saving frame " + ofToString(int(curPlayFrame)) + " from camera";
			do {
				err = XsMemoryReadFrame(hCam,LOLONG(memOffset),HILONG(memOffset),curFrame, frame.pBuffer);
				if(err){
					report="error: Trouble reading frame from camera: "+ofToString(err);
					cout << "Error reading frame from camera: " << err << endl;
					handleError();
				}
			} while(err);
			if( !err ) {
				lastFrame=curPlayFrame;
				if(bFetching) curPlayFrame++;
				save.saveImage(uPixels(),width,height,frameIndex());
			}
			else handleError();
		}
		else{
			report="report: All frames saved to computer";
			bFramesSaved=true;
			curPlayFrame=0;
			bFetching=false;
		}
	}
}

void highSpeed::nextFrame(){
	if(curPlayFrame<nFrames) curPlayFrame++;
}

void highSpeed::prevFrame(){
	if(curPlayFrame>0) curPlayFrame--;
}