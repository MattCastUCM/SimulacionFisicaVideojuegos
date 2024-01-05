#include "SceneP4.h"

#include "../Systems/SpringSystem.h"

SceneP4::SceneP4() : springs_(nullptr) {
	springs_ = new SpringSystem();
}

SceneP4::~SceneP4() {
	delete springs_;
}

void SceneP4::update(double t) {
	springs_->update(t);
}

void SceneP4::keyPress(unsigned char key) {
	springs_->keyPress(key);
}
