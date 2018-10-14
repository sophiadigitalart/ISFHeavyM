#pragma once

#include "ofMain.h"
#include "ofxShadertoy.h"
#include "ofxGui.h"
#include "ofxGifEncoder.h"

class ofApp : public ofBaseApp{
private:
    ofxPanel gui;
    ofxFloatSlider effectStrength;
    ofxShadertoy shadertoy;
    ofTexture inputImage;
    ofTexture noiseImage;
	bool useCamera;

	// gif
	void onGifSaved(string & fileName);
	void exit();
	void captureFrame();

	vector <ofTexture *> txs; // for previewing
	vector <ofxGifEncoder::ofxGifFrame *> pxs;

	int frameW, frameH;
	int nFrames;

	ofVideoGrabber vid;
	ofxGifEncoder gifEncoder;
public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	//8 bits red, 8 bits green, 8 bits blue, from 0 to 255 in 256 steps
	ofFbo rgbaFbo; // with alpha
};
