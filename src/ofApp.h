#pragma once

#include "ofMain.h"
#include "ofxCv.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

        void audioIn(float * input, int bufferSize, int nChannels);
    

    
    
    
        ofVideoGrabber cam;
        ofxCv::ObjectFinder facefinder;
        ofxCv::ObjectFinder handfinder;
        ofImage sunglasses;
        ofImage speechbubble;
        ofImage hat;
        ofImage tie;
    
        vector <float> left;
        vector <float> right;
        vector <float> volHistory;
        float curVol = 0.0;
    
        int 	bufferCounter;
        int 	drawCounter;
    
        float smoothedVol;
        float scaledVol;
        float mappedVol;
        float bubbleX;
        float bubbleY;
    
        ofSoundStream soundStream;
    
        bool bFill;
        bool bWireframe;
        ofSpherePrimitive sphere;
        float sphereY;
    
       
		
};
