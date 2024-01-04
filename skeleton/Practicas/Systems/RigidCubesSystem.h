#pragma once

#include "../Particles/SRigidBody.h"
#include "../Particles/DRigidBody.h"
#include "../ParticleForceSystem.h"
#include "../ForceGenerators/ExplosionForceGenerator.h"
#include "../ParticleGenerators/GaussianParticleGenerator.h"
#include "../../checkMemLeaks.h"

class RigidCubesSystem : public ParticleForceSystem {
protected:
	ExplosionForceGenerator* expl_;
	bool explAct_;

public:
	RigidCubesSystem(PxPhysics* gPhysics, PxScene* gScene) : ParticleForceSystem({ 0.0f, -9.8f, 0.0f }, 100)
	{
		explAct_ = false;

		Particle::visual v;
		v.size = 1.0f;
		v.geometry = new physx::PxBoxGeometry(100, 0.1, 100);
		v.color = { 1.0f, 1.0f, 1.0f, 1.0f };

		Particle::physics p;
		p.damp = 0.998f;
		p.pos = { 0, -20, -100 };
		p.vel = { 0, 0, 0 };
		p.acc = { 0, 0, 0 };
		p.mass = 1;

		SRigidBody* suelo = new SRigidBody(v, p, -1, gPhysics, gScene);
		particles_.push_back(suelo);



		v.size = 0.5f;
		v.geometry = new physx::PxBoxGeometry(v.size, v.size, v.size);
		v.color = { 1.0f, 0, 0, 1.0f };

		p.damp = 0.998f;
		p.pos = { 0, 0, 0 };
		p.vel = { 0, 0, 0 };
		p.acc = { 0, 0, 0 };
		// EN MASS SE GUARDA LA DENSIDAD
		p.mass = 10;

		DRigidBody* part = new DRigidBody(v, p, 10, gPhysics, gScene);
		GaussianParticleGenerator* gen = new GaussianParticleGenerator(0.1, 0, 0.01, 20, false, true, true, true);
		gen->changeModelPart(part);
		gen->changeGenerateN(1);
		generators_.insert({ "a", gen });

		gen->setOrigin({ 0.0f, 0.0f, -100.0f });

		delete part;


		partForceReg_ = new ParticleForceRegistry();
		expl_ = new ExplosionForceGenerator(10000, 50, 1000, { 0, -20, -100 }, 2);
		forces_.insert(expl_);
	}

	inline void keyPress(unsigned char key) {
		switch (tolower(key)) {
		case '1':
			explAct_ = !explAct_;
			if (!explAct_) {
				for (auto p : particles_) {
					partForceReg_->deleteParticleRegistry(p);
				}
			}
			else {
				expl_->resetTime();
				for (auto p : particles_) {
					partForceReg_->addForce(expl_, p);
				}
			}
			break;
		default: break;
		}
	}

	inline virtual void generateParticles(double t) {
		// Recorrer generadores (generar partículas nuevas y añadirlas a la lista)
		for (auto pg : generators_) {
			// El update se encarga de generar las partículas
			// según el tiempo de generación de cada generador
			auto parts = pg.second->update(t);
			for (auto p : parts) {
				particles_.push_back(p);
				
				if(explAct_) partForceReg_->addForce(expl_, p);
			}
		}
	}
};

