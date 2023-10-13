#include "SceneP2.h"


SceneP2::SceneP2() {
	floor_ = new RenderItem(CreateShape(physx::PxBoxGeometry(400, 1, 400)), FLOOR_TR_, FLOOR_COLOR_);
	partSys_ = new ParticleSystem();
}

SceneP2::~SceneP2() {
	DeregisterRenderItem(floor_);
	delete partSys_;
}

void SceneP2::update(double t) {
	partSys_->update(t);
}
