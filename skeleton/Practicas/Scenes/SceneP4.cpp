#include "SceneP4.h"

#include "../Systems/SpringSystem.h"

SceneP4::SceneP4() : springs_(nullptr), sprActive_(false) {
	springs_ = new SpringSystem();
	
	springs_->setActive(false);
}

SceneP4::~SceneP4() {
	delete springs_;
}

void SceneP4::update(double t) {
	springs_->update(t);
}

void SceneP4::keyPress(unsigned char key) {
	switch (tolower(key)) {
		case 'g': 
			sprActive_ = !sprActive_; 
			springs_->setActive(sprActive_);
			break;
		default: break;
	}
}
