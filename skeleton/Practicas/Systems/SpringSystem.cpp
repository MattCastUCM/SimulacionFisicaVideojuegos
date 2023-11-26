#include "SpringSystem.h"

#include "../ForceGenerators/SpringForceGenerator.h"
#include "../ForceGenerators/ElasticRubberForceGenerator.h"
#include "../ForceGenerators/BuoyancyForceGenerator.h"

#include "../ForceGenerators/GravityForceGenerator.h"
#include "../ForceGenerators/WindForceGenerator.h"


SpringSystem::SpringSystem() : ParticleForceSystem(), staticPart_(nullptr), 
	gr_(nullptr), activatedGr_(false), wind_(nullptr), activatedWind_(false)
{
	
	partForceReg_ = new ParticleForceRegistry();
	
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
		activateRubberBand();
		break;
	case '4':
		activateSlinky();
		break;
	case '5':
		activateBuoyancy();
		break;
	case 'g':
		activatedGr_ = !activatedGr_;
		toggleGravity(activatedGr_);
		break;
	case 'v':
		activatedWind_ = !activatedWind_;
		toggleWind(activatedWind_);
		break;
	default: break;
	}

	for (auto fg : forces_) fg->keyPress(key);
}

void SpringSystem::activatePartToStatic() {
	clearPartForces();

	Particle::visual v;
	v.size = 1.0f;
	//v.geometry = new physx::PxBoxGeometry(v.size / 2, v.size * 2, v.size * 2);
	v.geometry = new physx::PxBoxGeometry(v.size * 2, v.size / 2, v.size * 2);
	v.color = { 1.0f, 1.0f, 1.0f, 1.0f };

	Particle::physics p;
	p.damp = 0.998f;
	//p.pos = { -10, 0, -50 };
	p.pos = { 0, 20, -50 };
	p.vel = { 0, 0, 0 };
	p.acc = { 0, 0, 0 };
	p.mass = 1 / 5000.0f;
	staticPart_ = new Particle(v, p, -1);
	particles_.push_back(staticPart_);

	Particle* part = new Particle(true);
	particles_.push_back(part);
	part->changeLifetime(-1);
	//part->setPos({ 10, 0, -50 });
	part->setPos({ 0, 10, -50 });
	part->setInvMass(1 / 10.0f);


	// No deberían tener un restingLength muy inferior a la distancia con la que aparecen inicialmente
	//float k = 500, restingLength = 10;
	float k = 10, restingLength = 7;

	// No hace falta añadir la partícula estática a las fuerzas
	SpringForceGenerator* spr = new SpringForceGenerator(k, restingLength, staticPart_);
	forces_.insert(spr);
	partForceReg_->addForce(spr, part);

	toggleGravity(true);
}

void SpringSystem::activatePartToPart() {
	clearPartForces();

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


	// No deberían tener un restingLength muy inferior a la distancia con la que aparecen inicialmente
	float k = 10, restingLength = 10;

	SpringForceGenerator* spr1 = new SpringForceGenerator(k, restingLength, p2);
	forces_.insert(spr1);

	SpringForceGenerator* spr2 = new SpringForceGenerator(k, restingLength, p1);
	forces_.insert(spr2);

	partForceReg_->addForce(spr1, p1);
	partForceReg_->addForce(spr2, p2);
}

void SpringSystem::activateRubberBand() {
	clearPartForces();

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


	// No deberían tener un restingLength muy inferior a la distancia con la que aparecen inicialmente
	float k = 10, restingLength = 10;

	ElasticRubberForceGenerator* spr1 = new ElasticRubberForceGenerator(k, restingLength, p2);
	forces_.insert(spr1);

	ElasticRubberForceGenerator* spr2 = new ElasticRubberForceGenerator(k, restingLength, p1);
	forces_.insert(spr2);

	partForceReg_->addForce(spr1, p1);
	partForceReg_->addForce(spr2, p2);
}


