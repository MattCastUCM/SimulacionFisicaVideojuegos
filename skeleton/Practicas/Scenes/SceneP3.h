#pragma once


class ParticleSystem;

#include "../ForceGenerator.h"

class SceneP3 {
private:
	RenderItem* floor_;
	ParticleSystem* gravity_;

public:
	SceneP3();
	~SceneP3();

	void update(double t);
	void keyPress(unsigned char key) { };
};

