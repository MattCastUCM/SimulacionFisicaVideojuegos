#pragma once

#include "../ParticleForceSystem.h"
#include "../ForceGenerators/GravityForceGenerator.h"

class GravitySystem : public ParticleForceSystem {
public:
	GravitySystem() : ParticleForceSystem() {
		partForceReg_ = new ParticleForceRegistry();

		Particle* p = new Particle(true);
		particles_.push_back(p);
		p->changeLifetime(-1);
		p->setPos({ -10, 50, -100});
		
		GravityForceGenerator* gr = new GravityForceGenerator({ 0,-9.8f, 0 });
		partForceReg_->addForce(gr, p);
		
		p = new Particle(true);
		particles_.push_back(p);
		p->changeLifetime(-1);
		p->setPos({ 10, 50, -100 });

		gr = new GravityForceGenerator({ 0, -20.0f, 0 });
		partForceReg_->addForce(gr, p);
	}

};

