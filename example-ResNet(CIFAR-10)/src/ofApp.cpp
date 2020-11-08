#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofLogToConsole();
	ofSetWindowShape(512 * 3, 512);

	ofTrueTypeFont::setGlobalDpi(72);
	font.load("verdana.ttf", 30, true, true);

	ofTrueTypeFont::setGlobalDpi(72);
	font2.load("verdana.ttf", 300, true, true);

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
			validation_path[imgList.getFileName()].push_back(path);
		}
		cout << "*";
	}

	index.push_back("airplane");
	index.push_back("automobile");
	index.push_back("bird");
	index.push_back("cat");
	index.push_back("deer");
	index.push_back("dog");
	index.push_back("frog");
	index.push_back("horse");
	index.push_back("ship");
	index.push_back("truck");
}

//--------------------------------------------------------------
void ofApp::update() {
	float elapseTime = ofGetElapsedTimeMillis() - time;
	if (elapseTime > 1000) {

		int id = int(ofRandom(0, 9));
		string label = index[id];
		int num = int(ofRandom(0, 10));
		string img_path = validation_path[label][num];
		img.load(img_path);
		img.update();

		runtime->upload(img.getPixelsRef(), 1);
		runtime->Run();
		auto result = runtime->GetOutputArrayPtr(0);

		probabilistic_result.clear();
		for (int i = 0; i < 10; i++) {
			if (result[i] > 0) {
				probabilistic_result.push_back(result[i]);
			}
			else {
				probabilistic_result.push_back(0);
			}

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
		ofTranslate(200, 0);
		auto p = probabilistic_result[i] * (512 - 50);
		if (i == max_index)
			ofSetColor(255, 0, 0);
		else
			ofSetColor(255);
		ofDrawRectangle(0, 512 - 50, 200, -p);
		font.drawString(index[i], 0, 512 - 10);
	}
	ofPopStyle();
	ofPopMatrix();

	font2.drawString(index[max_index], 512 * 2 + 1600, 512 - 90);
	ofDrawBitmapString("Sorce 1x28x28", 20, 20);
	ofDrawBitmapString("Softmax 10ch", 512 + 0, 20);
	ofDrawBitmapString("Estimate number", 512 * 2 + 1600 + 20, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
