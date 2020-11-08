#pragma once

#define WITH_CUDA

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxnnabla.hpp"

class ofApp : public ofBaseApp{

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
		
		ofxnnabla::Runtime<float>* runtime;
		vector<float> probabilistic_result;
		size_t max_index;

		ofTrueTypeFont  font;
		ofTrueTypeFont  font2;

		vector<string> index;
		map<string, vector<string>> validation_path;
		ofImage img;
		float time;
};
