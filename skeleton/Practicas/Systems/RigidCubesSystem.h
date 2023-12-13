#pragma once

#include "../ParticleSystem.h"

class RigidCubesSystem : public ParticleSystem {
public:
	RigidCubesSystem(PxPhysics* gPhysics, PxScene* gScene) : ParticleSystem({ 0.0f, -9.8f, 0.0f }, 100) 
	{
		PxRigidStatic* suelo = gPhysics->createRigidStatic(PxTransform({ 0, -20, -100 }));

		PxShape* shape = CreateShape(PxBoxGeometry(100, 0.1, 100));
		suelo->attachShape(*shape);

		gScene->addActor(*suelo);
		RenderItem* item = new RenderItem(shape, suelo, { 1, 1, 1, 1 });


		Particle::visual v;
		v.size = 1.0f;
		v.geometry = new physx::PxBoxGeometry(v.size, v.size, v.size);
		v.color = { 1.0f, 0, 0, 1.0f };

		Particle::physics p;
		p.damp = 0.998f;
		p.pos = { 0, 0, 0 };
		p.vel = { 0, 0, 0 };
		p.acc = { 0, 0, 0 };
		p.mass = 1 / 5.4f;

		Particle* part = new Particle(v, p, -1, gPhysics, gScene, true);
		ParticleGenerator* gen = new ParticleGenerator(1, true);
		gen->changeModelPart(part);
		gen->changeGenerateN(1);
		generators_.insert({ "a", gen });

		gen->setOrigin({ 0.0f, 0.0f, -100.0f });

		gScene->removeActor(*part->getRigid());
		delete part;
	}
};

