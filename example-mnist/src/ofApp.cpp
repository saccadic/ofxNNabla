#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofLogToConsole();
	ofSetWindowShape(512 * 3, 512);

	ofTrueTypeFont::setGlobalDpi(72);
	font.load("verdana.ttf", 50, true, true);

	ofTrueTypeFont::setGlobalDpi(72);
	font2.load("verdana.ttf", 500, true, true);

	runtime = new ofxnnabla::Runtime<float>();
	runtime->LoadModel(
		ofToDataPath("model.nnp", true),
		"Executor"
	);

	ofDirectory root(ofToDataPath("validation", true));
	auto numberList = root.getFiles();
	cout << "Search validation files [";
	for (int i = 0; i < numberList.size(); i++) {
		auto imgList = numberList[i];
		ofDirectory imgDir(imgList.getAbsolutePath());
		auto imgList_path = imgDir.getFiles();
		for (int t = 0; t < imgList_path.size(); t++) {
			string path = imgList_path[t].getAbsolutePath();
			validation_path[ofToInt(imgList.getFileName())].push_back(path);
		}
		cout << "*";
	}
	cout << "]" << endl;
}

//--------------------------------------------------------------
void ofApp::update() {
	float elapseTime = ofGetElapsedTimeMillis() - time;
	if (elapseTime > 100) {

		int id = int(ofRandom(0, 9));
		auto pathList = validation_path[id];
		int id_path = int(ofRandom(0, pathList.size() - 1));
		string img_path = pathList[id_path];
		img.load(img_path);
		img.update();

		runtime->upload(img.getPixelsRef(), 1.0 / 255.0);
		runtime->Run();
		auto result = runtime->GetOutputArrayPtr(0);

		probabilistic_result.clear();
		for (int i = 0; i < 10; i++) {
			probabilistic_result.push_back(result[i]);
		}
		auto iter = std::max_element(
			probabilistic_result.begin(),
			probabilistic_result.end()
		);
		max_index = std::distance(probabilistic_result.begin(), iter);

		time = ofGetElapsedTimeMillis();
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	img.draw(0, 0, 512, 512);
	ofPushMatrix();
	ofPushStyle();
	ofTranslate(512 + 10, 0);
	for (int i = 0; i < probabilistic_result.size(); i++) {
		ofTranslate(40, 0);
		auto p = probabilistic_result[i] * (512 - 50);
		if (i == max_index)
			ofSetColor(255, 0, 0);
		else
			ofSetColor(255);
		ofDrawRectangle(0, 512 - 50, 30, -p);
		font.drawString(ofToString(i), 0, 512 - 10);
	}
	ofPopStyle();
	ofPopMatrix();

	font2.drawString(ofToString(max_index), 512 * 2 + 100, 512 - 70);
	ofDrawBitmapString("Sorce 1x28x28", 20, 20);
	ofDrawBitmapString("Softmax 10ch", 512 + 0, 20);
	ofDrawBitmapString("Estimate number", 1024 + 20, 20);
}
