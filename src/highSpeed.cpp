#include "highSpeed.h"

extern string report;

highSpeed::highSpeed()
{
	y=0,x=0;
	err = XS_SUCCESS;
	hCam = NULL;
	nItems = 10;
	nExp=10000000,nMinExp=0,maxWidth=0,maxHeight=0,nHi=0;
	nPixDepth=0,nImgSize=0,nPeriod=0,curPlayFrame=0;
	bNewFrame=bPlaying=bOpen=bRecording=bDriverLoaded=bInit=false;
	bFramesSaved=bFramesReviewed=bFetching=false;
}
	
highSpeed::~highSpeed()
{
	if(bRecording) stopRecord();
	if(bOpen) XsCloseCamera(hCam);
	if(bDriverLoaded) XsUnloadDriver();
	free(pix);
}

void highSpeed::intitialize(int cameraNum)
{
	err = XsLoadDriver(0);
	if( err == XS_SUCCESS )
	{
		bDriverLoaded=true;
		//fills the xsArray with available cameras, based on the filter value (in this case, looking for N-type cameras on Gig-E)
		err = XsEnumCameras(&xsArray[0],&nItems,XS_EF_GE_N);
		if( err == XS_SUCCESS && nItems>0)
		{
			err = XsOpenCamera(xsArray[cameraNum].nCameraId,&hCam);
			if( err == XS_SUCCESS )
			{
				bOpen=true;
				// 4 - Get the parameter structure from the camera

				xsCfg.cbSize = sizeof(XS_SETTINGS);	// don't forget this!!!
				err = XsReadCameraSettings(hCam,&xsCfg);
				//err = XsReadDefaultSettings( hCam, &xsCfg );
				if( err==XS_SUCCESS )
				{
					//XsSetParameter(hCam, &xsCfg, XSP_TRIGIN_CFG, XS_TIC_EDGE_HI);  //this is for setting the trigger with the bnc on cam
					XsSetParameter(hCam, &xsCfg, XSP_SYNCIN_CFG,XS_SIC_INTERNAL);
					XsSetParameter(hCam, &xsCfg, XSP_REC_MODE, XS_RM_CIRCULAR);
					//unsigned long newDesign;
					XsGetCameraInfo(hCam,XSI_LIVE_BUF_SIZE,&memOffset,&nHi);
					err = XsRefreshCameraSettings(hCam,&xsCfg);
					if( err == XS_SUCCESS )
					{
						// 9 - Get the image size
						updateFrameSize();
					}
					else ofLog(OF_LOG_WARNING,"Unable to configure the camera!!!\n");
				}
				else ofLog(OF_LOG_WARNING,"Unable to read camera settings!!!\n");
			}
			else ofLog(OF_LOG_WARNING,"Unable to open camera %i!!!\n", cameraNum);
		}
		else ofLog(OF_LOG_WARNING,"Unable to enumerate cameras!!!\n");
	}
	bFramesCaptured=bFramesSaved=justTriggered=false;
}

void highSpeed::setExposure(double timeInMillis)
{
	XsGetParameterAttribute(hCam,&xsCfg,XSP_EXPOSURE,XS_ATTR_MIN,&nMinExp);
	nExp=timeInMillis*1000000;
	if( nExp<nMinExp )
		nExp = nMinExp;
	else if( nExp>(nPeriod-3000) )
		nExp = nPeriod-3000;
	XsSetParameter(hCam,&xsCfg,XSP_EXPOSURE,nExp);
	refreshSettings();
}
	
unsigned long highSpeed::getExposure()
{
	XsGetParameter(hCam,&xsCfg,XSP_EXPOSURE,&nExp);
	return nExp;
}
	
void highSpeed::setFPS(unsigned long fps)
{
	nFPS=fps;
	if(nFPS>1500) nFPS=1500;
	else if(nFPS<1) nFPS=1;
	nPeriod=1000000000/nFPS;
	XsSetParameter(hCam,&xsCfg,XSP_PERIOD,nPeriod);
	refreshSettings();
}
	
unsigned long highSpeed::getFPS()
{
	XsGetParameter(hCam,&xsCfg,XSP_PERIOD,&nPeriod);
	return nFPS=1000000000/nPeriod;
}

void highSpeed::refreshSettings(string component)
{
	err = XsRefreshCameraSettings(hCam,&xsCfg);
	if( err ){
		ofLog(OF_LOG_WARNING,"Unable to configure the camera!!!\n");
		handleError();
	}
	hsText.allocate(width, height, GL_RGB);
}

long highSpeed::getMaxWidth()
{
	return maxWidth;
}

long highSpeed::getMaxHeight()
{
	return maxHeight;
}

void highSpeed::updateFrameSize()
{
	unsigned long size=0;
	if(bInit) free(frame.pBuffer),free(pix);
	XsGetCameraInfo(hCam,XSI_SNS_WIDTH,&maxWidth,&nHi);
	XsGetCameraInfo(hCam,XSI_SNS_HEIGHT,&maxHeight,&nHi);
	XsGetParameter( hCam, &xsCfg, XSP_ROIWIDTH, &width );
	XsGetParameter( hCam, &xsCfg, XSP_ROIHEIGHT, &height );
	XsGetParameter( hCam, &xsCfg, XSP_PIX_DEPTH, &nPixDepth);

	nImgSize = (nPixDepth/8)*maxWidth*maxHeight;   //sets the nImgSize by the pixel depth (related to nPixDepth/8)

	pix=(unsigned char *)malloc(nImgSize);
	frame.pBuffer  = malloc(nImgSize);
	frame.nBufSize = nImgSize;
	frame.nImages = 1;
	bInit=true;
	
	hsText.allocate(width, height, GL_RGB);
}
	
