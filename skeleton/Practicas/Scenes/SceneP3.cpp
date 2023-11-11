#include "SceneP3.h"

#include "../Systems/GravitySystem.h"
#include "../Systems/WindSystem.h"

SceneP3::SceneP3() {
	//floor_ = new RenderItem(CreateShape(physx::PxBoxGeometry(400, 1, 400)), FLOOR_TR_, FLOOR_COLOR_);
	//gravity_ = new GravitySystem();
	wind_ = new WindSystem();
}

SceneP3::~SceneP3() {
	//DeregisterRenderItem(floor_);
	//delete gravity_;
	delete wind_;
}

void SceneP3::update(double t) {
	//gravity_->update(t);
	wind_->update(t);
}
