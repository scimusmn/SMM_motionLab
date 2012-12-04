#ifndef _THREADED_SAVE
#define _THREADED_SAVE

#include "ofMain.h"
#include "ofxThread.h"

// this is not a very exciting example yet
// but ofThread provides the basis for ofNetwork and other
// operations that require threading.
//
// please be careful - threading problems are notoriously hard
// to debug and working with threads can be quite difficult


class ofThreadImageSave : public ofxThread{

	public:

	bool	newCommand;
	unsigned char * pixels;
	int w, h;

	    int count;  // threaded fucntions that share data need to use lock (mutex)
	                // and unlock in order to write to that data
	                // otherwise it's possible to get crashes.
	                //
	                // also no opengl specific stuff will work in a thread...
	                // threads can't create textures, or draw stuff on the screen
	                // since opengl is single thread safe

		//--------------------------
		ofThreadImageSave(){
			w=h=0,pixels=NULL;
		}

		void start(){
            startThread(true, false);   // blocking, verbose
        }

        void stop(){
            stopThread();
        }

		//--------------------------
		void threadedFunction(){

			while( isThreadRunning() != 0 ){
				if( lock() ){
					if(pixels){
						int bpp=24;
						FIBITMAP * bmp	= FreeImage_ConvertFromRawBits(pixels, w,h, w*bpp/8, bpp, 0,0,0, true);
						char name[256];
						sprintf(name,"data\\temp\\%03i.jpg",count);
						string realName="\""+ofToDataPath(name,true)+"\"";
						cout <<realName<< endl;
						FREE_IMAGE_FORMAT fif = FIF_JPEG;
						FreeImage_Save(fif, bmp, name, 0);
						if (bmp != NULL){
							FreeImage_Unload(bmp);
						}

					}
					unlock();
					stop();
				}
			}
		}

		//--------------------------
		void saveImage(unsigned char * pix, int _w, int _h, int cnt){
			
			if( lock() ){
				pixels=pix;
				for(unsigned int i=0; i<_w*_h*3; i++){
					if(i%(_w*3)==328*3){
						pixels[i]=pixels[i-3];
						pixels[i+1]=pixels[i-2];
						pixels[i+2]=pixels[i-1];
					}
				}
				w=_w,h=_h,count=cnt;
				start();
				unlock();
			}
			
		}
	
	bool isRunning(){

		return (isThreadRunning() !=0);
	}



};

#endif