#include "SceneP2.h"

#include "../ParticleSystem.h"
#include "../Systems/WaterSystem.h"
#include "../Systems/FireworksSystem.h"
#include "../Systems/MistSystem.h"

SceneP2::SceneP2() {
	water_ = new WaterSystem();
	
	fireworks_ = new FireworksSystem();
	fireworks_->update(5);

	mist_ = new MistSystem({0, -0.098, 0});
	//mist_->update(10);
}

SceneP2::~SceneP2() {
	delete water_;
	delete fireworks_;
	delete mist_;
}

void SceneP2::update(double t) {
	//water_->update(t);
	fireworks_->update(t);
	//mist_->update(t);
}
