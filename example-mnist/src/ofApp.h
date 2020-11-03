#pragma once

//#define WITH_CUDA

#include<algorithm>

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxnnabla.hpp"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		map<int, vector<string>> validation_path;
		ofImage img;

		ofxnnabla::Runtime<float> *runtime;
		vector<float> probabilistic_result;
		size_t max_index;

		ofTrueTypeFont  font;
		ofTrueTypeFont  font2;

		float time;
};
