#include "ofApp.h"
using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(120);
    facefinder.setup("haarcascade_frontalface_default.xml");
    facefinder.setPreset(ObjectFinder::Fast);
    facefinder.getTracker().setSmoothingRate(.3);
//    handfinder.setup("haarcascade_hand.xml");
//    handfinder.setPreset(ObjectFinder::Fast);
//    handfinder.getTracker().setSmoothingRate(.3);
    cam.initGrabber(640, 480);
    sunglasses.loadImage("sunglasses.png");
    speechbubble.loadImage("speechbubble.png");
    hat.loadImage("hat.png");
    tie.loadImage("tie.png");
    ofEnableAlphaBlending();
    
    soundStream.listDevices();
    int bufferSize = 256;
    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    //volHistory.assign(400, 0.0);
    
    bufferCounter	= 0;
    //drawCounter		= 0;
    smoothedVol     = 0.0;
    scaledVol		= 0.0;
    
    soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
    soundStream.start();
    float width = ofGetWidth() * .12;
    sphere.setRadius( width );
    

}

//--------------------------------------------------------------
void ofApp::update(){
    cam.update();
    if(cam.isFrameNew()) {
        facefinder.update(cam);
        //handfinder.update(cam);

    }
    mappedVol = ofMap(smoothedVol, 0, .01, 0, 10);
    //sphereY = ofMap(cos(ofGetElapsedTimef()), -1, 1, 0, ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.draw(0, 0);
    
    for(int i = 0; i < facefinder.size(); i++) {
        ofRectangle object = facefinder.getObjectSmoothed(i);
        sunglasses.setAnchorPercent(.5, .5);
        bubbleX= object.x+object.width+20;
        bubbleY = object.y- object.height;
        float scaleAmount = .85 * object.width / sunglasses.getWidth();
        ofPushMatrix();
        ofTranslate(object.x + object.width / 2., object.y + object.height * .42);
        ofScale(scaleAmount, scaleAmount);
        sunglasses.draw(0, 0);
        ofPopMatrix();
//        ofPushMatrix();
//        ofTranslate(object.getPosition());
//        ofDrawBitmapStringHighlight(ofToString(facefinder.getLabel(i)), 0, 0);
//        ofLine(ofVec2f(), toOf(facefinder.getVelocity(i)) * 10);
//        ofPopMatrix();
        float hatScale = 1.1 * object.width / hat.getWidth();
        ofPushMatrix();
        ofTranslate(object.x, object.y-object.height/2);
        ofScale(hatScale, hatScale);
        hat.draw(0,0);
        ofPopMatrix();
        tie.setAnchorPercent(.5, .5);
        float tieScale = 1.1 * object.height / tie.getHeight();
        ofPushMatrix();
        ofTranslate(object.x + object.width / 2., object.y+object.height*2);
        ofScale(tieScale, tieScale);
        tie.draw(0,0);
        ofPopMatrix();
        
    }
    ofDrawBitmapString(ofToString(mappedVol), 100, 100);
    if (mappedVol > 1){
        ofPushMatrix();
        
        speechbubble.draw(bubbleX, bubbleY);
        ofDrawBitmapString("BLAH BLAH BLAH", bubbleX+speechbubble.getWidth()/4, bubbleY+speechbubble.getHeight()/2);
        //ofRect(0, 0, 100, 100);
        ofPopMatrix();
        
    }


    
    //float spinX = sin(ofGetElapsedTimef()*.35f);
    //float spinY = cos(ofGetElapsedTimef()*.075f);
//    ofEnableDepthTest();
//    sphere.setPosition(ofGetWidth()*.5f, sphereY, 0);
    //sphere.rotate(spinX, 1.0, 0.0, 0.0);
    //sphere.rotate(spinY, 0, 1.0, 0.0);

//        ofSetColor(255);
//        sphere.draw();
}




//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    
    curVol = 0.0;
    
    // samples are "interleaved"
    int numCounted = 0;
    
    //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
    for (int i = 0; i < bufferSize; i++){
        left[i]		= input[i*2]*0.5;
        right[i]	= input[i*2+1]*0.5;
        
        curVol += left[i] * left[i];
        curVol += right[i] * right[i];
        numCounted+=2;
    }
    
    //this is how we get the mean of rms :)
    curVol /= (float)numCounted;
    
    // this is how we get the root of rms :)
    curVol = sqrt( curVol );
    
    smoothedVol *= 0.93;
    smoothedVol += 0.07 * curVol;
    
    bufferCounter++;
    
}
