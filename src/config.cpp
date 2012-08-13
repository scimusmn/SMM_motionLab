#include "config.h"

static configuration config;

configuration & cfg(){
	return config;
};

void configuration::readGeneralConfig(string cfgFile){
	ifstream config(ofToDataPath(cfgFile).c_str());
	while (config.peek()!=EOF) {
		string nextLine;
		getline(config, nextLine);
		vector<string> token=ofSplitString(nextLine, "=\r\n");
		if(token.size()){
			if(token[0]=="DESTINATION_FOLDER"){
				dest=token[1];
			}
			else if(token[0]=="PORT"){
				portNumber=ofToInt(token[1]);
			}
			else if(token[0]=="STARTING_NUMBER"){
				startingNumber=ofToInt(token[1]);
			}
			else if(token[0]=="NUM_SETS"){
				numSets=ofToInt(token[1]);
			}
			else if(token[0]=="FOLDER_ROOT_NAME"){
				folderRoot=token[1];
				cout << token[1];
			}
			else if(token[0]=="FILE_ROOT_NAME"){
				fileRoot=token[1];
			}
			else if(token[0]=="FULLSCREEN"){
				ofSetFullscreen(true);			//set fullscreen mode 
			}
			else if(token[0]=="FRAME_SIZE"){
				camWid=ofToInt(token[1].substr(0,token[1].find('x')));
				camHgt=ofToInt(token[1].substr(token[1].find('x')+1));
			}
			else if(token[0]=="FRAMES_PER_SECOND"){
				framesPerSec=ofToInt(token[1]);
			}
			else if(token[0]=="TIME_TO_RECORD"){
				recordTime=ofToFloat(token[1]);
			}
		}
	}
	config.close();
}