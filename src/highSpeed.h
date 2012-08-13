#ifndef HIGH_SPEED
#define HIGH_SPEED

#include "ofMain.h"
#include "ofExtended.h"
#include "XstrmAPI.h"
#include "threadedImageSave\threadImageSave.h"

#define MAKELONGLONG(a,b)	((LONGLONG)(((ULONG)(a)) | ((LONGLONG)((ULONG)(b))) << 32))
#define LOLONG(a)			((LONG)a)
#define HILONG(a)			((LONG)(((LONGLONG)a)>>32))

class highSpeed {
protected:
	XS_ERROR err;
	XS_ENUMITEM xsArray[10];
	unsigned long i,nItems;

	XS_HANDLE hCam;
	XS_SETTINGS xsCfg;
	XS_FRAME frame;
	unsigned long nExp,nMinExp,maxWidth,maxHeight,nHi;
	unsigned long nPixDepth,nPeriod,nFPS;
	unsigned long nPreFrames,curPlayFrame,nStartIndex;
	unsigned long startAddLo,startAddHi,memOffset,nFrameSize;
	unsigned long lastFrame;
	bool justTriggered;
	unsigned long trigIndex;
	bool bNewFrame,bOpen,bRecording,bDriverLoaded,bInit;
	bool bPlaying,bFramesCaptured,bFramesSaved,bFramesReviewed,bFetching;
	unsigned char * pix;

public:
	unsigned long nFrames,nImgSize;
	unsigned long x,y;
	unsigned long width, height;
	highSpeed();
	~highSpeed();

	//---------- camera settings functions
	void intitialize(int cameraNum=0);
	void setExposure(double timeInMillis);
	unsigned long getExposure();
	void setFPS(unsigned long fps);
	unsigned long getFPS();
	void refreshSettings(string component="");
	void setROI(int x, int y, int w, int h);
	void setRotation(XS_ROT_ANGLE angle);
	void handleError(string filter="");

	//---------- image processing functions
	void getStartAddress();
	void grabFrame();
	void processFrame(unsigned char * pixels=0);
	unsigned long imageSize();
	long getMaxWidth();
	long getMaxHeight();
	void updateFrameSize();
	unsigned char * pixels();
	unsigned char * uPixels();

	//-------- recording control funcs
	void setRecordingVar(bool boolForVarNotMode);
	void beginRecord(double secondsToRecord, double portionBeforeTrigger=.5);
	void stopRecord();
	void toggleRecord(double secondsToRecord, double portionBeforeTrigger=.5);
	void trigger();

	//-------- state checking funcs
	bool isRecording();
	bool isOpen();
	bool isPlaying();
	bool framesCaptured(){ return bFramesCaptured; }
	bool framesReviewed(){ return bFramesReviewed; }
	bool isFetching(){ return bFetching; }
	bool retrieved(){ return bFramesSaved;}
	bool frameIsNew();

	//-------- preview/playback functions
	void setCaptured(bool capd){ bFramesCaptured=capd; }
	void resetFlags();
	void play();
	void pause();
	void resetPlayback();
	void fetchFrames();
	void nextFrame();
	void prevFrame();
	double percentPlayed(){ return double(curPlayFrame)/double(nFrames);}
	int frameIndex(){ return curPlayFrame; }

	void fetch(){ bFetching=true; }

	//--------- of program funcs
	void onExit();
	void draw(int x, int y);
	void draw(int x, int y, int w, int h);
	void update();

	ofTexture	hsText;
	ofThreadImageSave	save;
};

#endif //HIGH_SPEED