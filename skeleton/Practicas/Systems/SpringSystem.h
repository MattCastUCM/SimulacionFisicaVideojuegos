#pragma once

#include "../ParticleForceSystem.h"
#include "../ForceGenerators/SpringForceGenerator.h"

class SpringSystem : public ParticleForceSystem {
private:
	SpringForceGenerator* spr1_, *spr2_;

public:
	SpringSystem() : ParticleForceSystem() {
		partForceReg_ = new ParticleForceRegistry();
		
	}


	inline void setActive(bool active) {
		ParticleForceSystem::setActive(active);

		if(active) {
			float k = 10, restingLength = 10;

			Particle* p1 = new Particle(true);
			particles_.push_back(p1);
			p1->changeLifetime(-1);
			p1->setPos({ -10, 0, -50 });
			p1->setInvMass(1 / 200.0f);

			Particle* p2 = new Particle(true);
			particles_.push_back(p2);
			p2->changeLifetime(-1);
			p2->setPos({ 10, 0, -50 });
			p2->setInvMass(1 / 10.0f);


			spr1_ = new SpringForceGenerator(k, restingLength, p2);
			forces_.push_back(spr1_);
			
			spr2_ = new SpringForceGenerator(k, restingLength, p1);
			forces_.push_back(spr2_);
			
			partForceReg_->addForce(spr1_, p1);
			partForceReg_->addForce(spr2_, p2);

		}
	}
};

