#include "SceneP3.h"

#include "../Systems/GravitySystem.h"
#include "../Systems/WindSystem.h"
#include "../Systems/ExplosionSystem.h"

SceneP3::SceneP3() : gravity_(nullptr), wind_(nullptr), expl_(nullptr), grActive_(false), wndActive_(false), explActive_(false) {
	gravity_ = new GravitySystem();
	wind_ = new WindSystem();
	expl_ = new ExplosionSystem();

	gravity_->setActive(false);
	wind_->setActive(false);
	expl_->setActive(false);
}

SceneP3::~SceneP3() {
	delete gravity_;
	delete wind_;
	delete expl_;
}

void SceneP3::update(double t) {
	gravity_->update(t);
	wind_->update(t);
	expl_->update(t);
}

void SceneP3::keyPress(unsigned char key) {
	switch (tolower(key)) {
		case 'g': 
			grActive_ = !grActive_; 
			gravity_->setActive(grActive_);
			break;
		case 'v': 
			wind_->keyPress(key); 
			wndActive_ = !wndActive_; 
			wind_->setActive(wndActive_);
			break;
		case 'e': 
			explActive_ = !explActive_; 
			expl_->setActive(explActive_);
			break;
		default: break;
	}
	if(wndActive_) wind_->keyPress(key);
}
