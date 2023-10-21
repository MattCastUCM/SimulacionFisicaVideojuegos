#include "SceneP2.h"

#include "../Systems/WaterSystem.h"
#include "../Systems/FireworksSystem.h"


SceneP2::SceneP2() {
	//floor_ = new RenderItem(CreateShape(physx::PxBoxGeometry(400, 1, 400)), FLOOR_TR_, FLOOR_COLOR_);
	waterSystem_ = new WaterSystem();
	fireworksSystem_ = new FireworksSystem();
}

SceneP2::~SceneP2() {
	//DeregisterRenderItem(floor_);
	delete waterSystem_;
	delete fireworksSystem_;
}

void SceneP2::update(double t) {
	//waterSystem_->update(t);
	fireworksSystem_->update(t);
}
