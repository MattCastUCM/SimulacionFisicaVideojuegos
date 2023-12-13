#pragma once

#include "../ParticleForceSystem.h"
#include "../ForceGenerators/ExplosionForceGenerator.h"

class RigidCubesSystem : public ParticleForceSystem {
protected:
	ExplosionForceGenerator* expl_;
	bool explAct_;

public:
	RigidCubesSystem(PxPhysics* gPhysics, PxScene* gScene) : ParticleForceSystem({ 0.0f, -9.8f, 0.0f }, 100)
	{
		explAct_ = false;

		PxRigidStatic* suelo = gPhysics->createRigidStatic(PxTransform({ 0, -20, -100 }));

		PxShape* shape = CreateShape(PxBoxGeometry(100, 0.1, 100));
		suelo->attachShape(*shape);

		gScene->addActor(*suelo);
		RenderItem* item = new RenderItem(shape, suelo, { 1, 1, 1, 1 });


		Particle::visual v;
		v.size = 0.5f;
		v.geometry = new physx::PxBoxGeometry(v.size, v.size, v.size);
		v.color = { 1.0f, 0, 0, 1.0f };

		Particle::physics p;
		p.damp = 0.998f;
		p.pos = { 0, 0, 0 };
		p.vel = { 0, 0, 0 };
		p.acc = { 0, 0, 0 };
		p.mass = 1 / 50.0f;

		Particle* part = new Particle(v, p, 10, gPhysics, gScene, true);
		ParticleGenerator* gen = new ParticleGenerator(0.5f);
		gen->changeModelPart(part);
		gen->changeGenerateN(1);
		generators_.insert({ "a", gen });

		gen->setOrigin({ 0.0f, 0.0f, -100.0f });

		gScene->removeActor(*part->getRigidActor());
		delete part;


		partForceReg_ = new ParticleForceRegistry();
		expl_ = new ExplosionForceGenerator(10000, 50, 1000, { 0, -20, -100 });
		forces_.insert(expl_);
	}

	inline void keyPress(unsigned char key) {
		switch (tolower(key)) {
		case '1':
			explAct_ = !explAct_;
			if (explAct_) {
				for (auto part : particles_) {
					partForceReg_->addForce(expl_, part);
				}
			}
			else {
				for (auto part : particles_) {
					partForceReg_->deleteParticleRegistry(part);
				}
			}
			break;
		default: break;
		}
	}


};

