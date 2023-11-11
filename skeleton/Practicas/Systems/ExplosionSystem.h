#pragma once

#include "../ParticleForceSystem.h"
#include "../ForceGenerators/ExplosionForceGenerator.h"
#include "../ParticleGenerators/GaussianParticleGenerator.h"

class ExplosionSystem : public ParticleForceSystem {
protected:
	ExplosionForceGenerator* expl_;

public:
	ExplosionSystem(const Vector3& g = { 0.0f, -10.0f, 0.0f }) : ParticleForceSystem(g) {
		srand(time(NULL));

		partForceReg_ = new ParticleForceRegistry();

		Particle* p = new Particle(true, -1);
		p->setVel({ 1,1,1 });
		p->setInitVel({ 1,1,1 });
		expl_ = new ExplosionForceGenerator(1000, 50, 0.1, { 0, 0, -100 });

		GaussianParticleGenerator* pg = new GaussianParticleGenerator(0.1, 0, 1, 1, false, true, true, true);
		pg->changeModelPart(p);
		pg->changeGenerateN(200);
		pg->setOrigin({ 0, 0, -100 });

		// Genera las N partículas solo al principio, no hace 
		// falta ir generando más conforme vaya pasando el tiempo.
		// Luego las añade a la lista de partículas y al registro
		// de fuerzas y se les asigna una masa inversa aleatoria
		// entre 1 y 1/15
		auto parts = pg->generateParticles();
		for (auto part : parts) {
			particles_.push_back(part);
			partForceReg_->addForce(expl_, part);

			int rndMass = rand() % 15 + 1;
			part->setInvMass(1.0f / rndMass);
		}
		for (auto part : particles_) part->setVel({ 0,0,0 });

		delete p;
	}

	

};

