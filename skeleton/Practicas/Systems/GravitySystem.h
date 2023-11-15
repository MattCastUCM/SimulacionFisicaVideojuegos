#pragma once

#include "../ParticleForceSystem.h"
#include "../ForceGenerators/GravityForceGenerator.h"

class GravitySystem : public ParticleForceSystem {
private:
	GravityForceGenerator* gr1_, *gr2_;
public:
	GravitySystem() : ParticleForceSystem() {
		partForceReg_ = new ParticleForceRegistry();
		
		gr1_ = new GravityForceGenerator({ 0,-9.8f, 0 });
		gr2_ = new GravityForceGenerator({ 0, -20.0f, 0 });
		
	}


	inline void setActive(bool active) {
		ParticleForceSystem::setActive(active);

		if(active) {
			Particle* p = new Particle(true);
			particles_.push_back(p);
			p->changeLifetime(-1);
			p->setPos({ -10, 50, -100 });
			partForceReg_->addForce(gr1_, p);

			p = new Particle(true);
			particles_.push_back(p);
			p->changeLifetime(-1);
			p->setPos({ 10, 50, -100 });
			partForceReg_->addForce(gr2_, p);
		}
	}
};

