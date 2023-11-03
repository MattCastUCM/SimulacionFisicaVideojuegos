#pragma once

#include "../ForceSystem.h"
#include "../ForceGenerators/GravityForceGenerator.h"

class GravitySystem : public ForceSystem {
public:
	GravitySystem() : ForceSystem() {
		Particle* p = new Particle(true);
		particles_.push_back(p);
		p->changeLifetime(-1);

		
		GravityForceGenerator* gr = new GravityForceGenerator({ 0,-9.8f, 0 });
		partForceReg_->addRegistry(gr, p);
		partForceReg_->addParticle(gr, p);

		p = new Particle(true);
		particles_.push_back(p);
		p->changeLifetime(-1);
		gr = new GravityForceGenerator({ 0, -20.0f, 0 });
		partForceReg_->addRegistry(gr, p);
		partForceReg_->addParticle(gr, p);
	}

	~GravitySystem() { }

};

