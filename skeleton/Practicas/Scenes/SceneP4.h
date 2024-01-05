#pragma once

#include "../ForceGenerator.h"
class ParticleSystem;

class SceneP4 {
private:
	ParticleSystem* springs_;

public:
	SceneP4();
	~SceneP4();

	void update(double t);
	void keyPress(unsigned char key);
};

