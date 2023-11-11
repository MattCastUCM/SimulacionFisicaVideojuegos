#pragma once


class ParticleSystem;

#include "../ForceGenerator.h"

class SceneP3 {
private:
	RenderItem* floor_;
	ParticleSystem* gravity_, * wind_, *expl_;
	bool grActive_, wndActive_, explActive_;

public:
	SceneP3();
	~SceneP3();

	void update(double t);
	void keyPress(unsigned char key);
};

