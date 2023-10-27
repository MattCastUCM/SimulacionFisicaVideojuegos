#include "SceneP2.h"

#include "../Systems/WaterSystem.h"
#include "../Systems/FireworksSystem.h"
#include "../Systems/MistSystem.h"

SceneP2::SceneP2() {
	//floor_ = new RenderItem(CreateShape(physx::PxBoxGeometry(400, 1, 400)), FLOOR_TR_, FLOOR_COLOR_);
	water_ = new WaterSystem();
	fireworks_ = new FireworksSystem();
	//mist_ = new MistSystem();
}

SceneP2::~SceneP2() {
	//DeregisterRenderItem(floor_);
	delete water_;
	delete fireworks_;
	//delete mist_;
}

void SceneP2::update(double t) {
	//water_->update(t);
	fireworks_->update(t);
	//mist_->update(t);
}
