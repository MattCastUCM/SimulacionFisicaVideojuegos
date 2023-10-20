#pragma once

#include "ParticleSystem.h"

class SceneP2 {
private:
	const Vector4 FLOOR_COLOR_ = Vector4(0.0f, 1.0f, 1.0f, 1.0f);
	const physx::PxTransform* FLOOR_TR_ = new physx::PxTransform(0, -10, 0);
	RenderItem* floor_;

	ParticleSystem* waterSystem_;


public:
	SceneP2();
	~SceneP2();

	void update(double t);
	
};

