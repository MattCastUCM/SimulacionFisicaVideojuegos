#pragma once

#include "../ForceGenerator.h"
class ParticleSystem;

class SceneP3 {
private:
	ParticleSystem* gravity_, * wind_, *expl_;
	bool grActive_, wndActive_, explActive_;

public:
	SceneP3();
	~SceneP3();

	void update(double t);
	void keyPress(unsigned char key);
};

