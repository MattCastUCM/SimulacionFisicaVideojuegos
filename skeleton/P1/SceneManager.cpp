#include "SceneManager.h"

#include "../RenderUtils.hpp"
#include "../core.hpp"

SceneManager::SceneManager() {
	cam = GetCamera();

	floor_ = new RenderItem(CreateShape(physx::PxBoxGeometry(400, 1, 400)), FLOOR_TR_, FLOOR_COLOR_);
}

SceneManager::~SceneManager() {
	DeregisterRenderItem(floor_);
	for (auto p : particles_) delete p;
}

void SceneManager::update(float t) {
	for (auto p : particles_)		
		p->update(t);

	for (auto p : particles_) {
		particles_.erase(
			remove_if(particles_.begin(), particles_.end(), [](Particle* p) {
				if (p->isAlive()) return false;
				else {

					delete p;
					return true;
				}
			}), particles_.end()
		);
	}
}

void SceneManager::keyPress(unsigned char key) {
	switch (tolower(key)) {
		case ' ':
			break;
		case 'q':
			addParticle(key);
			break;
		case 'e':
			addParticle(key);
			break;
		case 'r':
			addParticle(key);
			break;

		default:
			break;
	}
}

void SceneManager::addParticle(unsigned char key) {
	//Vector3 vel = cam->getDir();
	//Vector3 pos = cam->getEye() + vel * 3;
	//Vector3 realAcc = { 0.0f, /*-9.8*/-1.0f, 0.0f };

	float realMass, realSpd, simSpd, damp = 0.998f;
	Vector4 color;

	Vector3 pos = { -50,0,-100 } /*cam->getEye()*/;
	Vector3 vel = { 1, 0, 0 } /*cam->getDir()*/;
	Vector3 realAcc = { 0.0f, /*-9.8*/-1.0f, 0.0f };



	// Bola de cañón
	if (key == tolower('q')) {
		realMass = 5.4f;
		realSpd = 350.0f;
		simSpd = 5.0f;		// Entre 5 y 25
		color = { 1.0f, 0.0f, 0.0f, 1.0f };
	}
	// Misil de un tanque
	else if (key == tolower('e')) {
		realMass = 15.0f;
		realSpd = 1800.0f;
		simSpd = 15.0f;		// Entre 5 y 25
		color = { 0.0f, 1.0f, 0.0f, 1.0f };
	}
	// Bala de pistola
	else {
		realMass = 0.055f;
		realSpd = 330.0f;
		simSpd = 25.0f;		// Entre 5 y 25
		color = { 0.0f, 0.0f, 1.0f, 1.0f };
	}


	Particle* part = new Particle(1.0f, pos, vel * simSpd, realSpd, realAcc, damp, realMass, color);
	particles_.push_back(part);
}
