#pragma once

#include "../ParticleForceSystem.h"
#include "../ForceGenerators/ExplosionForceGenerator.h"
#include "../ParticleGenerators/GaussianParticleGenerator.h"
#include "../../checkMemLeaks.h"

class ExplosionSystem : public ParticleForceSystem {
protected:
	ExplosionForceGenerator* expl_;
	GaussianParticleGenerator* gen_;


public:
	ExplosionSystem(const Vector3& g = { 0.0f, -10.0f, 0.0f }) : ParticleForceSystem(g) {
		srand(time(NULL));

		partForceReg_ = new ParticleForceRegistry();

		Particle* p = new Particle(true, 5);
		expl_ = new ExplosionForceGenerator(10000, 50, 1000, { 0, 0, -100 });
		forces_.insert(expl_);

		gen_ = new GaussianParticleGenerator(0.1, 0, 1, 1, false, true, true, true);
		gen_->changeModelPart(p);
		gen_->changeGenerateN(100);
		gen_->setOrigin({ 0, 0, -100 });
		generators_.insert({ "expl", gen_ });

		delete p;
	}

	inline void setActive(bool active) {
		ParticleForceSystem::setActive(active);

		if (active) {
			expl_->resetTime();
			expl_->setActive(true);
			// Genera las N part�culas solo al activar el generador, no hace 
			// falta ir generando m�s conforme vaya pasando el tiempo.
			// Luego las a�ade a la lista de part�culas y al registro
			// de fuerzas y se les asigna una masa inversa aleatoria
			// entre 1 y 1/15
			auto parts = gen_->generateParticles();
			for (auto part : parts) {
				particles_.push_back(part);
				partForceReg_->addForce(expl_, part);

				int rndMass = rand() % 15 + 1;
				part->setInvMass(1.0f / rndMass);
			}
			
		}
	} 

	
};

