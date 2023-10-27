#include "SceneP1.h"
#include "../Particles/Projectile.h"

SceneP1::SceneP1() {
	cam = GetCamera();

	floor_ = new RenderItem(CreateShape(physx::PxBoxGeometry(400, 1, 400)), FLOOR_TR_, FLOOR_COLOR_);
}

SceneP1::~SceneP1() {
	DeregisterRenderItem(floor_);
	for (auto p : particles_) delete p;
}

void SceneP1::update(double t) {
	// Actualiza las part�culas
	for (auto p : particles_)		
		p->update(t);

	// Refresh de las part�culas
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

// A�ade part�culas de distintos tipos dependiendo de la tecla pulsada
void SceneP1::keyPress(unsigned char key) {
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

// Crea una part�cula y la a�ade a la lista
void SceneP1::addParticle(unsigned char key) {
	Particle::visual v;
	v.size = 1.0f;
	//v.geometry = &physx::PxSphereGeometry(2.0f);
	v.geometry = &physx::PxBoxGeometry(2.0f, 2.0f, 2.0f);


	Particle::physics p;
	p.damp = 0.998f;
	/*p.vel = cam->getDir();
	p.pos = cam->getEye();*/
	p.pos = { -50,0,-100 };
	p.vel = { 1, 0, 0 };
	float simSpd;

	// Bola de ca��n
	if (key == tolower('q')) {
		p.vel *= 350.0f;
		p.acc = { 0.0f, -0.5f, 0.0f };
		p.mass = 5.4f;
		simSpd = 50.0f;
		v.color = { 1.0f, 0.0f, 0.0f, 1.0f };
	}
	// Misil de un tanque
	else if (key == tolower('e')) {
		p.vel *= 1800.0f;
		p.acc = { 0.0f, -0.01f, 0.0f };
		p.mass = 15.0f;
		simSpd = 70.0f;
		v.color = { 0.0f, 1.0f, 0.0f, 1.0f };
	}
	// Bala de pistola
	else {
		p.vel *= 330.0f;
		p.acc = { 0.0f, -0.098f, 0.0f };
		p.mass = 0.055f;
		simSpd = 100.0f;
		v.color = { 0.0f, 0.0f, 1.0f, 1.0f };
	}


	Projectile* part = new Projectile(v, p, simSpd);
	particles_.push_back(part);
}
