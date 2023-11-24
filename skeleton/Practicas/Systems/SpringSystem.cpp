#include "SpringSystem.h"

SpringSystem::SpringSystem() : ParticleForceSystem() {
	partForceReg_ = new ParticleForceRegistry();
	gr_ = nullptr;
}

void SpringSystem::keyPress(unsigned char key) {
	switch (tolower(key)) {
	case '1':
		activatePartToStatic();
		break;
	case '2':
		activatePartToPart();
		break;
	case '3':
		activateSlinky();
		break;
	case '4':
		activateFlotation();
		break;
	case 'g':
		activateGravity();
		break;

	default: break;
	}
}

void SpringSystem::activatePartToStatic() {
	clearPartForces();

	float k = 10, restingLength = 10;

	Particle::visual v;
	v.size = 1.0f;
	v.geometry = new physx::PxBoxGeometry(v.size / 2, v.size * 2, v.size * 2);
	v.color = { 1.0f, 1.0f, 1.0f, 1.0f };

	Particle::physics p;
	p.damp = 0.998f;
	p.pos = { -10, 0, -50 };
	p.vel = { 0, 0, 0 };
	p.acc = { 0, 0, 0 };
	p.mass = 1 / 5000.0f;
	Particle* p1 = new Particle(v, p, -1);
	particles_.push_back(p1);

	Particle* p2 = new Particle(true);
	particles_.push_back(p2);
	p2->changeLifetime(-1);
	p2->setPos({ 10, 0, -50 });
	p2->setInvMass(1 / 10.0f);


	SpringForceGenerator* spr1_ = new SpringForceGenerator(k, restingLength, p2);
	forces_.push_back(spr1_);

	SpringForceGenerator* spr2_ = new SpringForceGenerator(k, restingLength, p1);
	forces_.push_back(spr2_);

	//partForceReg_->addForce(spr1_, p1);
	partForceReg_->addForce(spr2_, p2);

	GravityForceGenerator* gr = new GravityForceGenerator({ 0, -1, 0 });
	forces_.push_back(gr);
	partForceReg_->addForce(gr, p2);
}

void SpringSystem::activatePartToPart() {
	clearPartForces();

	float k = 10, restingLength = 10;

	Particle* p1 = new Particle(true);
	particles_.push_back(p1);
	p1->changeLifetime(-1);
	p1->setPos({ -10, 0, -50 });
	p1->setInvMass(1 / 10.0f);

	Particle* p2 = new Particle(true);
	particles_.push_back(p2);
	p2->changeLifetime(-1);
	p2->setPos({ 10, 0, -50 });
	p2->setInvMass(1 / 10.0f);


	SpringForceGenerator* spr1 = new SpringForceGenerator(k, restingLength, p2);
	forces_.push_back(spr1);

	SpringForceGenerator* spr2 = new SpringForceGenerator(k, restingLength, p1);
	forces_.push_back(spr2);

	partForceReg_->addForce(spr1, p1);
	partForceReg_->addForce(spr2, p2);
}

void SpringSystem::activateSlinky() {
	clearPartForces();

	/*float k = 5, restingLength = 1;

	std::vector<Particle*> parts;
	for (int i = 0; i < 5; i++) {
		Particle* p = new Particle(true);
		p->changeLifetime(-1);
		p->setPos({ 0.0f, 20.0f - i, -50 });
		p->setInvMass(1 / 10.0f);

		particles_.push_back(p);
		parts.push_back(p);
	}

	Particle* prevPart = parts[parts.size() - 1];
	std::vector<SpringForceGenerator*> springs;
	for (int i = parts.size() - 1; i >= 0; i--) {
		SpringForceGenerator* spr = new SpringForceGenerator(k, restingLength + 3, parts[i]);
		forces_.push_back(spr);

		partForceReg_->addForce(spr, prevPart);
		prevPart = parts[i];
	}*/


	float k = 10, restingLength = 10;

	Particle* p1 = new Particle(true);
	particles_.push_back(p1);
	p1->changeLifetime(-1);
	p1->setPos({ -10, 0, -50 });
	p1->setInvMass(1 / 10.0f);

	Particle* p2 = new Particle(true);
	particles_.push_back(p2);
	p2->changeLifetime(-1);
	p2->setPos({ 10, 0, -50 });
	p2->setInvMass(1 / 10.0f);


	SpringForceGenerator* spr1 = new SpringForceGenerator(k, restingLength, p2);
	forces_.push_back(spr1);
	partForceReg_->addForce(spr1, p1);

}

void SpringSystem::activateGravity() {
	gr = new GravityForceGenerator({ 0, -1, 0 });
	forces_.push_back(gr);
	for (auto p : particles_) {
		partForceReg_->addForce(gr, p);
	}
}

void SpringSystem::activateFlotation() {
	clearPartForces();

	float k = 10, restingLength = 10;

	Particle::visual v;
	v.size = 30.0f;
	v.geometry = new physx::PxBoxGeometry(v.size, 0.1f, v.size);
	v.color = { 0.0f, 1.0f, 1.0f, 1.0f };

	Particle::physics p;
	p.damp = 0.998f;
	p.pos = { 0, 0, -50 };
	p.vel = { 0, 0, 0 };
	p.acc = { 0, 0, 0 };
	p.mass = 1 / 5000.0f;
	Particle* p1 = new Particle(v, p, -1);
	particles_.push_back(p1);


	v.size = 1.0f;
	v.geometry = new physx::PxBoxGeometry(v.size, v.size, v.size);
	v.color = { 1.0f, 0.0f, 0.0f, 1.0f };

	p.damp = 0.998f;
	p.pos = { 0, 0, -50 };
	p.vel = { 0, 0, 0 };
	p.acc = { 0, 0, 0 };
	p.mass = 1 / 10.0f;
	Particle* p2 = new Particle(v, p, -1);
	particles_.push_back(p2);


	SpringForceGenerator* spr1_ = new SpringForceGenerator(k, restingLength, p2);
	forces_.push_back(spr1_);

	SpringForceGenerator* spr2_ = new SpringForceGenerator(k, restingLength, p1);
	forces_.push_back(spr2_);

	partForceReg_->addForce(spr1_, p1);
	partForceReg_->addForce(spr2_, p2);
}
