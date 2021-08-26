#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofEnableDepthTest();

	for (int i = 0; i < 150; i++) {

		ofMesh mesh, frame;
		frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
		glm::vec3 base = glm::vec3(ofRandom(-100, 100), ofRandom(-100, 100), ofRandom(-100, 100));
		switch ((int)ofRandom(3)) {
		case 0:
			mesh.addVertex(base + glm::vec3(5, 0, 0));
			mesh.addVertex(base + glm::vec3(-5, 0, 0));
			frame.addVertex(base + glm::vec3(5, 0, 0));
			frame.addVertex(base + glm::vec3(-5, 0, 0));
			break;
		case 1:
			mesh.addVertex(base + glm::vec3(0, 5, 0));
			mesh.addVertex(base + glm::vec3(0, -5, 0));
			frame.addVertex(base + glm::vec3(0, 5, 0));
			frame.addVertex(base + glm::vec3(0, -5, 0));
			break;
		case 2:
			mesh.addVertex(base + glm::vec3(0, 0, 5));
			mesh.addVertex(base + glm::vec3(0, 0, -5));
			frame.addVertex(base + glm::vec3(0, 0, 5));
			frame.addVertex(base + glm::vec3(0, 0, -5));
			break;
		}

		int hue = ofRandom(255);
		this->hue_list.push_back(hue);
		ofColor mesh_color;
		mesh_color.setHsb(hue, 255, 255);

		mesh.addColor(mesh_color);
		mesh.addColor(mesh_color);

		frame.addColor(ofColor(239)); 
		frame.addColor(ofColor(239));

		frame.addIndex(0);
		frame.addIndex(1);

		this->mesh_list.push_back(mesh);
		this->frame_list.push_back(frame);

		auto direction = glm::vec3(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
		direction = glm::normalize(direction) * 5;
		this->direction_list.push_back(direction);

		auto noise_seed = glm::vec3(glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000)));
		this->noise_seed_list.push_back(noise_seed);

		this->noise_step_list.push_back(0.f);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = this->mesh_list.size() - 1; i > -1; i--) {

		while (true) {

			this->noise_step_list[i] += 0.01;

			auto angle_x = ofMap(ofNoise(this->noise_seed_list[i].x, this->noise_step_list[i]), 0, 1, -PI, PI);
			auto angle_y = ofMap(ofNoise(this->noise_seed_list[i].y, this->noise_step_list[i]), 0, 1, -PI, PI);
			auto angle_z = ofMap(ofNoise(this->noise_seed_list[i].z, this->noise_step_list[i]), 0, 1, -PI, PI);

			auto rotation_x = glm::rotate(glm::mat4(), angle_x, glm::vec3(1, 0, 0));
			auto rotation_y = glm::rotate(glm::mat4(), angle_y, glm::vec3(0, 1, 0));
			auto rotation_z = glm::rotate(glm::mat4(), angle_z, glm::vec3(0, 0, 1));

			glm::vec3 rotate_direction = glm::vec4(this->direction_list[i], 0) * rotation_z * rotation_y * rotation_x;

			auto right = this->mesh_list[i].getVertex(this->mesh_list[i].getNumVertices() - 2);
			auto left = this->mesh_list[i].getVertex(this->mesh_list[i].getNumVertices() - 1);
			auto center = (right + left) / 2;

			auto feature = center + rotate_direction;
			if (glm::length(feature) < 300) {

				this->mesh_list[i].addVertex(right + rotate_direction);
				this->mesh_list[i].addVertex(left + rotate_direction);

				this->frame_list[i].addVertex(right + rotate_direction);
				this->frame_list[i].addVertex(left + rotate_direction);

				this->hue_list[i] = (int)(this->hue_list[i] + 6) % 255;

				ofColor mesh_color;
				mesh_color.setHsb(this->hue_list[i], 255, 255);

				this->mesh_list[i].addColor(mesh_color);
				this->mesh_list[i].addColor(mesh_color);

				this->frame_list[i].addColor(ofColor(239));
				this->frame_list[i].addColor(ofColor(239));

				break;
			}
		}

		if (this->mesh_list[i].getNumVertices() > 150) {

			this->mesh_list[i].removeVertex(0);
			this->mesh_list[i].removeVertex(1);

			this->mesh_list[i].removeColor(0);
			this->mesh_list[i].removeColor(1);
		}
		else {

			int index = this->mesh_list[i].getNumVertices() - 1;
			this->mesh_list[i].addIndex(index); this->mesh_list[i].addIndex(index - 2); this->mesh_list[i].addIndex(index - 3);
			this->mesh_list[i].addIndex(index); this->mesh_list[i].addIndex(index - 1); this->mesh_list[i].addIndex(index - 3);
		}

		if (this->frame_list[i].getNumVertices() > 100) {

			this->frame_list[i].removeVertex(0);
			this->frame_list[i].removeVertex(1);

			this->frame_list[i].removeColor(0);
			this->frame_list[i].removeColor(1);
		}
		else {

			int index = this->frame_list[i].getNumVertices() - 1;
			this->frame_list[i].addIndex(index); this->frame_list[i].addIndex(index - 2);
			this->frame_list[i].addIndex(index - 1); this->frame_list[i].addIndex(index - 3);

			if (index == 99) {

				this->frame_list[i].addIndex(98);
				this->frame_list[i].addIndex(99);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 0.25);

	for (int i = 0; i < this->mesh_list.size(); i++) {

		this->mesh_list[i].draw();
		this->frame_list[i].drawWireframe();
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}