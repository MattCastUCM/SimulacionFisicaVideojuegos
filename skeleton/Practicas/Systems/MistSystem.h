#pragma once

#include "../ParticleSystem.h"
#include "../Particles/Water.h"
#include "../Generators/MistGenerator.h"

class MistSystem : public ParticleSystem {
public:
	MistSystem(const Vector3& g = { 0.0f, -10.0f, 0.0f }) : ParticleSystem(g) {
		
		Particle::visual v;
		v.size = 0.05f;
		v.geometry = new physx::PxSphereGeometry(v.size);
		v.color = { 1.0f, 1.0f, 1.0f, 1.0f };

		Particle::physics p;
		p.damp = 0.998f;
		p.pos = { 0.0f, 0.0f, 0.0f };
		p.vel = { 1.0f, 1.0f, 1.0f };
		//p.vel *= 10;
		p.acc = g / 2;
		p.mass = 5.4f;

		Particle* part = new Particle(v, p, 1);


		MistGenerator* mist = new MistGenerator(0.1, 0, 0.01, 100, false, true, true, true);
		mist->changeModelPart(part);
		mist->changeGenerateN(100);
		generators_.insert({ "mist", mist });

		mist->setOrigin({ 0.0f, 0.0f, 0.0f });

		delete part;

		update(10);
	}
};

