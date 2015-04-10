#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    // 画面基本設定
    ofSetFrameRate(60);
    ofBackground(63);
    ofSetCircleResolution(32);
    
    // ランダムな場所と速度を指定
    position.x = ofRandom(ofGetWidth());
    position.y = ofRandom(ofGetHeight());
    velocity.x = ofRandom(-10, 10);
    velocity.y = ofRandom(-10, 10);
    
    //Capture Setting
    sampleRate = 44100;
    channels = 2;
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    //If you want to use grabber
#ifdef USE_CAM
    vidGrabber.setDesiredFrameRate(30);
    vidGrabber.initGrabber(SC_WIDTH, SC_HEIGHT);
#endif

    fileName = "testMovie";
    fileExt = ".mov"; // ffmpeg uses the extension to determine the container type. run 'ffmpeg -formats' to see supported formats
    
    // override the default codecs if you like
    // run 'ffmpeg -codecs' to find out what your implementation supports (or -formats on some older versions)
    vidRecorder.setVideoCodec("mpeg4");
    vidRecorder.setVideoBitrate("3200k"); //def is 800k
    vidRecorder.setAudioCodec("mp3");
    vidRecorder.setAudioBitrate("192k");
    
    //    soundStream.listDevices();
    //    soundStream.setDeviceID(11);
    soundStream.setup(this, 0, channels, sampleRate, 256, 4);
    
    bRecording = false;
    ofEnableAlphaBlending();
    
    
    //Setup FBO
    cap.allocate(SC_WIDTH,SC_HEIGHT);
    cap.begin();
    ofClear(0, 0, 0);
    cap.end();
//    img = new ofImage;
    //Set Image time for RGB (without RGBA)
    converted.setImageType(OF_IMAGE_COLOR);

    

}

//--------------------------------------------------------------
void ofApp::update(){
    // 円の座標を更新
    position += velocity;

    // 画面からはみ出ないように
    if (position.x < 0 || position.x > ofGetWidth()) {
        velocity.x *= -1;
    }
    if (position.y < 0 || position.y > ofGetHeight()) {
        velocity.y *= -1;
    }
    
    
    //Video Capture
#ifdef USE_CAM
    vidGrabber.update();
#endif

    if(bRecording){
        cap.readToPixels(recordPixels);
        convertPixels();
        
            vidRecorder.addFrame(converted);

    }

}

//--------------------------------------------------------------
void ofApp::draw(){


    cap.begin();
    ofClear(0, 0, 0);
    ofSetColor(255, 255, 255);

#ifdef USE_CAM
    vidGrabber.draw(0,0);
#endif
    ofSetColor(255);
    // 円を描画
    ofCircle(position.x, position.y, 10);
    
    
    stringstream ss;
    ss << "video queue size: " << vidRecorder.getVideoQueueSize() << endl
    << "audio queue size: " << vidRecorder.getAudioQueueSize() << endl
    << "FPS: " << ofGetFrameRate() << endl
    << (bRecording?"pause":"start") << " recording: r" << endl
    << (bRecording?"close current video file: c":"") << endl;
    
    ofSetColor(0,0,0,100);
    ofRect(0, 0, 260, 75);
    ofSetColor(255, 255, 255);
    ofDrawBitmapString(ss.str(),15,15);
    
    cap.end();
    cap.draw(0,0);
    if(bRecording){
        ofSetColor(255, 0, 0);
        ofCircle(ofGetWidth() - 20, 20, 5);
    }


}

void ofApp::audioIn(float *input, int bufferSize, int nChannels){
    if(bRecording){
        vidRecorder.addAudioSamples(input, bufferSize, nChannels);
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

    if(key=='r'){
        bRecording = !bRecording;
        if(bRecording && !vidRecorder.isInitialized()) {
            vidRecorder.setup(fileName+ofGetTimestampString()+fileExt, SC_WIDTH, SC_HEIGHT, 30, sampleRate, channels);
        }
    }
    if(key=='c'){
        bRecording = false;
        vidRecorder.close();
    }
    
}

//--------------------------------------------------------------
void ofApp::convertPixels(){
    
    
    //Make Array for treating each pixcels
    unsigned char * pixels = recordPixels.getPixels();
    unsigned char pixs2[SC_WIDTH*SC_HEIGHT*3];
    
        for(int i = 0; i < SC_WIDTH*SC_HEIGHT; i++)
        {
            pixs2[i*3]   = pixels[i*4];
            pixs2[i*3+1] = pixels[i*4+1];
            pixs2[i*3+2] = pixels[i*4+2];
        }

    //Copy the pixels into converted buffer with 3channel
    converted.setFromExternalPixels(pixs2, SC_WIDTH, SC_HEIGHT, 3);
    
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
