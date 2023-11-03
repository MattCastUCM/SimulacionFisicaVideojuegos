#pragma once


class ForceSystem;

#include "../ForceGenerator.h"

class SceneP3 {
private:
	RenderItem* floor_;
	ForceSystem* gravity_;

public:
	SceneP3();
	~SceneP3();

	void update(double t);
	void keyPress(unsigned char key) { };
};

