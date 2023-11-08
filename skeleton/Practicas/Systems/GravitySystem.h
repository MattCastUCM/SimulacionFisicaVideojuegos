#pragma once

#include "../ParticleSystem.h"
#include "../ForceGenerators/GravityForceGenerator.h"

class GravitySystem : public ParticleSystem {
public:
	GravitySystem() : ParticleSystem() {
		partForceReg_ = new ParticleForceRegistry();

		Particle* p = new Particle(true);
		particles_.push_back(p);
		p->changeLifetime(-1);
		p->setPos(p->getPos() + Vector3(10, 0, 0));
		
		GravityForceGenerator* gr = new GravityForceGenerator({ 0,-9.8f, 0 });
		partForceReg_->addRegistry(gr, p);
		partForceReg_->addParticle(gr, p);

		
		p = new Particle(true);
		particles_.push_back(p);
		p->changeLifetime(-1);
		p->setPos(p->getPos() + Vector3(-10, 0, 0));

		gr = new GravityForceGenerator({ 0, -20.0f, 0 });
		partForceReg_->addRegistry(gr, p);
		partForceReg_->addParticle(gr, p);
	}

	~GravitySystem() { }

};

