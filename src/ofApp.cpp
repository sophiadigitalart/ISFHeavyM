#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofDisableArbTex();
    if (!shadertoy.load("shaders/shader.frag")) {
        ofLogError() << "Error loading shader!";
    }
    ofImage image("images/image.jpg");
    // textures in GLSL are flipped vertically, so we need to flip them back up
    // to use with Shadertoy shaders - most images are preset to use VFlip there
    // this has the same effect of setting VFlip on the texture in Shadertoy
	image.mirror(false, false);
	inputImage.allocate(image.getPixelsRef());
    // this is the same as setting Wrap to "repeat" in Shadertoy
	inputImage.setTextureWrap(GL_REPEAT, GL_REPEAT);

    noiseImage = ofxShadertoy::createRGBANoiseSmall();
    // this is the same as setting Wrap to "repeat" in Shadertoy
    noiseImage.setTextureWrap(GL_REPEAT, GL_REPEAT);
    shadertoy.setTexture(0, inputImage);
    shadertoy.setTexture(1, noiseImage);
    ofSetFrameRate(60);
    shadertoy.setAdvanceTime(true);
    //gui.setup();
    //gui.add(effectStrength.setup("effect strength", 0.5, 0.0, 1.0));
	useCamera = false;
	// gif
	frameW = 120;
	frameH = 68;

	vid.initGrabber(frameW, frameH);
	rgbaFbo.allocate(frameW, frameH, GL_RGB);

	gifEncoder.setup(frameW, frameH, .25, 256);
	ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &ofApp::onGifSaved);

	ofBackground(50, 40, 50);
	// isf
	chain.setup(frameW, frameH);
}

//--------------------------------------------------------------
void ofApp::update(){
	vid.update();
	chain.update();
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
	rgbaFbo.begin();
	ofClear(0, 0, 0, 1);
    shadertoy.setDimensions(frameW, frameH);
    shadertoy.begin();
	shadertoy.setUniform1f("effectStrength", 0.5f);// effectStrength);
    ofDrawRectangle(0, 0, frameW, frameH);
    shadertoy.end();
	rgbaFbo.end();
	rgbaFbo.draw(460, 200);
	chain.draw(200, 100);
    //gui.draw();

	for (int i = 0; i < nFrames; i++) {
		txs[i]->draw(i* (frameW / 2 + 5), frameH, frameW / 2, frameH / 2);
	}
	vid.draw(0, 400);

	ofDrawBitmapString("KEYS\n----\nspacebar: capture frame\ns: save gif", frameW + 10, 20);
}

void ofApp::captureFrame() {

	ofPixels fboPixels;
	rgbaFbo.readToPixels(fboPixels);
	gifEncoder.addFrame(fboPixels.getPixels(), frameW, frameH);
	
	//gifEncoder.addFrame(fbo.)
	ofTexture * tx = new ofTexture();
	tx->allocate(frameW, frameH, GL_RGB);
	tx->loadData(vid.getPixels(), GL_RGB);//, frameW, frameH
	txs.push_back(tx);

	nFrames++;
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
	string ext = "";
	string fileName = "";
	for (int i = 0; i < dragInfo.files.size(); i++) {
		cout << dragInfo.files[i] << endl;

		string absolutePath = dragInfo.files[i];
		// use the last of the dropped files
		int dotIndex = absolutePath.find_last_of(".");
		int slashIndex = absolutePath.find_last_of("\\");

		if (dotIndex != std::string::npos && dotIndex > slashIndex) {
			ext = absolutePath.substr(dotIndex + 1);
			fileName = absolutePath.substr( slashIndex + 1, dotIndex - slashIndex - 1);
			if (ext == "frag") {
				// shadertoy want mainImage()
				if (!shadertoy.load(dragInfo.files[i])) {
					ofLogError() << "Error loading shader: " << dragInfo.files[i];
				}

			}
			else if (ext == "fs") {
				if (i == 0) {
					chain.load(dragInfo.files[i]);
					chain.setEnable(0, false);

				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	switch (key) {
	case 'f':
        ofToggleFullscreen();
		break;
	case 'c':
		useCamera = !useCamera;
		if (useCamera) {
			shadertoy.setTexture(0, vid.getTexture());
		}
		else {
			shadertoy.setTexture(0, inputImage);
		}
		break;
	case ' ':
		captureFrame();
		break;
	case 's':
		cout << "start saving\n" << endl;
		gifEncoder.save("test.gif");
		break;
	default:
		break;
	}
}
void ofApp::onGifSaved(string & fileName) {
	cout << "gif saved as " << fileName << endl;
	ofLogNotice("onGifSaved: " + fileName);
	gifEncoder.reset();
	ofLogNotice("onGifSaved reset");
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}


//--------------------------------------------------------------
void ofApp::exit() {
	gifEncoder.exit();
}