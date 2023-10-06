#pragma once

#include "Particle.h"

class SceneP1 {
private:
	const Vector4 FLOOR_COLOR_ = Vector4(0.0f, 1.0f, 1.0f, 1.0f);
	const physx::PxTransform* FLOOR_TR_ = new physx::PxTransform(0, -10, 0);
	RenderItem* floor_;

	std::vector<Particle*> particles_;
	Camera* cam;

public:
	SceneP1();
	~SceneP1();

	void update(float t);
	void keyPress(unsigned char key);


private:
	void addParticle(unsigned char key);



};

