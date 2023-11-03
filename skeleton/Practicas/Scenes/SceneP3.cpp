#include "SceneP3.h"

#include "../Systems/GravitySystem.h"

SceneP3::SceneP3() {
	//floor_ = new RenderItem(CreateShape(physx::PxBoxGeometry(400, 1, 400)), FLOOR_TR_, FLOOR_COLOR_);
	gravity_ = new GravitySystem();
}

SceneP3::~SceneP3() {
	//DeregisterRenderItem(floor_);
	delete gravity_;

}

void SceneP3::update(double t) {
	gravity_->update(t);
}
