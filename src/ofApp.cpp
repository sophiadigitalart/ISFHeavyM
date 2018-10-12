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
	image.mirror(true, false);
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
    gui.setup();
    gui.add(effectStrength.setup("effect strength", 0.5, 0.0, 1.0));
	// gif
	frameW = 320;
	frameH = 240;

	vid.initGrabber(frameW, frameH);
	rgbaFbo.allocate(frameW, frameH, GL_RGBA);
	gifEncoder.setup(frameW, frameH, .25, 256);
	ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &ofApp::onGifSaved);

	ofBackground(50, 40, 50);
}

//--------------------------------------------------------------
void ofApp::update(){
	vid.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	rgbaFbo.begin();
	ofClear(0, 0, 0, 1);
    shadertoy.setDimensions(ofGetWindowWidth(), ofGetWindowHeight());
    shadertoy.begin();
    shadertoy.setUniform1f("effectStrength", effectStrength);
    ofDrawRectangle(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    shadertoy.end();
	rgbaFbo.end();
	rgbaFbo.draw(460, 400);
    gui.draw();

	for (int i = 0; i < nFrames; i++) {
		txs[i]->draw(i* (frameW / 2 + 5), frameH, frameW / 2, frameH / 2);
	}
	vid.draw(0, 0);
	ofEnableAlphaBlending();
	ofDisableAlphaBlending();
	ofDrawBitmapString("KEYS\n----\nspacebar: capture frame\ns: save gif", frameW + 10, 20);
}
void ofApp::onGifSaved(string &fileName) {
	cout << "gif saved as " << fileName << endl;
}
void ofApp::captureFrame() {
	/* gifEncoder.addFramePx(
		&vid.getPixels(),
		vid.getWidth(),
		vid.getHeight(),
		vid.getPixelsRef().getBitsPerPixel(),
		.1f
	); */
	ofPixels pix;
	rgbaFbo.getTextureReference().readToPixels(pix);
	gifEncoder.addFramePx(
		&pix,
		vid.getWidth(),
		vid.getHeight(),
		vid.getPixelsRef().getBitsPerPixel(),
		.1f
	);
	//gifEncoder.addFrame(fbo.)
	ofTexture * tx = new ofTexture();
	tx->allocate(frameW, frameH, GL_RGB);
	tx->loadData(vid.getPixels(), GL_RGB);//, frameW, frameH
	txs.push_back(tx);

	nFrames++;
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'f') {
        ofToggleFullscreen();
    }
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	switch (key) {
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
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::exit() {
	gifEncoder.exit();
}