void SpringSystem::activateSlinky() {
	clearPartForces();

	staticPart_ = new Particle(true);
	particles_.push_back(staticPart_);
	staticPart_->changeLifetime(-1);
	staticPart_->setPos({ 0, 20, -50 });
	staticPart_->setInvMass(1 / 10.0f);

	std::vector<Particle*> parts;
	for (int i = 0; i < 4; i++) {
		Particle* p = new Particle(true);
		p->changeLifetime(-1);
		p->setPos({ 0.0f, 20.0f - (i + 1) * 5, -50 });
		p->setInvMass(1 / 10.0f);

		particles_.push_back(p);
		parts.push_back(p);
	}

	float k = 100, restingLength = 1;
	Particle* prevPart = staticPart_;
	std::vector<SpringForceGenerator*> springs;
	for (int i = 0; i < parts.size(); i++) {
		/*SpringForceGenerator* spr1 = new SpringForceGenerator(k, restingLength, parts[i]);
		forces_.insert(spr1);*/

		SpringForceGenerator* spr2 = new SpringForceGenerator(k, restingLength, prevPart);
		forces_.insert(spr2);

		//partForceReg_->addForce(spr1, prevPart);
		partForceReg_->addForce(spr2, parts[i]);
		prevPart = parts[i];
	}
}

void SpringSystem::activateBuoyancy() {
	clearPartForces();

	Particle::visual v;
	v.size = 30.0f;
	v.geometry = new physx::PxBoxGeometry(v.size, 0.02f, v.size);
	v.color = { 0.0f, 1.0f, 1.0f, 1.0f };

	Particle::physics p;
	p.damp = 0.998f;
	p.pos = { 0, 0, -50 };
	p.vel = { 0, 0, 0 };
	p.acc = { 0, 0, 0 };
	p.mass = 1 / 5000.0f;
	staticPart_ = new Particle(v, p, -1);
	particles_.push_back(staticPart_);


	v.size = 1.0f;
	v.geometry = new physx::PxBoxGeometry(v.size, v.size, v.size);
	v.color = { 1.0f, 0.0f, 0.0f, 1.0f };

	p.damp = 0.998f;
	p.pos = { 0, 0, -50 };
	p.vel = { 0, 0, 0 };
	p.acc = { 0, 0, 0 };
	p.mass = 1 / 1.0f;
	Particle* part = new Particle(v, p, -1);
	particles_.push_back(part);


	float V = v.size * v.size * v.size;
	float waterD = 1000.0f;
	BuoyancyForceGenerator* b = new BuoyancyForceGenerator(v.size, V, waterD, staticPart_);
	forces_.insert(b);
	partForceReg_->addForce(b, part);

	gr_ = new GravityForceGenerator({ 0, -9.81f, 0 });
	forces_.insert(gr_);
	partForceReg_->addForce(gr_, part);
}



#include <iostream>
void SpringSystem::toggleGravity(bool activate) {
	activatedGr_ = activate;
	
	if (activatedGr_) {
		std::cout << "Gravity activated\n";
		if (gr_ == nullptr) {
			gr_ = new GravityForceGenerator({ 0, -9.81f, 0 });
			forces_.insert(gr_);
		}
		for (auto p : particles_) {
			if (p != staticPart_) partForceReg_->addForce(gr_, p);
		}
	}
	else {
		std::cout << "Gravity deactivated\n";
		partForceReg_->deleteForceRegistry(gr_);
		forces_.erase(gr_);
		gr_ = nullptr;
	}
}

void SpringSystem::toggleWind(bool activate){
	activatedWind_ = activate;

	if (activatedWind_) {
		std::cout << "Wind activated\n";
		if (wind_ == nullptr) {
			wind_ = new WindForceGenerator({ 100, 0, 0 }, 0.5f, 0.0);
			forces_.insert(wind_);
		}

		for (auto p : particles_) {
			if (p != staticPart_) partForceReg_->addForce(wind_, p);
		}
	}
	else {
		std::cout << "Wind deactivated\n";
		partForceReg_->deleteForceRegistry(wind_);
		forces_.erase(wind_);
		wind_ = nullptr;
	}

}