void highSpeed::setROI(int _x, int _y, int w, int h)
{
	unsigned long roiStep=0;
	XsGetCameraInfo(hCam,XSI_ROI_STEP,&roiStep,&nHi);
	XsGetCameraInfo(hCam,XSI_SNS_WIDTH,&maxWidth,&nHi);
	XsGetCameraInfo(hCam,XSI_SNS_HEIGHT,&maxHeight,&nHi);
	if(_x>=0&&_x+w<=maxWidth){
		if(_y>=0&&_y+h<=maxHeight){
			x=_x,y=_y;
			int found=w;
			int diff=2048;
			int num=0;
			for(int i=0; i<maxWidth/roiStep+1; i++){
				int k=i*roiStep;
				if(abs(w-(k))<diff) diff=abs(w-(k)),found=k;
			}
			width=found;
			height=h;
			XsSetParameter(hCam,&xsCfg,XSP_ROIX,x);
			XsSetParameter(hCam,&xsCfg,XSP_ROIY,y);
			XsSetParameter(hCam,&xsCfg,XSP_ROIWIDTH,width);
			XsSetParameter(hCam,&xsCfg,XSP_ROIHEIGHT,height);
			refreshSettings();
		}
	}
	updateFrameSize();
}

void highSpeed::setRotation(XS_ROT_ANGLE angle)
{
	if(angle == XS_ROT_90 || angle == XS_ROT_270){
		int tWid=width;
		width=height;
		height=tWid;
	}
	XsSetParameter(hCam,&xsCfg,XSP_ROT_ANGLE,angle);
	refreshSettings();
}

void highSpeed::processFrame(unsigned char * pixels)
{
	if( err == XS_SUCCESS && !bNewFrame ){
		bNewFrame=true;
	}
}

void highSpeed::getStartAddress()
{

	XsGetParameter( hCam, &xsCfg,XSP_STARTADDRLO,&startAddLo);
	XsGetParameter( hCam, &xsCfg,XSP_STARTADDRHI,&startAddHi);

	XsGetParameter( hCam, &xsCfg,XSP_FRAMES,&nFrames);

	cout << nStartIndex << " is the start index" << endl;
}

void XSTREAMAPI grabCallback(void* nUserData, XS_ERROR nErrCode, unsigned long cbFlags)
{
	highSpeed & cam = *((highSpeed*)nUserData);
	cam.processFrame();
}
	
void highSpeed::grabFrame()
{
	if(bOpen && !err && !bRecording && !bPlaying){
		err = XsQueueOneFrame(hCam,&frame,grabCallback,XS_CF_DONE,this);
		//err = XsSynchGrab(hCam,&frame,10000);
		if( err ) handleError();
	}
	else if(bOpen && !err && bRecording && !bPlaying){
		unsigned long frameIndex=0;
		err = XsMemoryPreview(hCam,&frame,&frameIndex);
		if(justTriggered){
			nStartIndex=(frameIndex>=nPreFrames)?frameIndex-nPreFrames:nFrames-(nPreFrames-frameIndex);
			justTriggered=false;
		}
		//cout << frameIndex << " is the current frame index" << endl;
		if( !err ) bNewFrame=true;
		else handleError();
	}
	else if(bOpen && !err && !bRecording && bPlaying && lastFrame != curPlayFrame){
		unsigned long numFrames=0;
		if(curPlayFrame<nFrames-10){
			int curFrame=nStartIndex+curPlayFrame;
			curFrame = curFrame%nFrames;
			//cout << curFrame << " is the current frame\n";
			report="report: Loading frame " + ofToString(int(curPlayFrame)) + " from camera";
			do { //add a delay opt out for breaking
				err = XsMemoryReadFrame(hCam,startAddLo,startAddHi,curFrame, frame.pBuffer);
				if(err){
					cout << "Error playing frame from camera: " << err << endl;
					handleError();
				}
			} while(err);
			if( !err ) {
				lastFrame=curPlayFrame;
				if(bPlaying) curPlayFrame+=20;
				bNewFrame=true;
			}
			else handleError();
		}
		else{
			report="report: Frames reviewed and ready to save";
			bFramesReviewed=true;
			curPlayFrame=0;
			bPlaying=false;
		}
	}

}

unsigned long highSpeed::imageSize()
{
	return nImgSize;
}
	
bool highSpeed::frameIsNew()
{
	bool ret=bNewFrame;
	bNewFrame=false;
	return ret;
}
	
unsigned char * highSpeed::pixels()
{
	return (unsigned char*)pix;
}

unsigned char * highSpeed::uPixels()
{
	return (unsigned char*)frame.pBuffer;
}
	
bool highSpeed::isOpen()
{
	return bOpen;
}
	
bool highSpeed::isPlaying()
{
	return bPlaying;
}

void highSpeed::onExit()
{
	
}

void highSpeed::handleError(string filter)
{
	if(err == XS_E_HARDWARE_FAULT) {
		char buf[256];
		XsGetHardwareError(hCam,buf,256);
		report="error: " + string(buf) + ", call exhibit maintenance";
	}
	else cout << "Error: #" << err << endl;
}

void highSpeed::resetFlags()
{
	bFramesCaptured=bFramesSaved=bFramesReviewed=false;
}

void highSpeed::draw(int x, int y)
{
	ofSetColor(0xffffff);
	hsText.draw(x,y);
}

void highSpeed::draw(int x, int y, int w, int h)
{
	ofSetColor(0xffffff);
	hsText.draw(x,y,w,h);
}

void highSpeed::update()
{
	if(frameIsNew()&&!bFetching){
		hsText.loadData(uPixels(),width,height,GL_BGR);
	}
	else if(!bFetching) grabFrame();
	else if(bFetching){ 
		fetchFrames();
	}
}